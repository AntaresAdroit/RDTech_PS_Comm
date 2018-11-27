#include <Stream.h>

class DPS_powerSupply
{
  public:
    void                  setDPS_voltage (uint16_t centivolts);
    void                  setDPS_amperage (uint16_t milliamps);
    void                  setDPS_outputOn (bool turnOn);
    void                  initDPS(Stream &stream, uint8_t deviceAddress);

  private:
    Stream                 *_stream;
    uint8_t                _deviceAddress;
    uint32_t               _tsLastCommandSent;      //Timestamp of last command sent
    bool                   _lastResponseRecieved;   //Has the last response been recieved?
    void                   sendData(uint8_t functionCode, uint16_t registerAddress, uint16_t registerValue);
    uint16_t               crc_DPS( const unsigned char *input_str, size_t num_bytes );
    void                   init_crc16_tab( void );
    bool                   crc_tab16_init;
    uint16_t               crc_tab16[256];
    static const uint16_t  CRC_POLY_16 = 0xA001;
    static const uint16_t  CRC_START = 0xFFFF;
  
};



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
  /*old debugging stuff
  for(int i=0;i<8;i++){
    Serial.print(message[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  */
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

}  /* init_crc16_tab */





DPS_powerSupply supply1;

void setup() {
  Serial1.begin(19200);
  Serial.begin(57600);
  delay(1000);
  


  supply1.initDPS(Serial1, 0x01);


}

void loop() {

  supply1.setDPS_voltage(600);
  delay(500);
  supply1.setDPS_amperage(400);
  delay(500);
  supply1.setDPS_outputOn(true);
  delay(7000);
  supply1.setDPS_voltage(900);
  delay(7000);
  supply1.setDPS_outputOn(false);
  delay(500);
  supply1.setDPS_voltage(600);
  delay(500);
  supply1.setDPS_amperage(1000);
  delay(500);
  supply1.setDPS_outputOn(true);
  delay(7000);
  supply1.setDPS_voltage(900);
  delay(7000);
  supply1.setDPS_outputOn(false);
  delay(7000);

  /*
  if (Serial1.available() > 0)
    Serial.print(Serial1.read());


  //Serial.println();
  delay(1);
  */
  
}
