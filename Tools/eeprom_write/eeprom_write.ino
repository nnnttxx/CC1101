//----------------------------------------------------------------------------//
//                 Write eeprom data to internal EEPROM memory                //
//                 by Chris                                                   //
//                                                                            //
//----------------------------------------------------------------------------//

#include <EEPROM.h>
#include <cc1100.h>

#define CC1100_FREQUENCY CC1100_FREQ_868MHZ
#define CC1100_MODE      CC1100_MODE_MSK_250_kb
#define CC1100_MY_ADDR   0x05
#define CC1100_CHANNEL   0x01

int a = 0;
int value;

//-----------------------------------------------------------------------------

//------------------[ paste data from array python tool here ]-----------------
#define EEPROM_LEN 313
uint8_t eeprom_cc1101[EEPROM_LEN] = {
  //00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
  0x06, 0x2E, 0x06, 0x47, 0xD3, 0x91, 0xFF, 0x04, 0x05, 0x00, 0x00, 0x06, 0x00, 0x21, 0x65, 0x6A,
  0x87, 0x83, 0x3B, 0x22, 0xF8, 0x15, 0x07, 0x30, 0x18, 0x14, 0x6C, 0x07, 0x00, 0x92, 0x87, 0x6B,
  0xFB, 0x56, 0x17, 0xE9, 0x2A, 0x00, 0x1F, 0x41, 0x00, 0x59, 0x7F, 0x3F, 0x81, 0x35, 0x09, 0x07,
  0x2E, 0x80, 0x07, 0x57, 0x43, 0x3E, 0x0E, 0x45, 0xFF, 0x00, 0x0C, 0x00, 0x21, 0x65, 0x6A, 0x0E,
  0x3B, 0x73, 0xA0, 0xF8, 0x00, 0x07, 0x0C, 0x18, 0x1D, 0x1C, 0xC7, 0x40, 0xB2, 0x02, 0x26, 0x09,
  0xB6, 0x17, 0xEA, 0x0A, 0x00, 0x19, 0x41, 0x00, 0x59, 0x7F, 0x3F, 0x81, 0x3F, 0x0B, 0x07, 0x2E,
  0x80, 0x07, 0x57, 0x43, 0x3E, 0x0E, 0x45, 0xFF, 0x00, 0x0B, 0x00, 0x21, 0x65, 0x6A, 0x2D, 0x3B,
  0x73, 0xA0, 0xF8, 0x00, 0x07, 0x0C, 0x18, 0x1D, 0x1C, 0xC7, 0x00, 0xB2, 0x02, 0x26, 0x09, 0xB6,
  0x17, 0xEA, 0x0A, 0x00, 0x11, 0x41, 0x00, 0x59, 0x7F, 0x3F, 0x81, 0x3F, 0x0B, 0x07, 0x2E, 0x80,
  0x07, 0x57, 0x43, 0x3E, 0x0E, 0x45, 0xFF, 0x00, 0x08, 0x21, 0x65, 0x6A, 0x5B, 0xF8, 0x13, 0xA0,
  0xF8, 0x47, 0x07, 0x0C, 0x18, 0x1D, 0x1C, 0xC7, 0x00, 0xB2, 0x02, 0x26, 0x09, 0xB6, 0x17, 0xEA,
  0x0A, 0x00, 0x11, 0x41, 0x00, 0x59, 0x7F, 0x3F, 0x81, 0x3F, 0x0B, 0x07, 0x2E, 0x80, 0x07, 0x57,
  0x43, 0x3E, 0x0E, 0x45, 0xFF, 0x00, 0x06, 0x00, 0x21, 0x65, 0x6A, 0xCA, 0x83, 0x13, 0xA0, 0xF8,
  0x34, 0x07, 0x0C, 0x19, 0x16, 0x6C, 0x43, 0x40, 0x91, 0x02, 0x26, 0x09, 0x56, 0x17, 0xA9, 0x0A,
  0x00, 0x11, 0x41, 0x00, 0x59, 0x7F, 0x3F, 0x81, 0x3F, 0x0B, 0x07, 0x2E, 0x80, 0x07, 0x57, 0x43,
  0x3E, 0x0E, 0x45, 0xFF, 0x00, 0x08, 0x00, 0x21, 0x65, 0x6A, 0xF5, 0x83, 0x13, 0xA0, 0xF8, 0x15,
  0x07, 0x0C, 0x19, 0x16, 0x6C, 0x03, 0x40, 0x91, 0x02, 0x26, 0x09, 0x56, 0x17, 0xA9, 0x0A, 0x00,
  0x11, 0x41, 0x00, 0x59, 0x7F, 0x3F, 0x81, 0x3F, 0x0B, 0x0B, 0x1B, 0x6D, 0x67, 0x50, 0x85, 0xC9,
  0xC1, 0x03, 0x17, 0x1D, 0x26, 0x50, 0x86, 0xCD, 0xC0, 0x6C, 0x1C, 0x06, 0x3A, 0x51, 0x85, 0xC8,
  0xC0, 0x17, 0x1D, 0x26, 0x69, 0x51, 0x86, 0xCC, 0xC3
};
//----------------------------------[ end ]------------------------------------

