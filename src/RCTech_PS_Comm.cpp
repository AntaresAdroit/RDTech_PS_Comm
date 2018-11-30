#include <Arduino.h>
#include <RCTech_PS_Comm.h>

void DPS_powerSupply::initDPS (Stream &stream,uint8_t deviceAddress){
  _stream = &stream;
  _deviceAddress = deviceAddress;
  crc_tab16_init = false;
}

//Pass in centivolts (2400 = 24.00V, for example)
void DPS_powerSupply::setDPS_voltage (uint16_t centivolts){
  sendData(0x06, 0x0000, centivolts);
}

//Pass in milliamps (5000 = 5.00A, for example)
void DPS_powerSupply::setDPS_amperage (uint16_t milliamps){
  sendData(0x06, 0x0001, milliamps);
}

//Pass in milliamps (5000 = 5.00A, for example)
void DPS_powerSupply::setDPS_voltageamperage (uint16_t centivolts, uint16_t milliamps){
  sendData2(0x10, 0x0000, 0x0002, 0x04, centivolts, milliamps);
}  

//Turns output from supply on if true, off if false
void DPS_powerSupply::setDPS_outputOn (bool turnOn){
  sendData(0x06, 0x0009, turnOn);
}

//sends data to the power supply
void DPS_powerSupply::sendData(uint8_t functionCode, uint16_t registerAddress, uint16_t registerValue){
  byte message[] = {_deviceAddress, functionCode, registerAddress>>8, registerAddress, registerValue>>8, registerValue};
  uint16_t crcVal = crc_DPS(message, 6);
  message[6] = crcVal>>8;
  message[7] = crcVal;
  Serial1.write(message, 8);
}

//sends data to the power supply
void DPS_powerSupply::sendData2(uint8_t functionCode, uint16_t registerAddress, uint16_t numberofAddresses, uint8_t numberofBytes, uint16_t registerValue, uint16_t register2Value){
  byte message[] = {_deviceAddress, functionCode, registerAddress>>8, registerAddress, numberofAddresses>>8, numberofAddresses, numberofBytes, registerValue>>8, registerValue, register2Value>>8, register2Value};
  uint16_t crcVal = crc_DPS(message, 11);
  message[11] = crcVal>>8;
  message[12] = crcVal;
  Serial1.write(message, 13);
}





//CRC function
//Modified from https://github.com/lammertb/libcrc/blob/master/src/crc16.c
//which is is licensed under the MIT License
uint16_t DPS_powerSupply::crc_DPS( const unsigned char *input_str, size_t num_bytes ) {

  uint16_t crc;
  const unsigned char *ptr;
  size_t a;

  if ( ! crc_tab16_init ) 
    init_crc16_tab();

  crc = CRC_START;
  ptr = input_str;

  if ( ptr != NULL ) 
    for (a=0; a<num_bytes; a++) {
      crc = (crc >> 8) ^ crc_tab16[ (crc ^ (uint16_t) *ptr++) & 0x00FF ];
    }

  
  uint8_t  crc_a   = crc;
  uint8_t  crc_b   = crc>>8;
  crc = crc_a<<8;
  crc = crc + crc_b;
 
  return crc;
}

//CRC Table building function
static void DPS_powerSupply::init_crc16_tab( void ) {

  uint16_t i;
  uint16_t j;
  uint16_t crc;
  uint16_t c;

  for (i=0; i<256; i++) {
    crc = 0;
    c   = i;
    for (j=0; j<8; j++) {
      if ( (crc ^ c) & 0x0001 ) 
        crc = ( crc >> 1 ) ^ CRC_POLY_16;
      else                      
        crc =   crc >> 1;
      c = c >> 1;
    }
    crc_tab16[i] = crc;
  }

  crc_tab16_init = true;

}


