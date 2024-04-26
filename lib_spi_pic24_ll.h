/**
 * @file    lib_spi_pic24_ll.h 
 * @author 	Alexis ROLLAND
 * @date	2023/04
 * @brief 	Low level library for SPI / PIC24 (PIC24FJ128GA010 fully tested)
 *          v2 : 2024-04 
 *
 */

#ifndef	__LIB_SPI_PIC24_LL_H__
#define	__LIB_SPI_PIC24_LL_H__
#include <xc.h>
#include <stddef.h>     // for size_t

//-----------------------------------------------------------------------------
typedef uint16_t*    regAddr;       /**<    Alias to uint16_t*  */
//-----------------------------------------------------------------------------
// Enums & types for the CS support
typedef enum {
            GPIO_PORTA = 0,             /**< GPIO port is PORTA */
            GPIO_PORTB = 1,             /**< GPIO port is PORTB */
            GPIO_PORTC = 2,             /**< GPIO port is PORTC */
            GPIO_PORTD = 3,             /**< GPIO port is PORTD */
            GPIO_PORTE = 4,             /**< GPIO port is PORTF */
            GPIO_PORTF = 5,             /**< GPIO port is PORTG */        
            GPIO_PORTG = 6              /**< GPIO port is PORTG */                
} gpio_port_t;

typedef struct{
    gpio_port_t port;                       /**<    GPIO Port   */
    uint8_t     bitNumber;                  /**<    Bit of GPIO register associated with GPIO   */
} gpio_pin_t;  

#define getTRIS(port) {Tab_TRIS_addr[(uint8_t)port]};   /**< macro based getTRIS  */
#define getLAT(port) {Tab_LAT_addr[(uint8_t)port]};     /**< macro based getLAT  */
//-----------------------------------------------------------------------------
// Masks for SPIxSTAT register
#define SPIEN_MASK  (0x0001 << 15)  /**< SPIxSTAT[15] */
#define SPITBF_MASK (0x0001 << 1)   /**< SPIxSTAT[1] */
#define SPIRBF_MASK (0x0001 << 0)   /**< SPIxSTAT[0] */

// Masks for SPIxCON1 register
#define MODE16_MASK (0x0001 << 10)  /**< SPIxCON1[10] */
#define SMP_MASK    (0x0001 << 9)   /**< SPIxCON1[9] */
#define CKE_MASK    (0x0001 << 8)   /**< SPIxCON1[8] */
#define SSEN_MASK   (0x0001 << 7)   /**< SPIxCON1[7] */
#define CKP_MASK    (0x0001 << 6)   /**< SPIxCON1[6] */
#define MSTEN_MASK  (0x0001 << 5)   /**< SPIxCON1[5] */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
typedef enum    {   _SPI1,      /**< Value for SPI1 module */
                    _SPI2       /**< Value for SPI2 module */
} spi_id_t;
//-----------------------------------------------------------------------------
/** Modes SPI :
 *  Mode 0 : Clock Polarity = CLK_IDLE_IS_LOW  / Clock Phase = IDLE_TO_ACTIVE_CPHASE
 *  Mode 1 : Clock Polarity = CLK_IDLE_IS_LOW  / Clock Phase = ACTIVE_TO_IDLE_CPHASE
 *  Mode 2 : Clock Polarity = CLK_IDLE_IS_HIGH / Clock Phase = IDLE_TO_ACTIVE_CPHASE
 *  Mode 3 : Clock Polarity = CLK_IDLE_IS_HIGH / Clock Phase = ACTIVE_TO_IDLE_CPHASE
 */ 
typedef enum    {   CLK_IDLE_IS_LOW,    /**< Idle state for clock is a low level; active state is a high level) */
                    CLK_IDLE_IS_HIGH    /**< Idle state for clock is a high level; active state is a low level) */
} clock_polarity_t;      
          
typedef enum    {   IDLE_TO_ACTIVE_CPHASE,  /**< Serial output data changes on transition from Idle clock state to active clock state (see Clock Polarity) */
                    ACTIVE_TO_IDLE_CPHASE   /**< Serial output data changes on transition from active clock state to idle clock state (see Clock Polarity) */
} clock_phase_t;

typedef enum    {   MID_SMP,    /**< MID_SMP => Sample point = milieu donnée émise (courant) */
                    END_SMP     /**< END_SMP => Sample point = Fin donnée émise */
} spiSamplePoint_t;
                    
typedef enum    {   BITS8,      /**< 8 bits data format  */
                    BITS16      /**< 16 bits data format  */
} spiDataFormat_t;
                    
/**
 * Débit Bus :                                                                  
 * Fclkspi = Fcy / (PriPrescaler * SecPrescaler)   
 */                               
