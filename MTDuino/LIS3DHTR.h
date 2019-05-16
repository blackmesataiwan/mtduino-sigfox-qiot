/*
   LIS3DHTR 3-axis accelerometer
   Modify for Mighty-Net MTDuino from https://github.com/ControlEverythingCommunity/LIS3DHTR

   Author : Paster @ Mighty-Net @ 201809
   Email : paster@might.com.tw

   MTDuino
   https://github.com/Mighty-Net/MTDuino

   This code is designed to work with the LIS3DHTR_I2CS I2C Mini Module available from ControlEverything.com.
   https://www.controleverything.com/content/Accelorometer?sku=LIS3DHTR_I2CS#tabs-0-product_tabset-2

   DataSheet :
   https://www.st.com/resource/en/datasheet/cd00274221.pdf

   Interface :
   I2C

   Amend : Tim Hsu @ QNAP @ 201905016
*/
#ifndef HEADER_LIS3DHTR
#define HEADER_LIS3DHTR

class LIS3DHTR {
  public:
    void begin( void );
    void init( int Addr );
    float* read( int Addr );
  private:
    float *result;
};

#endif
