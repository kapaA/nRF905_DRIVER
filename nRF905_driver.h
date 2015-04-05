/*  ============================================================================
    Copyright (C) 2015 Achuthan Paramanathan.
    ============================================================================
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.
    ============================================================================
    Revision Information:
        File name: nRF905_driver.h
        Version:   v0.0
        Date:      05-04-2015
    ==========================================================================*/


/**
 * frame size
*/
#define PACKET_SIZE      32
#define PAYLOAD_SIZE     28
#define HEADER_SIZE       4
#define NRF_BUFFER_SIZE  10
#define REDUCED_PAYLOAD_SIZE 20


/**
 * Typedef struct containing the payload structure
 * Size: 28 bytes
*/
typedef struct{
  char reserved[28]; // 28 bytes
} pload; //Payload split-up



/**
 * Typedef struct containing the frame structure
 * Size: One frame is 32 bytes
*/
typedef struct
{
  unsigned int  type     :8; // 1 byte
  unsigned char srcAddr;     // 1 byte
  unsigned char destAddr;    // 1 byte
  unsigned char seqNr;       // 1 byte
  pload payload;             // 28 bytes
} frame;

/*============================================================================*/
/*                           EXPORTED DIFINES                                 */
/*============================================================================*/

// Port config for nRF905 ( pin function)
#define TX_EN       5    //TX_EN=1 TX mode, TX_EN=0 RX mode
#define TRX_CE      4    //Enables chip for receive and transmit
#define PWR_UP      3    //Power up chip

#define CD          7     //Carrier Detect
#define AM          9     //Address Match
#define DR          2     //Receive and transmit Data Ready

#define CSN         10    //SPI enable, active low

/*---------------------------------------------------------------------------*/
#define WC                  0x00	//Write Configuration Register Command
#define RC                  0x10	//Read  Configuration Register Command
#define WTP                 0x20	//Write TX Payload Command
#define RTP                 0x21	//Read  TX Payload Command
#define WTA                 0x22	//Write TX Address Command
#define RTA                 0x23	//Read  TX Address Command
#define RRP                 0x24	//Read  RX Payload Command
#define WC3                 0x03        //Write Configuration from byte 3
#define WC4                 0x04        //Write Configuration from byte 4

#define CH_NO_FREQ_422_4MHz  0x000      
#define CH_NO_FREQ_422_5MHz  0x001      
#define CH_NO_FREQ_425_0MHz  0x01a     
#define CH_NO_FREQ_427_5MHz  0x033      

#define CH_NO_FREQ_430_0MHz  0x04c      
#define CH_NO_FREQ_433_0MHz  0x06a      
#define CH_NO_FREQ_433_1MHz  0x06b      
#define CH_NO_FREQ_433_2MHz  0x06c     
#define CH_NO_FREQ_434_7MHz  0x07b      
#define CH_NO_FREQ_473_5MHz  0x1ff      

#define CH_NO_FREQ_844_8MHz  0x000      

#define CH_NO_FREQ_862_0MHz  0x056      
#define CH_NO_FREQ_868_0MHz  0x074      
#define CH_NO_FREQ_868_2MHz  0x075      
#define CH_NO_FREQ_868_4MHz  0x076     
#define CH_NO_FREQ_869_8MHz  0x07d      
#define CH_NO_FREQ_895_8MHz  0x0ff     
#define CH_NO_FREQ_896_0MHz  0x100      
#define CH_NO_FREQ_900_0MHz  0x114      
#define CH_NO_FREQ_902_2MHz  0x11f      
#define CH_NO_FREQ_902_4MHz  0x120      
#define CH_NO_FREQ_915_0MHz  0x15f      
#define CH_NO_FREQ_927_8MHz  0x19f      

#define CH_NO_FREQ_947_0MHz  0x1ff     

#define CH_NO_FREQ           CH_NO_FREQ_433_0MHz
#define CH_NO_BYTE           CH_NO_FREQ & 0xff
#define CH_NO_BIT8           CH_NO_FREQ >> 8

#define PA_PWR__10dBm        0x00                     
#define PA_PWR_2dBm          0x04                     
#define PA_PWR_6dBm          0x08                     
#define PA_PWR_10dBm         0x0c   
                  
#define HFREQ_PLL_433MHz     0x00                      
#define HFREQ_PLL_868MHz     0x02                     
#define HFREQ_PLL_915MHz     0x02                     

#define TX_AFW_1BYTE         0x01*16                      
#define TX_AFW_2BYTE         0x02*16                      
#define TX_AFW_3BYTE         0x03*16                     
#define TX_AFW_4BYTE         0x04*16                      
#define RX_AFW_1BYTE         0x01                    
#define RX_AFW_2BYTE         0x02                     
#define RX_AFW_3BYTE         0x03                      
#define RX_AFW_4BYTE         0x04                      
#define RX_PW_3BYTE          3                         
#define RX_PW_4BYTE          4

#define RX_PW_32BYTE         32                        
#define TX_PW_3BYTE          3                        
#define TX_PW_4BYTE          4

#define TX_PW_32BYTE         32       

#define CRC_MODE_16BIT       0x80                      
#define CRC_MODE_8BIT        0x00                   
#define CRC_EN               0x40                      
#define CRC16_EN             0xc0                      
#define CRC8_EN              0x40                      
#define XOF_20MHz            0x20                      
#define XOF_16MHz            0x18                      
#define XOF_12MHz            0x10                      
#define XOF_8MHz             0x08                     
#define XOF_4MHz             0x00                      
#define UP_CLK_EN            0x40                     
#define UP_CLK_FREQ_500kHz   0x03                      
#define UP_CLK_FREQ_1MHz     0x02                      
#define UP_CLK_FREQ_2MHz     0x01                     
#define UP_CLK_FREQ_4MHz     0x00                      

#define UP_CLK_EN_500kHz     0x43                      
#define UP_CLK_EN_1MHz       0x42                      
#define UP_CLK_EN_2MHz       0x41                      
#define UP_CLK_EN_4MHz       0x40                                      

/*============================================================================*/
/*                      EXPORTED FUNCTION DECLARATION                         */
/*============================================================================*/

int TXPacket(void);


/**
 * Description: configuring the nrf chip
 * @param: void
 * @return: global return code
*/
int nrf(void);


/**
 * Description: initializes the nrf module
 * @param: void
 * @return: global return code
*/
int initNRF(void);


/**
 * Description: set payload width
 * @param: type: TX or RX, pw: size
 * @return: void
*/
void setNRFpw(int type, char pw);


/**
 * Description: clock out the frame
 * @param: type: 
 * @return: void
*/
// frame clockOut(int size);


