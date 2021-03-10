/* XT26_CMD.c - SPI and QPI interface command set */
 
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

#include "XT26_CMD.h"
#include "XT26_HAL_EXAMPLE.h"



void Set_Timer( SPI_NAND_INFO *fptr  )
{
    fptr->Timeout_Value = (fptr->Tus)/COUNT_ONE_MICROSECOND;
    fptr->Timer_counter = 0;
}

BOOL Check_Timer( SPI_NAND_INFO *fptr  )
{
    if( fptr->Timer_counter < fptr->Timeout_Value ){
        fptr->Timer_counter = fptr->Timer_counter + 1;
        return TIMENOTOUT;
    }else{
        return TIMEOUT;
    }
}

/*
 * Function:     SPI_NAND_RESET
 * Arguments:    None
 * Return Value: LLD_Success
 * Description:  The reset command FFh resets the read/program/erase operation            
 */
LLD_STATUS SPI_NAND_RESET(void)
{
    FLASH_WRITE(SPI_NAND_CMD_RESET,ADDRESS_NULL,IO_NULL,0,IO_NULL,NULL,0);

    /* Wait reset finish */
	if( WaitFlashReady() == READY )
    {
        return LLD_Success;
    } 
    else
    {
        return LLD_OperationTimeOut;
    }
   
}

/*
 * Function:       WaitFlashReady
 * Arguments:      None             
 * Description:    If flash is ready return READY.             
 *                 If flash is time-out return TIMEOUT.
 * Return Message: READY, TIMEOUT
 */
BOOL WaitFlashReady( void )
{
    SPI_NAND_INFO flash_info;
    flash_info.Tus = FLASH_TIMEOUT_VALUE;
    Set_Timer( &flash_info );

    while( Check_Timer( &flash_info ) != TIMEOUT )
    {
        if( CheckStatus( SR0_OIP ) == READY )
            return READY;
    }

    return TIMEOUT;
}

/*
 * Function:     CheckStatus
 * Arguments:    check_flag -> the status bit to check
 * Return Value: READY, BUSY
 * Description:  Check status register bit 7 ~ bit 0
 */
BOOL CheckStatus( uint8 check_flag )
{
    uint8 status;
    
    SPI_NAND_GET_FEATURE(SPI_NAND_STATUS_REG, &status );
    if( (status & check_flag) == check_flag )
        return BUSY;
    else
        return READY;
}

/*
 * Function:       IsFlashQIO
 * Arguments:      None.
 * Description:    If flash QE bit = 1: return TRUE
 *                                 = 0: return FALSE.
 * Return Message: TRUE, FALSE
 */
BOOL IsFlashQIO( void )
{
    uint8  status;
    SPI_NAND_GET_FEATURE(SPI_NAND_FEATURE_REG, &status );
    if( (status & FLASH_QE_MASK) == FLASH_QE_MASK )
        return TRUE;
    else
        return FALSE;
}
/*
 * Function:       SPI_NAND_GET_FEATURE
 * Arguments:      addr, Set Feature address 
 *                 status_reg, 8 bit buffer to store Feature register value
 * Description:    Check Features Settings.              
 * Return Message: LLD_Success
  */
LLD_STATUS SPI_NAND_GET_FEATURE( uint8 addr, uint8 *status_reg )
{
   FLASH_READ(SPI_NAND_CMD_GET_FEATURE,addr,0,SIO,1,SIO,status_reg,1);
   return LLD_Success;
}

/*
 * Function:       SPI_NAND_SET_FEATURE
 * Arguments:      addr, Set Feature address
 *                 value, 8 bit Feature register value to update
 * Description:    Write Features Settings.           
 * Return Message: LLD_Success
 */
LLD_STATUS SPI_NAND_SET_FEATURE( uint8 addr, uint8 value )
{
	FLASH_WRITE(SPI_NAND_CMD_SET_FEATURE,addr,SIO,1,SIO,&value,1);
    return LLD_Success;
}


/*
 * Function:       SPI_NAND_ECC_STAT_READ
 * Arguments:      status_reg, 8 bit buffer to store register value
 * Description:    Check ECC status              
 * Return Message: LLD_Success
  */

LLD_STATUS SPI_NAND_ECC_STAT_READ( uint8 *status_reg )
{
    uint8  status;
    SPI_NAND_GET_FEATURE(SPI_NAND_STATUS_REG, &status );
	*status_reg = status >> SPI_NAND_ECC_BITS_POS;

    return LLD_Success;
}


/*
 * Function:       SPI_NAND_ECC_SWITCH
 * Arguments:      sw, set or clear ECC_EN bit
 *          
 * Description:    ecc enable or disable.           
 * Return Message: LLD_Success
*/

