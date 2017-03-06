/*-----------------------------------------------------------------------------
  '                     TX DEMO
  '                     -------
  '
  '
  '  - need PinChangeInt library
  '  - *.eep file must flashed to Arduino first -> use eeprom Tool
  '
  '
  '-----------------------------------------------------------------------------*/
#include <cc1100.h>
#include "Sleeper.h"

//---------------------------=[Global variables]=----------------------------

//--------------------------[Global Task variables]--------------------------
uint32_t prev_millis_1s_timer = 0;

const uint16_t INTERVAL_1S_TIMER = 2500;        // interval at which to blink (milliseconds)

//--------------------------[Global CC1100 variables]------------------------
uint8_t Tx_fifo[FIFOBUFFER], Rx_fifo[FIFOBUFFER];
uint8_t My_addr, Rx_addr, Pktlen, Lqi, Rssi;

//--------------------------[class constructors]-----------------------------
//init CC1100 constructor
CC1100 cc1100;

Sleeper g_sleeper;
/* usage
  // Power down for 10 seconds.
  g_sleeper.SleepMillis(10000);
*/


//---------------------------------[SETUP]-----------------------------------
void setup()
{
  // init serial Port for debugging
  Serial.begin(115200); Serial.println();

  // init CC1101 RF-module and get My_address from EEPROM
  if (cc1100.begin(CC1100_MODE_GFSK_1_2_kb, CC1100_FREQ_868MHZ, 1, 10, 1)) // modulation mode, frequency, channel, PA level in dBm, own address
  {
#ifndef CC1100_DEBUG
    Serial.println(F("Init successful"));
#endif
  }

  /*
    cc1100.sidle();                          //set to ILDE first
    cc1100.set_mode(CC1100_MODE_GFSK_1_2_kb); //set modulation array mode
    cc1100.set_ISM(CC1100_FREQ_868MHZ);      //set frequency
    cc1100.set_channel(0x01);                //set channel
    cc1100.set_output_power_level(0);        //set PA level in dbm
    cc1100.set_myaddr(0x01);                 //set my own address
  */

  cc1100.show_main_settings();             //shows setting debug messages to UART
  cc1100.show_register_settings();         //shows current CC1101 register values
  My_addr = cc1100.get_myaddr();
  cc1100.powerdown();

  Rx_addr = 0x03;                          //receiver address
  Pktlen = 0x07;                           //set packet len to 0x13

  Serial.println(F("CC1101 TX Demo"));   //welcome message
}
//---------------------------------[LOOP]-----------------------------------
void loop()
{
  uint32_t time_stamp = millis();                              //generate time stamp

  Tx_fifo[3] = (uint8_t)(time_stamp >> 24);                    //split 32-Bit timestamp to 4 byte array
  Tx_fifo[4] = (uint8_t)(time_stamp >> 16);
  Tx_fifo[5] = (uint8_t)(time_stamp >> 8);
  Tx_fifo[6] = (uint8_t)(time_stamp);

  //detachPinChangeInterrupt(GDO2);                             //disable pin change interrupt
  cc1100.wakeup();
  cc1100.send_packet(My_addr, Rx_addr, Tx_fifo, Pktlen, 0);     //sents package over air. ACK is received via GPIO polling
  cc1100.powerdown();

  Serial.print(F("Tx time: ")); Serial.print(millis() - time_stamp); Serial.println(F("ms"));
  prev_millis_1s_timer = millis();

  //cc1100.get_tempK();
  g_sleeper.SleepMillis(1000);
}
