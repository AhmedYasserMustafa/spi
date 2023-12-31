/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sau.c
* Version      : Code Generator for RL78/G1F V1.01.06.02 [08 Nov 2021]
* Device(s)    : R5F11BBC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for SAU module.
* Creation Date: 9/14/2023
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_csi00_rx_address;        /* csi00 receive buffer address */
volatile uint16_t  g_csi00_rx_length;          /* csi00 receive data length */
volatile uint16_t  g_csi00_rx_count;           /* csi00 receive data count */
volatile uint8_t * gp_csi00_tx_address;        /* csi00 send buffer address */
volatile uint16_t  g_csi00_send_length;        /* csi00 send data length */
volatile uint16_t  g_csi00_tx_count;           /* csi00 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* enables input clock supply */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0000_SAU_CK01_fCLK_0 | _0004_SAU_CK00_fCLK_4;
    R_CSI00_Create();
}
/***********************************************************************************************************************
* Function Name: R_CSI00_Create
* Description  : This function initializes the CSI00 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Create(void)
{
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;
    CSIMK00 = 1U;   /* disable INTCSI00 interrupt */
    CSIIF00 = 0U;   /* clear INTCSI00 interrupt flag */
    /* Set INTCSI00 high priority */
    CSIPR100 = 0U;
    CSIPR000 = 0U;
    SIR00 = _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;
    SMR00 = _0020_SMR00_DEFAULT_VALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS | 
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
    SCR00 = _0004_SCR00_DEFAULT_VALUE | _C000_SAU_RECEPTION_TRANSMISSION | _0000_SAU_TIMING_1 | 
            _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0000_SAU_STOP_NONE | 
            _0003_SAU_LENGTH_8;
    SDR00 = _CE00_SAU0_CH0_BAUDRATE_DIVISOR;
    SO0 |= _0100_SAU_CH0_CLOCK_OUTPUT_1;
    SO0 &= (uint16_t)~_0001_SAU_CH0_DATA_OUTPUT_1;
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;
    /* Set SI00 pin */
    PM5 |= 0x01U;
    /* Set SO00 pin */
    P5 |= 0x02U;
    PM5 &= 0xFDU;
    /* Set SCK00 pin */
    P3 |= 0x01U;
    PM3 &= 0xFEU;
}
/***********************************************************************************************************************
* Function Name: R_CSI00_Start
* Description  : This function starts the CSI00 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Start(void)
{
    SO0 |= _0100_SAU_CH0_CLOCK_OUTPUT_1;
    SO0 &= (uint16_t)~_0001_SAU_CH0_DATA_OUTPUT_1;
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;
    SS0 |= _0001_SAU_CH0_START_TRG_ON;
    CSIIF00 = 0U;   /* clear INTCSI00 interrupt flag */
    CSIMK00 = 0U;   /* enable INTCSI00 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_CSI00_Stop
* Description  : This function stops the CSI00 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Stop(void)
{
    CSIMK00 = 1U;   /* disable INTCSI00 interrupt */
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;
    SOE0 &= (uint16_t)~_0001_SAU_CH0_OUTPUT_ENABLE;
    CSIIF00 = 0U;   /* clear INTCSI00 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_CSI00_Send_Receive
* Description  : This function sends and receives CSI00 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_CSI00_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_csi00_tx_count = tx_num;        /* send data count */
        gp_csi00_tx_address = tx_buf;     /* send buffer pointer */
        gp_csi00_rx_address = rx_buf;     /* receive buffer pointer */
        CSIMK00 = 1U;                     /* disable INTCSI00 interrupt */

        if (0U != gp_csi00_tx_address)
        {
            SIO00 = *gp_csi00_tx_address;    /* started by writing data to SDR[7:0] */
            gp_csi00_tx_address++;
        }
        else
        {
            SIO00 = 0xFFU;
        }

        g_csi00_tx_count--;
        CSIMK00 = 0U;                     /* enable INTCSI00 interrupt */
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
