
/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

 /**
 * @defgroup nrf_twi_hal TWI HAL
 * @{
 * @ingroup nrf_twi_master
 *
 * @brief Hardware access layer for the two-wire interface (TWI) peripheral.
 */
#ifndef NRF_TWI_H__
#define NRF_TWI_H__

#include "nrf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "library\Physic\Header\I_I2C.h"


/**
  * @brief Two-wire interface master 0. (TWI)
  */
volatile static bool isi2cbusy[1];


static uint8_t _step_i2c[I2Cindex_total];
static uint8_t _step1_i2c[I2Cindex_total];
static bool _f_txdsent[I2Cindex_total];
static bool _f_error[I2Cindex_total];
static bool _f_rxdready[I2Cindex_total];

static bool _f_stopped[I2Cindex_total];
/**
 * @enum nrf_twi_tasks_t
 * @brief TWI tasks.
 */
typedef enum
{
    /*lint -save -e30 -esym(628,__INTADDR__)*/
    NRF_TWI_TASKS_STARTRX    = offsetof(NRF_TWI_Type, TASKS_STARTRX), /**< Start receive sequence. */
    NRF_TWI_TASKS_STARTTX    = offsetof(NRF_TWI_Type, TASKS_STARTTX), /**< Start transmit sequence. */
    NRF_TWI_TASKS_STOP       = offsetof(NRF_TWI_Type, TASKS_STOP),    /**< Stop transaction. */
    NRF_TWI_TASKS_SUSPEND    = offsetof(NRF_TWI_Type, TASKS_SUSPEND), /**< Suspend transaction. */
    NRF_TWI_TASKS_RESUME     = offsetof(NRF_TWI_Type, TASKS_RESUME)   /**< Resume transaction. */
    /*lint -restore*/
} nrf_twi_tasks_t;

/**
 * @enum nrf_twi_events_t
 * @brief TWI events.
 */
typedef enum
{
    /*lint -save -e30*/
    NRF_TWI_EVENTS_STOPPED   = offsetof(NRF_TWI_Type, EVENTS_STOPPED),  /**< Stopped. */
    NRF_TWI_EVENTS_RXDREADY  = offsetof(NRF_TWI_Type, EVENTS_RXDREADY), /**< RXD byte received. */
    NRF_TWI_EVENTS_TXDSENT   = offsetof(NRF_TWI_Type, EVENTS_TXDSENT),  /**< TXD byte sent. */
    NRF_TWI_EVENTS_ERROR     = offsetof(NRF_TWI_Type, EVENTS_ERROR),    /**< Error. */
    NRF_TWI_EVENTS_BB        = offsetof(NRF_TWI_Type, EVENTS_BB),       /**< Byte boundary, generated before each byte that is sent or received. */
    NRF_TWI_EVENTS_SUSPENDED = offsetof(NRF_TWI_Type, EVENTS_SUSPENDED) /**< Entered the suspended state. */
    /*lint -restore*/
} nrf_twi_events_t;

/**
 * @enum nrf_twi_int_mask_t
 * @brief TWI interrupts.
 */
typedef enum
{
    NRF_TWI_INT_SUSPENDED_MASK  = TWI_INTENSET_SUSPENDED_Msk,   /**< TWI interrupt on suspend event. */
    NRF_TWI_INT_BB_MASK         = TWI_INTENSET_BB_Msk,          /**< TWI interrupt on byte boundary event. */
    NRF_TWI_INT_ERROR_MASK      = TWI_INTENSET_ERROR_Msk,       /**< TWI interrupt on error event. */
    NRF_TWI_INT_TXDSENT_MASK    = TWI_INTENSET_TXDSENT_Msk,     /**< TWI interrupt on txdsent event. */
    NRF_TWI_INT_RXDREADY_MASK   = TWI_INTENSET_RXDREADY_Msk,    /**< TWI interrupt on rxdready event. */
    NRF_TWI_INT_STOPPED_MASK    = TWI_INTENSET_STOPPED_Msk,     /**< TWI interrupt on stopped event. */
} nrf_twi_int_mask_t;

