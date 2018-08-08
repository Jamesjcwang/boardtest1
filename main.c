/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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

/** @file
 * @defgroup uart_example_main main.c
 * @{
 * @ingroup uart_example
 * @brief UART Example Application main file.
 *
 * This file contains the source code for a sample application using UART.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "library\Physic\Header\I_Int.h"
#include "library\Physic\Header\I_IO.h"
#include "library\Physic\Header\I_UART.h"
#include "library\Physic\Header\I_I2C.h"
#include "library\Physic\Header\I_ADC.h"
#include "library\Physic\Header\I_Timer.h"
#include "Library\Communication\Header\I_ProtectionICModule.h"
#include "Library\Communication\Header\I_Flash.h"
#include "Library\Communication\Header\I_GetShaID.h"
#include "Library\Component\Header\I_Delayobject.h"
#include "Library\Physic\Header\I_SPIM.h"
#include "Library\Component\Header\I_Model.h"

#include "Library\Communication\Header\I_UARTTXRX.h"

#include "Library\Component\Header\I_Datastruct.h"
#include "Library\Communication\Header\I_PT.h"
#include "Library\Component\Header\I_CRC.h"
#include "Library\Application\Header\I_test.h"


#include "library\nrf_power.h"

#include "library\nrf_gpiote.h"
#include "D:\test\Nordic\segger_rtt\SEGGER_RTT.h"

//#define ENABLE_LOOPBACK_TEST  /**< if defined, then this example will be a loopback test, which means that TX should be connected to RX to get data loopback. */

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */
typedef enum
{
    pc_exterdevice,
    exterdevice_pc,
    dataflow_total

} enum_dataflowdir;




typedef enum
{
  crc_flash,
  crc_pt,
  crc_sha,
  crc_proic,
  crc_uart,
  flashpara,
  ptpara,
  shapara,
  propara,
  uarttxrxpara,
  delaypara_flash,
  delaypara_pt,
  delaypara_sha,
  delaypara_pro,
  delaypara_uart,
  pointindextotal
}indexenum;
IODevice IOComponent;
UARTDevice UARTComponent;
InterruptController INTComponent;
SPIMDevice SPIMComponent;
PowerDevice POWERComponent;
I2CDevice I2CComponent;
ADCDevice ADCComponent;
TIMERDevice TimerComponent;
ProtectionICDevice ProtectionICComponent;
ShaDevice shacomponent;
ExternalFlashDevice exflashcomponent;
DelayObject DelayComponent;



ModelDevice modelcomponent;
UARTTXRXDevice uarttxrxcomp;

PTDevice ptcomp;

CRCDevice crccomp;


ListCollection listcomp;

testParameter testpara;
testDevice testcomp;

//DataManageDevice datamancomp;
//DataManagParameter promanpara;
//DataManagParameter shamanpara;
//DataManagParameter flashmanpara;
//packetstruct packetpara;




 volatile  uint32_t i,j,k,l,m,_f,_f1,_f2,_f3,_f4,_f5,_f6,_s1,devicelsbbits,devicemsbbits;
 volatile uint32_t _p,_count1;
volatile  uint32_t _temps,_tempc,_tempadd,_tempuartreceivedata;
volatile uint32_t _flagpowerrailcheck,_flaggetdeviceid,_flaguarttest,_flaguarttxfinish;
volatile uint32_t _flagprotectic,_flagi2creset;
volatile uint32_t _flaginttest,_flaggetADCvalue,_flagadcstarted,_flagadcend,_flagvbat;
volatile uint32_t _temppowerflag;
volatile uint8_t _tempbyte,_tempcount;
volatile uint32_t _buffer,_buffer2;
volatile bool _flaggetflashid, _flaggetshasn;
uint32_t structindex[pointindextotal];
uint32_t modelindex[dataflow_total];
uint32_t step_vbat;
IOBaseAddress ioport[32]={GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE,GPIO_PORT0_BASE,
                          GPIO_PORT0_BASE,GPIO_PORT0_BASE};
