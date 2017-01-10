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

//---------------------------=[Global variables]=----------------------------

//--------------------------[Global Task variables]--------------------------
uint32_t prev_millis_1s_timer = 0;

const uint16_t INTERVAL_1S_TIMER = 2500;        // interval at which to blink (milliseconds)

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
  Serial.begin(9600); Serial.println();

  // init CC1101 RF-module and get My_address from EEPROM
  if(cc1100.begin(CC1100_MODE_GFSK_38_4_kb, CC1100_FREQ_868MHZ, 1, 10, 1)) // modulation mode, frequency, channel, PA level in dBm, own address
  {
    Serial.println(F("Init successful"));
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
  cc1100.receive();                        //set to RECEIVE mode

  // init interrrupt function for available packet
  //attachPinChangeInterrupt(GDO2, rf_available_int, HIGH);

  Serial.println(F("CC1101 TX Demo"));   //welcome message
}
//---------------------------------[LOOP]-----------------------------------
void loop()
{

  // one second update timer
  if (millis() - prev_millis_1s_timer >= INTERVAL_1S_TIMER)
  {
    Rx_addr = 0x03;                                              //receiver address

    uint32_t time_stamp = millis();                              //generate time stamp

    Tx_fifo[3] = (uint8_t)(time_stamp >> 24);                    //split 32-Bit timestamp to 4 byte array
    Tx_fifo[4] = (uint8_t)(time_stamp >> 16);
    Tx_fifo[5] = (uint8_t)(time_stamp >> 8);
    Tx_fifo[6] = (uint8_t)(time_stamp);

    Pktlen = 0x07;                                                //set packet len to 0x13

    //detachPinChangeInterrupt(GDO2);                             //disable pin change interrupt
    cc1100.send_packet(My_addr, Rx_addr, Tx_fifo, Pktlen, 0);     //sents package over air. ACK is received via GPIO polling
    
    cc1100.spi_write_strobe(SFRX);                                //flush the RX_fifo content
    _delay_us(100); 
    
    //attachPinChangeInterrupt(GDO2, rf_available_int, HIGH);      //enable pin change interrupt

    Serial.print(F("Tx time: ")); Serial.print(millis() - time_stamp); Serial.println(F("ms"));
    prev_millis_1s_timer = millis();

    //cc1100.get_tempK();
  }

}
//--------------------------[end loop]----------------------------

//-----interrrupt function not needed for this demo
//---------------------[ check incomming RF packet ]-----------------------
void rf_available_int(void)
{
  detachInterrupt(GDO2);

  if (cc1100.packet_available() == TRUE) {
    cc1100.get_payload(Rx_fifo, pktlen, rx_addr, sender, rssi_dbm, lqi); //stores the payload data to Rx_fifo
    cc1101_packet_available = TRUE;                                      //set flag that an package is in RX buffer
  }

  attachInterrupt(GDO2, rf_available_int, HIGH);
}
