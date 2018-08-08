
#include <stdbool.h>
#include <stdint.h>
#include "library\Communication\Header\I_PT.h"


static void _PT_Startup(PTParameter* parameters)
 {

     pt_iocomponent->IO_PinWrite(parameters->ioport,parameters->sckpin,0);

     pt_iocomponent->IO_PinWrite(parameters->ioport,parameters->mosipin,0);

     pt_iocomponent->IO_PinWrite(parameters->ioport,parameters->sspin,parameters->sspin);

     pt_spicomponent->SPIM_IntConfigure(parameters->spiport,
                                              SPI_Ready,true,0,true);

  // parameters->spicomponent->SPIM_IntHandler(parameters->spiport,
   //                                         _pt_handler);



}

Enum_result _PT_alarm_Routine(bool* flag,bool reset,PTParameter* parameters,
                              uint32_t delayindex)
 {
   if (reset)
   {
        parameters->step_alarm=0;

   }

   switch (parameters->step_alarm)
   {
   case 0:
       if (*flag==true)
       {
         *flag=false;

         parameters->step_alarm=2;

       }
      break;

   case 2:
        if (parameters->_count<parameters->retrycount)
        {
            parameters->step_alarm=5;
        }
        else
        {
             parameters->step_alarm=1100;
        }


      break;

   case 5:

       pt_iocomponent->IO_PinWrite(parameters->ioport,parameters->sspin,
                                 parameters->sspin);

       parameters->step_alarm=10;

      pt_delaycomponent->Delay_Routine(true,1,7,delayindex);


      break;


   case 10:
      if (pt_delaycomponent->Delay_Routine(true,1,7,delayindex)==true)
        {
           _PT_Startup(parameters);
           parameters->step_alarm=20;

        }

   case 20:

        if (pt_delaycomponent->Delay_Routine(true,1,7,delayindex)==true)
        {

         pt_delaycomponent->Delay_Routine(true,1,1,delayindex);
         parameters->step_alarm=30;

        }
      break;
   case 30:

       parameters->tempvalue=parameters->cmdreset;
       pt_spicomponent->SPIM_Transmit(true,parameters->spiport,
                                               &(parameters->tempvalue));
       parameters->step_alarm=40;
      break;
   case 40:

    if (pt_spicomponent->SPIM_Transmit(false,parameters->spiport,
                                               &(parameters->tempvalue))==true)
       {
        //parameters->_f1=0;
        //parameters->spicomponent->SPIM_Receive(parameters->spiport,
         //                                     &(parameters->_p));
        parameters->step_alarm=50;


       }
    else if (pt_delaycomponent->Delay_Routine(true,1,1,delayindex)==true)
       {


            parameters->step_alarm=2;
       }
      break;
   case 50:

      if (parameters->tempvalue==parameters->cmdreset)
       {
          parameters->step_alarm=0;
          return (successful);
       }
      else if (pt_delaycomponent->Delay_Routine(true,1,1,delayindex)==true)
       {
          parameters->step_alarm=1000;


       }
       else
       {
        parameters->step_alarm=30;

       }
      break;

   case 1000:

       // error log
        parameters->step_alarm=2;
       break;

   case 1100:
       //error log
         parameters->step_alarm=0;
         return (failure);
    }
   return (None);
 }



 void _PT_Routine(bool reset,uint32_t ptparaindex,uint32_t crcparaindex,
                  uint32_t delayparaindex,uint32_t modelinindex,uint32_t modeloutindex)
 {
     PTParameter* parameters;
     bool tempbool;
     uint8_t headerlength;
     parameters=(PTParameter*)pt_listcomp->
                           GetAt(&ptlist,ptparaindex);
    if (reset)
     {
        _PT_Startup(parameters);
        headerlength=3;

        parameters->step=0;
        parameters->_count=0;
        _PT_alarm_Routine(false,true,parameters,delayparaindex);

      }


   switch ( parameters->step)
    {
    case 0:

    if (pt_modelcomp->Get(modelinindex,&parameters->tempcmd,&parameters->templength,
        &parameters->tempvalue,0,false)!=notvalid_model&&parameters->tempcmd==parameters->cmdft)
        {

            parameters->_count2=0;
            parameters->step=5;
        }

        break;

    case 5:


            pt_iocomponent->IO_PinWrite(parameters->ioport,parameters->sspin,0);
            pt_delaycomponent->Delay_Routine(true,1,7,
                                        delayparaindex);

            parameters->step=10;

         break;

    case 10:

      if (pt_delaycomponent->Delay_Routine(false,1,7,delayparaindex)==true)
       {

         parameters->flag_sendsuccessful=false;
         parameters->tempcount=0;
         parameters->step=20;

       }
        break;

    case 20:


       if (pt_modelcomp->Get(modelinindex,&parameters->tempcmd,&parameters->templength,
                              &parameters->tempvalue,parameters->tempcount++,false)
                              ==overflow_model)
        {

      SEGGER_RTT_printf(0,"PT%d\r\n", 30);
          pt_modelcomp->Get(modelinindex,&parameters->tempcmd,&parameters->templength,
                              &parameters->tempvalue,1,true);
          parameters->tempcount=0;

         parameters->step=100;

        }
     else
        {


         pt_spicomponent->SPIM_Transmit(true,parameters->spiport,
                                                 &(parameters->tempvalue));

         pt_delaycomponent->Delay_Routine(true,1,1,
                                        delayparaindex);
         parameters->step=60;


        }

     break;

    case 60:
      if (pt_spicomponent->SPIM_Transmit(false,parameters->spiport,
                                                 &(parameters->tempvalue))==true)
       {
       // parameters->spicomponent->SPIM_Receive(parameters->spiport,
       //                                       &(parameters->_p));
       if (parameters->tempvalue==0xFF)
       {

         parameters->flag_sendsuccessful=true;

       }

        parameters->step=20;
       // parameters->_f1=0;

       }
       else if (pt_delaycomponent->Delay_Routine(false,1,1,
                 delayparaindex)==true)
       {



           pt_modelcomp->Get(modelinindex,&parameters->tempcmd,&parameters->templength,
                              &parameters->tempvalue,1,true);
           parameters->step=1000;
         //  parameters->flag_alarm=true;

       }
        break;
    case 70:


      switch (parameters->_tempresult)
      {
      case None:


        break;
      case successful:
       // parameters->step=20;
       // parameters->model->inputptr=0;

        break;
      case failure:
         //parameters->model->output_cmd=parameters->model->input_cmd;
        // parameters->model->outputlength=1;

       //  parameters->model->outputptr=0;
       //  parameters->step=1000;
       break;
      }

    case 100:
        if (parameters->flag_sendsuccessful==true)
        {
            parameters->_count2++;
            if (parameters->_count2>parameters->retrycount)
            {
             parameters->step=1000;
            }
            else
            {
             pt_delaycomponent->Delay_Routine(true,1,7,
                                        delayparaindex);
             parameters->step=10;
            }
   SEGGER_RTT_printf(0,"PT%d\r\n",100);
        }
        else
        {
            parameters->_count2=0;
            parameters->step=105;
        }

        break;

    case 105:

     if (parameters->tempcount<headerlength)
        {

         parameters->tempvalue=0;
         pt_spicomponent->SPIM_Transmit(true,parameters->spiport,
                                                 &(parameters->tempvalue));
         pt_delaycomponent->Delay_Routine(true,1,1,
                                       delayparaindex);
         parameters->step=110;

        }
     else
        {

         parameters->step=115;

        }

      break;
    case 110:

       if (pt_spicomponent->SPIM_Transmit(false,parameters->spiport,
                                                 &(parameters->tempvalue))==true)
       {
        //parameters->spicomponent->SPIM_Receive(parameters->spiport,
        //                                      &(parameters->_p));
         *(parameters->receivedata+parameters->tempcount++)=
                            parameters->tempvalue;
         if (parameters->tempvalue=0xFF)
         {
             parameters->flag_sendsuccessful=true;
         }

        parameters->step=105;
        //parameters->_f1=0;

       }
       else if (pt_delaycomponent->Delay_Routine(true,1,1,
                 delayparaindex)==true)
       {

         parameters->step=1000;

       }
        break;


    case 115:
        if (parameters->flag_sendsuccessful==true)
        {
            parameters->_count2++;
            if (parameters->_count2>parameters->retrycount)
            {
             parameters->step=1000;
            }
            else
            {
             pt_delaycomponent->Delay_Routine(true,1,7,
                                        delayparaindex);
             parameters->step=117;
            }

        }
        else
        {
            parameters->_count2=0;
            parameters->step=120;
        }

        break;

    case 117:
         if ( pt_delaycomponent->Delay_Routine(true,1,7, delayparaindex)==true)
         {
             parameters->tempcount=0;
             parameters->step=105;
         }

        break;


    case 120:

            parameters->flag_sendsuccessful=false;
            parameters->_count2=0;
            parameters->tempcount=0;
            parameters->step=130;

        break;

    case 130:
        if (parameters->tempcount<*(parameters->receivedata+2))
        {
         parameters->tempvalue=0;
         pt_spicomponent->SPIM_Transmit(true,parameters->spiport,
                                                 &(parameters->tempvalue));
         pt_delaycomponent->Delay_Routine(true,1,1,
                                       delayparaindex);
         parameters->step=140;

        }
     else
        {
         parameters->step=150;

        }

      break;
    case 140:

       if (pt_spicomponent->SPIM_Transmit(true,parameters->spiport,
                                                 &(parameters->tempvalue))==true)
       {
        //parameters->spicomponent->SPIM_Receive(parameters->spiport,
        //                                      &(parameters->_p));
         *(parameters->receivedata+parameters->tempcount++)=
                            parameters->tempvalue;
         if (parameters->tempvalue=0xFF)
         {
             parameters->flag_sendsuccessful=true;
         }

        parameters->step=130;
        //parameters->_f1=0;

       }
       else if (pt_delaycomponent->Delay_Routine(true,1,1,
                 delayparaindex)==true)
       {

         parameters->step=1000;

       }
        break;

    case 150:
       if (parameters->flag_sendsuccessful==true)
        {
            parameters->_count2++;
            if (parameters->_count2>parameters->retrycount)
            {
             parameters->step=1000;
            }
            else
            {
             pt_delaycomponent->Delay_Routine(true,1,7,
                                        delayparaindex);
             parameters->step=160;
            }

        }
        else
        {
            parameters->flag_sendsuccessful=false;
            parameters->_count2=0;
            parameters->tempcount=0;
            parameters->step=200;
        }

        break;

    case 160:
        if ( pt_delaycomponent->Delay_Routine(true,1,7, delayparaindex)==true)
           {
               parameters->step=130;
               parameters->tempcount=0;
           }
         break;

    case 200:

     if (pt_modelcomp->Set(modeloutindex,crcparaindex,parameters->tempcmd,
                            parameters->receivedata,headerlength+*(parameters->receivedata+2),
                            false)==successful_model)
       {
         pt_iocomponent->IO_PinWrite(parameters->ioport,parameters->sspin,
                                 parameters->sspin);
       }

          parameters->step=0;


       break;


    case 1000:


       if (pt_modelcomp->Set(modeloutindex,crcparaindex,parameters->tempcmd,
                           parameters->receivedata,1,false)==successful_model)

       {
           pt_iocomponent->IO_PinWrite(parameters->ioport,parameters->sspin,
                                 parameters->sspin);
            parameters->step=0;
       }



       break;


  }

   parameters->_tempresult=
  _PT_alarm_Routine(&(parameters->flag_alarm),false,parameters,delayparaindex);


 }