LLD_STATUS SPI_NAND_ECC_SWITCH(uint8 sw)
{
	uint8 feature;
	SPI_NAND_GET_FEATURE(SPI_NAND_FEATURE_REG,&feature);

	if(sw)
	{
		SPI_NAND_SET_FEATURE(SPI_NAND_FEATURE_REG, feature|ECC_EN_MASK);
	}
	else
	{
		SPI_NAND_SET_FEATURE(SPI_NAND_FEATURE_REG, feature|(~ECC_EN_MASK));

	}
	
	return LLD_Success;

}


/*
 * Function:       SPI_NAND_QUAD_SWITCH
 * Arguments:      sw, set or clear QE bit
 *          
 * Description:    quad mode enable or disable.           
 * Return Message: LLD_Success
*/

LLD_STATUS SPI_NAND_QUAD_SWITCH(uint8 sw)
{
	uint8 feature;
	SPI_NAND_GET_FEATURE(SPI_NAND_FEATURE_REG,&feature);

	if(sw)
	{
		SPI_NAND_SET_FEATURE(SPI_NAND_FEATURE_REG, feature|FLASH_QE_MASK);
	}
	else
	{
		SPI_NAND_SET_FEATURE(SPI_NAND_FEATURE_REG, feature&(~FLASH_QE_MASK));
	}
	return LLD_Success;

}

/*
 * Function:       SPI_NAND_WREN
 * Arguments:      None
 * Description:    The WREN instruction is for setting rite Enable Latch
 *                 (WEL) bit.
 * Return Message: LLD_Success
 */
LLD_STATUS SPI_NAND_WREN( void )
{
	FLASH_WRITE(SPI_NAND_CMD_WREN,0,IO_NULL,0,IO_NULL,NULL,0);
    return LLD_Success;
}

/*
 * Function:       SPI_NAND_WRDI
 * Arguments:      None
 * Description:    The WRDI instruction is to reset
 *                 Write Disable Latch (WEL) bit.
 * Return Message: LLD_Success
 */
LLD_STATUS SPI_NAND_WRDI( void )
{
	FLASH_WRITE(SPI_NAND_CMD_WRDI,0,IO_NULL,0,IO_NULL,NULL,0);
    return LLD_Success;
}

/*
 * ID Command
 */

/*
 * Function:       SPI_NAND_RDID
 * Arguments:      identification, 16 bit buffer to store id
 * Description:    The RDID instruction is to read the manufacturer ID
 *                 of 1-byte and followed by Device ID of 1-byte.
 * Return Message: LLD_Busy,LLD_Success
 */
LLD_STATUS SPI_NAND_RDID( uint16 *identification )
{
    uint8  gdata_buffer[2];

    /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;

    FLASH_READ(SPI_NAND_CMD_RDID, 0x00, 0,SIO,1,SIO,gdata_buffer,2);

    // Store identification
    *identification =  (gdata_buffer[0] << 8) | gdata_buffer[1];

    return LLD_Success;
}

/*
 * Function:       SPI_NAND_RUID
 * Arguments:      uid , 128 bit buffer to store uid
 * Description:    The RUID instruction is to read the unique id 
 * Return Message: LLD_Busy,LLD_Success
 */
LLD_STATUS SPI_NAND_RUID( uint8 *uid )
{
    /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;

    FLASH_READ(SPI_NAND_CMD_RUID, 0x00, 0,SIO,4,SIO,uid,16);
    return LLD_Success;
}

/*
 * Read Command
 */
/*
 * Function:       SPI_NAND_READ
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The READ instruction is for reading data from array to cache.
 * Return Message: LLD_AddrInvalid, LLD_Busy, LLD_Success,LLD_OperationTimeOut
 */
LLD_STATUS SPI_NAND_READ( uint32 flash_address )
{
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddrInvalid;

    /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;
	
	FLASH_READ(SPI_NAND_CMD_READ, flash_address >> FLASH_ADDR_TO_PAGE_SHIFT, 0,SIO,3,SIO,NULL,0);

    /* Wait data transfer from array to cache finish */
    if( WaitFlashReady() == READY )
    {
        return LLD_Success;
    } 
    else
    {
        return LLD_OperationTimeOut;
    }
}

/*
 * Random Data Read Command
 */
/*
 * Function:       SPI_NAND_READ_CACHE
 * Arguments:      col_address, 16 bit flash memory address
 *                 data_buf, data buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 addr_flag, define wrap bit and Plane select bit 
 * Description:    The READ_CACHE instruction is for reading data out from cache on SO.
 * Return Message: LLD_Success
 */
