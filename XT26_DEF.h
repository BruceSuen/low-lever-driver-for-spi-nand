/* XT26_DEF.h -Flash device information define */
 
/**************************************************************************
* Copyright 2021 XTX Technology Inc. All Rights Reserved. 
*
* This software is owned and published by XTX Technology Inc.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software constitutes driver source code for use in programming XTX's 
* Flash memory components. This software is licensed by XTX to be adapted only 
* for use in systems utilizing XTX's Flash memories. XTX is not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  XTX is providing this source code "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the source code herein.  
*
* XTX MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE, 
* REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED 
* USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY, 
* FITNESS FOR A  PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.  XTX WILL 
* HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR 
* OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE, 
* INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL, 
* SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS, 
* EVEN IF XTX HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Copyright notice must be included with 
* this software, whether used in part or whole, at all times.  
*/

#ifndef    __XT26_DEF_H__
#define    __XT26_DEF_H__

#include <stdio.h>
#include "XT26_TYPE.h"

/*
  Compiler Option
*/



/* Select your flash device type */

/*1G bit product*/
//#define XT26Q01C
#define XT26G01C

/*2G bit product*/
//#define XT26Q02C
//#define XT26G02C


/*4G bit product*/
//#define XT26Q04C
//#define XT26G04C



// variable
#define    TRUE     1
#define    FALSE    0
#define  TIMEOUT    0
#define  TIMENOTOUT 1

// Support I/O mode
#define    SIO              0
#define    DIO              1
#define    QIO              2
#define    IO_NULL          0xFF


/* Timer Parameter */

#define  TPERIOD    240             // ns, The time period of timer count one
#define  COUNT_ONE_MICROSECOND  16  // us, The loop count value within one micro-second

// system flags
#define  PASS        0
#define  FAIL        1
#define  BUSY        0
#define  READY       1
#define  PROTECTED   0
#define  UNPROTECTED 1


// Flash control register mask define
#define  SR0_OIP               0x01
#define  SR1_WEL               0x02
#define  SR2_EraseFail         0x04
#define  SR2_ProgramFail       0x08
#define  FLASH_QE_MASK         0x01
#define  ECC_EN_MASK           0x10


//Register
#define   SPI_NAND_LOCK_REG    (0xA0)
#define   SPI_NAND_FEATURE_REG (0xB0)
#define   SPI_NAND_STATUS_REG  (0xC0)
#define   SPI_NAND_DRIVER_REG  (0xD0)

#define   SPI_NAND_ECC_BITS_POS (4)

// Flash information define
#define FLASH_TIMEOUT_VALUE   10000



/*
  Flash ID, Timing Information Define
  (The following information could get from device specification)
*/

/*1Gbit 1.8v product*/
#ifdef XT26Q01C
#define    FlashID          0x0B21
#define    FlashSize        0x8000000      // 128 MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif

/*1Gbit 3v product*/

#ifdef XT26G01A
#define    FlashID          0x0BE1
#define    FlashSize        0x8000000      // 128 MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif

#ifdef XT26G01B
#define    FlashID          0x0BF1
#define    FlashSize        0x8000000      // 128 MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif

#ifdef XT26G01C
#define    FlashID          0x0B11
#define    FlashSize        0x8000000      // 128 MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif

/*2Gbit 1.8v product*/

#ifdef XT26Q02C
#define    FlashID          0x0B22
#define    FlashSize        0x10000000      // 256MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif


/*2Gbit 3v product*/

#ifdef XT26G02A
#define    FlashID          0x0BE2
#define    FlashSize        0x10000000      // 256MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif

#ifdef XT26G02B
#define    FlashID          0x0BF2
#define    FlashSize        0x10000000      // 256MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif


#ifdef XT26G02C
#define    FlashID          0x0B12
#define    FlashSize        0x10000000      // 256MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif

#ifdef XT26G02D
#define    FlashID          0x0BD2
#define    FlashSize        0x10000000      // 256MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif

#ifdef XT26G02E
#define    FlashID          0x2C24
#define    FlashSize        0x10000000      // 256MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif




/*4Gbit 1.8v product*/

#ifdef XT26Q04C
#define    FlashID          0x0B23
#define    FlashSize        0x200000000     // 512 MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 12        // for 4K page
#endif
/*4Gbit 3v product*/

#ifdef XT26G04A
#define    FlashID          0x0BE3
#define    FlashSize        0x200000000     // 512 MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 11        // for 2K page
#endif


#ifdef XT26G04C
#define    FlashID          0x0B13
#define    FlashSize        0x200000000     // 512 MB
#define    FLASH_ADDR_TO_PAGE_SHIFT 12        // for 4K page
#endif


#endif    /* end of __XT26_DEF_H__  */

