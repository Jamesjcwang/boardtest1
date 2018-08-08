
#include <stdbool.h>
#include <stdint.h>
#include "library\Physic\Header\I_Timer.h"
#include "library\Component\Header\I_DataStruct.h"

#ifndef __I_Delayobject_h__
#define __I_Delayobject_h__
typedef struct
{
    enum_Timer_BaseAddress timerbaseaddress;
    uint32_t step;

} DelayObjectParameter;



typedef struct {

  bool (* Delay_Routine)(bool reset,uint32_t sec,uint32_t divide,
                         uint32_t paraindex);
  uint32_t(*ParameterInitial)(enum_Timer_BaseAddress timerbaseaddress);

} DelayObject;

ListStruct_uint8 delaylist;
ListCollection* Delay_listcomp;
TIMERDevice* Delay_timercomp;

extern void CreateDelayobject(DelayObject* delayobject,
                              ListCollection* listcomp,
                              TIMERDevice* timercomp);


#endif

