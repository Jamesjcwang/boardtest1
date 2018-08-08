#ifndef __I_I2C_h__
#define __I_I2C_h__

#include <stdbool.h>
#include <stdint.h>
#include "library\nrf.h"
#include "library\Physic\Header\I_IO.h"
//#include "I_DataStruct.h"
#include "D:\test\Nordic\segger_rtt\SEGGER_RTT.h"
typedef struct {                                    /*!< TWI Structure                                                         */
  __O  uint32_t  TASKS_STARTRX;                     /*!< Start TWI receive sequence                                            */
  __I  uint32_t  RESERVED0;
  __O  uint32_t  TASKS_STARTTX;                     /*!< Start TWI transmit sequence                                           */
  __I  uint32_t  RESERVED1[2];
  __O  uint32_t  TASKS_STOP;                        /*!< Stop TWI transaction                                                  */
  __I  uint32_t  RESERVED2;
  __O  uint32_t  TASKS_SUSPEND;                     /*!< Suspend TWI transaction                                               */
  __O  uint32_t  TASKS_RESUME;                      /*!< Resume TWI transaction                                                */
  __I  uint32_t  RESERVED3[56];
  __IO uint32_t  EVENTS_STOPPED;                    /*!< TWI stopped                                                           */
  __IO uint32_t  EVENTS_RXDREADY;                   /*!< TWI RXD byte received                                                 */
  __I  uint32_t  RESERVED4[4];
  __IO uint32_t  EVENTS_TXDSENT;                    /*!< TWI TXD byte sent                                                     */
  __I  uint32_t  RESERVED5;
  __IO uint32_t  EVENTS_ERROR;                      /*!< TWI error                                                             */
  __I  uint32_t  RESERVED6[4];
  __IO uint32_t  EVENTS_BB;                         /*!< TWI byte boundary, generated before each byte that is sent or
                                                         received                                                              */
  __I  uint32_t  RESERVED7[3];
  __IO uint32_t  EVENTS_SUSPENDED;                  /*!< TWI entered the suspended state                                       */
  __I  uint32_t  RESERVED8[45];
  __IO uint32_t  SHORTS;                            /*!< Shortcut register                                                     */
  __I  uint32_t  RESERVED9[64];
  __IO uint32_t  INTENSET;                          /*!< Enable interrupt                                                      */
  __IO uint32_t  INTENCLR;                          /*!< Disable interrupt                                                     */
  __I  uint32_t  RESERVED10[110];
  __IO uint32_t  ERRORSRC;                          /*!< Error source                                                          */
  __I  uint32_t  RESERVED11[14];
  __IO uint32_t  ENABLE;                            /*!< Enable TWI                                                            */
  __I  uint32_t  RESERVED12;
  __IO uint32_t  PSELSCL;                           /*!< Pin select for SCL                                                    */
  __IO uint32_t  PSELSDA;                           /*!< Pin select for SDA                                                    */
  __I  uint32_t  RESERVED13[2];
  __I  uint32_t  RXD;                               /*!< RXD register                                                          */
  __IO uint32_t  TXD;                               /*!< TXD register                                                          */
  __I  uint32_t  RESERVED14;
  __IO uint32_t  FREQUENCY;                         /*!< TWI frequency                                                         */
  __I  uint32_t  RESERVED15[24];
  __IO uint32_t  ADDRESS;                             /*!< Address used in the TWI transfer   */
  __I  uint32_t  RESERVED16[668];
  __IO uint32_t  POWER;

} NRF_TWI_Type;

typedef enum
{
    I2C0,
    I2C1,
    I2Cindex_total

} enum_I2Cindex;

typedef enum {
I2C0_BASE   =            0x40003000,
I2C1_BASE   =            0x40004000

} I2CBaseAddress;

typedef enum
{
I2C_FREQUENCY_K100 =0x01980000,
I2C_FREQUENCY_K250 =0x04000000,
I2C_FREQUENCY_K400 =0x06680000

} EnumI2CFrequency;


typedef enum {

EVENTS_STOPPED  =2,
EVENTS_RXDREADY =4,
EVENTS_TXDSENT  =128,
EVENTS_ERROR    =0x200,
EVENTS_BB       =0x4000,
EVENTS_SUSPENDED=0x40000,
EVENTS_RXSTART=0x80000,
EVENTS_TXSTART=0x100000,
EVENTS_LASTRX=0x800000,
EVENTS_LASTTX=0x1000000
} I2CMISenum;

typedef enum{
I2C_STOPPED,
I2C_RXDREADY,
I2C_TXDSENT,
I2C_BB,
I2C_SUSPENDED,
I2C_OverRunError,
I2C_ANACK,
I2C_DNACK

} I2CeventKind;

typedef enum
{
    i2c_success,
    i2c_fail,
    i2c_stop,
    i2c_null
} enum_i2cresult;

typedef enum
{
    open,
    close,
    null

} enum_i2coperation;


typedef struct {

 enum_i2cresult (*I2C_Send)(enum_i2coperation operation,
                            I2CBaseAddress port,uint8_t address,uint8_t *data);

 enum_i2cresult (*I2C_Receive)(enum_i2coperation operation,
                            I2CBaseAddress port,uint8_t address,uint8_t *data);

 void (*I2C_IntConfigure)(I2CBaseAddress port, I2CMISenum interrupt,
                           bool enable,uint32_t priority,bool controllerenable);

 void (*I2C_IntClear)(I2CBaseAddress port, I2CMISenum interrupt);
 void (*I2C_Configure)(I2CBaseAddress port , EnumI2CFrequency frequency,
                    PinAndPinset sclpin,PinAndPinset sdapin);

 uint32_t (*I2C_IntStatus)(I2CBaseAddress port,I2CMISenum interrupt);


} I2CDevice;


InterruptController* I2C_intcontroller;



extern void CreateI2CDevice(I2CDevice* device, InterruptController* intcontroller,
                            EnumI2CFrequency frequency,
                            I2CBaseAddress port,
                            PinAndPinset sdapin,PinAndPinset sclpin);






#endif