PinAndPinset iopin[32]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,
                      GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,
                      GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_10,GPIO_PIN_11,
                      GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15,
                      GPIO_PIN_16,GPIO_PIN_17,GPIO_PIN_18,GPIO_PIN_19,
                      GPIO_PIN_20,GPIO_PIN_21,GPIO_PIN_22,GPIO_PIN_23,
                      GPIO_PIN_24,GPIO_PIN_25,GPIO_PIN_26,GPIO_PIN_27,
                      GPIO_PIN_28,GPIO_PIN_29,GPIO_PIN_30,GPIO_PIN_31};
DriveStrength iolength[32]={NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0D1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0D1,NRF_GPIO_PIN_S0D1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1,
                          NRF_GPIO_PIN_S0S1,NRF_GPIO_PIN_S0S1};
Pull_Push iotype[32]={NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL,
                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL};
PinMode iomode[32]={GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_IN};
PinMode iobufferconnected[32]=
                 {GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_IN,
                  GPIO_DIR_MODE_IN,GPIO_DIR_MODE_IN,GPIO_DIR_MODE_OUT,
                  GPIO_DIR_MODE_OUT,GPIO_DIR_MODE_IN};

/**
 * @brief Function for main application entry.
 */


// void _adc_handler(enum_ADC_MIS event)
 //{ //SEGGER_RTT_printf(0,"Times %d\r\n",event);

 //     if (event==ADC_Event_STOPPED)
//         {
     //  SEGGER_RTT_printf(0,"Times %d\r\n",60);


//         }

//       if (event==ADC_Event_STARTED)
//         {
     //  SEGGER_RTT_printf(0,"Times %d\r\n",55);
 //      _flagadcstarted=1;

 //        }
 //      if (event==ADC_Event_DONE)
 //        {
       //  SEGGER_RTT_printf(0,"Times %d\r\n",54);
 //        _flaggetADCvalue=1;


 //        }
 //      if (event==ADC_Event_END)
 //        {
      // SEGGER_RTT_printf(0,"Times %d\r\n",53);
 //        _flagadcend=1;
 //        }

 //      if (event==ADC_Event_CH0LIMITH)
 //        {
     //  SEGGER_RTT_printf(0,"Times %d\r\n",57);

  //       }
  //     if (event==ADC_Event_CH0LIMITL)
 //        {
      // SEGGER_RTT_printf(0,"Times %d\r\n",58);

  //       }
 //      if (event==ADC_Event_RESULTDONE)
 //        {
      // SEGGER_RTT_printf(0,"Times %d\r\n",59);

  //       }


 //}
  void _power_handler(PowerMISenum event)
 {
        SEGGER_RTT_printf(0,"Times %c\r\n",-1);
     if (event==SLEEPENTER)
     {
      //     SEGGER_RTT_printf(0,"Times %c\r\n",666);

     }

 }



