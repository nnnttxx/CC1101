#ifndef cc1100_H
#define cc1100_H

#include <Arduino.h>
//#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


/*------------------------------[debug output 1/0]----------------------------*/
#define CC1100_DEBUG 1          //must defined in main project

/*----------------------------------[standard]--------------------------------*/
#define TRUE  !FALSE
#define FALSE 0

//|=====================[ setting EEPROM addresses]=============================
#define EEPROM_ADDRESS_CC1100_FREQUENCY 0x000  //ISM band
#define EEPROM_ADDRESS_CC1100_MODE      0x001  //modulation mode
#define EEPROM_ADDRESS_CC1100_MY_ADDR   0x002  //receiver address
#define EEPROM_ADDRESS_CC1100_CHANNEL   0x003  //channel number

//**************************** pins ******************************************//
#define SCK_PIN  13
#define MISO_PIN 12
#define MOSI_PIN 11
#define SS_PIN   10
#define GDO0     A0                // for temperature measurement      
#define GDO2      2                // rx interrrupt
                                                         
/*----------------------[CC1100 - misc]---------------------------------------*/
#define CFG_REGISTER        47  
#define PA_TABLESIZE         8
#define FIFOBUFFER          66  //size of Fifo Buffer
#define RSSI_OFFSET_868MHZ  74  //dec = 74
#define TX_RETRIES_MAX       5 //tx_retries_max
#define ACK_TIMEOUT          100    //ACK timeout in ms
#define CC1100_COMPARE_REGISTER 0x00  //register compare 0=no compare 1=compare
#define BROADCAST_ADDRESS       0x00  //broadcast address

#define CC1100_FREQ_315MHZ      0x01
#define CC1100_FREQ_434MHZ      0x02
#define CC1100_FREQ_868MHZ      0x03
#define CC1100_FREQ_915MHZ      0x04
#define CC1100_FREQ_2430MHZ     0x05

#define CC1100_MODE_GFSK_1_2_kb     0x01
#define CC1100_MODE_GFSK_38_4_kb    0x02
#define CC1100_MODE_GFSK_100_kb     0x03
#define CC1100_MODE_MSK_250_kb      0x04
#define CC1100_MODE_MSK_500_kb      0x05
#define CC1100_MODE_OOK_4_8_kb      0x06

#define CC1100_TEMP_ADC_MV      3.225  //3.3V/1023 . mV pro digit
#define CC1100_TEMP_CELS_CO      2.47  //Temperature coefficient 2.47mV per Grad Celsius

/*---------------------------[CC1100 - R/W offsets]---------------------------*/
#define WRITE_SINGLE_BYTE   0x00
#define WRITE_BURST         0x40
#define READ_SINGLE_BYTE    0x80
#define READ_BURST          0xC0
/*---------------------------[END R/W offsets]--------------------------------*/

/*------------------------[CC1100 - FIFO commands]----------------------------*/
#define TXFIFO_BURST        0x7F    //write burst only
#define TXFIFO_SINGLE_BYTE  0x3F    //write single only
#define RXFIFO_BURST        0xFF    //read burst only
#define RXFIFO_SINGLE_BYTE  0xBF    //read single only
#define PATABLE_BURST       0x7E    //power control read/write
#define PATABLE_SINGLE_BYTE 0xFE    //power control read/write
/*---------------------------[END FIFO commands]------------------------------*/

/*----------------------[CC1100 - config register]----------------------------*/
#define IOCFG2   0x00         // GDO2 output pin configuration
#define IOCFG1   0x01         // GDO1 output pin configuration
#define IOCFG0   0x02         // GDO0 output pin configuration
#define FIFOTHR  0x03         // RX FIFO and TX FIFO thresholds
#define SYNC1    0x04         // Sync word, high byte
#define SYNC0    0x05         // Sync word, low byte
#define PKTLEN   0x06         // Packet length
#define PKTCTRL1 0x07         // Packet automation control
#define PKTCTRL0 0x08         // Packet automation control
#define ADDR     0x09         // Device address
#define CHANNR   0x0A         // Channel number
#define FSCTRL1  0x0B          // Frequency synthesizer control
#define FSCTRL0  0x0C         // Frequency synthesizer control
#define FREQ2    0x0D         // Frequency control word, high byte
#define FREQ1    0x0E         // Frequency control word, middle byte
#define FREQ0    0x0F         // Frequency control word, low byte
#define MDMCFG4  0x10         // Modem configuration
#define MDMCFG3  0x11         // Modem configuration
#define MDMCFG2  0x12         // Modem configuration
#define MDMCFG1  0x13         // Modem configuration
#define MDMCFG0  0x14         // Modem configuration
#define DEVIATN  0x15          // Modem deviation setting
#define MCSM2    0x16         // Main Radio Cntrl State Machine config
#define MCSM1    0x17         // Main Radio Cntrl State Machine config
#define MCSM0    0x18         // Main Radio Cntrl State Machine config
#define FOCCFG   0x19          // Frequency Offset Compensation config
#define BSCFG    0x1A         // Bit Synchronization configuration
#define AGCCTRL2 0x1B         // AGC control
#define AGCCTRL1 0x1C         // AGC control
#define AGCCTRL0 0x1D         // AGC control
#define WOREVT1  0x1E         // High byte Event 0 timeout
#define WOREVT0  0x1F         // Low byte Event 0 timeout
#define WORCTRL  0x20         // Wake On Radio control
#define FREND1   0x21         // Front end RX configuration
#define FREND0   0x22         // Front end TX configuration
#define FSCAL3   0x23         // Frequency synthesizer calibration
#define FSCAL2   0x24         // Frequency synthesizer calibration
#define FSCAL1   0x25         // Frequency synthesizer calibration
#define FSCAL0   0x26         // Frequency synthesizer calibration
#define RCCTRL1  0x27         // RC oscillator configuration
#define RCCTRL0  0x28         // RC oscillator configuration
#define FSTEST   0x29         // Frequency synthesizer cal control
#define PTEST    0x2A         // Production test
#define AGCTEST  0x2B         // AGC test
#define TEST2    0x2C         // Various test settings
#define TEST1    0x2D         // Various test settings
#define TEST0    0x2E         // Various test settings
/*-------------------------[END config register]------------------------------*/

