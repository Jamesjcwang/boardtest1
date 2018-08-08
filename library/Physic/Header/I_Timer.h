
#include <stdbool.h>
#include <stdint.h>
#include "library\Physic\Header\I_Int.h"
#include "library\nrf.h"

#ifndef __I_TIMER_h__
#define __I_TIMER_h__
typedef struct {                                    /*!< TIMER Structure                                                       */
  __O  uint32_t  TASKS_START;                       /*!< Start Timer.                                                          */
  __O  uint32_t  TASKS_STOP;                        /*!< Stop Timer.                                                           */
  __O  uint32_t  TASKS_COUNT;                       /*!< Increment Timer (In counter mode).                                    */
  __O  uint32_t  TASKS_CLEAR;                       /*!< Clear timer.                                                          */
  __O  uint32_t  TASKS_SHUTDOWN;                    /*!< Shutdown timer.                                                       */
  __I  uint32_t  RESERVED0[11];
  __O  uint32_t  TASKS_CAPTURE[4];                  /*!< Capture Timer value to CC[n] registers.                               */
  __I  uint32_t  RESERVED1[60];
  __IO uint32_t  EVENTS_COMPARE[4];                 /*!< Compare event on CC[n] match.                                         */
  __I  uint32_t  RESERVED2[44];
  __IO uint32_t  SHORTS;                            /*!< Shortcuts for Timer.                                                  */
  __I  uint32_t  RESERVED3[64];
  __IO uint32_t  INTENSET;                          /*!< Interrupt enable set register.                                        */
  __IO uint32_t  INTENCLR;                          /*!< Interrupt enable clear register.                                      */
  __I  uint32_t  RESERVED4[126];
  __IO uint32_t  MODE;                              /*!< Timer Mode selection.                                                 */
  __IO uint32_t  BITMODE;                           /*!< Sets timer behaviour.                                                 */
  __I  uint32_t  RESERVED5;
  __IO uint32_t  PRESCALER;                         /*!< 4-bit prescaler to source clock frequency (max value 9). Source
                                                         clock frequency is divided by 2^SCALE.                                */
  __I  uint32_t  RESERVED6[11];
  __IO uint32_t  CC[4];                             /*!< Capture/compare registers.                                            */
  __I  uint32_t  RESERVED7[683];
  __IO uint32_t  POWER;                             /*!< Peripheral power control.                                             */
} NRF_TIMER_Type;
typedef enum
{
    TIMER0=0x40008000,
    TIMER1=0x40009000,
    TIMER2=0x4000A000,
    TIMER3=0x4001A000,
    TIMER4=0x4001B000

} enum_Timer_BaseAddress;

typedef enum
{
    index_Timer0,
    index_Timer1,
    index_Timer2,
    index_Timer3,
    index_Timer4,
    Timerindex_total

} enum_Timerindex;


typedef enum
{
    Timer_COMPARE0=0x00010000,
    Timer_COMPARE1=0x00020000,
    Timer_COMPARE2=0x00040000,
    Timer_COMPARE3=0x00080000,
    Timer_COMPARE4=0x00100000,
    Timer_COMPARE5=0x00200000



} enum_Timer_Mis;

typedef enum
{
   Timer_16bit=0,
   Timer_8bit=1,
   Timer_24bit=2,
   Timer_32bit=3,

} enum_Timer_Bitmode;

typedef enum
{
    Timer_FREQ_16MHz  ,
    Timer_FREQ_8MHz   ,
    Timer_FREQ_4MHz   ,
    Timer_FREQ_2MHz   ,
    Timer_FREQ_1MHz   ,
    Timer_FREQ_500kHz ,
    Timer_FREQ_250kHz ,
    Timer_FREQ_125kHz ,
    Timer_FREQ_62500Hz,
    Timer_FREQ_31250Hz

} enum_Timer_Prescaler;



typedef struct {


 bool (*TIMER_Start)(bool reset,enum_Timer_BaseAddress port,
                     uint32_t period,uint32_t divide);

 void (*TIMER_Stop)(enum_Timer_BaseAddress port);

 void (*TIMER_Clear)(enum_Timer_BaseAddress port);


 void (*TIMER_IntConfigure)(enum_Timer_BaseAddress port, enum_Timer_Mis interrupt,
                           bool enable,uint32_t priority,bool controllerenable);

 void (*TIMER_IntClear)(enum_Timer_BaseAddress port,enum_Timer_Mis interrupt);

 uint32_t (*TIMER_IntStatus)(enum_Timer_BaseAddress port,enum_Timer_Mis interrupt);

} TIMERDevice;

InterruptController* Timer_intcontroller;
uint32_t Timer_frequency;
extern void CreateTimerDevice(TIMERDevice* device,
                              InterruptController* intcontroller,
                              enum_Timer_BaseAddress port,
                              enum_Timer_Bitmode bitmode,
                              enum_Timer_Prescaler prescaler
                              );



#endif