/**
 * @enum nrf_twi_shorts_mask_t
 * @brief Types of TWI shortcuts.
 */
typedef enum
{
    NRF_TWI_SHORTS_BB_SUSPEND_MASK = TWI_SHORTS_BB_SUSPEND_Msk, /**< Shortcut between bb event and suspend task. */
    NRF_TWI_SHORTS_BB_STOP_MASK    = TWI_SHORTS_BB_STOP_Msk,    /**< Shortcut between bb event and stop task. */
} nrf_twi_shorts_mask_t;

/**
 * @enum nrf_twi_frequency_t
 * @brief TWI master clock frequency.
 */
typedef enum
{
    NRF_TWI_FREQ_100K = TWI_FREQUENCY_FREQUENCY_K100, /**< 100 kbps. */
    NRF_TWI_FREQ_250K = TWI_FREQUENCY_FREQUENCY_K250, /**< 250 kbps. */
    NRF_TWI_FREQ_400K = TWI_FREQUENCY_FREQUENCY_K400  /**< 400 kbps. */
} nrf_twi_frequency_t;

/**
 * @enum nrf_twi_error_t
 * @brief TWI error source.
 */
typedef enum
{
    NRF_TWI_ERROR_ADDRESS_NACK = TWI_ERRORSRC_ANACK_Msk,    /**< NACK received after sending the address. */
    NRF_TWI_ERROR_OVERRUN_NACK = TWI_ERRORSRC_OVERRUN_Msk,  /**< Byte received in RXD register before read of the last received byte (data loss). */
    NRF_TWI_ERROR_DATA_NACK    = TWI_ERRORSRC_DNACK_Msk,    /**< NACK received after sending a data byte. */
} nrf_twi_error_t;

/**
 * @brief Function for activating a specific TWI task.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] task  Task.
 */
static void nrf_twi_task_set(NRF_TWI_Type * p_twi, nrf_twi_tasks_t task);

/**
 * @brief Function for returning the address of a specific TWI task register.
 *
 * @param[in]  p_twi TWI instance.
 * @param[in]  task  Task.
 *
 * @return Task address.
 */
__STATIC_INLINE uint32_t * nrf_twi_task_address_get(NRF_TWI_Type * p_twi,
                                                    nrf_twi_tasks_t task);

/**
 * @brief Function for clearing a specific event.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] event Event.
 */
__STATIC_INLINE void nrf_twi_event_clear(NRF_TWI_Type  * p_twi,
                                         nrf_twi_events_t event);
/**
 * @brief Function for returning the state of a specific event.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] event Event.
 *
 * @retval true If the event is set.
 * @retval false If the event is not set.
 */
__STATIC_INLINE bool nrf_twi_event_check(NRF_TWI_Type  * p_twi,
                                         nrf_twi_events_t event);

/**
 * @brief Function for returning the address of a specific TWI event register.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] event Event.
 *
 * @return Address.
 */
__STATIC_INLINE uint32_t * nrf_twi_event_address_get(NRF_TWI_Type  * p_twi,
                                                     nrf_twi_events_t event);

/**
 * @brief Function for setting a shortcut.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] short_mask Shortcuts mask.
 */


/**
 * @brief Function for enabling a specific interrupt.
 *
 * @param[in] p_twi    TWI instance.
 * @param[in] int_mask Interrupts mask.
 */
__STATIC_INLINE void nrf_twi_int_enable(NRF_TWI_Type * p_twi, uint32_t int_mask);

/**
 * @brief Function for retrieving the state of a given interrupt.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] int_mask  Interrupts mask.
 *
 * @retval true If the interrupts are enabled.
 * @retval false If the interrupts are not enabled.
 */
