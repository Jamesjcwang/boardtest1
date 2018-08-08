#include <stdbool.h>
#include <stdint.h>
#include "library\Physic\Header\I_Timer.h"
#include "library\Component\Header\I_Delayobject.h"
#define Flagdefinition(I)  _f[I]
#define Functionset(T,X)   (T->X)=1


static void _Delay_start(enum_Timer_BaseAddress port)
 {
      Delay_timercomp->TIMER_IntConfigure(port,
                                              Timer_COMPARE0|Timer_COMPARE1
                                              |Timer_COMPARE2|Timer_COMPARE3
                                              |Timer_COMPARE4|Timer_COMPARE5,true,0,
                                              true);

 }
static bool  _Delay_Routine(bool reset,uint32_t sec,
                            uint32_t divide,uint32_t paraindex)
 {
    volatile   uint32_t i,j;
     DelayObjectParameter* parameter;
     i=Delay_listcomp->GetAt(&delaylist,paraindex);

   if (i)
    {
       parameter=(DelayObjectParameter*)i;
    }
   else
    {
       return (false);
    }

   if (reset==true)
   {

       parameter->step=0;
   }

   switch ( parameter->step)
   {

   case 0:

      Delay_timercomp->TIMER_Stop(parameter->timerbaseaddress);
      Delay_timercomp->TIMER_Clear(parameter->timerbaseaddress);
      Delay_timercomp->TIMER_Start(true,parameter->timerbaseaddress,sec,divide);
     parameter->step=10;

     return(false);

   case 10:

      if (Delay_timercomp->TIMER_Start(false,parameter->timerbaseaddress,sec,divide)==true)
      {

        parameter->step=20;

        return(true);

      }
      return(false);
   case 20:

        return(true);


     }

 }


uint32_t _Delay_ParameterInitial(enum_Timer_BaseAddress timerbaseaddress)
{
     ListStruct_uint8 tempdata;
    _Delay_start(timerbaseaddress);

    tempdata.value=(uint32_t)malloc(sizeof(DelayObjectParameter));
    ((DelayObjectParameter*)(tempdata.value))->timerbaseaddress=timerbaseaddress;

    return(Delay_listcomp->Insert(&delaylist,&tempdata));

}

  void CreateDelayobject(DelayObject* delayobject,
                         ListCollection* listcomp,
                         TIMERDevice* timercomp)
  {

     delayobject->Delay_Routine=_Delay_Routine;
     delayobject->ParameterInitial=_Delay_ParameterInitial;
     Delay_listcomp=listcomp;
     Delay_timercomp=timercomp;

  }
