/**
 * @file 	lib_spi_pic24_ll.c
 * @author 	Alexis ROLLAND
 * @date	2022-02
 * @brief 	Low level library for SPI / PIC24 (PIC24FJ128GA010 fully tested)
 *          v2 : 2024-02
 *
 */

#include "lib_spi_pic24_ll.h" // Inclusion du fichier .h  renommé

/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/
const   regAddr Tab_TRIS_addr[]={   (regAddr)&TRISA,
                                    (regAddr)&TRISB,
                                    (regAddr)&TRISC,
                                    (regAddr)&TRISD,
                                    (regAddr)&TRISE,
                                    (regAddr)&TRISF,
                                    (regAddr)&TRISG,    
};

const   regAddr Tab_LAT_addr[]={    (regAddr)&LATA,
                                    (regAddr)&LATB,
                                    (regAddr)&LATC,
                                    (regAddr)&LATD,
                                    (regAddr)&LATE,
                                    (regAddr)&LATF,
                                    (regAddr)&LATG,    
};
//-----------------------------------------------------------------------------
/*	Implémentation du code */
spi_err_t   spi_init(spi_id_t spi_id, spi_config_t* pSpiCFG, spi_desc_t *pSpi)
{
    uint16_t    tmpReg;
    pSpi->spiID = spi_id;
    switch(spi_id){
        case _SPI1:
            pSpi->pSPIxSTAT = (regAddr)&SPI1STAT;
            pSpi->pSPIxCON1 = (regAddr)&SPI1CON1;
            pSpi->pSPIxCON2 = (regAddr)&SPI1CON2;
            pSpi->pSPIBUF = (regAddr)&SPI1BUF;
            pSpi->pIFSreg = (regAddr)&IFS0;
            pSpi->IFSMask = SPI1IF_MASK;
            break;
        case _SPI2:
            pSpi->pSPIxSTAT = (regAddr)&SPI2STAT;
            pSpi->pSPIxCON1 = (regAddr)&SPI2CON1;
            pSpi->pSPIxCON2 = (regAddr)&SPI2CON2;
            pSpi->pSPIBUF = (regAddr)&SPI2BUF;
            pSpi->pIFSreg = (regAddr)&IFS2;
            pSpi->IFSMask = SPI2IF_MASK;
            break;
        default:
            return SPI_UNKNOWN_MODULE;
            break;
    }
    
    // Configuration effective
    // SPIxCON1
    
    // Secondary Presecaler
    switch(pSpiCFG->spiSecondaryPrescaler){
        case SEC_PRE_1: tmpReg = 0b111;break;
        case SEC_PRE_2: tmpReg = 0b110;break;
        case SEC_PRE_3: tmpReg = 0b101;break;
        case SEC_PRE_4: tmpReg = 0b100;break;
        case SEC_PRE_5: tmpReg = 0b011;break;
        case SEC_PRE_6: tmpReg = 0b010;break;
        case SEC_PRE_7: tmpReg = 0b001;break;
        case SEC_PRE_8: tmpReg = 0b000;break;    
    }
    tmpReg <<= 2;
    
    // Primary Presecaler
    switch(pSpiCFG->spiPrimaryPrescaler){
        case PRI_PRE_1  : tmpReg += 0b11;break;
        case PRI_PRE_4  : tmpReg += 0b10;break;
        case PRI_PRE_16 : tmpReg += 0b01;break;
        case PRI_PRE_64 : tmpReg += 0b00;break;
    } 
    
    // Data format
    pSpi->spiDataFormat = pSpiCFG->spiDataFormat;
    if (pSpiCFG->spiDataFormat == BITS8) tmpReg &= ~(MODE16_MASK);
    else tmpReg |= MODE16_MASK;
    
    // Sample point
    if (pSpiCFG->spiSamplePoint == MID_SMP) tmpReg &= ~(SMP_MASK);
    else tmpReg |= SMP_MASK;
    
    // Clock Phase
    if (pSpiCFG->spiClockPhase == IDLE_TO_ACTIVE_CPHASE) tmpReg &= ~(CKE_MASK);
    else tmpReg |= CKE_MASK;
    
    // Clock Polarity
    if (pSpiCFG->spiClockPolarity == CLK_IDLE_IS_LOW) tmpReg &= ~(CKP_MASK);
    else tmpReg |= CKP_MASK;
    
    // SSEN : SSx pin is not used by module; pin is controlled by port function (0))
    tmpReg &= ~(SSEN_MASK);
    
    // MSTEN : Master Mode (1)
    tmpReg |= MSTEN_MASK;
    
    *(pSpi->pSPIxCON1) = tmpReg;
    
    //------------------------------------------
    // SPIxCON2
    tmpReg = 0x0000;
    *(pSpi->pSPIxCON2) = tmpReg;
     
    //------------------------------------------
    // SPIxSTAT
    tmpReg = 0x0000;
    // SPIEN : Enable SPI module
    tmpReg |= SPIEN_MASK;
    
    *(pSpi->pSPIxSTAT) = tmpReg;
    
    //------------------------------------------
    // IFS
    //*(pSpi->pIFSreg) &= ~pSpi->IFSMask;
    ClrIFS();
    
    //------------------------------------------
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_init_cs(spi_desc_t *pSpi,const spi_cs_t *pCs){
    regAddr CStrisAddr = getTRIS(pCs->port);         /**< Get the TRIS address for the CS line   */
    *CStrisAddr &= ~((0x0001)<<(pCs->bitNumber));    /**< Config CS line as GPIO output  */
    spi_deassertCS(pSpi,pCs);                        /**< Deasserts the CS line  (CS <- 1) */
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_assertCS(const  spi_desc_t *pSpi, const spi_cs_t *pCs){
    regAddr CSlatAddr = getLAT(pCs->port);
    *CSlatAddr &= ~((0x0001)<<(pCs->bitNumber));
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_deassertCS(const  spi_desc_t *pSpi, const spi_cs_t *pCs){
    regAddr CSlatAddr = getLAT(pCs->port);
    *CSlatAddr |= ((0x0001)<<(pCs->bitNumber));
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_raw_byte(const spi_desc_t *pSpi, uint8_t TxData, uint8_t *pRxData){
    uint16_t    dummy;   
    if (pSpi->spiDataFormat != BITS8) return SPI_BAD_DATA_FORMAT;
    
    while (*(pSpi->pSPIxSTAT) & SPITBF_MASK);   /**< Wait for empty TX Buffer   */
    *(pSpi->pSPIBUF) = TxData;                  /**< Fill the TX buffer with data to send   */
    while(!(*(pSpi->pIFSreg) & pSpi->IFSMask)); /**< Wait for the end of Tx */
    dummy = *(pSpi->pSPIBUF);                   /**< SPIxBUFF MUST be read...   */
    ClrIFS();                                   /**< Clear SPIxIF Flag  */
    
    if (pRxData != NULL) *pRxData = (uint8_t)dummy;
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_raw_word(const spi_desc_t *pSpi, uint16_t TxData, uint16_t *pRxData){
    uint16_t    dummy;   
    if (pSpi->spiDataFormat != BITS16) return SPI_BAD_DATA_FORMAT;
    
    while (*(pSpi->pSPIxSTAT) & SPITBF_MASK);   /**< Wait for empty TX Buffer   */
    *(pSpi->pSPIBUF) = TxData;                  /**< Fill the TX buffer with data to send   */
    while(!(*(pSpi->pIFSreg) & pSpi->IFSMask)); /**< Wait for the end of Tx */
    dummy = *(pSpi->pSPIBUF);                   /**< SPIxBUFF MUST be read...   */
    ClrIFS();                                   /**< Clear SPIxIF Flag  */
    
    if (pRxData != NULL) *pRxData = dummy;
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_raw_bytes(const spi_desc_t *pSpi, const uint8_t *pTxData, uint8_t *pRxData, size_t len){
    spi_err_t res;
    size_t  i;
    if (pSpi->spiDataFormat != BITS8) return SPI_BAD_DATA_FORMAT;
    for (i=0;i<len;i++){
        res = spi_transfer_raw_byte(pSpi,(pTxData==NULL)?0xFF:pTxData[i],(pRxData==NULL)?NULL:&pRxData[i]);
        if (res != SPI_OK) return res;
    }
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_raw_words(const spi_desc_t *pSpi, const uint16_t *pTxData, uint16_t *pRxData, size_t len){
    spi_err_t res;
    size_t  i;
    if (pSpi->spiDataFormat != BITS16) return SPI_BAD_DATA_FORMAT;
    for (i=0;i<len;i++){
        res = spi_transfer_raw_word(pSpi,(pTxData==NULL)?0xFF:pTxData[i],(pRxData==NULL)?NULL:&pRxData[i]);
        if (res != SPI_OK) return res;
    }
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_byte_reg(const spi_desc_t *pSpi, uint8_t reg, uint8_t dataOut, uint8_t *pdataIn){
    spi_err_t res;
    if (pSpi->spiDataFormat != BITS8) return SPI_BAD_DATA_FORMAT;
    
    res = spi_transfer_raw_byte(pSpi, reg, NULL);
    if (res != SPI_OK) return res;
    res = spi_transfer_raw_byte(pSpi, dataOut, pdataIn);
    if (res != SPI_OK) return res;
    
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_word_reg(const spi_desc_t *pSpi, uint16_t reg, uint16_t dataOut, uint16_t *pdataIn){
    spi_err_t res;
    if (pSpi->spiDataFormat != BITS16) return SPI_BAD_DATA_FORMAT;
    
    res = spi_transfer_raw_word(pSpi, reg, NULL);
    if (res != SPI_OK) return res;
    res = spi_transfer_raw_word(pSpi, dataOut, pdataIn);
    if (res != SPI_OK) return res;
    
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_byte_regs(const spi_desc_t *pSpi, uint8_t reg, const uint8_t *out, uint8_t *in, size_t len){
    spi_err_t res;
    size_t i = 0;
    if (pSpi->spiDataFormat != BITS8) return SPI_BAD_DATA_FORMAT;
    
    res = spi_transfer_raw_byte(pSpi, reg, NULL);
    if (res != SPI_OK) return res;
    
    for (i=0; i < len; i++){
        res = spi_transfer_raw_byte(pSpi,(out==NULL)?0xFF:out[i],(in==NULL)?NULL:&in[i]);
        if (res != SPI_OK) return res;
    }
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_word_regs(const spi_desc_t *pSpi, uint16_t reg, const uint16_t *out, uint16_t *in, size_t len){
    spi_err_t res;
    size_t i = 0;
    if (pSpi->spiDataFormat != BITS16) return SPI_BAD_DATA_FORMAT;
    
    res = spi_transfer_raw_word(pSpi, reg, NULL);
    if (res != SPI_OK) return res;
    
    for (i=0; i < len; i++){
        res = spi_transfer_raw_word(pSpi,(out==NULL)?0xFF:out[i],(in==NULL)?NULL:&in[i]);
        if (res != SPI_OK) return res;
    }
    return SPI_OK;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------


