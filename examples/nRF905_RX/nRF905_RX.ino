
#include <NRF905.h>
#include <SPI.h>
#define BUF_LEN          32
#define CONF_LEN         10

//NRF905 nrf905;

unsigned char rx_buf[BUF_LEN]= {0};
unsigned char read_config_buf[CONF_LEN];
unsigned char rx_address[4]= {0xcc,0xcc,0xcc,0xcc};

void putstring(unsigned char *str)
{
    while(*str){
        Serial.write(*str++);
    }
}

void setup()
{
    char i;
    pinMode(10,OUTPUT);
    nrf905=NRF905(10);
    
    /** pin/port configuration */
    nrf905.init(); 

  
    /***************************************************
        default configuration, need to specify frequency
        choose Z-Wave frequency band, support :
        US		908.42Mhz
        EUROPE		868.42MHz
        AFRICA		868.42MHz
        CHINA		868.42MHz
        HK		919.82MHz
        JAPAN		853.42MHz
        AUSTRALIA	921.42MHz
        NEW_ZEALAND	921.42MHz
        BRASIL		921.42MHz
        RUSSIA		896MHz
    */ 
	nrf905.write_config(US); 	
    
    /***********************************************************
	read register configuration, check register value written */
    nrf905.read_config(read_config_buf);
    
    /** serial communication configurate */
    Serial.begin(9600);
    
    /** test configuration */
    for(i=0; i<CONF_LEN; i++){
        Serial.print(read_config_buf[i],HEX);
        Serial.print(' ');
    }
    
}

void loop()
{
    /** recieve data packet with default RX address */
    nrf905.RX(rx_buf);
    
    /** recieve data packet with specified RX address */
//    nrf905.RX(rx_buf, rx_address );
    
    // NOTE: TX_Address and RX_Address must be the same
    
    /** send recieved data to PC through serial port */
    putstring(rx_buf);
    delay(1);
}

