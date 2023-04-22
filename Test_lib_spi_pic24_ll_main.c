/**
 * @file Template_Main_C.c 
 * @author 	Alexis ROLLAND
 * @date	2021-12-30
 * @brief 	Template for PIC24 main app
 *  
 *
 */
#include "lib_test_lib_spi_pic24_ll.h"   // Inclusion du fichier .h "Applicatif" renommé

/**
 * Insérer Ici les bits de configuration pour le MCU 						  
 * Copier / Coller depuis le fichier Config_Pic24.txt					  
 */
/*--------------------------------------------------------------------------*/
/* PIC24FJ128GA010															*/
/*--------------------------------------------------------------------------*/
#pragma config POSCMOD = HS         // Primary Oscillator Select (EC, XT, HS, NONE)
#pragma config OSCIOFNC = ON        // Primary Oscillator Output Function (ON, OFF)
#pragma config FCKSM = CSDCMD       // Clock Switching and Monitor (CSECME, CSECMD, CSDCMD)
#pragma config FNOSC = PRI          // Oscillator Select (FRC, FRCPLL, PRI, PRIPLL, SOSC, LPRC, FRCDIV)
#pragma config IESO = OFF           // Internal External Switch Over Mode (ON, OFF)
#pragma config WDTPS = PS256        // Watchdog Timer Postscaler (PS1, PS2, PS4, PS8,..., PS32768)
#pragma config FWPSA = PR32         // WDT Prescaler (PR32, PR128)
#pragma config WINDIS = ON          // Watchdog Timer Window (ON, OFF)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable (ON, OFF)
#pragma config ICS = PGx2           // Comm Channel Select (PGx1, PGx2)
#pragma config BKBUG = OFF          // Background Debug (ON, OFF)
#pragma config GWRP = OFF           // General Code Segment Write Protect (ON, OFF)
#pragma config GCP = OFF            // General Code Segment Code Protect (ON, OFF)
#pragma config JTAGEN = OFF         // JTAG Port Enable (ON, OFF)
//----------------------------------------------------------------------------
/* Directives de compilation - Macros		*/



/* Déclarations des variables globales 	*/
extern spi_desc_t  mySpi;
uint16_t RxData[4];


/* Programme Principal			*/
int main(void)
{
// Variables locales au main
 


Initialiser();		// Appel fonction d'initialisation

while(1)
    {
    __delay_ms(100);
    CS_LOW();
    
    
    spi_transfert(&mySpi, 0x0011, &RxData[0]);
    spi_transfert(&mySpi, 0x0022, &RxData[1]);
    spi_transfert(&mySpi, 0x0044, &RxData[2]);
    spi_transfert(&mySpi, 0x0088, &RxData[3]);
    
    
    CS_HIGH();
    LATAbits.LATA0 = ~LATAbits.LATA0;
    }
}					

