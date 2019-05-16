/*
    LB-M8Q104A(4-1)(GPS Module) & LB-A8105102A(1-1)(Temperature/Humidity Module) Sample Code for Mighty-Net MTDuino
    Reference : https://github.com/Mighty-Net/MTDuino
*/

#include <Arduino.h>        // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <Wire.h>
#include "SI1132.h"
#include "MS5607.h"
#include "TinyGPS++.h" // You must include the lib : TinyGPSPlus(https://github.com/mikalhart/TinyGPSPlus)
#include "SI7020.h"
#include "LIS3DHTR.h"

#include "Timer.h"

/*
#include <MemoryFree.h>
*/

#define BAUDRATE 9600
#define GPS_MODULE_BM_I2C_ADDR 0x77
#define GPS_MODULE_UV_I2C_ADDR 0x60
#define BT_MODULE_HT_I2C_ADDR 0x40
#define BT_MODULE_3A_I2C_ADDR 0x19

// GPIO
int PIN_GPS_POWER = 7; // GPS power GPIO D7
int PIN_GPS_TX = 25;   // GPS tx GPIO A1
int PIN_GPS_RX = 26;   // GPS rx GPIO A2
int PIN_BT_TX = 10;    // BT tx GPIO D10
int PIN_BT_RX = 11;    // BT rx GPIO D11
int LED_POW = 3;       // sigfox status LED pin D3
int RF_PWEN = 5;       // sigfox power control pin D5

// Library
TinyGPSPlus gps;
SI1132 si1132;
MS5607 ms5607;
SI7020 si7020;
LIS3DHTR lis3dhtr;

//Timer
Timer All_timer;
Timer debug_timer;

Uart Serial3(&sercom4, PIN_GPS_RX, PIN_GPS_TX, SERCOM_RX_PAD_1, UART_TX_PAD_0);

int count, i, INT;
char value;

double *bm;
float *uv;
double lat;
double lng;
float *a3;
float *ht;

char hex_uv[10];
char hex_bm[10];
char hex_t[10];
char hex_h[10];
char hex_lat[10];
char hex_lng[10];
char hex_ax[10];
char hex_ay[10];
char hex_az[10];

void SERCOM4_Handler()
{
  while (sercom4.availableDataUART())
  {
    gps.encode(sercom4.readDataUART());
  }
  Serial3.IrqHandler();
}

void sigfox_atcommand_tx(char *wBuffer)
{
  Serial1.print(wBuffer); // Send Data to backend
  delay(5000);
  sigfox_atcommand_rx(); // Get feedback and show on Terminal function
}

void sigfox_atcommand_rx()
{
  char temp;
  while (Serial1.available() <= 0)
    ;
  while (Serial1.available() > 0)
  {
    temp = Serial1.read();
    SerialUSB.print(temp);
    delay(10);
  }
}

void sigfox_send_data(String data)
{
  char raw_data[20];
  data = "AT$SF=" + data + "\r";
  data.toCharArray(raw_data, 20);
  sigfox_atcommand_tx(raw_data);
}

void debug()
{
  SerialUSB.println(F("Debug info : "));
  SerialUSB.println(F("====================="));
  
  //Memory check
  /*
  SerialUSB.print(F("Free Ram : "));
  SerialUSB.println(freeMemory(), DEC);
  */
  
  // Debug
  SerialUSB.print(F("UV : "));
  SerialUSB.println(String((int)uv[2], HEX));
  SerialUSB.print(F("BM : "));
  SerialUSB.println(String((int)bm[1], HEX));
  SerialUSB.print(F("GPS_LAT : "));
  SerialUSB.println(String((int)(lat * 100), HEX));
  SerialUSB.print(F("GPS_LNG : "));
  SerialUSB.println(String((int)(lng * 100), HEX));
  SerialUSB.print(F("MEMS_X : "));
  SerialUSB.println(String((int)a3[0], HEX));
  SerialUSB.print(F("MEMS_Y : "));
  SerialUSB.println(String((int)a3[1], HEX));
  SerialUSB.print(F("MEMS_Z : "));
  SerialUSB.println(String((int)a3[2], HEX));
  SerialUSB.print(F("T : "));
  SerialUSB.println(String((int)(ht[1] * 100), HEX));
  SerialUSB.print(F("H : "));
  SerialUSB.println(String((int)(ht[0] * 100), HEX));

  SerialUSB.println(F("====================="));
}

