/*
   SI7020-A20 Humidity and Temperature Sensor
   Modify for Mighty-Net MTDuino from https://github.com/ControlEverythingCommunity/SI7020-A20

   Author : Paster @ Mighty-Net @ 201809
   Email : paster@might.com.tw

   MTDuino
   https://github.com/Mighty-Net/MTDuino

   This code is designed to work with the SI7020-A20_I2CS I2C Mini Module available from ControlEverything.com.
   https://www.controleverything.com/content/Humidity?sku=SI7020-A20_I2CS#tabs-0-product_tabset-2

   DataSheet :
   https://www.silabs.com/documents/public/data-sheets/Si7020-A20.pdf

   Interface :
   I2C

   Amend : Tim Hsu @ QNAP @ 201905016
*/
#ifndef HEADER_SI7020
#define HEADER_SI7020

class SI7020 {
  public:
    void begin( void );
    void init( int Addr );
    float* read( int Addr, bool isFahrenheit );
  private:
    float *result;
};

#endif
