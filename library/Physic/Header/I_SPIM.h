
#include <stdbool.h>
#include <stdint.h>
#include "library\Physic\Header\I_Int.h"
#include "library\Physic\Header\I_IO.h"
#include "library\nrf.h"


#ifndef __I_SPI_h__
#define __I_SPI_h__
typedef enum{

 MsbFirst= 0UL ,
 LsbFirst= 1UL


}EnumSpiOrder;





typedef enum
{

 Leading =0UL,
 Trailing =1UL

} EnumSpiCpha;

typedef enum
{

ActiveHigh =0UL,
ActiveLow =1UL

} EnumSpiCpoc;

typedef enum
{
FREQUENCY_K125 =0x02000000UL,
FREQUENCY_K250 =0x04000000UL,
FREQUENCY_K500 =0x08000000UL,
FREQUENCY_M1 =0x10000000UL,
FREQUENCY_M2 =0x20000000UL,
FREQUENCY_M4 =0x40000000UL,
FREQUENCY_M8 =-0x00000000

} EnumSpiFrequency;

typedef enum
{
    _slave_1,
    _slave_2,
    _slave_3,
    _slave_4,
    _slave_5,
    _slave_6,
    _slave_7,
    _slave_8

}EnumSpiSlave;


typedef enum
{
    SPI0_Base=0x40003000UL,
    SPI1_Base=0x40004000UL,
    SPI2_Base=0x40023000UL

}SPIBaseAddress;

typedef enum
{
    SPI0,
    SPI1,
    SPI2,
    SPIindex_total

} enum_SPIindex;


typedef enum
{

  SPI_Ready= 4UL

} SPIMISenum;


typedef struct {


 bool (*SPIM_Transmit)(bool reset,SPIBaseAddress port  ,uint8_t *data);

 //bool (*SPIM_Receive)(SPIBaseAddress port ,uint32_t *data);

 void (*SPIM_IntConfigure)(SPIBaseAddress port, SPIMISenum interrupt,
                           bool enable,uint32_t priority,bool controllerenable);

 void (*SPIM_IntClear)(SPIBaseAddress port, SPIMISenum interrupt);

 uint32_t (*SPIM_IntStatus)(SPIBaseAddress port,SPIMISenum interrupt);



} SPIMDevice;
InterruptController* SPIM_intcontroller;





extern void CreateSPIMDevice(SPIMDevice* device, InterruptController* intcontroller,
                             SPIBaseAddress port,
                             EnumSpiFrequency frequency,EnumSpiCpha  cpha,
                             EnumSpiCpoc cpoc,EnumSpiOrder order,IOBaseAddress ioport,
                             PinAndPinset sckpin,PinAndPinset mosipin, PinAndPinset misopin,
                             PinAndPinset sspin );






































#endif
