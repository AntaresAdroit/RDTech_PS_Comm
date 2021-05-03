//Tested on a Pololu A-Star 32U4 board which used the ATmega 32U4 Microcontroller.
//Serial1 is the hardware serial port on the board and utilizes digital pins 0 and 1.

#include <Stream.h>
#include <RCTech_PS_Comm.h>

DPS_powerSupply supply1;

void setup() {
  Serial1.begin(19200);
  //Serial.begin(57600);
  delay(1000);

  supply1.initDPS(Serial1, 0x01);
}

void loop() {

  supply1.setDPS_voltage(600);
  delay(500);
  supply1.setDPS_amperage(400);
  delay(500);
  supply1.setDPS_outputOn(true);
  delay(3000);
  supply1.setDPS_outputOn(false);
  delay(3000);
  supply1.setDPS_voltageamperage(700, 1000);
  delay(500);
  supply1.setDPS_outputOn(true);
  delay(3000);
  supply1.setDPS_outputOn(false);
  delay(3000);
  supply1.setDPS_fourStates(500,500,true,true);
  delay(3000);
  supply1.setDPS_fourStates(800,200,false,false);
  delay(10000);
}




