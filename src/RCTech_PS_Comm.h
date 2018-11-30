#include <Stream.h>

class DPS_powerSupply
{
  public:
    void                  setDPS_voltage (uint16_t centivolts);
    void                  setDPS_amperage (uint16_t milliamps);
    void                  setDPS_voltageamperage (uint16_t centivolts, uint16_t milliamps);
    void                  setDPS_outputOn (bool turnOn);
    void                  initDPS(Stream &stream, uint8_t deviceAddress);

  private:
    Stream                 *_stream;
    uint8_t                _deviceAddress;
    uint32_t               _tsLastCommandSent;      //Timestamp of last command sent
    bool                   _lastResponseRecieved;   //Has the last response been recieved?
    void                   sendData(uint8_t functionCode, uint16_t registerAddress, uint16_t registerValue);
    void                   sendData2(uint8_t functionCode, uint16_t registerAddress, uint16_t numberofAddresses, uint8_t numberofBytes, uint16_t registerValue, uint16_t register2Value);
    uint16_t               crc_DPS( const unsigned char *input_str, size_t num_bytes );
    void                   init_crc16_tab( void );
    bool                   crc_tab16_init;
    uint16_t               crc_tab16[256];
    static const uint16_t  CRC_POLY_16 = 0xA001;
    static const uint16_t  CRC_START = 0xFFFF;
  
};