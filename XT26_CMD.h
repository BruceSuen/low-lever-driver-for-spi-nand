/* XT26_CMD.h - SPI and QPI interface command hex code and function prototype */
 
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
#ifndef    __XT26_SPI_NAND_H__
#define    __XT26_SPI_NAND_H__

#include    "XT26_DEF.h"

/*** XT26 series command hex code definition ***/
// Read ID Command
#define    SPI_NAND_CMD_RDID                       0x9F    //RDID (Read Identification)
#define    SPI_NAND_CMD_RUID                       0x4B    //RUID (Read unique ID)
// Feature Commands
#define    SPI_NAND_CMD_GET_FEATURE                0x0F    //Get features
#define    SPI_NAND_CMD_SET_FEATURE                0x1F    //Set features
//Control Commands
#define    SPI_NAND_CMD_WREN                       0x06    //Write Enable
#define    SPI_NAND_CMD_WRDI                       0x04    //Write Disable
#define    SPI_NAND_CMD_RESET                      0xFF    //Reset command
//Read Commands
#define    SPI_NAND_CMD_READ                       0x13    //Array Read
#define    SPI_NAND_CMD_READ_CACHE                 0x03    //Read From Cache
#define    SPI_NAND_CMD_READ_CACHE2                0x3B    //Read From Cache*2
#define    SPI_NAND_CMD_READ_CACHE2_DUALIO         0xBB    //Read From Cache*2 DualIO
#define    SPI_NAND_CMD_READ_CACHE4                0x6B    //Read From Cache*4
#define    SPI_NAND_CMD_READ_CACHE4_QUADIO         0xEB    //Read From Cache*4 Quad IO

// Program Commands
#define    SPI_NAND_CMD_PP_LOAD                    0x02    //Page Program Load
#define    SPI_NAND_CMD_4PP_LOAD                   0x32    //Quad IO Page Program Load

#define    SPI_NAND_CMD_PP_RAND_LOAD               0x84    //Page Program Random Input
#define    SPI_NAND_CMD_4PP_RAND_LOAD              0x34    //Page Program Random Input X4
#define    SPI_NAND_CMD_4PP_RAND_LOAD_QUADIO       0x72    //Page Program Random Input Quad IO
#define    SPI_NAND_CMD_PROGRAM_EXEC               0x10    //Program Execute

//Erase Command
#define    SPI_NAND_CMD_BE                         0xD8    //BLock Erase



#define    ADDRESS_NULL   0xFFFFFFFF



// LLD Status
typedef enum {
    LLD_Success,
    LLD_Busy,
    LLD_OperationTimeOut,
    LLD_ProgramFailed,
    LLD_EraseFailed,
    LLD_ReadIDFailed,
    LLD_CmdInvalid,
    LLD_DataInvalid,
    LLD_AddrInvalid,
    LLD_QuadNotEnable
}LLD_STATUS;


/* Flash Information */
typedef struct {
    /* Timer Variable */
    uint32  Tus;        // time-out length in us
    uint32  Timeout_Value;
    uint32  Timer_counter;
}SPI_NAND_INFO;


/* Basic functions */


/* Utility functions */
BOOL CheckStatus( uint8 check_flag );
BOOL WaitFlashReady( void );
BOOL IsFlashQIO( void );

/* Flash commands */

//Read ID Functions
LLD_STATUS SPI_NAND_RDID( uint16 *identification );
LLD_STATUS SPI_NAND_RUID( uint8 *uid );


//Feature operation Functions
LLD_STATUS SPI_NAND_GET_FEATURE( uint8 addr, uint8 *status_reg );
LLD_STATUS SPI_NAND_SET_FEATURE( uint8 addr, uint8 value );
LLD_STATUS SPI_NAND_ECC_STAT_READ( uint8 *status_reg );
LLD_STATUS SPI_NAND_ECC_SWITCH(uint8 sw);
LLD_STATUS SPI_NAND_QUAD_SWITCH(uint8 sw);

//Control Functions
LLD_STATUS SPI_NAND_WREN( void );
LLD_STATUS SPI_NAND_WRDI( void );
LLD_STATUS SPI_NAND_RESET( void );

// Read Functions
LLD_STATUS SPI_NAND_READ( uint32 flash_address );
LLD_STATUS SPI_NAND_READ_CACHE( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag );
LLD_STATUS SPI_NAND_READ_CACHE2( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag );
LLD_STATUS SPI_NAND_READ_CACHE2_DUALIO( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag );
LLD_STATUS SPI_NAND_READ_CACHE4( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag );
LLD_STATUS SPI_NAND_READ_CACHE4_QUADIO( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag );

// Program Functions
LLD_STATUS SPI_NAND_PP_LOAD( uint16 col_address, uint8 *data_buf, uint32 Length, uint16 addr_flag );
LLD_STATUS SPI_NAND_4PP_LOAD( uint16 col_address, uint8 *data_buf, uint32 Length, uint16 addr_flag );
LLD_STATUS SPI_NAND_PP_RAND_LOAD( uint16 col_address, uint8 *data_buf, uint32 Length, uint16 addr_flag );
LLD_STATUS SPI_NAND_4PP_RAND_LOAD( uint16 col_address, uint8 *data_buf, uint32 Length, uint16 addr_flag );
LLD_STATUS SPI_NAND_4PP_RAND_LOAD_QUADIO( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag );
LLD_STATUS SPI_NAND_PROGRAM_EXEC( uint32 address );

// Erase Function
LLD_STATUS SPI_NAND_BE( uint32 flash_address );




#endif    /* __XT26_SPI_NAND_H__ */