uint32_t _PT_ParameterInitial(SPIBaseAddress spiport,
                              IOBaseAddress ioport,enum_CmdID cmdft,
                              enum_CmdID cmdreset,
                              PinAndPinset sckpin,PinAndPinset mosipin,
                              PinAndPinset misopin,PinAndPinset sspin)
{
     ListStruct_uint8 tempdata;

    tempdata.value=(uint32_t)malloc(sizeof(PTParameter));
    ((PTParameter*)(tempdata.value))->cmdft=cmdft;
    ((PTParameter*)(tempdata.value))->cmdreset=cmdreset;
    ((PTParameter*)(tempdata.value))->ioport=ioport;
    ((PTParameter*)(tempdata.value))->spiport=spiport;
    ((PTParameter*)(tempdata.value))->misopin=misopin;
    ((PTParameter*)(tempdata.value))->mosipin=mosipin;
    ((PTParameter*)(tempdata.value))->sckpin=sckpin;
    ((PTParameter*)(tempdata.value))->sspin=sspin;
    return(pt_listcomp->Insert(&ptlist,&tempdata));



}


 void CreatePTHandshakeModule(PTDevice* module,
                              IODevice* iocomponent,
                              DelayObject* delaycomponent,
                              SPIMDevice* spicomponent,
                              CRCDevice* crccomp,
                              ListCollection* listcomp,ModelDevice* modelcomp)
 {

    module->PT_Routine=_PT_Routine;
    module->ParameterInitial=_PT_ParameterInitial;
    pt_crccomp=crccomp;
    pt_delaycomponent=delaycomponent;
    pt_iocomponent=iocomponent;
    pt_listcomp=listcomp;
    pt_spicomponent=spicomponent;
    pt_modelcomp=modelcomp;

 }