/*------------------------[CC1100-command strobes]----------------------------*/
#define SRES    0x30          // Reset chip
#define SFSTXON 0x31          // Enable/calibrate freq synthesizer
#define SXOFF   0x32          // Turn off crystal oscillator.
#define SCAL    0x33          // Calibrate freq synthesizer & disable
#define SRX     0x34          // Enable RX.
#define STX     0x35          // Enable TX.
#define SIDLE   0x36          // Exit RX / TX
#define SAFC    0x37          // AFC adjustment of freq synthesizer
#define SWOR    0x38          // Start automatic RX polling sequence
#define SPWD    0x39          // Enter pwr down mode when CSn goes hi
#define SFRX    0x3A          // Flush the RX FIFO buffer.
#define SFTX    0x3B          // Flush the TX FIFO buffer.
#define SWORRST 0x3C          // Reset real time clock.
#define SNOP    0x3D          // No operation.
/*-------------------------[END command strobes]------------------------------*/

/*----------------------[CC1100 - status register]----------------------------*/
#define PARTNUM        0xF0    // Part number
#define VERSION        0xF1    // Current version number
#define FREQEST        0xF2    // Frequency offset estimate
#define LQI            0xF3          // Demodulator estimate for link quality
#define RSSI            0xF4    // Received signal strength indication
#define MARCSTATE      0xF5    // Control state machine state
#define WORTIME1       0xF6          // High byte of WOR timer
#define WORTIME0       0xF7    // Low byte of WOR timer
#define PKTSTATUS      0xF8    // Current GDOx status and packet status
#define VCO_VC_DAC     0xF9    // Current setting from PLL cal module
#define TXBYTES        0xFA    // Underflow and # of bytes in TXFIFO
#define RXBYTES        0xFB    // Overflow and # of bytes in RXFIFO
#define RCCTRL1_STATUS 0xFC           //Last RC Oscillator Calibration Result
#define RCCTRL0_STATUS 0xFD           //Last RC Oscillator Calibration Result
//--------------------------[END status register]-------------------------------

class CC1100
{
  private:
    uint8_t _modulation;
    uint8_t _ISM;
    uint8_t _channel;
    uint8_t _patable[PA_TABLESIZE];
    uint8_t _paTableIndex;
    uint8_t _output_power_leveldBm;
    uint8_t _my_addr;
  
    void spi_begin(void);
    void spi_end(void);
    uint8_t spi_putc(uint8_t data);

  public:
    uint8_t begin(uint8_t cc1100_mode_select, uint8_t cc1100_freq_select, 
                      uint8_t cc1100_channel_select, uint8_t cc1100_pa_select, uint8_t My_addr);
    void end(void);

    void spi_write_strobe(uint8_t spi_instr);
    void spi_write_register(uint8_t spi_instr, uint8_t value);
    uint8_t spi_read_register(uint8_t spi_instr);    
    void spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length);
    void spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t length);    

    void reset(void);
    void powerdown(void);
    void wakeup(void);
    void sidle(void);
    void transmit(void);
    void receive(void);

    void show_register_settings(void);
    void show_main_settings(void);

    uint8_t packet_available(void);
    uint8_t wait_for_packet(uint8_t milliseconds);

    uint8_t get_payload(uint8_t rxbuffer[], uint8_t &pktlen_rx,uint8_t &my_addr,
                                uint8_t &sender, int8_t &rssi_dbm, uint8_t &lqi);

    void write_tx_fifo(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer, uint8_t length);
    void read_rx_fifo(uint8_t rxbuffer[], uint8_t &pktlen);

    void clear_rx_buffer(uint8_t *rxbuffer);
    void clear_tx_buffer(uint8_t *txbuffer);

    uint8_t send_packet(uint8_t my_addr, uint8_t rx_addr, uint8_t *txbuffer, uint8_t pktlen, uint8_t tx_retries);
    void send_acknowledge(uint8_t my_addr, uint8_t tx_addr);

    uint8_t check_acknowledge(uint8_t *rxbuffer, uint8_t pktlen, uint8_t sender, uint8_t my_addr);

    int8_t rssi_convert(uint8_t Rssi);
    uint8_t check_crc(uint8_t lqi);
    uint8_t lqi_convert(uint8_t lqi);
    uint16_t get_tempK(void);

    void set_myaddr(uint8_t addr);
    void set_channel(uint8_t channel);
    void set_ISM(uint8_t ism_freq);
    void set_modulation(uint8_t mode);
    void set_output_power_level(int8_t dbm);
    void write_patable(uint8_t *patable_arr);
    void set_data_whitening(bool cfg);
    void set_manchaster_encoding(bool cfg);
    
    uint8_t get_myaddr(void);
    uint8_t get_channel(void);
    uint8_t get_output_power_level(void);
    uint8_t get_patable_index(void);
    uint8_t get_modulation(void);
    uint8_t get_ISM(void);
    
    
};
//extern CC1100 cc1100;

//=======================[CC1100 special functions]=============================


#endif // CC1100_H