__STATIC_INLINE bool nrf_twi_int_enable_check(NRF_TWI_Type * p_twi, uint32_t int_mask);

/**
 * @brief Function for disabling a specific interrupt.
 *
 * @param[in] p_twi     TWI instance.
 * @param[in] int_mask  Interrupts mask.
 */
__STATIC_INLINE void nrf_twi_int_disable(NRF_TWI_Type * p_twi, uint32_t int_mask);

/**
 * @brief Function for setting the TWI master clock frequency.
 *
 * @param[in] p_twi           TWI instance.
 * @param[in] frequency       TWI frequency value.
 */
__STATIC_INLINE void nrf_twi_frequency_set(NRF_TWI_Type      * p_twi,
                                           nrf_twi_frequency_t frequency);

/**
 * @brief Function for retrieving the TWI frequency.
 *
 * @param[in] p_twi TWI instance.
 *
 * @return Frequency.
 */
__STATIC_INLINE nrf_twi_frequency_t nrf_twi_frequency_get(NRF_TWI_Type * p_twi);

/**
 * @brief Function for retrieving the TWI error source.
 * @details Error sources are cleared after read.
 *
 * @param[in] p_twi TWI instance.
 *
 * @return Error source mask.
 */
__STATIC_INLINE uint32_t nrf_twi_error_source_get(NRF_TWI_Type * p_twi);

/**
 * @brief Function for enabling TWI.
 *
 * @param[in] p_twi TWI instance.
 */
__STATIC_INLINE void nrf_twi_enable(NRF_TWI_Type * p_twi);

/**
 * @brief Function for disabling TWI.
 *
 * @param[in] p_twi TWI instance.
 */


/**
 * @brief Function for configuring TWI pins.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] scl   SCL pin number.
 * @param[in] sda   SDA pin number.
 */
__STATIC_INLINE void nrf_twi_pins_set(NRF_TWI_Type * p_twi, uint32_t scl, uint32_t sda);

/**
 * @brief Function for reading RX data from TWI.
 *
 * @param[in] p_twi TWI instance.
 *
 * @return RX data.
 */
static uint8_t nrf_twi_rxd_get(NRF_TWI_Type * p_twi);
/**
 * @brief Function for writing data to TWI.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] data  Data to be transmitted.
 */
__STATIC_INLINE void nrf_twi_txd_set(NRF_TWI_Type * p_twi, uint8_t data);

/**
 * @brief Function for setting the slave address.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] addr  Address of next transaction.
 */
__STATIC_INLINE void nrf_twi_address_set(NRF_TWI_Type * p_twi, uint8_t addr);

/**
 *@}
 **/


#ifndef SUPPRESS_INLINE_IMPLEMENTATION

static void nrf_twi_task_set(NRF_TWI_Type * p_twi, nrf_twi_tasks_t task)
{
    *((volatile uint32_t *)((uint8_t *)p_twi + (uint32_t)task)) = 0x1UL;
}

__STATIC_INLINE uint32_t * nrf_twi_task_address_get(NRF_TWI_Type * p_twi,
                                                    nrf_twi_tasks_t task)
{
    return (uint32_t *)((uint8_t *)p_twi + (uint32_t)task);
}

__STATIC_INLINE void nrf_twi_event_clear(NRF_TWI_Type  * p_twi,
                                         nrf_twi_events_t event)
{
    *((volatile uint32_t *)((uint8_t *)p_twi + (uint32_t)event)) = 0x0UL;
}

__STATIC_INLINE bool nrf_twi_event_check(NRF_TWI_Type  * p_twi,
                                         nrf_twi_events_t event)
{
    return (bool)*(volatile uint32_t *)((uint8_t *)p_twi + (uint32_t)event);
}

