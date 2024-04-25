/**
 * @file    lib_test_lib_spi_ll_pic24.h 
 * @author 	Alexis ROLLAND
 * @date	2024-04
 * @brief 	Header file for Lib app for lib_spi_ll_pic24 test app
 *  
 *
 */
#ifndef	__LIB_TEST_LIB_SPI_LL_PIC24_H__
#define	__LIB_TEST_LIB_SPI_LL_PIC24_H__

#ifndef FCY
#define FCY 4000000UL
#endif

#include <libpic30.h>
#include <xc.h>

#include "lib_spi_pic24_ll.h"

#define SPI_MODULE  _SPI2


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
void    mainTask(void);

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
 
 


#endif  /*  __LIB_TEST_LIB_SPI_LL_PIC24_H__ */



