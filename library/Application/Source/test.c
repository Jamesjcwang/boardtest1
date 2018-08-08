#include <stdbool.h>
#include <stdint.h>
#include "library\Application\Header\I_test.h"
volatile uint32_t _tempintflag,_tempdeviceid1,_tempdeviceid2;
 uint8_t _i,_p;

void wheellock(bool enable,testParameter* parameter )
{
    if (enable)
    {

     test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_16,GPIO_PIN_16);

    }
    else
    {

    test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_16,0);
    }

}

void MCUPowerOn(bool enable,testParameter* parameter)
{

       if (enable)
    {
    test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_15,GPIO_PIN_15);

    }
    else
    {

    test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_15,0);
    }

}

void nMosOn(bool enable,testParameter* parameter)
{

       if (enable)
    {
    test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_4,GPIO_PIN_4);

    }
    else
    {

    test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_4,0);
    }

}

void TrickleChargetestOn(bool enable,testParameter* parameter)
{

       if (enable)
    {

     test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_30,GPIO_PIN_30);

    }
    else
    {

     test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_30,0);
    }

}
void I2CresetOn(bool enable,testParameter* parameter)
{

       if (enable)
    {
     test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_30,0);

    }
    else
    {

    test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_10,GPIO_PIN_10);
    }

}

void gpiote_handler1(void)
{


    nrf_gpiote_event_clear(NRF_GPIOTE_EVENTS_IN_0);
    //UARTComponent.UART_Send(UART0_BASE,false,&_p,1);
    _tempintflag=true;


}



void GPIOinttest(bool enable,testParameter* parameter )
{
//    if (enable)
//    {
 //   parameter->intcomp->Configure(GPIOTE_IRQn,true,0);

//    parameter->intcomp->IntHandler(GPIOTE_IRQn,gpiote_handler1);
//    nrf_gpiote_int_enable(1);

//    nrf_gpiote_event_configure(0,28,NRF_GPIOTE_POLARITY_HITOLO);
//     nrf_gpiote_event_enable(0);
//    for (_i=0;_i<10000;_i++)
//    {
//
//    }
//
//        for (_i=0;_i<10000;_i++)
//    {
//
//    }
//    while(_tempintflag==false)
//    {
//
//    }
//     test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_29,0);
//    _p=0x10;
//
//
//    parameter->compara->output_cmd=0x08;
//    parameter->compara->outputlength=2;
//    *(parameter->compara->outputrawdata)=0x01;
//    parameter->compara->flagsend=true;
//    }
//else
 //  {
//    nrf_gpiote_event_disable(0);

 //  }


}

void GetDeviceID(testParameter* para)
{


  test_powercomp->Get_DeviceID(&(para->_tempdeviceid1),
                                &(para->_tempdeviceid2));

   para->receivedata[0]=para->_tempdeviceid1;
   para->receivedata[1]=para->_tempdeviceid1;
   para->templength=2;
}

