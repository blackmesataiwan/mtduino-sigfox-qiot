/*
   SI1132 UV Sensor and Light Sensor
   Modify for Mighty-Net MTDuino from https://github.com/ControlEverythingCommunity/SI1132

   Author : Paster @ Mighty-Net @ 201809
   Email : paster@might.com.tw

   MTDuino
   https://github.com/Mighty-Net/MTDuino

   This code is designed to work with the SI1132_I2CS I2C Mini Module available from ControlEverything.com.
   https://www.controleverything.com/content/Light?sku=SI1132_I2CS#tabs-0-product_tabset-2

   DataSheet :
   https://www.silabs.com/documents/public/data-sheets/Si1132.pdf

   Interface :
   I2C
*/
#ifndef HEADER_SI1132
#define HEADER_SI1132

class SI1132 {
  public:
    void begin( void );
    void init( int Addr );
    float* read( int Addr );
  private:
};

#endif
