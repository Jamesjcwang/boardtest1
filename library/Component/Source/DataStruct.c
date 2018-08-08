
#include "library\Component\Header\I_DataStruct.h"

 uint32_t _Insert(ListStruct_uint8* head, ListStruct_uint8* data )
  {
      ListStruct_uint8* tempptr;
      ListStruct_uint8* ptr;

      ptr=(ListStruct_uint8*)malloc(sizeof(ListStruct_uint8));

      ptr->next=0;
      ptr->value=data->value;
      uint8_t tempcount;
      tempcount=0;
      tempptr=head;


         while(tempptr->next!=0)
         {
           tempcount++;

           if (tempcount>200)
           {
               return(0);
           }
           tempptr=tempptr->next;

         }
        ptr->index=tempcount;
        tempptr->next=ptr;

        return(tempcount);
  }

 bool _GetFirst(ListStruct_uint8* head,ListStruct_uint8* datagetted)
   {
       ListStruct_uint8* tempptr;

       tempptr=head->next;
       if (head->next!=0)
       {
         datagetted->next=(head->next)->next;

         datagetted->value=(head->next)->value;

          head->next=(head->next)->next;
          free(tempptr);

       return (true);
       }
       else
       {
         return (false);
       }

   }
bool _RemoveAt(ListStruct_uint8* head, uint8_t index)
{  ListStruct_uint8* tempdata1;
   ListStruct_uint8* tempdata2;

   if (head->next==0)
   {
       return(false);
   }
   tempdata1=head;

    while(tempdata1->next!=0)
    {
       tempdata2=tempdata1;
       tempdata1=tempdata1->next;
       if (tempdata1->index==index)
       {
           tempdata2->next=tempdata1->next;
           free(tempdata1);
           return(true);
       }

    }
    return (false);

}

bool _Clean(ListStruct_uint8*head)
{
   ListStruct_uint8* tempdata;
   ListStruct_uint8* temppoint;
   if (head->next==0)
   {
       return(true);
   }
   tempdata=head->next;

    while(tempdata->next!=0)
    {
       temppoint=tempdata;
       tempdata=tempdata->next;
       free(temppoint);

    }
    free(tempdata);

    return (true);

}


uint32_t _GetAt(ListStruct_uint8* head, uint8_t index)
{
    ListStruct_uint8* tempdata;

   if (head->next==0)
   {
       return(0);
   }
    tempdata=head;

    while(tempdata->next!=0)
    {

       tempdata=tempdata->next;

      if (tempdata->index==index)
       {

        return(tempdata->value);
       }


    }

    //SEGGER_RTT_printf(0,"listxx %d\r\n", index);
    return (0);

}

void CreateListMoudle(ListCollection* device)
   {
     device->GetFirst=_GetFirst;
     device->Insert=_Insert;
     device->RemoveAt=_RemoveAt;
     device->GetAt=_GetAt;
     device->Clean=_Clean;

   }
