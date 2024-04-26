/**
 * @file    lib_test_lib_spi_ll_pic24.c 
 * @author 	Alexis ROLLAND
 * @date	2024-04
 * @brief 	Lib app for lib_spi_ll_pic24 test app
 *  
 *
 */


#include "lib_test_lib_spi_pic24_ll.h" // Inclusion du fichier .h "Applicatif" renommé

/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/
spi_desc_t  mySpi;
spi_cs_t    cs_pin;

/*	Implémentation du code */
void Initialiser(void)
{
    spi_config_t    spiCfg;
    
    
    // Leds
    TRISA &= 0xFF00;
    LATA = 0;
    
    cs_pin.port = GPIO_PORTD;   /**< P79_EECS is on RD12    */
    cs_pin.bitNumber = 12;      /**< P79_EECS is on RD12    */
    
    
    // SPI
    spiCfg.spiClockPolarity = CLK_IDLE_IS_LOW;
    spiCfg.spiClockPhase = ACTIVE_TO_IDLE_CPHASE;
    spiCfg.spiDataFormat = BITS8;
    spiCfg.spiSamplePoint = MID_SMP;
    spiCfg.spiPrimaryPrescaler = PRI_PRE_4;
    spiCfg.spiSecondaryPrescaler = SEC_PRE_8;
       
    spi_init(SPI_MODULE, &spiCfg, &mySpi);
    spi_init_cs(&mySpi, &cs_pin);
    
    // Send WREN
    spi_assertCS(&mySpi, &cs_pin);
    spi_transfer_raw_byte(&mySpi, 0x06, NULL);  // Send WREN Code
    spi_deassertCS(&mySpi, &cs_pin);
    

}
uint8_t     DataOut[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
uint8_t     DataIn[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t     StartAddr[2] = {0x00,0x00};

void    mainTask(void){
    static uint8_t  WriteOnce = 0;
    __delay_ms(500);
    LATAbits.LATA0 = 1;
    
    
    
    // Write 16 bytes @0x00 to @0x0F, with address values
    if (!WriteOnce) {
        spi_assertCS(&mySpi, &cs_pin);
        spi_transfer_raw_byte(&mySpi, 0x02, NULL);  // Send Write Code
        spi_transfer_raw_bytes(&mySpi, StartAddr, NULL, 2);  // Send Address
        spi_transfer_raw_bytes(&mySpi, DataOut, NULL, 16);  // Send Data
        spi_deassertCS(&mySpi, &cs_pin);
        ++WriteOnce;
    }
    __delay_ms(500);
    LATAbits.LATA0 = 0;
    
    
    // Read 16 bytes
    spi_assertCS(&mySpi, &cs_pin);
    spi_transfer_raw_byte(&mySpi, 0x03, NULL);  // Send Read Code
    spi_transfer_raw_bytes(&mySpi, StartAddr, NULL, 2);  // Send Address
    spi_transfer_raw_bytes(&mySpi, NULL, DataIn, 16);
    spi_deassertCS(&mySpi, &cs_pin);
    
    uint8_t i;
    for (i=0;i<16;++i){
        if (DataIn[i] != DataOut[i]) LATAbits.LATA0 = 1;
    }
    
    
}
