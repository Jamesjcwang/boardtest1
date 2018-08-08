#include <stdbool.h>
#include <stdint.h>
#include "library\Component\Header\I_CRC.h"
ListStruct_uint8 crclist;

bool crc_16bit(uint32_t index , uint8_t* length,
               uint8_t  *data)
{
   //  SEGGER_RTT_printf(0,"crc %d\r\n",000);
    uint8_t* tempdata;
    uint32_t _tempmaindata;
    uint8_t _temps1[8];

    uint8_t** _temps;
    uint8_t _i,_j,_bitcount;
    uint8_t _length;
    _length=*(length);
    uint8_t* _data;
   //  SEGGER_RTT_printf(0,"crc %d\r\n",00);
    CRCParameter* para;
    para=(CRCParameter*)CRC_listcomp->GetAt(&crclist,index);

    SEGGER_RTT_printf(0,"crc %d\r\n",para);

    _data=malloc(_length*sizeof(uint8_t));

   // *(_data)=cmd;
    *(_data+1)=_length;

    for (_i=2;_i<_length;_i++)
    {
      *(_data+_i)=*(data+_i-2);
    }

    _bitcount=para->bitcount;

    _temps=malloc(_bitcount*sizeof(uint8_t*));

    for (_i=0;_i<_bitcount;_i++)
    {
      *(_temps+_i)=malloc(8*sizeof(uint8_t));

    }

   SEGGER_RTT_printf(0,"crc %d\r\n",02);
    tempdata=malloc((_length+2)*sizeof(uint8_t));
    *(tempdata+_length)=0;
    *(tempdata+_length+1)=0;
   for (_i=0;_i<_length;_i++)
   {  *(tempdata+_i)=0;

      for (_j=0;_j<8;_j++)
       {
         *(_temps1+_j)=*(_data+_i)&(1<<_j);

         if ((7-(_j<<1))>0)
         {
           *(tempdata+_i)= *(tempdata+_i)|(_temps1[_j]<<(7-(_j<<1)));
         }
         else
         {
            *(tempdata+_i)= *(tempdata+_i)|(_temps1[_j]>>((_j<<1)-7));
         }

       }

   }
 SEGGER_RTT_printf(0,"crc %d\r\n",03);
 _tempmaindata=0;

   for (_i=0;_i<_bitcount;_i++)
   {


    if ((_bitcount-2-_i)>=0)
    {
        _tempmaindata=_tempmaindata|
         (((uint32_t)*(tempdata+_i))<<(8<<(_bitcount-2-_i)));
    }
    else
    {
         _tempmaindata=_tempmaindata|((uint32_t)*(tempdata+_i));
    }
   }
 SEGGER_RTT_printf(0,"crc %d\r\n",04);

 SEGGER_RTT_printf(0,"crc %d\r\n",_length);
    for (_j=2;_j<(_length+2);_j++)
    {
      for (_i=0;_i<8;_i++)
      {
         if ((_tempmaindata>>15)&1==1)
          {
            _tempmaindata=_tempmaindata<<1;
            _tempmaindata=_tempmaindata|((*(tempdata+_j)&(1<<(7-_i)))>>(7-_i));
            _tempmaindata=_tempmaindata^(uint32_t)(para->polynom);

          }

         else
          {
            _tempmaindata=_tempmaindata<<1;
            _tempmaindata=_tempmaindata|((*(tempdata+_j)&(1<<(7-_i)))>>(7-_i));
          }

      }


    }
     SEGGER_RTT_printf(0,"crc %d\r\n",05);
    for (_i=0;_i<_bitcount;_i++)
    {
      if ((_bitcount-2-_i)>=0)
      {
        *(para->crcresult+_i)=(_tempmaindata&(((uint32_t)(0xFF)<<(8<<(_bitcount-2-_i)))))
          >>(8<<(_bitcount-2-_i)) ;
      }
      else
      {
          *(para->crcresult+_i)=_tempmaindata&((uint32_t)(0xFF));
      }


    }

    for (_i=0;_i<_bitcount;_i++)
    {


      for (_j=0;_j<8;_j++)
       {
         *(*(_temps+_i)+_j)= *(para->crcresult+_i)&(1<<_j);
       }

          *(para->crcresult+_i)=0;

    }

      for (_i=0;_i<_bitcount;_i++)
    {
      for (_j=0;_j<8;_j++)
       {
          if ((7-(_j<<1))>0)
         {
           *(para->crcresult+_i)= *(para->crcresult+_i)|
           ((*(*(_temps+(_bitcount-1-_i))+_j))<<(7-(_j<<1)));
         }
         else
         {
           *(para->crcresult+_i)= *(para->crcresult+_i)|
           ((*(*(_temps+(_bitcount-1-_i))+_j))>>((_j<<1)-7));

         }
       }

     }

 //SEGGER_RTT_printf(0,"crc %d\r\n",05);
    for (_i=0;_i<para->bitcount;_i++)
    {
        *(data+_length-(para->bitcount)+_i)=*(para->crcresult+_i);

    }


     free(tempdata);
      for (_i=0;_i<_bitcount;_i++)
     {
         free(*(_temps+_i));

     }
     free(_temps);

    SEGGER_RTT_printf(0,"crc %d\r\n",00);

     for (_i=0;_i<_bitcount;_i++)
     {
       if((para->crcresult+_i)!=0)
       {
          return(false);
       }
     }
     return (true);

}

uint32_t _CRC_ParameterInitial(enum_crckind bitcount, uint32_t polynom)
{
     ListStruct_uint8 tempdata;

    tempdata.value=(uint32_t)malloc(sizeof(CRCParameter));

    ((CRCParameter*)(tempdata.value))->bitcount=bitcount;
    ((CRCParameter*)(tempdata.value))->polynom=polynom;

    return(CRC_listcomp->Insert(&crclist,&tempdata));



}
void CreateCRCDevice(CRCDevice* device,ListCollection* listcomp)
{
    device->crc=crc_16bit;
    device->ParameterInitial=_CRC_ParameterInitial;
    CRC_listcomp=listcomp;

}