LLD_STATUS SPI_NAND_READ_CACHE( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
	FLASH_READ(SPI_NAND_CMD_READ_CACHE, col_address|(addr_flag <<12), 8,SIO,2,SIO,data_buf,byte_length);
    return LLD_Success;
}

/*
 * Random Data Read Cache*2 Command
 */
/*
 * Function:       SPI_NAND_READ_CACHE2
 * Arguments:      col_address, 16 bit flash memory address
 *                 data_buf, data buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 addr_flag, define wrap bit and Plane select bit
 * Description:    The READ_CACHE2 instruction is for reading data out from cache on SI and SO.
 * Return Message: LLD_Success
 */
LLD_STATUS SPI_NAND_READ_CACHE2( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
    FLASH_READ(SPI_NAND_CMD_READ_CACHE2, col_address|(addr_flag <<12), 8,SIO,2,DIO,data_buf,byte_length);
    return LLD_Success;
}

/*
 * Random Data Read Cache*2 Dual I/O Command
 */
/*
 * Function:       SPI_NAND_READ_CACHE2_DUALIO
 * Arguments:      col_address, 16 bit flash memory address
 *                 data_buf, data buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 addr_flag, define wrap bit and Plane select bit
 * Description:    The READ_CACHE2_DUALIO instruction is for reading data out from cache on SI and SO.
 * Return Message: LLD_Success
 */
LLD_STATUS SPI_NAND_READ_CACHE2_DUALIO( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
    FLASH_READ(SPI_NAND_CMD_READ_CACHE2_DUALIO, col_address|(addr_flag <<12), 4,DIO,2,DIO,data_buf,byte_length);
    return LLD_Success;
}

/*
 * Random Data Read Cache*4 Command
 */
/*
 * Function:       SPI_NAND_READ_CACHE4
 * Arguments:      col_address, 16 bit flash memory address
 *                 data_buf, data buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 addr_flag, define wrap bit and Plane select bit 
 * Description:    The READ_CACHE4 instruction is for reading data out from cache on SI, SO,
 *                 WP and HOLD.
 * Return Message: LLD_QuadNotEnable, LLD_Success
 */
LLD_STATUS SPI_NAND_READ_CACHE4( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
    // Check QE bit
    if( IsFlashQIO() != TRUE )  return LLD_QuadNotEnable;
	FLASH_READ(SPI_NAND_CMD_READ_CACHE4, col_address|(addr_flag <<12), 8,SIO,2,QIO,data_buf,byte_length);
    return LLD_Success;
}

/*
 * Random Data Read Cache*4 Quad I/O Command
 */
/*
 * Function:       SPI_NAND_READ_CACHE4_QUADIO
 * Arguments:      col_address, 16 bit flash memory address
 *                 data_buf, data buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 addr_flag, define wrap bit and Plane select bit 
 * Description:    The READ_CACHE4_QUADIO instruction is for reading data out from cache on SI, SO,
 *                 WP and HOLD.
 * Return Message: LLD_QuadNotEnable, LLD_Success
 */
LLD_STATUS SPI_NAND_READ_CACHE4_QUADIO( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
    // Check QE bit
    if( IsFlashQIO() != TRUE )  return LLD_QuadNotEnable;
	FLASH_READ(SPI_NAND_CMD_READ_CACHE4_QUADIO, col_address|(addr_flag << 12), 2,QIO,2,QIO,data_buf,byte_length);
    return LLD_Success;
}

/*
 * Program load Command
 */
/*
 * Function:       SPI_NAND_PP_LOAD
 * Arguments:      col_address, 16 bit col address
 *                 data_buf, buffer of source data to program
 *                 byte_length, byte length of data to program
 *                 addr_flag, define Plane select bit (only for 2Gb and 4Gb)
 * Description:    load program data with cache reset first
 * Return Message: LLD_Busy, LLD_Success,
 */
LLD_STATUS SPI_NAND_PP_LOAD( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
    /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;

   
	  FLASH_WRITE(SPI_NAND_CMD_PP_LOAD,col_address| (addr_flag<<12),SIO,2,SIO,data_buf,byte_length);
    return LLD_Success;
}

/*
 * Quad IO Page Program load  Command
 */
/*
 * Function:       SPI_NAND_4PP_LOAD
 * Arguments:      col_address, 16 bit col address
 *                 data_buf, buffer of source data to program
 *                 byte_length, byte length of data to program
 *                 addr_flag, define Plane select bit 
 * Description:    load program data with 4 data input
 * Return Message: LLD_Busy, LLD_Success
 */