typedef enum    {   SEC_PRE_8,      /**< Secondary Prescaler is 1:8     */
                    SEC_PRE_7,      /**< Secondary Prescaler is 1:7     */
                    SEC_PRE_6,      /**< Secondary Prescaler is 1:6     */
                    SEC_PRE_5,      /**< Secondary Prescaler is 1:5     */
                    SEC_PRE_4,      /**< Secondary Prescaler is 1:4     */
                    SEC_PRE_3,      /**< Secondary Prescaler is 1:3     */
                    SEC_PRE_2,      /**< Secondary Prescaler is 1:2     */
                    SEC_PRE_1       /**< Secondary Prescaler is 1:1     */
} tSecPrescaler;
                    
typedef  enum   {   PRI_PRE_64,     /**< Primary Prescaler is 1:64  */
                    PRI_PRE_16,     /**< Primary Prescaler is 1:16  */
                    PRI_PRE_4,      /**< Primary Prescaler is 1:4   */
                    PRI_PRE_1       /**< Primary Prescaler is 1:1   */
} tPriPrescaler;
                    
typedef enum    {   SPI_OK,                 /**< Succes value                           */
                    SPI_ERROR,              /**< Non Specific Error                     */
                    SPI_BAD_DATA_FORMAT,    /**< error in data size : 8bits vs 16 bits  */
                    SPI_UNKNOWN_MODULE      /**< The SPI Module ID is unknown           */
} spi_err_t; 
                    
/** 
 * Type spi_config_t
 */
typedef struct{
    clock_polarity_t    spiClockPolarity;   /**< Clock Polarity : CLK_IDLE_IS_LOW or CLK_IDLE_IS_HIGH   */
    clock_phase_t       spiClockPhase;      /**< Clock Phase : IDLE_TO_ACTIVE_CPHASE or ACTIVE_TO_IDLE_CPHASE */
    spiSamplePoint_t    spiSamplePoint;     /**< Sample Point : MID_SMP or END_SMP */
    spiDataFormat_t     spiDataFormat;      /**< Data format : BITS8 or BITS16 */
    tPriPrescaler       spiPrimaryPrescaler;    /**< Primary Prescaler : PRI_PRE_x (x = 1, 4, 16 or 64) */
    tSecPrescaler       spiSecondaryPrescaler;  /**< Secondary Prescaler : SEC_PRE_x (x is between 1 and 8) */
    gpio_pin_t          spiCS;              /**< Use gpio_pint_t to specify CS line */
} spi_config_t;
                    
/** 
 *  Type spi_desc_t
 *  For internal use only
 */
typedef struct {
    spi_id_t    spiID;      /**< ID of the SPI module (_SPI1 or _SPI2)  */
    regAddr     pSPIxSTAT;  /**< Address of the real SPIxSTAT register  */
    regAddr     pSPIxCON1;  /**< Address of the real SPIxCON1 register  */
    regAddr     pSPIxCON2;  /**< Address of the real SPIxCON2 register  */
    regAddr     pSPIBUF;    /**< Address of the real SPIxBUF register  */
    spiDataFormat_t     spiDataFormat;  /**< SPI Data format    */
    gpio_pin_t          spiCS;  /**< GPIO pin used as CS    */
} spi_desc_t;            

/**
 * @brief  Asserts the CS Line
 * 
 * @param[in]  pSpi  Address of the Spi module descriptor
 * @return  SPI_OK   
 */
spi_err_t   spi_assertCS(const  spi_desc_t *pSpi);

/**
 * @brief  DeAsserts the CS Line
 * 
 * @param[in]  pSpi  Address of the Spi module descriptor
 * @return  SPI_OK   
 */
spi_err_t   spi_deassertCS(const  spi_desc_t *pSpi);
                            
/**
 * @brief   Initialize the SPI module
 * 
 * @param[in]   spi_id  ID of the target SPI module (_SPI1 or _SPI2)
 * @param[in]   pSpiCFG Address of the fully completed spi_config_t structure
 * @param[out]  pSPI    Spi module descriptor (fully completed)  	
 * 
 * @return  SPI_OK  on success
 * @return  SPI_UNKNOWN_MODULE if SPI module is unknown 
 */
spi_err_t   spi_init(spi_id_t spi_id, spi_config_t* pSpiCFG, spi_desc_t *pSpi);
    
 /**
  * @brief      Initiates a SPI transfer based using the Spi module descriptor  
  *             Data format is 8 bits
  * @param[in]  pSpi    Address of the initialized Spi module descriptor
  * @param[in]  TxData  Data to Tx
  * @param[out] pRxData Address of the location to store the Rx data or NULL   	
  * 
  * @return     SPI_OK 
  * @return     SPI_BAD_DATA_FORMAT
  * 
  * @attention  The CS line must be asserted by the user before calling this 
  *             function, and deasserted once the tranfert is fully completed
  */
spi_err_t   spi_transfer_raw_byte(const spi_desc_t *pSpi, uint8_t TxData, uint8_t *pRxData);