__STATIC_INLINE uint32_t * nrf_twi_event_address_get(NRF_TWI_Type  * p_twi,
                                                       nrf_twi_events_t event)
{
    return (uint32_t *)((uint8_t *)p_twi + (uint32_t)event);
}

 void nrf_twi_shorts_set(NRF_TWI_Type * p_twi, uint32_t short_mask)
{
    p_twi->SHORTS |= short_mask;
}

 void nrf_twi_shorts_clear(NRF_TWI_Type * p_twi, uint32_t short_mask)
{
    p_twi->SHORTS &= ~(short_mask);
}

__STATIC_INLINE void nrf_twi_int_enable(NRF_TWI_Type * p_twi, uint32_t int_mask)
{
    p_twi->INTENSET = int_mask;
}

__STATIC_INLINE bool nrf_twi_int_enable_check(NRF_TWI_Type * p_twi, uint32_t int_mask)
{
    return (bool)(p_twi->INTENSET & int_mask);
}

__STATIC_INLINE void nrf_twi_int_disable(NRF_TWI_Type * p_twi, uint32_t int_mask)
{
    p_twi->INTENCLR = int_mask;
}

__STATIC_INLINE void nrf_twi_frequency_set(NRF_TWI_Type      * p_twi,
                                           nrf_twi_frequency_t frequency)
{
    p_twi->FREQUENCY = frequency;
}

__STATIC_INLINE nrf_twi_frequency_t nrf_twi_frequency_get(NRF_TWI_Type * p_twi)
{
    return (nrf_twi_frequency_t)(p_twi->FREQUENCY);
}

__STATIC_INLINE uint32_t nrf_twi_error_source_get(NRF_TWI_Type * p_twi)
{
    uint32_t error_source = p_twi->ERRORSRC;

    p_twi->ERRORSRC = error_source;
    return error_source;
}

 void nrf_twi_enable(NRF_TWI_Type * p_twi)
{
    p_twi->ENABLE = (TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos);
}
void nrf_twi_disable(NRF_TWI_Type * p_twi)
{
    p_twi->ENABLE = (TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos);
}

__STATIC_INLINE void nrf_twi_pins_set(NRF_TWI_Type * p_twi, uint32_t scl, uint32_t sda)
{
    p_twi->PSELSCL = scl;
    p_twi->PSELSDA = sda;
}

 void nrf_twi_pins_get(NRF_TWI_Type * p_twi, uint32_t *scl, uint32_t* sda)
{
      *scl=p_twi->PSELSCL;
      *sda=p_twi->PSELSDA;
}


static uint8_t nrf_twi_rxd_get(NRF_TWI_Type * p_twi)
{
    return (uint8_t)p_twi->RXD;
}

__STATIC_INLINE void nrf_twi_txd_set(NRF_TWI_Type * p_twi, uint8_t data)
{
    p_twi->TXD = data;
}

__STATIC_INLINE void nrf_twi_address_set(NRF_TWI_Type * p_twi, uint8_t addr)
{
    p_twi->ADDRESS = addr;
}

 void nrf_twi_address_get(NRF_TWI_Type * p_twi, uint8_t* addr)
{
     *addr=p_twi->ADDRESS;
}




void _I2C_Configure(I2CBaseAddress port , EnumI2CFrequency frequency,
                    PinAndPinset sclpin,PinAndPinset sdapin)
    {
        volatile uint32_t tempi,_tempj;

      nrf_twi_pins_set((NRF_TWI_Type*)port,sclpin,sdapin);
      nrf_twi_frequency_set((NRF_TWI_Type*)port,frequency);

      nrf_twi_enable((NRF_TWIS_Type*)port);

    }

