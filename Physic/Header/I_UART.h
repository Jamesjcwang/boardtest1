#include <stdbool.h>
#include <stdint.h>
#include "library\Physic\Header\I_Int.h"
#include "library\Physic\Header\I_IO.h"

#ifndef __I_UART_h__
#define __I_UART_h__
typedef struct {                                    /*!< UART Structure                                                        */
  volatile uint32_t  TASKS_STARTRX;                     /*!< Start UART receiver                                                   */
 volatile  uint32_t  TASKS_STOPRX;                      /*!< Stop UART receiver                                                    */
  volatile  uint32_t  TASKS_STARTTX;                     /*!< Start UART transmitter                                                */
  volatile  uint32_t  TASKS_STOPTX;                      /*!< Stop UART transmitter                                                 */
 volatile const  uint32_t  RESERVED0[3];
  volatile  uint32_t  TASKS_SUSPEND;                     /*!< Suspend UART                                                          */
 volatile const  uint32_t  RESERVED1[56];
 volatile uint32_t  EVENTS_CTS;                        /*!< CTS is activated (set low). Clear To Send.                            */
  volatile uint32_t  EVENTS_NCTS;                       /*!< CTS is deactivated (set high). Not Clear To Send.                     */
  volatile uint32_t  EVENTS_RXDRDY;                     /*!< Data received in RXD                                                  */
 volatile const  uint32_t  RESERVED2[4];
  volatile uint32_t  EVENTS_TXDRDY;                     /*!< Data sent from TXD                                                    */
  volatile const  uint32_t  RESERVED3;
  volatile uint32_t  EVENTS_ERROR;                      /*!< Error detected                                                        */
  volatile const  uint32_t  RESERVED4[7];
  volatile uint32_t  EVENTS_RXTO;                       /*!< Receiver timeout                                                      */
 volatile const  uint32_t  RESERVED5[46];
  volatile uint32_t  SHORTS;                            /*!< Shortcut register                                                     */
  volatile const  uint32_t  RESERVED6[64];
  volatile uint32_t  INTENSET;                          /*!< Enable interrupt                                                      */
  volatile uint32_t  INTENCLR;                          /*!< Disable interrupt                                                     */
  volatile const  uint32_t  RESERVED7[93];
  volatile uint32_t  ERRORSRC;                          /*!< Error source                                                          */
  volatile const  uint32_t  RESERVED8[31];
  volatile uint32_t  ENABLE;                            /*!< Enable UART                                                           */
  volatile const  uint32_t  RESERVED9;
  volatile uint32_t  PSELRTS;                           /*!< Pin select for RTS                                                    */
  volatile uint32_t  PSELTXD;                           /*!< Pin select for TXD                                                    */
  volatile uint32_t  PSELCTS;                           /*!< Pin select for CTS                                                    */
  volatile uint32_t  PSELRXD;                           /*!< Pin select for RXD                                                    */
  volatile const  uint32_t  RXD;                               /*!< RXD register                                                          */
 volatile  uint32_t  TXD;                               /*!< TXD register                                                          */
  volatile const  uint32_t  RESERVED10;
 volatile uint32_t  BAUDRATE;                          /*!< Baud rate                                                             */
 volatile const  uint32_t  RESERVED11[17];
  volatile uint32_t  CONFIG;                            /*!< Configuration of parity and hardware flow control                     */
} NRF_UART_Type;
typedef enum {
//UART_INT_DMATX     =     0x20000 ,    // DMA TX interrupt
//UART_INT_DMARX     =     0x10000 ,    // DMA RX interrupt
//UART_INT_9BIT      =     0x1000  ,    // 9-bit address match int
//UART_INT_OE        =     0x400   ,    // Overrun Error Interrupt
UART_INT_CTS       =     1   ,    // CTS Modem Interrupt Mas
UART_INT_NCTS       =    2UL  ,     // CTS Modem Interrupt Mas
UART_INT_RX        =     4UL   ,    // Receive Interrupt Mask
UART_INT_TX        =     128UL   ,    // Transmit Interrupt Mask
UART_INT_EE        =     512UL   ,    // Break Error Interrupt M
//UART_INT_PE        =     0x100   ,    // Parity Error Interrupt
//UART_INT_FE        =     0x080   ,    // Framing Error Interrupt
UART_INT_RT        =     0x20000UL       // Receive Timeout Interru
//UART_INT_DSR       =     0x008   ,    // DSR Modem Interrupt Mas
//UART_INT_DCD       =     0x004   ,    // DCD Modem Interrupt Mas
//UART_INT_RI        =     0x001      // RI Modem Interrupt Mask

} UARTMISenum;

typedef enum{
TxFinished,
RxReceived,
RXTimeOut,
ParityError,
FramingError,
OverRunError,
BreakError

} UARTeventKind;

typedef enum {
  UART0_BASE     =     0x40002000UL,
//UART0_BASE         =     0x4000C000 , // UART0
//UART1_BASE         =     0x4000D000 , // UART1
//UART2_BASE         =     0x4000E000 , // UART2
//UART3_BASE         =     0x4000F000 , // UART3
//UART4_BASE         =     0x40010000 , // UART4
//UART5_BASE         =     0x40011000 , // UART5
//UART6_BASE         =     0x40012000 , // UART6
//UART7_BASE         =     0x40013000  // UART7

} UARTBaseAddress;

