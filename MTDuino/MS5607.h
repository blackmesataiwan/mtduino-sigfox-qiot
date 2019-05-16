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

#define MS5607_CMD_RESET	0x1E    // perform reset
#define MS5607_CMD_ADC_READ 0x00    // initiate read sequence
#define MS5607_CMD_ADC_CONV 0x40    // start conversion
#define MS5607_CMD_ADC_D1   0x00    // read ADC 1
#define MS5607_CMD_ADC_D2   0x10    // read ADC 2
#define MS5607_CMD_ADC_256  0x00    // set ADC oversampling ratio to 256
#define MS5607_CMD_ADC_512  0x02    // set ADC oversampling ratio to 512
#define MS5607_CMD_ADC_1024 0x04    // set ADC oversampling ratio to 1024
#define MS5607_CMD_ADC_2048 0x06    // set ADC oversampling ratio to 2048
#define MS5607_CMD_ADC_4096 0x08    // set ADC oversampling ratio to 4096
#define MS5607_CMD_PROM_RD  0xA0    // initiate readout of PROM registers

#ifndef HEADER_MS5607
#define HEADER_MS5607
class MS5607 {
  public:
    void begin( void );
    void init( int Addr );
    double* read( int Addr );
    unsigned long read_adc(int Addr, unsigned char aCMD);

  private:
    unsigned int C[8];
    double P;
    double TEMP;
    double *result;
};

#endif