int main(void)
{

    l=0;
    i=1;
    _f=0;
    _s1=0;
 SEGGER_RTT_printf(0,"Times %d\r\n",1);
/**
  physical Layer initialization

*/
   CreateListMoudle(&listcomp);
   CreateIntDevice(&INTComponent);

   CreateIODevice(&IOComponent,32,ioport,iopin,iolength,iotype,iomode,
                  iobufferconnected);


   CreateUARTDevice(&UARTComponent,&INTComponent,UART0_BASE,16000,
                    NRF_UART_BAUDRATE_9600,UART_CONFIG_WLEN_8,
                    UART_CONFIG_STOP_MASK,UART_CONFIG_PAR_NONE,
                    false,20,20,false,GPIO_PORT0_BASE,
                    GPIO_PIN_6,GPIO_PIN_8,GPIO_PIN_1,GPIO_PIN_2);

   CreateSPIMDevice(&SPIMComponent,&INTComponent,SPI1_Base,
                    FREQUENCY_K125,Leading,ActiveHigh,MsbFirst,
                    GPIO_PORT0_BASE,GPIO_PIN_22,GPIO_PIN_23,
                    GPIO_PIN_24,GPIO_PIN_25);
   CreateSPIMDevice(&SPIMComponent,&INTComponent,SPI0_Base,
                 FREQUENCY_K125,Leading,ActiveHigh,MsbFirst,
                 GPIO_PORT0_BASE,GPIO_PIN_14,GPIO_PIN_12,
                 GPIO_PIN_13,GPIO_PIN_11);
   CreateI2CDevice(&I2CComponent,&INTComponent,I2C_FREQUENCY_K100,
                   I2C0_BASE,GPIO_PIN_26,GPIO_PIN_27);
   CreateTimerDevice(&TimerComponent,&INTComponent,TIMER0,
                     Timer_32bit,Timer_FREQ_1MHz);
   CreateTimerDevice(&TimerComponent,&INTComponent,TIMER1,
                     Timer_32bit,Timer_FREQ_1MHz);
   CreateTimerDevice(&TimerComponent,&INTComponent,TIMER2,
                     Timer_32bit,Timer_FREQ_1MHz);
   CreateTimerDevice(&TimerComponent,&INTComponent,TIMER3,
                     Timer_32bit,Timer_FREQ_1MHz);
   CreateTimerDevice(&TimerComponent,&INTComponent,TIMER4,
                     Timer_32bit,Timer_FREQ_1MHz);

   CreateADCDevice(&ADCComponent,&INTComponent,SAADC,
                   0,ADC_AnalogIn0,ADC_AnalogIn0,ADC_Resp_Bypass,
                   ADC_ResN_Bypass,ADC_GAIN_1_6,ADC_internal,
                   ADC_40us,ADC_SingleEnd,ADC_14bit,ADC_Task,
                   32);

/**
  Object Layer initialization

*/


   CreateCRCDevice(&crccomp,&listcomp);

   structindex[crc_flash]=crccomp.ParameterInitial(_16bit,0x8005);
   structindex[crc_pt]=crccomp.ParameterInitial(_16bit,0x8005);

   structindex[crc_sha]=crccomp.ParameterInitial(_16bit,0x8005);
   structindex[crc_proic]=crccomp.ParameterInitial(_16bit,0x8005);
   structindex[crc_uart]=crccomp.ParameterInitial(_16bit,0x8005);

   CreateDelayobject(&DelayComponent,&listcomp,&TimerComponent);

   structindex[delaypara_pt]=DelayComponent.ParameterInitial(TIMER1);
   structindex[delaypara_flash]=DelayComponent.ParameterInitial(TIMER2);
   structindex[delaypara_pro]=DelayComponent.ParameterInitial(TIMER3);
   structindex[delaypara_sha]=DelayComponent.ParameterInitial(TIMER4);
   structindex[delaypara_uart]=DelayComponent.ParameterInitial(TIMER0);

   CreateModelDevice(&modelcomponent,&crccomp,&listcomp);
   modelindex[exterdevice_pc]=modelcomponent.NewModel();
   modelindex[pc_exterdevice]=modelcomponent.NewModel();


/**
  Process Layer initialization

*/

   CreateExFlashHandshakeModule(&exflashcomponent,&SPIMComponent,
                              &DelayComponent,&IOComponent,
                              &crccomp,&listcomp,&modelcomponent);

   structindex[flashpara]=exflashcomponent.ParameterInitial(
                          CmdGetFlashID,SPI1_Base,GPIO_PORT0_BASE,
                          GPIO_PIN_22,GPIO_PIN_23,
                          GPIO_PIN_24,GPIO_PIN_25);


   exflashcomponent.ExFlash_Routine(true,structindex[flashpara],
                                     structindex[crc_flash],
                                     structindex[delaypara_flash],
                                     modelindex[pc_exterdevice],
                                     modelindex[exterdevice_pc]);

   CreatePTHandshakeModule(&ptcomp,&IOComponent,&DelayComponent,
                           &SPIMComponent,&crccomp,&listcomp,&modelcomponent);

   structindex[ptpara]=ptcomp.ParameterInitial(
                           SPI0_Base,GPIO_PORT0_BASE,
                           CmdPt,CmdI2Creset,GPIO_PIN_14,
                           GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_15);

   ptcomp.PT_Routine(true,structindex[ptpara],structindex[crc_pt],
                     structindex[delaypara_pt],modelindex[pc_exterdevice],
                     modelindex[exterdevice_pc]);


   CreatePowerDevice(&POWERComponent,&INTComponent);



   POWERComponent.Get_DeviceID(&devicelsbbits,&devicemsbbits);

   CreatePreotectionHandshakeModule(&ProtectionICComponent,&I2CComponent,
                                    &DelayComponent,&IOComponent,
                                    &crccomp,&listcomp,&modelcomponent);

   structindex[propara]=ProtectionICComponent.ParameterInitial(
                        CmdGetProtectionICcellvalue,I2C0_BASE,GPIO_PORT0_BASE,0x08,
                        GPIO_PIN_27,GPIO_PIN_26,GPIO_PIN_29,NRF_GPIO_PIN_NOPULL,
                        NRF_GPIO_PIN_NOPULL);

   ProtectionICComponent.Routine(true,structindex[propara],
                                 structindex[crc_proic],
                                 structindex[delaypara_pro],
                                     modelindex[pc_exterdevice],
                                     modelindex[exterdevice_pc]);

   CreateShaHandshakeModule(&shacomponent,&I2CComponent,&DelayComponent,
                            &IOComponent,&crccomp,&listcomp,&modelcomponent);

   structindex[shapara]=shacomponent.ParameterInitial(CmdGetShaSN,I2C0_BASE,GPIO_PORT0_BASE,
                                    0x64,GPIO_PIN_27,GPIO_PIN_26,
                                    NRF_GPIO_PIN_NOPULL,NRF_GPIO_PIN_NOPULL);

   shacomponent.Routine(true,structindex[shapara],structindex[crc_sha],
                        structindex[delaypara_sha],modelindex[pc_exterdevice],
                        modelindex[exterdevice_pc]);


  // CreateDataManageDevice(&datamancomp);
   CreateUARTTXRXDevice(&uarttxrxcomp,&modelcomponent,&crccomp,
                        &DelayComponent,&UARTComponent,&IOComponent,
                        &listcomp);

   structindex[uarttxrxpara]=uarttxrxcomp.ParameterInitial(
                  0XEF,UART0_BASE,GPIO_PORT0_BASE,
                  GPIO_PIN_6,GPIO_PIN_8,GPIO_PIN_1,
                  GPIO_PIN_2,structindex[crc_uart],
                  structindex[delaypara_uart],modelindex[pc_exterdevice],
                  modelindex[exterdevice_pc]);

   uarttxrxcomp.UARTTXRX_Routine(true,structindex[uarttxrxpara]);

   POWERComponent.Power_IntConfigure(POWER,POFWARN|SLEEPENTER|SLEEPEXIT,true,0,true);
   POWERComponent.Power_IntHandler(POWER,_power_handler);


//GPIOinttest(true);


   IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_4,GPIO_PIN_4);
   IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_18,GPIO_PIN_18);

   IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_19,GPIO_PIN_19);

   IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_20,GPIO_PIN_20);

   IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_17,GPIO_PIN_17);


   IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_9,GPIO_PIN_9);

   IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_10,GPIO_PIN_10);
   IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_15,GPIO_PIN_15);

   CreatetesteModule(&testcomp,&INTComponent,&POWERComponent,
                     &crccomp,&IOComponent,&ADCComponent,
                     &modelcomponent);

   testpara.crcparaindex=structindex[crc_uart];
   testpara.modelinindex=modelindex[pc_exterdevice];
   testpara.modeloutindex=modelindex[exterdevice_pc];
    testcomp.test_Routine(true,&testpara);
  // testcomp.test_Routine(true,&testpara);