typedef enum {
UART_CONFIG_WLEN_8 =     0x00000060,  // 8 bit data
//UART_CONFIG_WLEN_7 =     0x00000040,  // 7 bit data
//UART_CONFIG_WLEN_6 =     0x00000020,  // 6 bit data
//UART_CONFIG_WLEN_5 =     0x00000000  // 5 bit data
}DataLength;

typedef enum {
UART_CONFIG_STOP_MASK =  0x00000008 , // Mask for extracting stop bits
//UART_CONFIG_STOP_ONE  =  0x00000000 , // One stop bit
//UART_CONFIG_STOP_TWO  =  0x00000008  // Two stop bits
} StopBitType;

typedef enum {
//UART_CONFIG_PAR_MASK  =  0x00000086 , // Mask for extracting parity
UART_CONFIG_PAR_NONE  =  0x00000000 , // No parity
//UART_CONFIG_PAR_EVEN  =  0x00000006 , // Even parity
//UART_CONFIG_PAR_ODD   =  0x00000002 , // Odd parity
UART_CONFIG_PAR_ONE   =  0x00000014  // Parity bit is one
//UART_CONFIG_PAR_ZERO  =  0x00000086  // Parity bit is zero
}ParityType;

typedef enum {
UART_FIFO_RX1_8       =  0x00000000 , // Receive interrupt at 1/8 Full
UART_FIFO_RX2_8       =  0x00000008 , // Receive interrupt at 1/4 Full
UART_FIFO_RX4_8       =  0x00000010 , // Receive interrupt at 1/2 Full
UART_FIFO_RX6_8       =  0x00000018 , // Receive interrupt at 3/4 Full
UART_FIFO_RX7_8       =  0x00000020  // Receive interrupt at 7/8 Full

}ReceiveIntLevel;

typedef enum {
UART_FIFO_TX1_8       =  0x00000000 , // Transmit interrupt at 1/8 Full
UART_FIFO_TX2_8       =  0x00000001 , // Transmit interrupt at 1/4 Full
UART_FIFO_TX4_8       =  0x00000002 , // Transmit interrupt at 1/2 Full
UART_FIFO_TX6_8       =  0x00000003 , // Transmit interrupt at 3/4 Full
UART_FIFO_TX7_8       =  0x00000004  // Transmit interrupt at 7/8 Full

} TranmitIntLevel;

typedef enum
{

    NRF_UART_BAUDRATE_1200   =  0x0004F000UL, /**< 1200 baud. */
    NRF_UART_BAUDRATE_2400   =  0x0009D000, /**< 2400 baud. */
    NRF_UART_BAUDRATE_4800   = 0x0013B000, /**< 4800 baud. */
    NRF_UART_BAUDRATE_9600   =  0x00275000, /**< 9600 baud. */
    NRF_UART_BAUDRATE_14400  =  0x003B0000, /**< 14400 baud. */
    NRF_UART_BAUDRATE_19200  =  0x004EA000, /**< 19200 baud. */
    NRF_UART_BAUDRATE_28800  =  0x0075F000, /**< 28800 baud. */
    NRF_UART_BAUDRATE_38400  =  0x009D5000, /**< 38400 baud. */
    NRF_UART_BAUDRATE_57600  =  0x00EBF000, /**< 57600 baud. */
    NRF_UART_BAUDRATE_76800  =  0x013A9000, /**< 76800 baud. */
    NRF_UART_BAUDRATE_115200 =  0x01D7E000, /**< 115200 baud. */
    NRF_UART_BAUDRATE_230400 =  0x03AFB000, /**< 230400 baud. */
    NRF_UART_BAUDRATE_250000 =  0x04000000, /**< 250000 baud. */
    NRF_UART_BAUDRATE_460800 =  0x075F7000, /**< 460800 baud. */
    NRF_UART_BAUDRATE_921600 =  0x0EBED000, /**< 921600 baud. */
    NRF_UART_BAUDRATE_1000000 =  0x10000000, /**< 1000000 baud. */

} Baudrate;


typedef enum
{
    Tx,
    Rx,
    both

} Typeofuart;

typedef enum
{
    uart0,
    uartindex_total
}   enum_uartindex;

typedef struct {


 bool (*UART_Send)(bool reset,UARTBaseAddress port , bool blocking ,uint8_t *data, uint32_t lengthofdata);

 bool (*UART_Receive)(UARTBaseAddress port , bool blocking ,uint8_t *data);
 void (*UART_ReceiveIntHandler) ( void(*Handler)(UARTBaseAddress port) );


} UARTDevice;
InterruptController* UART_intcontroller;

extern void CreateUARTDevice(UARTDevice* device, InterruptController* intcontroller,
                      UARTBaseAddress port , uint32_t clockfrequency,
                      Baudrate baudrate,DataLength length, StopBitType stopbit,
                      ParityType parity,bool fifoenable,ReceiveIntLevel receivelevel,
                      TranmitIntLevel transmitlevel,bool loopback,
                      IOBaseAddress ioport ,PinAndPinset txpin,PinAndPinset rxpin,
                      PinAndPinset cstpin,PinAndPinset rstpin);

#endif