bool _I2C_StartTXRX(I2CBaseAddress port ,uint8_t address,bool isTX,bool enable)
{
    volatile uint8_t _tempi;


    nrf_twi_address_set((NRF_TWI_Type*) port,address);

    for (_tempi=0;_tempi<100;_tempi++)
    {

    }


    if (enable)
    {


        if (isTX)
       {
        nrf_twi_task_set((NRF_TWI_Type*) port,NRF_TWI_TASKS_STARTTX);
        nrf_twi_shorts_set((NRF_TWI_Type*) port,0x0UL);
       }
       else
       {
        nrf_twi_shorts_set((NRF_TWI_Type*) port,0x1UL);
        nrf_twi_task_set((NRF_TWI_Type*) port,NRF_TWI_TASKS_STARTRX);
       }

    }
    else
    {
       nrf_twi_task_set((NRF_TWI_Type*) port,NRF_TWI_TASKS_STOP);

    }
   return(1);

}


 bool* getferrori2c(I2CBaseAddress port)
 {
    switch (port)
     {
      case I2C0_BASE:

       return(&_f_error[I2C0]);

      case I2C1_BASE:

       return(&_f_error[I2C1]);

     }
     return(0);
 }



 bool* getfrxdi2c(I2CBaseAddress port)
 {
    switch (port)
     {
      case I2C0_BASE:

       return(&_f_rxdready[I2C0]);

      case I2C1_BASE:

       return(&_f_rxdready[I2C1]);

     }
     return(0);
 }


 bool* getfsenti2c(I2CBaseAddress port)
 {
    switch (port)
     {
      case I2C0_BASE:

       return(&_f_txdsent[I2C0]);

      case I2C1_BASE:

       return(&_f_txdsent[I2C1]);

     }
     return(0);
 }

  bool* getfstopi2c(I2CBaseAddress port)
 {
    switch (port)
     {
      case I2C0_BASE:

       return(&_f_stopped[I2C0]);

      case I2C1_BASE:

       return(&_f_stopped[I2C1]);

     }
     return(0);
 }


 uint8_t* getstepi2c(I2CBaseAddress port)
 {
    switch (port)
      {
      case I2C0_BASE:

       return(&_step_i2c[I2C0]);

      case I2C1_BASE:

       return(&_step_i2c[I2C1]);

      }
     return(0);
 }

  uint8_t* getstepi2c_1(I2CBaseAddress port)
 {
    switch (port)
      {
      case I2C0_BASE:

       return(&_step1_i2c[I2C0]);

      case I2C1_BASE:

       return(&_step1_i2c[I2C1]);

      }
     return(0);
 }



enum_i2cresult _I2C_Send(enum_i2coperation operation,
                            I2CBaseAddress port,uint8_t address,uint8_t *data)
  {
    if (operation==open)
     {
      *getferrori2c(port)=false;
      *getfsenti2c(port)=false;
      *getfstopi2c(port)=false;
      *getstepi2c(port)=0;
        *data=nrf_twi_rxd_get((NRF_TWI_Type*)port);
      SEGGER_RTT_printf(0,"i2c %d\r\n",101);

      _I2C_StartTXRX(port,address,true,true);

      return(i2c_null);
     }
    else if (operation==close)
    {
        SEGGER_RTT_printf(0,"i2c %d\r\n",102);
      _I2C_StartTXRX(port,address,true,false);
      *getstepi2c(port)=100;
      return(i2c_null);


    }

 switch (*getstepi2c(port))
  {
    case 0:
SEGGER_RTT_printf(0,"i2c %d\r\n",0);
      nrf_twi_txd_set((NRF_TWI_Type*) port,*data);
SEGGER_RTT_printf(0,"i2c %d\r\n",*data);
SEGGER_RTT_printf(0,"i2c %d\r\n",15);
      *getstepi2c(port)=10;
      return (i2c_null);
    case 10:
       // SEGGER_RTT_printf(0,"i2c %d\r\n",9);

        if (*getfsenti2c(port)==true)
        {
          *getstepi2c(port)=20;
          SEGGER_RTT_printf(0,"i2c %d\r\n",10);
          return(i2c_null);

        }
        else if (*getferrori2c(port)==true)
        {  *getferrori2c(port)=false;
           *getstepi2c(port)=1000;
          SEGGER_RTT_printf(0,"i2c %d\r\n",11);

           return (i2c_fail);
        }
        return (i2c_null);

    case 20:
        SEGGER_RTT_printf(0,"i2c %d\r\n",20);
        *getfsenti2c(port)=false;
        nrf_twi_rxd_get((NRF_TWI_Type*) port);
        *getstepi2c(port)=0;
        return(i2c_success);
    case 100:
        if (*getfstopi2c(port)==true)
        {   *getfstopi2c(port)=false;
            *getstepi2c(port)=1000;
             SEGGER_RTT_printf(0,"i2c %d\r\n",101);
            return (i2c_stop);

        }
        return (i2c_null);

    case 1000:
        return (i2c_fail);


    }
  }