for (i=0;i<28;i++)
{

}



_flagprotectic=true;

    while (true)

    {

    //  testcomp.test_Routine(false,&testpara);
     uarttxrxcomp.UARTTXRX_Routine(false,structindex[uarttxrxpara]);
    testcomp.test_Routine(false,&testpara);
    // datamancomp.DataManage_ReceiveRoutine(false,&flashmanpara);
  //   datamancomp.DataManage_SendRoutine(false,&flashmanpara);

   //  datamancomp.DataManage_ReceiveRoutine(false,&promanpara);
   //  datamancomp.DataManage_SendRoutine(false,&promanpara);
   //  datamancomp.DataManage_ReceiveRoutine(false,&shamanpara);
   //  datamancomp.DataManage_SendRoutine(false,&shamanpara);
    ProtectionICComponent.Routine(false,structindex[propara],
                                  structindex[crc_proic],
                                  structindex[delaypara_pro],
                                  modelindex[pc_exterdevice],
                                  modelindex[exterdevice_pc]);

    shacomponent.Routine(false,structindex[shapara],structindex[crc_sha],
                        structindex[delaypara_sha],modelindex[pc_exterdevice],
                        modelindex[exterdevice_pc]);

    exflashcomponent.ExFlash_Routine(false,structindex[flashpara],
                        structindex[crc_flash],structindex[delaypara_flash]
                        ,modelindex[pc_exterdevice],
                        modelindex[exterdevice_pc]);
    ptcomp.PT_Routine(false,structindex[ptpara],structindex[crc_pt],
                     structindex[delaypara_pt],
                     modelindex[pc_exterdevice],
                     modelindex[exterdevice_pc]);

     if ((_flagpowerrailcheck==1 )&& (_temppowerflag==0))
     {
       //MCUPowerOn(true);
      // wheellock(true);
     //  nMosOn(true);
     //  TrickleChargetestOn(true);
     //  _temppowerflag=1;

     }
     else if((_flagpowerrailcheck==0) && (_temppowerflag==1))
    {
     //   MCUPowerOn(false);
     //  wheellock(false);
     //  nMosOn(false);
     //  TrickleChargetestOn(false);
     //  _temppowerflag=0;
    }
    if (_flaggetdeviceid==1)
    {
       // GetDeviceID();
    //    _flaggetdeviceid=0;
    }
  //  if (_flaguarttest==1)
  //  {
    //    i=0x01;
    //    UARTComponent.UART_Send(UART0_BASE,false,&i,1);
    //    _flaguarttest=0;
    //}
   // if (_flaggetflashid==true)
  //  {
  //      getflashID();
  //      _flaggetflashid=false;
  //  }

 // if (_flaggetshasn==true)
 //   {
  //      GetSNofSHA();
 //      _flaggetshasn=false;
//    }
    //if (_flagprotectic==1)
    // {


    //   GetProtectionIC();
    //  _flagprotectic=0;
    //}
        if (_flagi2creset==1)
    {
     //  i2creset(true);
   //     i2creset(false);
        _flagi2creset=0;
    }
           if (_flaginttest==1)
    {
   //     GPIOinttest(true);
   //
        _flaginttest=0;
    }
          if (_flagvbat==1)
    {
     //  vbatmeasure();
    //
        _flagvbat=0;
    }
    if (IOComponent.IO_PinRead(GPIO_PORT0_BASE,GPIO_PIN_28)==0)
    {
     //  IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_29,0);

    }
      if (IOComponent.IO_PinRead(GPIO_PORT0_BASE,GPIO_PIN_28)==1)
    {
      // IOComponent.IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_29,1);

    }


}


    return(0);
}



void HardFault_Handler(void)
{
     while (true)
    {
 //SEGGER_RTT_printf(0,"Times %d\r\n",300);
       //  SEGGER_RTT_printf(0,"receive %d\r\n",114);
      //  UARTComponent.UART_Send(UART0_BASE,false,&_p,1);

    }

}

 void NMI_Handler (void)
 {
        while (true)
    {
    //SEGGER_RTT_printf(0,"Times %d\r\n",300);
    }
 }

    void   MemoryManagement_Handler(void)
   {
               while (true)
    {
     SEGGER_RTT_printf(0,"Times %d\r\n",500);
    }
   }
    void   BusFault_Handler(void)
    {
                while (true)
    {
     SEGGER_RTT_printf(0,"Times %d\r\n",600);
    }
    }
     void  UsageFault_Handler(void)
{
            while (true)
    {
     SEGGER_RTT_printf(0,"Times %d\r\n",700);
    }
}




/** @} */