//-----------------------------------------------------------------------------
void setup() {

  // put your setup code here, to run once:
  Serial.begin(38400);
  Serial.println(F("EEPROM write sketch"));
  Serial.println(F("==================="));
  Serial.println();

  show_settings();
  show_menu();
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'r') {
      read_eeprom();
      show_menu();
    }
    if (c == 'w') {
      write_eeprom();
      show_menu();
    }
    if (c == 'b') {
      blank_eeprom();
      show_menu();
    }
  }
}


void blank_eeprom(void) {
  Serial.print(F("erase EEPROM..."));

  uint16_t count = 0;
  for (int i = 0; i < EEPROM_LEN; i++) {
    EEPROM.write(i, 0xFF);
    count++;
    Serial.print(F("."));
    if (count == 16) {
      Serial.println();
      count = 0;
    }
  }
  Serial.println();
  Serial.println(F("erase done!")); Serial.println();
}


void write_eeprom(void) {
  Serial.print(F("write EEPROM..."));

  uint16_t count = 0;
  for (int i = 0; i < EEPROM_LEN; i++) {
    EEPROM.write(i, eeprom_cc1101[i]);
    count++;
    Serial.print(F("."));
    if (count == 16) {
      Serial.println();
      count = 0;
    }
  }
  Serial.println();
  Serial.println(F("write done!")); Serial.println();
}


void read_eeprom(void) {
  Serial.println(F("read EEPROM...")); Serial.println();
  char hex_value[4];
  uint16_t newline;

  newline = 0;
  Serial.println(F("uint8_t eeprom = {"));
  for (int i = 0; i < EEPROM_LEN; i++) {
    value = EEPROM.read(i);
    sprintf(hex_value, "%02x", value);
    Serial.print("0x"); Serial.print(hex_value);
    if (i < EEPROM_LEN - 1) {
      Serial.print(F(","));
    }
    else {
      Serial.print(F("}"));
    }
    newline++;
    if (newline == 16) {
      Serial.println();
      newline = 0;
    }

  }
  Serial.println(); Serial.println(F("read done...")); Serial.println();
}

void show_menu(void)
{
  Serial.println(F("*------ Menu -------*"));
  Serial.println(F("*press 'w' for write*"));
  Serial.println(F("*press 'r' for read *"));
  Serial.println(F("*press 'b' for blank*"));
  Serial.println(F("====================="));
  Serial.println(); Serial.println();
}

void show_settings(void)
{
  Serial.println(F("Settings:"));

  //ISM Frequency - default 868MHz
  eeprom_cc1101[EEPROM_ADDRESS_CC1100_FREQUENCY] = CC1100_FREQUENCY;
  Serial.print(F("ISM Mode: ")); Serial.println(eeprom_cc1101[EEPROM_ADDRESS_CC1100_FREQUENCY]);

  //Modulation Mode - default 250 kbit
  eeprom_cc1101[EEPROM_ADDRESS_CC1100_MODE] = CC1100_MODE;
  Serial.print(F("Modulation Mode: ")); Serial.println(eeprom_cc1101[EEPROM_ADDRESS_CC1100_MODE]);

  //My Address - default address 1
  eeprom_cc1101[EEPROM_ADDRESS_CC1100_MY_ADDR] = CC1100_MY_ADDR;
  Serial.print(F("My Addr: ")); Serial.println(eeprom_cc1101[EEPROM_ADDRESS_CC1100_MY_ADDR]);

  //Channel - default 1
  eeprom_cc1101[EEPROM_ADDRESS_CC1100_CHANNEL] = CC1100_CHANNEL;
  Serial.print(F("Channel: ")); Serial.println(eeprom_cc1101[EEPROM_ADDRESS_CC1100_CHANNEL]);
  Serial.println(); Serial.println();
}
