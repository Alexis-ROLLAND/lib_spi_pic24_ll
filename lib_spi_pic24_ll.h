/**
 * @file    lib_spi_pic24_ll.h 
 * @author 	Alexis ROLLAND
 * @date	2023/04
 * @brief 	Low level library for SPI / PIC24 (PIC24FJ128GA010 fully tested)
 *  
 *
 */

#ifndef	__LIB_SPI_PIC24_LL_H__
#define	__LIB_SPI_PIC24_LL_H__
#include <xc.h>
#include <stddef.h>     // for size_t

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
                    
/*-----------------------------------------------------------------------------*/
/* Débit Bus :                                                                  */
/* Fclkspi = Fcy / (PriPrescaler * SecPrescaler)                                  */
/*-----------------------------------------------------------------------------*/
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
                    SPI_BAD_DATA_FORMAT,      /**< error in data size : 8bits vs 16 bits  */
                    SPI_UNKNOWN_MODULE      /**< The SPI Module ID is unknown           */
                    } spi_err_t; 
                    
/** Type spi_config_t
 * 
 * 
 */
typedef struct{
    clock_polarity_t    spiClockPolarity;
    clock_phase_t       spiClockPhase;
    spiSamplePoint_t    spiSamplePoint;
    spiDataFormat_t     spiDataFormat;
    tPriPrescaler       spiPrimaryPrescaler;
    tSecPrescaler       spiSecondaryPrescaler;
    } spi_config_t;
                    
/** Type spi_desc_t
 * 
 * 
 */
typedef struct {
    spi_id_t    spiID;
    uint16_t    *pSPIxSTAT;
    uint16_t    *pSPIxCON1;
    uint16_t    *pSPIxCON2;
    uint16_t    *pSPIBUF;
    spiDataFormat_t     spiDataFormat;
    } spi_desc_t;            

                            
/**
 * @brief   Initialize the SPI module
 * 
 * @param[in]   ID of the target SPI module (_SPI1 or _SPI2)
 * @param[in]   Address of the fully completed spi_config_t structure
 * @param[out]  Spi module descriptor  	
 * 
 * @return  SPI_OK
 * @return  SPI_UNKNOWN_MODULE 
 */
spi_err_t   spi_init(spi_id_t spi_id, spi_config_t* pSpiCFG, spi_desc_t *pSpi);
    
 /**
  * @brief   Initiates a SPI transfer based using the Spi module descriptor  
  * 
  * @info a "word" is 16bits wide
  * 
  * @param[in]  Address of the Spi module descriptor
  * @param[in]  Data to Tx
  * @param[out] Address of the location to store the Rx data or NULL   	
  * 
  * @return     SPI_OK or SPI_UNKNOWN_MODULE 
  * @return     SPI_UNKNOWN_MODULE 
  * @return     SPI_BAD_DATA_FORMAT
  * 
  * @attention : The CS line must be asserted by the user before calling this 
  *              function, and deasserted once the tranfert is fully completed
  * 
  */
spi_err_t   spi_transfer_raw_byte(spi_desc_t *pSpi, uint8_t TxData, uint8_t *pRxData);
spi_err_t   spi_transfer_raw_word(spi_desc_t *pSpi, uint16_t TxData, uint16_t *pRxData);

/**
 * @brief   Initiates a SPI NbBytes transfer based using the Spi module descriptor  
 * 
 * @info a "word" is 16bits wide
 * 
 * @param[in]  pSpi  Address of the Spi module descriptor
 * @param[in]  pTxData Address of Data to Tx
 * @param[out] pRxData Address of the location to store the Rx data   	
 * @param[in]  len : number of bytes to transfer 
 * 
 * @return     SPI_OK or SPI_UNKNOWN_MODULE 
 * @return     SPI_UNKNOWN_MODULE 
 * @return     SPI_BAD_DATA_FORMAT
 *
 * @attention : The CS line must be asserted by the user before calling this 
 *              function, and deasserted once the tranfert is fully completed
 */
spi_err_t   spi_transfer_raw_bytes(spi_desc_t *pSpi, const uint8_t *pTxData, uint8_t *pRxData, size_t len);
spi_err_t   spi_transfer_raw_words(spi_desc_t *pSpi, const uint16_t *pTxData, uint16_t *pRxData, size_t len);

/**
 * @brief   Transfer one byte to/from a given register address
 *
 * This function is a shortcut function for easier handling of SPI devices that
 * implement a register based access scheme.
 *
 * @info a "word" is 16bits wide
 * 
 * @param[in]   pSpi    Address of the Spi module descriptor
 * 
 * @param[in]   reg     register address to transfer data to/from
 * @param[in]   dataOut byte to send
 * @param[out]   pdataIn Address of the location of the read data or NULL 
 * 
 * @return     SPI_OK
 * @return     SPI_UNKNOWN_MODULE 
 * @return     SPI_BAD_DATA_FORMAT
 */
spi_err_t   spi_transfer_byte_reg(spi_desc_t *pSpi, uint8_t reg, uint8_t dataOut, uint8_t *pdataIn);
spi_err_t   spi_transfer_word_reg(spi_desc_t *pSpi, uint16_t reg, uint16_t dataOut, uint16_t *pdataIn);

/**
 * @brief   Transfer a number of bytes to/from a given register address
 *
 * This function is a shortcut function for easier handling of SPI devices that
 * implement a register based access scheme.
 *
 * @info a "word" is 16bits wide
 * 
 * @param[in]   bus     SPI device to use
 * 
 * @param[in]   reg     fisrt register address to transfer data to/from
 * @param[in]   out     buffer to send data from, set NULL if only receiving
 * @param[out]  in      buffer to read into, set NULL if only sending
 * @param[in]   len     number of bytes to transfer
 *
 * @return     SPI_OK
 * @return     SPI_UNKNOWN_MODULE 
 * @return     SPI_BAD_DATA_FORMAT
 */
spi_err_t   spi_transfer_byte_regs(spi_desc_t *pSpi, uint8_t reg, const uint8_t *out, uint8_t *in, size_t len);
spi_err_t   spi_transfer_word_regs(spi_desc_t *pSpi, uint16_t reg, const uint16_t *out, uint16_t *in, size_t len);


#endif


