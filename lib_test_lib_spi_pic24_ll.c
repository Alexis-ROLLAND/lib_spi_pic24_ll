/**
 * @file Template_lib_Appli_C.c 
 * @author 	Alexis ROLLAND
 * @date	2021-12-30
 * @brief 	Template for PIC24 main app
 *  
 *
 */


#include "lib_test_lib_spi_pic24_ll.h" // Inclusion du fichier .h "Applicatif" renommé

/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/
spi_desc_t  mySpi;

/*	Implémentation du code */
void Initialiser(void)
{
    spi_config_t    spiCfg;
    
    // Leds
    TRISA &= 0xFF00;
    LATA = 0;
            
    // CS Line 
    TRIS_CS = 0;    // Output
    CS_HIGH();      // Set CS to High level
    
    // SPI
    spiCfg.spiClockPolarity = CLK_IDLE_IS_LOW;
    spiCfg.spiClockPhase = ACTIVE_TO_IDLE_CPHASE;
    spiCfg.spiDataFormat = BITS8;
    spiCfg.spiSamplePoint = MID_SMP;
    spiCfg.spiPrimaryPrescaler = PRI_PRE_4;
    spiCfg.spiSecondaryPrescaler = SEC_PRE_8;
    
    spi_init(SPI_MODULE, &spiCfg, &mySpi);
    
    
    

}

