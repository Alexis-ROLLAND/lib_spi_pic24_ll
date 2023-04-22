/**
 * @file Template_lib_Appli_H.h 
 * @author 	Alexis ROLLAND
 * @date	2021-12-30
 * @brief 	Template for PIC24 main app
 *  
 *
 */
#ifndef	__LIB_APP_H__
#define	__LIB_APP_H__

#ifndef FCY
#define FCY 4000000UL
#endif

#include <libpic30.h>
#include <xc.h>

#include "lib_spi_pic24_ll.h"

#define SPI_MODULE  _SPI1

#define CS_PIN      LATBbits.LATB2      //b23
#define TRIS_CS     TRISBbits.TRISB2

#define CS_LOW()    CS_PIN=0
#define CS_HIGH()   CS_PIN=1

/**
 * @brief Global init function/task 
 * 
 * @param	None
 * 
 * @return  Nothing 
 *
 */
void Initialiser(void);

/**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
 
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
 
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
 
 


#endif



