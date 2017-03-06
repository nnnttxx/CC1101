/*-----------------------------------------------------------------------------
  '                     RX_DEMO
  '                     -------
  '  - needs PinChangeInt library
  '  - *.eep file must flashed to Arduino first -> use eeprom Tool
  '
  '
  '
  '
  '-----------------------------------------------------------------------------*/
#include <cc1100.h>

//---------------------------=[Global variables]=----------------------------
uint32_t rf_timecode = 0;
uint32_t rf_timecode_backup = 0;
//--------------------------[Global Task variables]--------------------------

//--------------------------[Global CC1100 variables]------------------------
uint8_t Tx_fifo[FIFOBUFFER], Rx_fifo[FIFOBUFFER];
uint8_t My_addr, Tx_addr, Rx_addr, Pktlen, pktlen, Lqi, Rssi;
uint8_t rx_addr, sender, lqi;
int8_t rssi_dbm;
volatile uint8_t cc1101_packet_available;

//--------------------------[class constructors]-----------------------------
//init CC1100 constructor
CC1100 cc1100;

//---------------------------------[SETUP]-----------------------------------
void setup()
{
  // init serial Port for debugging
  Serial.begin(115200); Serial.println();

  // init CC1101 RF-module and get My_address from EEPROM
  if (cc1100.begin(CC1100_MODE_GFSK_1_2_kb, CC1100_FREQ_868MHZ, 1, 10, 3))    // modulation mode, frequency, channel, PA level in dBm, own address
  {
    Serial.println(F("Init successful"));
  }

  //  cc1100.sidle();                          //set to ILDE first
  //  cc1100.set_mode(CC1100_MODE_GFSK_1_2_kb); //set modulation array mode
  //  cc1100.set_ISM(CC1100_FREQ_868MHZ);      //set frequency
  //  cc1100.set_channel(0x01);                //set channel
  //  cc1100.set_output_power_level(0);        //set PA level in dbm
  //  cc1100.set_myaddr(0x03);                 //set my own address

  cc1100.spi_write_register(IOCFG2, 0x07);  //Asserts when a packet has been received with CRC OK. 
                                            //De-asserts when the first byte is read from the RX FIFO. 

  cc1100.show_main_settings();             //shows setting debug messages to UART
  cc1100.show_register_settings();         //shows current CC1101 register values
  cc1100.receive();                        //set to RECEIVE mode

  Serial.println(F("CC1101 RX Demo"));   //welcome message

  // init interrrupt function for available packet
  attachInterrupt(digitalPinToInterrupt(GDO2), rf_available_int, RISING);
}

//---------------------------------[LOOP]-----------------------------------
void loop()
{
  //if valid package is received
  if (cc1101_packet_available == TRUE) {
    rf_timecode = ((uint32_t)Rx_fifo[3] << 24) +
                  ((uint32_t)Rx_fifo[4] << 16) +
                  ((uint16_t)Rx_fifo[5] <<  8) +
                  Rx_fifo[6];
    Serial.print(F("TX_Timecode: ")); Serial.print(rf_timecode); Serial.println(F("ms"));

    rf_timecode_backup = millis();
    cc1101_packet_available = FALSE;

  /*
    // measure temperature
    cc1100.get_tempK(); 
    cc1100.receive();
    */
  }

}
//--------------------------[end loop]----------------------------

//---------------------[ check incomming RF packet ]-----------------------
void rf_available_int(void)
{
  detachInterrupt(digitalPinToInterrupt(GDO2));
  //uint32_t time_stamp = millis();                                   //generate time stamp

  if (cc1100.packet_available())
  {
    cc1100.get_payload(Rx_fifo, pktlen, rx_addr, sender, rssi_dbm, lqi); //stores the payload data to Rx_fifo
    cc1101_packet_available = TRUE;                                      //set flag that an package is in RX buffer
  }
  //Serial.print(F("RX duration: ")); Serial.print(millis() - time_stamp); Serial.println(F("ms")); Serial.println();

  attachInterrupt(digitalPinToInterrupt(GDO2), rf_available_int, RISING);
}
