/*  ============================================================================
    Copyright (C) 2015 Achuthan Paramanathan.
    ============================================================================
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.
    ============================================================================
    Revision Information:
        File name: nRF905_driver.cpp
        Version:   v0.0
        Date:      05-04-2015
    ==========================================================================*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS                               */
/*============================================================================*/

#include "Arduino.h"
#include "SPI/SPI.h"
#include "nRF905_driver.h"



/*============================================================================*/
/*                   LOCAL EXPORTED FUNCTION DECLARATIONS                     */
/*============================================================================*/


/*============================================================================*/
/*                           PRIVATE DIFINES                                  */
/*============================================================================*/

/**
 * nRF payload buffer types
*/
#define RX 0
#define TX 1

/*============================================================================*/
/*                           LOCAL VARIABLES                                  */
/*============================================================================*/


/*============================================================================*/
/*                           Hardware configuration                           */
/*============================================================================*/



/*==============================================================================
** Function...: nrf
** Return.....: int
** Description: initializes the pins used by NRF
** Created....: 05.04.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
int nrf(void)
{
  
  pinMode(CSN, OUTPUT);
  
  
  pinMode(DR, INPUT);	// Init DR for input
  pinMode(AM, INPUT);// Init AM for input  
  pinMode(CD, INPUT);// Init CD for input
  pinMode(PWR_UP, OUTPUT);
  
  digitalWrite(CSN, HIGH); // Spi disable
  digitalWrite(PWR_UP, HIGH);// nRF905 power on
  
  pinMode(TRX_CE, OUTPUT);
  digitalWrite(TRX_CE, LOW);// Set nRF905 in standby mode
  
  pinMode(TX_EN, OUTPUT);
  digitalWrite(TX_EN, LOW);// set radio in Rx mode
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin();
  
  /** wait for nrf905 STANDBY */
  delay(3);
  
  return 0;
}


/*------------------------------------------------------------------------------
** Function...: nrfConf
** Return.....: GLOB_RET
** Description: configure nrf chip
** Created....: 05.04.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
int initNRF(void)
{
  int i;
  char *nRFAddress = "MOTE";
  unsigned char nRFConfig[10];
  
  nRFConfig[0] = CH_NO_BYTE; //Sets center frequency together with HFREQ_PLL
  //Output power, Band 433 or 868/915 MHz
  nRFConfig[1] = PA_PWR_2dBm|HFREQ_PLL_433MHz|CH_NO_BIT8;
  nRFConfig[2] = TX_AFW_4BYTE|RX_AFW_4BYTE; //Tx and Rx Address width
  nRFConfig[3] = RX_PW_4BYTE; //RX payload width
  nRFConfig[4] = TX_PW_4BYTE; //Tx payload width
  nRFConfig[5] = nRFAddress[0];
  nRFConfig[6] = nRFAddress[1];
  nRFConfig[7] = nRFAddress[2];
  nRFConfig[8] = nRFAddress[3];
  nRFConfig[9] = CRC8_EN|XOF_16MHz; //CRC check. Crystal oscillator frequency.

  // Spi enable for write a spi command
  digitalWrite(CSN, LOW); // Spi Enabled
  
  //send nRF configuration
  SPI.transfer(WC);
  for(i=0;i<10;i++)
  {
    SPI.transfer(nRFConfig[i]);
  }
  
  digitalWrite(CSN,HIGH);	// Disable Spi
  
  // Spi enable for write a spi command
  digitalWrite(CSN, LOW); // Spi Enabled
  
  //send Write TX Address
  SPI.transfer(WTA);
  for(i=0;i<10;i++)
  {
    SPI.transfer(nRFAddress[i]);
  }
  
  digitalWrite(CSN,HIGH);	// Disable Spi
  
  digitalWrite(TRX_CE,HIGH);
  
  return 0;
}


/*------------------------------------------------------------------------------
** Function...: setNRFpw
** Return.....: void
** Description: set payload width
** Created....: 05.04.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/

void setNRFpw(int type, char pw)
{
  //configure RX payload width
  if(type == RX)
  {
    digitalWrite(CSN, LOW); // Spi Enabled
    
    SPI.transfer(WC3); //Write to nRF configuration bit 3
    delay(1);
    SPI.transfer(pw);
    
    digitalWrite(CSN,HIGH);	// Disable Spi
    
  }
  //configure TX payload width
  else if(type == TX)
  {
    digitalWrite(CSN, LOW); // Spi Enabled
    
    SPI.transfer(WC4); //Write to nRF configuration bit 4
    delay(1);
    SPI.transfer(pw);
    
    digitalWrite(CSN,HIGH);	// Disable Spi
  } // end else if
}


/*------------------------------------------------------------------------------
** Function...: TXPacket
** Return.....: void
** Description: transmit frame
** Created....: 05.04.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
int TXPacket(void)
{
  
  digitalWrite(TRX_CE,HIGH); // Start transmitting
  while(!DR);
  digitalWrite(TX_EN,LOW);

  return 0;
}


/*------------------------------------------------------------------------------
** Function...: clockIn
** Return.....: int
** Description: clock payload in to nrf register
** Created....: 05.04.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
int clockIn(frame packet, int size)
{
  char *p = (char *)&packet;
  int i;
  
  digitalWrite(TRX_CE,LOW);
  digitalWrite(TX_EN,HIGH);
  digitalWrite(CSN,LOW);	// Disable Spi

  SPI.transfer(WTP); //Write TX Payload

  for(i=0;i<size;i++)
  SPI.transfer(p[i]);
  digitalWrite(CSN,HIGH);

  return 0;
}


/*------------------------------------------------------------------------------
** Function...: clockOut
** Return.....: frame
** Description: clock payload out from nrf register
** Created....: 05.04.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
frame clockOut(int size)
{
  //The same as RXpacket used before
  int i;
  char SpiRxBuf[PACKET_SIZE];
  frame *f = (frame *)&SpiRxBuf;

  digitalWrite(TRX_CE,LOW);
  digitalWrite(CSN,LOW);
  
  SPI.transfer(RRP); //Read RX Payload
  for (i=0;i<size;i++)
  SpiRxBuf[i] = SPI.transfer(0);
  digitalWrite(CSN,HIGH);
  digitalWrite(TRX_CE,HIGH);
  
  return f[0];
}