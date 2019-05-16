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

   Amend : Tim Hsu @ QNAP @ 201905016
*/
#include "SI1132.h"
#include "Arduino.h"
#include <wire.h>

void SI1132::begin( void ) {
  SerialUSB.println( "SI1132 begin()" );
}

void SI1132::init( int Addr ) {
  result = (float *)malloc(3 * sizeof(float));
  int response = 0;
  // Enable UVindex measurement coefficients
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COFF-1 register
  Wire.write(0x13);
  // Default value
  Wire.write(0x29);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COFF-2 register
  Wire.write(0x14);
  // Default value
  Wire.write(0x89);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COFF-3 register
  Wire.write(0x15);
  // Default value
  Wire.write(0x02);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COFF-4 register
  Wire.write(0x16);
  // Default value
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();


  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_WR register
  Wire.write(0x17);
  // Enable uv, Visible, IR
  Wire.write(0xF0);
  // Stop I2C Transmission
  Wire.endTransmission();


  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select command register
  Wire.write(0x18);
  // Select CHLIST register in RAM
  Wire.write(0x01 | 0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(10);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_RD register
  Wire.write(0x2E);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  // Read 1 byte of data
  response = Wire.read();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select HW_KEY register
  Wire.write(0x07);
  // Default value
  Wire.write(0x17);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_WR register
  Wire.write(0x17);
  // Small IR photodiode
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COMMAND register
  Wire.write(0x18);
  // Select ALS_IR_ADCMUX register in RAM
  Wire.write(0x0E | 0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(10);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_RD register
  Wire.write(0x2E);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  // Read 1 byte of data
  response = Wire.read();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_WR register
  Wire.write(0x17);
  // Set ADC Clock divided / 1
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COMMAND register
  Wire.write(0x18);
  // Select ALS_IR_ADC_GAIN register in RAM
  Wire.write(0x1E | 0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(10);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_RD register
  Wire.write(0x2E);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  // Read 1 byte of data
  response = Wire.read();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_WR register
  Wire.write(0x17);
  // Set 511 ADC Clock
  Wire.write(0x70);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COMMAND register
  Wire.write(0x18);
  // Select ALS_IR_ADC_COUNTER register in RAM
  Wire.write(0x1D | 0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(10);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_RD register
  Wire.write(0x2E);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  // Read 1 byte of data
  response = Wire.read();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_WR register
  Wire.write(0x17);
  // Set ADC Clock divided / 1
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COMMAND register
  Wire.write(0x18);
  // Select ALS_VIS_ADC_GAIN register in RAM
  Wire.write(0x11 | 0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(10);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_RD register
  Wire.write(0x2E);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  // Read 1 byte of data
  response = Wire.read();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_WR register
  Wire.write(0x17);
  // High Signal Range
  Wire.write(0x20);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COMMAND register
  Wire.write(0x18);
  // Select ALS_IR_ADC_MISC register in RAM
  Wire.write(0x1F | 0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(10);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_RD register
  Wire.write(0x2E);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  // Read 1 byte of data
  response = Wire.read();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_WR register
  Wire.write(0x17);
  // Set 511 ADC Clock
  Wire.write(0x70);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COMMAND register
  Wire.write(0x18);
  // Select ALS_VIS_ADC_COUNTER register in RAM
  Wire.write(0x10 | 0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(10);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_RD register
  Wire.write(0x2E);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  // Read 1 byte of data
  response = Wire.read();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_WR register
  Wire.write(0x17);
  // High Signal Range
  Wire.write(0x20);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COMMAND register
  Wire.write(0x18);
  // Select ALS_VIS_ADC_MISC register in RAM
  Wire.write(0x12 | 0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(10);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select PARAM_RD register
  Wire.write(0x2E);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  // Read 1 byte of data
  response = Wire.read();
  delay(300);
}

float* SI1132::read( int Addr ) {
  unsigned int data[4];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select COMMAND register
  Wire.write(0x18);
  // Start ALS conversion
  Wire.write(0x0E);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(500);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x22);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 4 byte of data
  Wire.requestFrom(Addr, 4);

  // Read 4 bytes of data
  // visible lsb, visible msb, ir lsb, ir msb
  if (Wire.available() == 4)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }

  result[0] = (data[1] * 256.0 + data[0]);
  result[1] = (data[3] * 256 + data[2]);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x2C);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // uv lsb, uv msb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  // Convert the data
  result[2] = (data[1] * 256 + data[0]);
  return result;
}