void getskuid(testParameter* parameter)
{   uint32_t value;
    if(test_iocomp->IO_PinRead(GPIO_PORT0_BASE,GPIO_PIN_18)==1)
    {
        value=1;

    }
    if(test_iocomp->IO_PinRead(GPIO_PORT0_BASE,GPIO_PIN_19)==1)
    {
        value=value|2;

    }
      parameter->templength=1;
      parameter->receivedata[0]=value;


}



 void _test_Routine(bool reset,testParameter* parameter)
{

    if (reset==true)
    {
        parameter->step=0;

    }
    switch (parameter->step)
    {
    case 0:


        if (test_modelcomp->Get(parameter->modelinindex,
                               &parameter->tempcmd,
                               &parameter->templength,
                               &parameter->tempvalue,
                               1,
                               false)!=notvalid_model)
       {

         if (parameter->tempcmd==CmdInttest)
         {
               parameter->step=10;
         }
         else if (parameter->tempcmd==CmdMcuon)
         {
           SEGGER_RTT_printf(0,"Mcu %d\r\n",1);
           parameter->step=20;
         }
         else if (parameter->tempcmd==Cmdsteerlock)
         {
             parameter->step=30;
         }

          else if (parameter->tempcmd==Cmdprecharge)
           {
               parameter->step=40;
           }
          else if (parameter->tempcmd==Cmdtricklecharge)
           {
               parameter->step=50;
           }

          else if (parameter->tempcmd==CmdGetDeviceID)
           {
                parameter->step=60;
           }

          else if (parameter->tempcmd==CmdI2Creset)
           {
                parameter->step=70;
           }
          else if (parameter->tempcmd==CmdSkuID)
           {
                parameter->step=80;
           }
          else if (parameter->tempcmd==CmdSleep)
           {
                parameter->step=90;
           }
          else if (parameter->tempcmd==CmdVBAT)
           {
                parameter->step=100;
           }


        }

     break;

    case 10:
        GPIOinttest(true,&parameter);
        parameter->step=200;
      //  parameter->compara->flagreceive=false;
       break;
    case 20:
        if (parameter->tempvalue==0x01)
        {
          MCUPowerOn(true,&parameter);
        }
        else
        {
          MCUPowerOn(false,&parameter);
        }
        parameter->receivedata[0]=0x01;
        parameter->templength=1;
        parameter->step=200;
       break;
    case 30:
        if (parameter->tempvalue==0x01)
        {
          wheellock(true,&parameter);
        }
        else
        {
          wheellock(false,&parameter);
        }
        parameter->receivedata[0]=0x01;
        parameter->templength=1;
        parameter->step=200;
       break;

    case 40:
        if (parameter->tempvalue==0x01)
        {
          nMosOn(true,&parameter);
        }
        else
        {
          nMosOn(false,&parameter);
        }
        parameter->receivedata[0]=0x01;
        parameter->templength=1;
        parameter->step=200;
       break;
    case 50:
        if (parameter->tempvalue==0x01)
        {
          TrickleChargetestOn(true,&parameter);
        }
        else
        {
          TrickleChargetestOn(false,&parameter);
        }
        parameter->receivedata[0]=0x01;
        parameter->templength=1;
        parameter->step=200;
       break;

    case 60:

        GetDeviceID(parameter);
        parameter->step=200;


        break;

    case 70:
          if (parameter->tempvalue==0x01)
        {
          I2CresetOn(true,&parameter);
        }
        else
        {
          I2CresetOn(false,&parameter);
        }
        parameter->templength=1;
        parameter->receivedata[0]=0x01;
        parameter->step=0;
        break;
    case 80:

            getskuid(parameter);
            parameter->step=200;
            break;

    case 90:

       test_powercomp->Power_Sleep(POWER);
       parameter->templength=1;
       parameter->receivedata[0]=0x01;
       parameter->step=200;
       break;

    case 100:
        test_iocomp->IO_PinWrite(GPIO_PORT0_BASE,GPIO_PIN_3,GPIO_PIN_3);
 SEGGER_RTT_printf(0,"ADC %d\r\n",1);
         test_adccomp->ADC_Sample(true,channel_ADC_0,&parameter->tempvalue,true);
                  test_modelcomp->Get(parameter->modelinindex,
                               &parameter->tempcmd,
                               &parameter->templength,
                               &parameter->tempvalue,
                               1,true);

         parameter->step=110;
       break;

    case 110:
      SEGGER_RTT_printf(0,"ADC %d\r\n",3);

        if ( test_adccomp->ADC_Sample(false,channel_ADC_0,&parameter->tempvalue,true)==true)
        {

 parameter->templength=2;
           parameter->receivedata[0]=(parameter->tempvalue&0xff);
           parameter->receivedata[1]=(parameter->tempvalue>>8)&0xff;
           parameter->step=210;

         }
        break;
    case 200:
        test_modelcomp->Get(parameter->modelinindex,
                               &parameter->tempcmd,
                               &parameter->templength,
                               &parameter->tempvalue,
                               1,true);
        parameter->step=210;
        break;

    case 210:

      if (test_modelcomp->Set(parameter->modeloutindex,
                                parameter->crcparaindex,
                                parameter->tempcmd,
                                parameter->receivedata,parameter->templength
                                ,false)==successful_model)
       {
            parameter->step=0;
       }

       break;

    }

}
void CreatetesteModule(testDevice* testcomp,
                       InterruptController* intcomp,
                       PowerDevice* powercomp,CRCDevice* crccomp,
                       IODevice* iocomp,ADCDevice* adccomp,
                       ModelDevice* modelcomp)
{
    testcomp->test_Routine=_test_Routine;
    test_intcomp=intcomp;
    test_powercomp=powercomp;
    test_crccomp=crccomp;
    test_iocomp=iocomp;
    test_adccomp=adccomp;
    test_modelcomp=modelcomp;
}
