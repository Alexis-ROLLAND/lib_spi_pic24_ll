/**
 * @file 	lib_spi_pic24_ll.c
 * @author 	Alexis ROLLAND
 * @date	2022-02
 * @brief 	Low level library for SPI / PIC24 (PIC24FJ128GA010 fully tested)
 *  
 *
 */

#include "lib_spi_pic24_ll.h" // Inclusion du fichier .h  renommé

/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/


/*	Implémentation du code */
spi_err_t   spi_init(spi_id_t spi_id, spi_config_t* pSpiCFG, spi_desc_t *pSpi)
{
    uint16_t    tmpReg;
    pSpi->spiID = spi_id;
    switch(spi_id){
        case _SPI1:
            pSpi->pSPIxSTAT = (uint16_t*)&SPI1STAT;
            pSpi->pSPIxCON1 = (uint16_t*)&SPI1CON1;
            pSpi->pSPIxCON2 = (uint16_t*)&SPI1CON2;
            pSpi->pSPIBUF = (uint16_t*)&SPI1BUF;
            IFS0bits.SPI1IF = 0;
            break;
        case _SPI2:
            pSpi->pSPIxSTAT = (uint16_t*)&SPI2STAT;
            pSpi->pSPIxCON1 = (uint16_t*)&SPI2CON1;
            pSpi->pSPIxCON2 = (uint16_t*)&SPI2CON2;
            pSpi->pSPIBUF = (uint16_t*)&SPI2BUF;
            IFS2bits.SPI2IF = 0;
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
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_raw_byte(spi_desc_t *pSpi, uint8_t TxData, uint8_t *pRxData){
    uint16_t    dummy;   
    if (pSpi->spiDataFormat != BITS8) return SPI_BAD_DATA_FORMAT;
    switch(pSpi->spiID)
    {
        case _SPI1:
            while(SPI1STATbits.SPITBF);     // Attente buffer Tx vide
            SPI1BUF = TxData;
            while(!IFS0bits.SPI1IF);        // Attente fin Tx
            dummy = SPI1BUF;           // SPIxBUFF MUST be read...
            IFS0bits.SPI1IF = 0;
            break;
            
        case _SPI2:
            while(SPI2STATbits.SPITBF);     // Attente buffer Tx vide
            SPI2BUF = TxData;
            while(!IFS2bits.SPI2IF);        // Attente fin Tx
            dummy = SPI2BUF;           // SPIxBUFF MUST be read...
            IFS2bits.SPI2IF = 0;
            break;
        default: return SPI_UNKNOWN_MODULE;
    }
    if (pRxData != NULL) *pRxData = (uint8_t)dummy;
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_raw_word(spi_desc_t *pSpi, uint16_t TxData, uint16_t *pRxData){
    uint16_t    dummy;   
    if (pSpi->spiDataFormat != BITS16) return SPI_BAD_DATA_FORMAT;
    switch(pSpi->spiID)
    {
        case _SPI1:
            while(SPI1STATbits.SPITBF);     // Attente buffer Tx vide
            SPI1BUF = TxData;
            while(!IFS0bits.SPI1IF);        // Attente fin Tx
            dummy = SPI1BUF;           // SPIxBUFF MUST be read...
            IFS0bits.SPI1IF = 0;
            break;
            
        case _SPI2:
            while(SPI2STATbits.SPITBF);     // Attente buffer Tx vide
            SPI2BUF = TxData;
            while(!IFS2bits.SPI2IF);        // Attente fin Tx
            dummy = SPI2BUF;           // SPIxBUFF MUST be read...
            IFS2bits.SPI2IF = 0;
            break;
        default: return SPI_UNKNOWN_MODULE;
    }
    if (pRxData != NULL) *pRxData = dummy;
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_raw_bytes(spi_desc_t *pSpi, const uint8_t *pTxData, uint8_t *pRxData, size_t len){
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
spi_err_t   spi_transfer_raw_words(spi_desc_t *pSpi, const uint16_t *pTxData, uint16_t *pRxData, size_t len){
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
spi_err_t   spi_transfer_byte_reg(spi_desc_t *pSpi, uint8_t reg, uint8_t dataOut, uint8_t *pdataIn){
    spi_err_t res;
    if (pSpi->spiDataFormat != BITS8) return SPI_BAD_DATA_FORMAT;
    
    res = spi_transfer_raw_byte(pSpi, reg, NULL);
    if (res != SPI_OK) return res;
    res = spi_transfer_raw_byte(pSpi, dataOut, pdataIn);
    if (res != SPI_OK) return res;
    
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_word_reg(spi_desc_t *pSpi, uint16_t reg, uint16_t dataOut, uint16_t *pdataIn){
    spi_err_t res;
    if (pSpi->spiDataFormat != BITS16) return SPI_BAD_DATA_FORMAT;
    
    res = spi_transfer_raw_word(pSpi, reg, NULL);
    if (res != SPI_OK) return res;
    res = spi_transfer_raw_word(pSpi, dataOut, pdataIn);
    if (res != SPI_OK) return res;
    
    return SPI_OK;
}
//------------------------------------------------------------------------------
spi_err_t   spi_transfer_byte_regs(spi_desc_t *pSpi, uint8_t reg, const uint8_t *out, uint8_t *in, size_t len){
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
spi_err_t   spi_transfer_word_regs(spi_desc_t *pSpi, uint16_t reg, const uint16_t *out, uint16_t *in, size_t len){
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