LLD_STATUS SPI_NAND_4PP_LOAD( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
    /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;

	FLASH_WRITE(SPI_NAND_CMD_4PP_LOAD,col_address|(addr_flag<<12),SIO,2,QIO,data_buf,byte_length);
    return LLD_Success;
}


/*
 * Program load Random Data Command
 */
/*
 * Function:       SPI_NAND_PP_RAND_LOAD
 * Arguments:      col_address, 16 bit col address
 *                 data_buf, buffer of source data to program
 *                 byte_length, byte length of data to program
 *                 addr_flag, define Plane select bit 
 * Description:    load program data without cache reset 
 * Return Message: LLD_Success               
 */
LLD_STATUS SPI_NAND_PP_RAND_LOAD( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
     /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;
	FLASH_WRITE(SPI_NAND_CMD_PP_RAND_LOAD,col_address|(addr_flag<<12),SIO,2,SIO,data_buf,byte_length);
    
	return LLD_Success;
}


/*
 * Quad IO Page Program load Random Data Command
 */
/*
 * Function:       SPI_NAND_4PP_RAND_LOAD
 * Arguments:      col_address, 16 bit col address
 *                 data_buf, buffer of source data to program
 *                 byte_length, byte length of data to program
 *                 addr_flag, define Plane select bit 
 * Description:    load program random data with 4 data input
 * Return Message: LLD_Success
 */
LLD_STATUS SPI_NAND_4PP_RAND_LOAD( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
     /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;

	FLASH_WRITE(SPI_NAND_CMD_4PP_RAND_LOAD,col_address|(addr_flag<<12),SIO,2,QIO,data_buf,byte_length);

	return LLD_Success;
}

/*
 * Quad IO Page Program load Random Data QUAQ I/O Command
 */
/*
 * Function:       SPI_NAND_4PP_RAND_LOAD_QUADIO
 * Arguments:      col_address, 16 bit col address
 *                 data_buf, buffer of source data to program
 *                 byte_length, byte length of data to program
 *                 addr_flag, define Plane select bit 
 * Description:    load program random data with 4 data input
 * Return Message: LLD_Success
 */
LLD_STATUS SPI_NAND_4PP_RAND_LOAD_QUADIO( uint16 col_address, uint8 *data_buf, uint32 byte_length, uint16 addr_flag )
{
    /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;
	FLASH_WRITE(SPI_NAND_CMD_4PP_RAND_LOAD_QUADIO,col_address|(addr_flag<<12),QIO,2,QIO,data_buf,byte_length);
    return LLD_Success;
}

/*
 * Program Execute Command
 */
/*
 * Function:       SPI_NAND_PROGRAM_EXEC
 * Arguments:      address, 32 bit flash memory address
 * Description:    Enter block/page address,no data,execute
 * Return Message: LLD_Success,LLD_OperationTimeOut
 */
LLD_STATUS SPI_NAND_PROGRAM_EXEC( uint32 address )
{
    uint8 status;
	//send write enable command
    SPI_NAND_WREN();
	
	FLASH_WRITE(SPI_NAND_CMD_PROGRAM_EXEC,address>>FLASH_ADDR_TO_PAGE_SHIFT,SIO,3,IO_NULL,NULL,0);

    /* Wait page program finish */
    if( WaitFlashReady() == READY )
    {
        /* Check program result */
        SPI_NAND_GET_FEATURE(SPI_NAND_STATUS_REG, &status );
        if( (status & SR2_ProgramFail ) == SR2_ProgramFail )
            return LLD_ProgramFailed;
        else
            return LLD_Success;
    } 
    else
    {
        return LLD_OperationTimeOut;
    }
}

/*
 * Function:       SPI_NAND_BE
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The BE instruction is for erasing the data
 * Return Message: LLD_AddrInvalid, LLD_Busy, LLD_Success,
 *                 LLD_OperationTimeOut
 */
LLD_STATUS SPI_NAND_BE( uint32 flash_address )
{
    uint8 status;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddrInvalid;

    /* Check flash is busy or not */
    if( CheckStatus( SR0_OIP ) != READY ) return LLD_Busy;

    //send write enable command

    SPI_NAND_WREN();

    FLASH_WRITE(SPI_NAND_CMD_BE,flash_address>>FLASH_ADDR_TO_PAGE_SHIFT,SIO,3,IO_NULL,NULL,0);

      /* Wait page program finish */
    if( WaitFlashReady() == READY )
    {
        /* Check program result */
        SPI_NAND_GET_FEATURE(SPI_NAND_STATUS_REG, &status );
        if( (status & SR2_EraseFail ) == SR2_EraseFail )
            return LLD_EraseFailed;
        else
            return LLD_Success;
    } 
    else
    {
        return LLD_OperationTimeOut;
    }
}
