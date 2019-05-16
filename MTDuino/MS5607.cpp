/*
   MS5607-02BA03 Barometric Pressure Sensor
   Reference : https://github.com/Schm1tz1/arduino-MS5607

   Author : Paster @ Mighty-Net @ 201809
   Email : paster@might.com.tw

   MTDuino
   https://github.com/Mighty-Net/MTDuino

   DataSheet :
   http://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5607-02BA03%7FB2%7Fpdf%7FEnglish%7FENG_DS_MS5607-02BA03_B2.pdf%7FCAT-BLPS0035

   Interface :
   I2C

   Amend : Tim Hsu @ QNAP @ 201905016
*/
#include "MS5607.h"
#include "Arduino.h"
#include <wire.h>

void MS5607::begin( void ) {
  SerialUSB.println( "MS5607 begin()" );
}

void MS5607::init( int Addr ) {
  result = (double *)malloc(2 * sizeof(double));
  Wire.beginTransmission(Addr);
  Wire.write(MS5607_CMD_RESET);
  Wire.endTransmission();
  delay(3);

  for (int i = 0; i < 8; i++)
  {
    C[i] = 0x0000;
    Wire.beginTransmission(Addr);
    Wire.write(MS5607_CMD_PROM_RD + 2 * i);
    Wire.endTransmission();

    Wire.requestFrom(Addr, 2);

    unsigned int c = Wire.read();
    C[i] = (c << 8);
    c = Wire.read();
    C[i] += c;
    Wire.endTransmission();
  }
}

double* MS5607::read( int Addr ) {

  unsigned long D1 = 0, D2 = 0;

  double dT;
  double OFF;
  double SENS;

  D2 = read_adc(Addr, MS5607_CMD_ADC_D2 + MS5607_CMD_ADC_4096);
  D1 = read_adc(Addr, MS5607_CMD_ADC_D1 + MS5607_CMD_ADC_4096);

  // calculate 1st order pressure and temperature (MS5607 1st order algorithm)
  dT = D2 - C[5] * pow(2, 8);
  OFF = C[2] * pow(2, 17) + dT * C[4] / pow(2, 6);
  SENS = C[1] * pow(2, 16) + dT * C[3] / pow(2, 7);
  TEMP = (2000 + (dT * C[6]) / pow(2, 23));
  P = (((D1 * SENS) / pow(2, 21) - OFF) / pow(2, 15));

  // perform higher order corrections
  double T2 = 0., OFF2 = 0., SENS2 = 0.;
  if (TEMP < 2000) {
    T2 = dT * dT / pow(2, 31);
    OFF2 = 61 * (TEMP - 2000) * (TEMP - 2000) / pow(2, 4);
    SENS2 = 2 * (TEMP - 2000) * (TEMP - 2000);
    if (TEMP < -1500) {
      OFF2 += 15 * (TEMP + 1500) * (TEMP + 1500);
      SENS2 += 8 * (TEMP + 1500) * (TEMP + 1500);
    }
  }

  TEMP -= T2;
  OFF -= OFF2;
  SENS -= SENS2;
  P = (((D1 * SENS) / pow(2, 21) - OFF) / pow(2, 15));
  result[0] = TEMP;
  result[1] = P;
  return result;
}

unsigned long MS5607::read_adc(int Addr, unsigned char aCMD)
{
  unsigned long value = 0;
  unsigned long c = 0;

  Wire.beginTransmission(Addr);
  Wire.write(MS5607_CMD_ADC_CONV + aCMD);
  Wire.endTransmission();

  switch (aCMD & 0x0f)
  {
    case MS5607_CMD_ADC_256 : delayMicroseconds(900);
      break;
    case MS5607_CMD_ADC_512 : delay(3);
      break;
    case MS5607_CMD_ADC_1024: delay(4);
      break;
    case MS5607_CMD_ADC_2048: delay(6);
      break;
    case MS5607_CMD_ADC_4096: delay(10);
      break;
  }
  Wire.beginTransmission(Addr);
  Wire.write(MS5607_CMD_ADC_READ);
  Wire.endTransmission();

  Wire.requestFrom(Addr, 3);
  c = Wire.read();
  value = (c << 16);
  c = Wire.read();
  value += (c << 8);
  c = Wire.read();
  value += c;
  Wire.endTransmission();

  return value;
}
