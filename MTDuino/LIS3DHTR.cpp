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
#include "LIS3DHTR.h"
#include "Arduino.h"
#include <wire.h>

void LIS3DHTR::begin( void ) {
  SerialUSB.println( "LIS3DHTR begin()" );
}

void LIS3DHTR::init( int Addr ) {
  result = (float *)malloc(3 * sizeof(float));
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register 1
  Wire.write(0x20);
  // Enable X, Y, Z axis, power on mode, data rate selection = 10Hz
  Wire.write(0x27);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register 4
  Wire.write(0x23);
  // Set continuous update, +/- 2g, Self-test enabled
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

float* LIS3DHTR::read( int Addr ) {
  unsigned int data[6];
  for (int i = 0; i < 6; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((40 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);

    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
    delay(300);
  }

  // Convert the data

  result[0] = (float)((data[1] * 256) + data[0]) / (float)256;
  result[1] = (float)((data[3] * 256) + data[2]) / (float)256;
  result[2] = (float)((data[5] * 256) + data[4]) / (float)256;
  return result;
}