/**
  * @brief      Initiates a SPI transfer based using the Spi module descriptor  
  *             Data format is 16 bits
  * @param[in]  pSpi    Address of the initialized Spi module descriptor
  * @param[in]  TxData  Data to Tx
  * @param[out] pRxData Address of the location to store the Rx data or NULL   	
  * 
  * @return     SPI_OK 
  * @return     SPI_BAD_DATA_FORMAT
  * 
  * @attention  The CS line must be asserted by the user before calling this 
  *             function, and deasserted once the tranfert is fully completed
  * 
  */
spi_err_t   spi_transfer_raw_word(const spi_desc_t *pSpi, uint16_t TxData, uint16_t *pRxData);

/**
 * @brief   Initiates a SPI NbBytes transfer based using the Spi module descriptor  
 *          Data format is 8 bits
 * @param[in]  pSpi     Address of the Spi module descriptor
 * @param[in]  pTxData  Address of Data to Tx
 * @param[out] pRxData  Address of the location to store the Rx data   	
 * @param[in]  len      number of bytes to transfer 
 * 
 * @return     SPI_OK
 * @return     SPI_BAD_DATA_FORMAT
 *
 * @attention : The CS line must be asserted by the user before calling this 
 *              function, and deasserted once the tranfert is fully completed
 */
spi_err_t   spi_transfer_raw_bytes(const spi_desc_t *pSpi, const uint8_t *pTxData, uint8_t *pRxData, size_t len);

/**
 * @brief   Initiates a SPI NbBytes transfer based using the Spi module descriptor  
 *          Data format is 16 bits
 * @param[in]  pSpi     Address of the Spi module descriptor
 * @param[in]  pTxData  Address of Data to Tx
 * @param[out] pRxData  Address of the location to store the Rx data   	
 * @param[in]  len      number of bytes to transfer 
 * 
 * @return     SPI_OK
 * @return     SPI_BAD_DATA_FORMAT
 *
 * @attention : The CS line must be asserted by the user before calling this 
 *              function, and deasserted once the tranfert is fully completed
 */
spi_err_t   spi_transfer_raw_words(const spi_desc_t *pSpi, const uint16_t *pTxData, uint16_t *pRxData, size_t len);

/**
 * @brief   Transfer one byte to/from a given register address
 *
 * This function is a shortcut function for easier handling of SPI devices that
 * implement a register based access scheme (8 bits mode).
 * 
 * @param[in]   pSpi    Address of the Spi module descriptor
 * @param[in]   reg     register address to transfer data to/from
 * @param[in]   dataOut byte to send
 * @param[out]   pdataIn Address of the location of the read data or NULL 
 * 
 * @return     SPI_OK
 * @return     SPI_BAD_DATA_FORMAT
 */
spi_err_t   spi_transfer_byte_reg(const spi_desc_t *pSpi, uint8_t reg, uint8_t dataOut, uint8_t *pdataIn);

/**
 * @brief   Transfer one Word to/from a given register address
 *
 * This function is a shortcut function for easier handling of SPI devices that
 * implement a register based access scheme (16 bits mode).
 * 
 * @param[in]   pSpi    Address of the Spi module descriptor
 * @param[in]   reg     register address to transfer data to/from
 * @param[in]   dataOut Word to send
 * @param[out]  pdataIn Address of the location of the read data or NULL 
 * 
 * @return     SPI_OK
 * @return     SPI_BAD_DATA_FORMAT
 */
spi_err_t   spi_transfer_word_reg(const spi_desc_t *pSpi, uint16_t reg, uint16_t dataOut, uint16_t *pdataIn);

/**
 * @brief   Transfer a number of bytes to/from a given register address
 *
 * This function is a shortcut function for easier handling of SPI devices that
 * implement a register based access scheme (8 bits mode).
 * 
 * @param[in]   pSpi    Address of the Spi module descriptor
 * @param[in]   reg     first register address to transfer data to/from
 * @param[in]   out     buffer to send data from, set NULL if only receiving
 * @param[out]  in      buffer to read into, set NULL if only sending
 * @param[in]   len     number of bytes to transfer
 *
 * @return     SPI_OK
 * @return     SPI_BAD_DATA_FORMAT
 */
spi_err_t   spi_transfer_byte_regs(const spi_desc_t *pSpi, uint8_t reg, const uint8_t *out, uint8_t *in, size_t len);

/**
 * @brief   Transfer a number of Words to/from a given register address
 *
 * This function is a shortcut function for easier handling of SPI devices that
 * implement a register based access scheme (16 bits mode).
 * 
 * @param[in]   pSpi    Address of the Spi module descriptor
 * @param[in]   reg     first register address to transfer data to/from
 * @param[in]   out     buffer to send data from, set NULL if only receiving
 * @param[out]  in      buffer to read into, set NULL if only sending
 * @param[in]   len     number of Words to transfer
 *
 * @return     SPI_OK
 * @return     SPI_BAD_DATA_FORMAT
 */
spi_err_t   spi_transfer_word_regs(const spi_desc_t *pSpi, uint16_t reg, const uint16_t *out, uint16_t *in, size_t len);


#endif  /*  __LIB_SPI_PIC24_LL_H__  */