void send()
{
  SerialUSB.println(F(""));
  SerialUSB.println(F("+++++++++++++++++++++"));
  SerialUSB.println(F("Send Start !"));

  // send uv,bm
  SerialUSB.print(F("Send Data to Sigfox : "));

  sprintf(hex_uv, "%03x", (int)uv[2]);
  sprintf(hex_bm, "%06x", (int)bm[1]);

  SerialUSB.println(String("0") + String(hex_uv) + String(hex_bm));
  sigfox_send_data(String("0") + String(hex_uv) + String(hex_bm));
  delay(30 * 1000);

  // send humidity,trmperture
  SerialUSB.print(F("Send Data to Sigfox : "));

  sprintf(hex_t, "%04x", (int)(ht[1] * 100));
  sprintf(hex_h, "%04x", (int)(ht[0] * 100));

  SerialUSB.println(String("1") + String(hex_t) + String(hex_h));
  sigfox_send_data(String("1") + String(hex_t) + String(hex_h) + String("0"));
  delay(30 * 1000);

  // send lat,lng
  SerialUSB.print(F("Send Data to Sigfox : "));

  sprintf(hex_lat, "%05x", (int)(lat * 100));
  sprintf(hex_lng, "%06x", (int)(lng * 100));

  SerialUSB.println(String("2") + String(hex_lat) + String(hex_lng));
  sigfox_send_data(String("2") + String(hex_lat) + String(hex_lng));
  delay(30 * 1000);

  // send MEMS
  SerialUSB.print(F("Send Data to Sigfox : "));

  sprintf(hex_ax, "%03x", (int)a3[0]);
  sprintf(hex_ay, "%03x", (int)a3[1]);
  sprintf(hex_az, "%03x", (int)a3[2]);

  SerialUSB.println(String("3") + String(hex_ax) + String(hex_ay) + String(hex_az));
  sigfox_send_data(String("3") + String(hex_ax) + String(hex_ay) + String(hex_az));

  SerialUSB.println(F("Send End !"));
  SerialUSB.println(F("+++++++++++++++++++++"));
}

void setup()
{
  // I2C
  Wire.setClock(400000);
  Wire.begin();

  // UART
  Serial1.begin(BAUDRATE); // Set sigfox Baudrate:9600bps
  Serial3.begin(BAUDRATE); // Set GPS Baudrate:9600bps
  SerialUSB.begin(BAUDRATE);

  // GPS
  SerialUSB.println(F("Init GPS Module...."));
  pinMode(PIN_GPS_POWER, OUTPUT);
  digitalWrite(PIN_GPS_POWER, LOW);
  pinPeripheral(PIN_GPS_TX, PIO_SERCOM_ALT);
  pinPeripheral(PIN_GPS_RX, PIO_SERCOM_ALT);

  // Sigfox
  SerialUSB.println(F("Init Sigfox Module...."));
  pinMode(LED_POW, OUTPUT);    // Set LED_POW pin as output
  pinMode(RF_PWEN, OUTPUT);    // Set RF_PWEN pin as output
  digitalWrite(LED_POW, HIGH); // Set LED_POW to High
  digitalWrite(RF_PWEN, HIGH); // Set RF_PWEN to High, and Sigfox module power ON
  delay(3 * 1000);
  sigfox_atcommand_tx("AT$O=1,4\r");
  delay(100);

  // Sensor
  SerialUSB.println(F("Init Other Sensor...."));
  si1132.begin(); // UV
  si1132.init(GPS_MODULE_UV_I2C_ADDR);
  ms5607.begin(); // BM
  ms5607.init(GPS_MODULE_BM_I2C_ADDR);
  si7020.begin(); // Temperture & Humidity
  si7020.init(BT_MODULE_HT_I2C_ADDR);
  lis3dhtr.begin(); // 3A-MEMS
  lis3dhtr.init(BT_MODULE_3A_I2C_ADDR);

  SerialUSB.println(F("Start"));

  All_timer.after(1000, send);           // power on then send once
  All_timer.every(10 * 60 * 1000, send); // every 10 minute when call send function
  debug_timer.every(2000, debug);
  delay(10 * 1000);
}

void loop()
{
  // UV
  uv = si1132.read(GPS_MODULE_UV_I2C_ADDR);

  // BM
  bm = ms5607.read(GPS_MODULE_BM_I2C_ADDR);

  // GPS
  if (gps.location.lat() > 0 && gps.location.lng() > 0)
  {
    lat = gps.location.lat();
    lng = gps.location.lng();
  }

  // 3A-MEMS
  a3 = lis3dhtr.read(BT_MODULE_3A_I2C_ADDR);

  // Temperture & Humidity
  ht = si7020.read(BT_MODULE_HT_I2C_ADDR, false);

  debug_timer.update();
  All_timer.update();
}
