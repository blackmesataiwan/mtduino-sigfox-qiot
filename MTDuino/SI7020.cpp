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
*/
#include "SI7020.h"
#include "Arduino.h"
#include <wire.h>

void SI7020::begin( void ) {
  SerialUSB.println( "SI7020 begin()" );
}

void SI7020::init( int Addr ) {
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
}

float* SI7020::read( int Addr, bool isFahrenheit ) {
  float* result = (float*) malloc(2 * sizeof(float));
  unsigned int data[2];

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send humidity measurement command, NO HOLD MASTER
  Wire.write(0xF5);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // humidity msb, humidity lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  float humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;
  result[0] = humidity;

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send temperature measurement command, NO HOLD MASTER
  Wire.write(0xF3);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // temp msb, temp lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  float temp  = ((data[0] * 256.0) + data[1]);
  float ctemp = ((175.72 * temp) / 65536.0) - 46.85;
  float ftemp = ctemp * 1.8 + 32;
  result[1] = ctemp;
  if (isFahrenheit) {
    result[1] = ftemp;
  }
  return result;
}