enum_i2cresult _I2C_Receive(enum_i2coperation operation,
                            I2CBaseAddress port,uint8_t address,uint8_t *data)
  {


       if (operation==open)
     {
      *getferrori2c(port)=false;
      *getfstopi2c(port)=false;
      *getfrxdi2c(port)=false;
      *getstepi2c_1(port)=0;
       SEGGER_RTT_printf(0,"i2cR %d\r\n",200);
         *data=nrf_twi_rxd_get((NRF_TWI_Type*)port);
      _I2C_StartTXRX(port,address,false,true);

      return(i2c_null);
     }
    else if (operation==close)
    {
         SEGGER_RTT_printf(0,"i2cR %d\r\n",100);
        nrf_twi_shorts_set((NRF_TWI_Type*)port,0x2UL);
       _I2C_StartTXRX(port,address,false,false);
      *getstepi2c_1(port)=100;
      return(i2c_null);


    }

   switch (*getstepi2c_1(port))

   {
   case 0:
       if (*getfrxdi2c(port)==true)
       {
            SEGGER_RTT_printf(0,"i2cR %d\r\n",0);
         *getfrxdi2c(port)=false;
         *data=nrf_twi_rxd_get((NRF_TWI_Type*) port);
         nrf_twi_task_set((NRF_TWI_Type*) port,NRF_TWI_TASKS_RESUME);
         return (i2c_success);
       }
       else if (*getferrori2c(port)==true)
       {
 SEGGER_RTT_printf(0,"i2cR %d\r\n",1);
        *getferrori2c(port)=false;
        return (i2c_fail);
       }
         return (i2c_null);

   case 100:
      if (*getfrxdi2c(port)==true)
       {
            SEGGER_RTT_printf(0,"i2cR %d\r\n",10);
         *getfrxdi2c(port)=false;
         *data=nrf_twi_rxd_get((NRF_TWI_Type*) port);
         nrf_twi_task_set((NRF_TWI_Type*) port,NRF_TWI_TASKS_RESUME);
         *getstepi2c_1(port)=110;
         return (i2c_success);
       }
       else if (*getferrori2c(port)==true)
       {
 SEGGER_RTT_printf(0,"i2cR %d\r\n",11);
        *getferrori2c(port)=false;
        return (i2c_fail);
       }
         return (i2c_null);


   case 110:

      if (*getfstopi2c(port)==true)
      {
           SEGGER_RTT_printf(0,"i2cR %d\r\n",12);
          nrf_twi_shorts_clear((NRF_TWI_Type*)port,0x2UL);
          *data=nrf_twi_rxd_get((NRF_TWI_Type*)port);
           //  nrf_twi_task_set((NRF_TWI_Type*) port,NRF_TWI_TASKS_RESUME);
           nrf_twi_shorts_clear((NRF_TWI_Type*)port,0x1UL);
          // nrf_twi_disable((NRF_TWI_Type*)port);
          *getfstopi2c(port)=false;
          *getstepi2c_1(port)=0;
          return(i2c_stop);
      }
      else if (*getferrori2c(port)==true)
      {
           SEGGER_RTT_printf(0,"i2cR %d\r\n",13);
       *getstepi2c_1(port)=0;
       *getferrori2c(port)=false;
       return (i2c_fail);
      }
      return(i2c_null);


   }


  }

 void _I2C_IntConfigure(I2CBaseAddress port, I2CMISenum interrupt,
                           bool enable,uint32_t priority,bool controllerenable)
  {
      _I2C_IntClear(I2C0_BASE,EVENTS_BB|EVENTS_ERROR|EVENTS_RXDREADY|EVENTS_STOPPED|
                    EVENTS_SUSPENDED|EVENTS_TXDSENT);

      I2C_intcontroller->Configure(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn,controllerenable,priority);



     if (enable)
      {
          nrf_twi_int_enable((NRF_TWI_Type*) port,interrupt);
      }
      else
      {
         nrf_twi_int_disable((NRF_TWI_Type*) port,interrupt);
       }



  }
 void _I2C_IntClear(I2CBaseAddress port, I2CMISenum interrupt)
  {
      switch(interrupt)
    {

  case EVENTS_BB:

       nrf_twi_event_clear((NRF_TWI_Type*) port,NRF_TWI_EVENTS_BB);
        break;

    case EVENTS_ERROR:

       nrf_twi_event_clear((NRF_TWI_Type*) port,NRF_TWI_EVENTS_ERROR);
        break;

    case EVENTS_RXDREADY:

       nrf_twi_event_clear((NRF_TWI_Type*) port,NRF_TWI_EVENTS_RXDREADY);
        break;

    case EVENTS_STOPPED:

       nrf_twi_event_clear((NRF_TWI_Type*) port,NRF_TWI_EVENTS_STOPPED);
        break;

    case EVENTS_SUSPENDED:

       nrf_twi_event_clear((NRF_TWI_Type*) port,NRF_TWI_EVENTS_SUSPENDED);
        break;
    case EVENTS_TXDSENT:


       nrf_twi_event_clear((NRF_TWI_Type*) port,NRF_TWI_EVENTS_TXDSENT);
        break;
    }
  }
 uint32_t _I2C_IntStatus(I2CBaseAddress port,I2CMISenum interrupt)
  {
      switch(interrupt)
     {

  case EVENTS_BB:

      return( nrf_twi_event_check((NRF_TWI_Type*) port,NRF_TWI_EVENTS_BB));
         break;

    case EVENTS_ERROR:

      return( nrf_twi_event_check((NRF_TWI_Type*) port,NRF_TWI_EVENTS_ERROR));
       break;

    case EVENTS_RXDREADY:

      return( nrf_twi_event_check((NRF_TWI_Type*) port,NRF_TWI_EVENTS_RXDREADY));
       break;

    case EVENTS_STOPPED:

       return(nrf_twi_event_check((NRF_TWI_Type*) port,NRF_TWI_EVENTS_STOPPED));
        break;

    case EVENTS_SUSPENDED:

      return( nrf_twi_event_check((NRF_TWI_Type*) port,NRF_TWI_EVENTS_SUSPENDED));
       break;
    case EVENTS_TXDSENT:


      return( nrf_twi_event_check((NRF_TWI_Type*) port,NRF_TWI_EVENTS_TXDSENT));
       break;
    }
  }

  void I2C0Handler(void)
  {volatile uint32_t j;

    if (_I2C_IntStatus(I2C0_BASE,EVENTS_BB))
      {
   SEGGER_RTT_printf(0,"i2c %d\r\n",1);
         _I2C_IntClear(I2C0_BASE,EVENTS_BB);

      }


    if (_I2C_IntStatus(I2C0_BASE,EVENTS_ERROR))
      {
             SEGGER_RTT_printf(0,"i2c %d\r\n",2);
          _f_error[I2C0]=true;
          _I2C_IntClear(I2C0_BASE,EVENTS_ERROR);



      }

    if (_I2C_IntStatus(I2C0_BASE,EVENTS_RXDREADY))
      {

            SEGGER_RTT_printf(0,"i2c %d\r\n",3);
         *getfrxdi2c(I2C0_BASE)=true;

          _I2C_IntClear(I2C0_BASE,EVENTS_RXDREADY);


          //tempdata.value=  nrf_twi_rxd_get((NRF_TWI_Type*) I2C0_BASE);
          //I2C_listcomp->Insert(&_listdata_i2c[I2C0],&tempdata);

         // nrf_twi_task_set((NRF_TWI_Type*) port,NRF_TWI_TASKS_RESUME);
      }


    if (_I2C_IntStatus(I2C0_BASE,EVENTS_STOPPED))
      { SEGGER_RTT_printf(0,"i2c %d\r\n",41);
          _f_stopped[I2C0]=true;
          _I2C_IntClear(I2C0_BASE,EVENTS_STOPPED);

      }


    if (_I2C_IntStatus(I2C0_BASE,EVENTS_SUSPENDED))
      {
   SEGGER_RTT_printf(0,"i2c %d\r\n",4);
          _I2C_IntClear(I2C0_BASE,EVENTS_SUSPENDED);

      }

    if (_I2C_IntStatus(I2C0_BASE,EVENTS_TXDSENT))
      {
             SEGGER_RTT_printf(0,"i2c %d\r\n",5);
          _f_txdsent[I2C0]=true;
          _I2C_IntClear(I2C0_BASE,EVENTS_TXDSENT);

      }



  }
    void I2C1Handler(void)
  {
    if (_I2C_IntStatus(I2C1_BASE,EVENTS_BB))
      {
            _I2C_IntClear(I2C1_BASE,EVENTS_BB);

      }

    if (_I2C_IntStatus(I2C1_BASE,EVENTS_ERROR))
      {
         _f_error[I2C1]=true;
         _I2C_IntClear(I2C1_BASE,EVENTS_ERROR);

      }

    if (_I2C_IntStatus(I2C1_BASE,EVENTS_RXDREADY))
      {
         *getfrxdi2c(I2C1_BASE)=true;
         _I2C_IntClear(I2C1_BASE,EVENTS_RXDREADY);

      }


    if (_I2C_IntStatus(I2C1_BASE,EVENTS_STOPPED))
      {
         _f_stopped[I2C1]=true;
         _I2C_IntClear(I2C1_BASE,EVENTS_STOPPED);

      }


        if (_I2C_IntStatus(I2C1_BASE,EVENTS_SUSPENDED))
      {

          _I2C_IntClear(I2C1_BASE,EVENTS_SUSPENDED);

      }

        if (_I2C_IntStatus(I2C1_BASE,EVENTS_TXDSENT))
      {
          _f_txdsent[I2C1]=true;
          _I2C_IntClear(I2C1_BASE,EVENTS_TXDSENT);


      }
  }

void CreateI2CDevice(I2CDevice* device, InterruptController* intcontroller,
                            EnumI2CFrequency frequency,
                            I2CBaseAddress port,
                            PinAndPinset sdapin,PinAndPinset sclpin)
{
   uint8_t tempi;
   device->I2C_Configure=_I2C_Configure;
  // _I2C_Configure(port,frequency,sclpin,sdapin);
   device->I2C_IntClear=_I2C_IntClear;
   device->I2C_IntConfigure=_I2C_IntConfigure;
   device->I2C_IntStatus=_I2C_IntStatus;
   device->I2C_Receive=_I2C_Receive;
   device->I2C_Send=_I2C_Send;

   I2C_intcontroller=intcontroller;
   I2C_intcontroller->IntHandler(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn,I2C0Handler);

}




#endif //SUPPRESS_INLINE_IMPLEMENTATION

#endif //NRF_TWI_H__



