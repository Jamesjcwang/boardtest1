
#ifndef NRF_POWER_H__
#define NRF_POWER_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "library\Physic\Header\I_Power.h"


static void(*_power_raiseevent)(PowerMISenum eventkind);


static void _Power_Sleep(PowerBaseAddress port)
 {
   *((volatile uint32_t*)(0x40000500))=1UL;
 }
static void _Power_IntConfigure(PowerBaseAddress port, PowerMISenum interrupt,
                        bool enable,uint32_t priority,bool controllerenable)
  {
      Power_intcontroller->Configure(POWER_CLOCK_IRQn,controllerenable,
                                        priority);



     if (enable)
     {
        *(uint32_t*)(0x40000304)=0x64UL;
     }
     else
     {
     ((Powerstruc*)(port))->INTENCLR=interrupt;

     }




  }
static void _Power_IntClear(PowerBaseAddress port, PowerMISenum interrupt)
  {
     if (interrupt==POFWARN)
     {
      ((Powerstruc*)(port))->EVENTS_POFWARN=0UL;

     }
     if (interrupt==SLEEPENTER)
     {
      ((Powerstruc*)(port))->EVENTS_SLEEPENTER=0UL;

     }
     if (interrupt==SLEEPEXIT)
     {
      ((Powerstruc*)(port))->EVENTS_SLEEPEXIT=0UL;

     }

  }

static uint32_t _Power_IntStatus(PowerBaseAddress port,PowerMISenum interrupt)
 {
     if (interrupt==POFWARN)
     {
       return( ((Powerstruc*)(port))->EVENTS_POFWARN);

     }
     if (interrupt==SLEEPENTER)
     {
   return( ((Powerstruc*)(port))->EVENTS_SLEEPENTER);

     }
     if (interrupt==SLEEPEXIT)
     {

      return(((Powerstruc*)(port))->EVENTS_SLEEPEXIT);

     }
 }


static void _Power_IntHandler(PowerBaseAddress port, void(*Handler)(PowerMISenum eventkind) )
  {
     _power_raiseevent=Handler;
  }

static void Power_handler(void)
  {
         if (_Power_IntStatus(POWER,POFWARN))
      {

          _power_raiseevent(POFWARN)  ;
         // SEGGER_RTT_printf(0,"Times %d\r\n",2300);
           _Power_IntClear(POWER,POFWARN);
      }
          if (_Power_IntStatus(POWER,SLEEPENTER))
      {

          _power_raiseevent(SLEEPENTER)  ;
        //  SEGGER_RTT_printf(0,"Times %d\r\n",2300);
           _Power_IntClear(POWER,SLEEPENTER);
      }
          if (_Power_IntStatus(POWER,SLEEPEXIT))
      {

          _power_raiseevent(SLEEPEXIT)  ;
        //  SEGGER_RTT_printf(0,"Times %d\r\n",2300);
           _Power_IntClear(POWER,SLEEPEXIT);
      }


  }

  void _get_deviceid(uint32_t* lsbbits,uint32_t* msbbits)
  {
     volatile uint32_t _templsbbits,_tempmsbbits;
     _templsbbits=*((uint32_t*)0x10000060);
     _tempmsbbits=*((uint32_t*)0x10000064);

     *lsbbits=_templsbbits;
     *msbbits=_tempmsbbits;

  }

void CreatePowerDevice(PowerDevice* device, InterruptController* intcontroller)
{
  device->Power_Sleep=_Power_Sleep;
  device->Power_IntClear=_Power_IntClear;
  device->Power_IntConfigure=_Power_IntConfigure;
  device->Power_IntHandler=_Power_IntHandler;
  device->Power_IntStatus=_Power_IntStatus;
  device->Get_DeviceID=_get_deviceid;
  Power_intcontroller=intcontroller;
  Power_intcontroller->IntHandler(POWER_CLOCK_IRQn,Power_handler);

}







#endif // NRF_POWER_H__
