
/* XT26_HAL_EXAMPLE.c -Hardware Abstraction Layer example Code */
 
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
 
#include "XT26_HAL_EXAMPLE.h"



extern void  HAL_QSPI_GPIO_Init(void);
extern void  HAL_CS_Set(u8 sw);
extern void  HAL_CLK_Set(u8 sw);
extern void  HAL_HOLD_Set(u8 sw);
extern void  HAL_WP_Set(u8 sw);
extern void  HAL_SO_Set(u8 sw);
extern void  HAL_SI_Set(u8 sw);
extern uint8 HAL_SI_Get(void);
extern uint8 HAL_SO_Get(void);
extern uint8 HAL_WP_Get(void);
extern uint8 HAL_HOLD_Get(void);


/*
 * Function:       Initial_Spi
 * Arguments:      None
 * Description:    Initial spi flash state and wait flash warm-up
 *                 (enable read / write).
 * Return Message: None
 */

uint8 read_buffer[2112];
uint8 write_buffer[2112];


void Initial_Spi(void)
{
	#ifdef GPIO_SPI

	uint16 JDEC_ID;
	uint8  status;
	uint32 i,addr = 0;

	
	HAL_QSPI_GPIO_Init();
	SPI_NAND_RDID(&JDEC_ID);
	printf("\r\nJDEC_ID=0x%x\r\n",JDEC_ID);
	SPI_NAND_GET_FEATURE(0xA0, &status);
	printf("\r\nA0H=0x%x\r\n",status);
	SPI_NAND_GET_FEATURE(0xB0, &status);
	printf("\r\nB0H=0x%x\r\n",status);
	SPI_NAND_GET_FEATURE(0xC0, &status);
	printf("\r\nC0H=0x%x\r\n",status);

    printf("\r\n set A0H=0x00\r\n");
	SPI_NAND_SET_FEATURE(0xA0, 0x00);
	SPI_NAND_GET_FEATURE(0xA0, &status);
	printf("\r\nA0H=0x%x\r\n",status);

    printf("\r\nQuad Enable\r\n");
	SPI_NAND_QUAD_SWITCH(1);
	SPI_NAND_GET_FEATURE(0xB0, &status);
	printf("\r\nB0H=0x%x\r\n",status);


 

		addr = 0x100000;
		//erase 

		printf("\r\n erase block,addr=0x%x\r\n",addr);

		SPI_NAND_BE(addr);

		printf("\r\nprogram cmd=0x32\r\n");
		for(i=0; i < 2112;i++)
		{
			write_buffer[i] = i&0xFF;
		}

		// X4 program
		SPI_NAND_4PP_LOAD(0, write_buffer, 2112, 0);
		SPI_NAND_PROGRAM_EXEC(addr);

		//X1 read
		printf("\r\nread cmd=0x03\r\n");

		SPI_NAND_READ(addr);
		memset(read_buffer,0x00,2112);
		SPI_NAND_READ_CACHE(0, read_buffer, 2112, 0);

	#else
		
	
	#endif

   
}

/*
 * Function:       CS_Low, CS_High
 * Arguments:      None.
 * Description:    Chip select go low / high.
 * Return Message: None.
 */
void CS_Low(void)
{
	#ifdef GPIO_SPI
	HAL_CS_Set(0);
	#else
	// insert your code

    #endif
}

void CS_High(void)
{
	#ifdef GPIO_SPI
	HAL_CS_Set(1);
	#else
	// insert your code

    #endif
}


void SET_CLK_HIGH(void)
{
   
	HAL_CLK_Set(1);
}

void SET_CLK_LOW(void)
{
	HAL_CLK_Set(0);
}

void SET_SI_HIGH(void)
{
	HAL_SI_Set(1);
}

void SET_SI_LOW(void)
{
	HAL_SI_Set(0);
}

uint8 GET_SI(void)
{
	return HAL_SI_Get();
}

void SET_SO_HIGH(void)
{
	HAL_SO_Set(1);
}

void SET_SO_LOW(void)
{
	HAL_SO_Set(0);
}

uint8 GET_SO(void)
{
	return HAL_SO_Get();
}


void SET_WP_HIGH(void)
{
	HAL_WP_Set(1);
}

void SET_WP_LOW(void)
{
	HAL_WP_Set(0);
}

uint8 GET_WP(void)
{
	return HAL_WP_Get();
}


void SET_HOLD_HIGH(void)
{
	HAL_HOLD_Set(1);
}

void SET_HOLD_LOW(void)
{
	HAL_HOLD_Set(0);
}

uint8 GET_HOLD(void)
{
	return HAL_HOLD_Get();
}


#if 1
/*
 * Function:       InsertDummyCycle
 * Arguments:      dummy_cycle, number of dummy clock cycle
 * Description:    Insert dummy cycle of SCLK
 * Return Message: None.
 */
void InsertDummyCycle( uint8 dummy_cycle )
{
#ifdef GPIO_SPI
    uint8 i;
    for( i=0; i < dummy_cycle; i++ )
    {
        SET_CLK_LOW();
        SET_CLK_HIGH();
        SET_CLK_LOW();
    }
#else
    //--- insert your code here. ---//

#endif
}

/*
 * Function:       SendByte
 * Arguments:      byte_value, data transfer to flash
 *                 transfer_type, select different type of I/O mode.
 *                 Six mode:
 *                 SIO, single IO
 *                 DIO, dual IO
 *                 QIO, quad IO
 *                 DTSIO, double transfer rate SIO
 *                 DTDIO, double transfer rate DIO
 *                 DTQIO, double transfer rate QIO
 * Description:    Send one byte data to flash
 * Return Message: None.
 */
void SendByte( uint8 byte_value, uint8 transfer_type )
{
#ifdef GPIO_SPI
    uint16 i;
    uint8 cycle_cnt;

    switch( transfer_type )
    {
#ifdef SIO
    case SIO: // single I/O
        cycle_cnt = 8;
        for( i= 0; i < cycle_cnt; i++ )
        {
            SET_CLK_LOW();
            if ( (byte_value & 0x80) == 0x80 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }
            byte_value = byte_value << 1;
            SET_CLK_HIGH();
        }
        break;
#endif
#ifdef DIO
    case DIO: // dual I/O
        cycle_cnt = 4;
        for( i= 0; i < cycle_cnt; i++ )
        {
            SET_CLK_LOW();
           
            if ( (byte_value & 0x80) == 0x80 ){
                SET_SO_HIGH();
            }
            else{
                SET_SO_LOW();
            }
			  
            if ( (byte_value & 0x40) == 0x40 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }
			byte_value = byte_value << 2;
            SET_CLK_HIGH();
        }
        break;
#endif
#ifdef QIO
    case QIO: // quad I/O
        cycle_cnt = 2;
        for( i= 0; i < cycle_cnt; i++ )
        {
            SET_CLK_LOW();
			
            if ( (byte_value & 0x80) == 0x80 ){
                SET_HOLD_HIGH();
            }
            else{
                SET_HOLD_LOW();
            }
			
		   
            if ( (byte_value & 0x40) == 0x40 ){
                SET_WP_HIGH();
            }
            else{
                SET_WP_LOW();
            }
		
                
             if ( (byte_value & 0x20) == 0x20 ){
                SET_SO_HIGH();
            }
            else{
                SET_SO_LOW();
            }
			
                
             if ( (byte_value & 0x10) == 0x10 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }
          
            byte_value = byte_value << 4;
           SET_CLK_HIGH();
        }
        break;
#endif

#ifdef DTSIO
    case DTSIO: //  Double transfer rate single I/O
        cycle_cnt = 4;
        for( i= 0; i < cycle_cnt; i++ )
        {
           SET_CLK_LOW();
            if ( (byte_value & 0x80) == 0x80 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }
            byte_value = byte_value << 1;

             SET_CLK_HIGH();
            if ( (byte_value & 0x80) == 0x80 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }
            byte_value = byte_value << 1;
        }
        break;
#endif
#ifdef DTDIO
    case DTDIO: //  Double transfer rate dual I/O
        cycle_cnt = 2;
        for( i= 0; i < cycle_cnt; i++ )
        {
            SET_CLK_LOW();
     
            if ( (byte_value & 0x80) == 0x80 ){
                SET_SO_HIGH();
            }
            else{
                SET_SO_LOW();
            }
			
	  
            if ( (byte_value & 0x40) == 0x40 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }
            byte_value = byte_value << 2;

            SET_CLK_HIGH();
  
            if ( (byte_value & 0x80) == 0x80 ){
                SET_SO_HIGH();
            }
            else{
                SET_SO_LOW();
            }
			   
             if ( (byte_value & 0x40) == 0x40 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }
            byte_value = byte_value << 2;
        }
        break;
#endif
#ifdef DTQIO
    case DTQIO: //  Double transfer rate quad I/O
        SET_CLK_LOW();
        if ( (byte_value & 0x80) == 0x80 ){
                SET_HOLD_HIGH();
            }
            else{
                SET_HOLD_LOW();
            }
			
		   
            if ( (byte_value & 0x40) == 0x40 ){
                SET_WP_HIGH();
            }
            else{
                SET_WP_LOW();
            }
		
                
             if ( (byte_value & 0x20) == 0x20 ){
                SET_SO_HIGH();
            }
            else{
                SET_SO_LOW();
            }
			
                
             if ( (byte_value & 0x10) == 0x10 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }

      SET_CLK_HIGH();
        byte_value = byte_value << 4;
            
        if ( (byte_value & 0x80) == 0x80 ){
                SET_HOLD_HIGH();
            }
            else{
                SET_HOLD_LOW();
            }
			
		   
            if ( (byte_value & 0x40) == 0x40 ){
                SET_WP_HIGH();
            }
            else{
                SET_WP_LOW();
            }
		
                
             if ( (byte_value & 0x20) == 0x20 ){
                SET_SO_HIGH();
            }
            else{
                SET_SO_LOW();
            }
			
                
             if ( (byte_value & 0x10) == 0x10 ){
                SET_SI_HIGH();
            }
            else{
                SET_SI_LOW();
            }
        break;

#endif
    default:
        break;
    }
#else
    switch( transfer_type )
    {
#ifdef SIO
    case SIO: // Single I/O
        //--- insert your code here for single IO transfer. ---//
        break;
#endif
#ifdef DIO
    case DIO: // Dual I/O
        //--- insert your code here for dual IO transfer. ---//
        break;
#endif
#ifdef QIO
    case QIO: // Quad I/O
        //--- insert your code here for quad IO transfer. ---//
        break;
#endif
#ifdef DTSIO
    case DTSIO: // Double transfer rate Single I/O
        //--- insert your code here for DT single IO transfer. ---//
        break;
#endif
#ifdef DTDIO
    case DTDIO: // Double transfer rate Dual I/O
        //--- insert your code here for DT dual IO transfer. ---//
        break;
#endif
#ifdef DTQIO
    case DTQIO: // Double transfer rate Quad I/O
        //--- insert your code here for DT quad IO transfer. ---//
        break;
#endif
    default:
        break;
    }
#endif  /* End of SendByte */
}
/*
 * Function:       GetByte
 * Arguments:      byte_value, data receive from flash
 *                 transfer_type, select different type of I/O mode.
 *                 Six mode:
 *                 SIO, single IO
 *                 DIO, dual IO
 *                 QIO, quad IO
 *                 PIO, parallel IO
 *                 DTSIO, double transfer rate SIO
 *                 DTDIO, double transfer rate DIO
 *                 DTQIO, double transfer rate QIO
 * Description:    Get one byte data to flash
 * Return Message: 8 bit data
 */
uint8 GetByte( uint8 transfer_type )
{
	  uint16 i;
    uint8 cycle_cnt;
    uint8 data_buf = 0;

#ifdef GPIO_SPI
    
 

    switch( transfer_type )
    {
#ifdef SIO
    case SIO: // single I/O
        // Set GPIO as input
      
        // End  GPIO
		cycle_cnt = 8;

        for( i= 0; i < cycle_cnt; i++ )
        {
         SET_CLK_LOW();
		 
		 if(GET_SO())
		 {
			data_buf = data_buf | (0x80 >> i);
		 }
           
         SET_CLK_HIGH();
        }
        break;
#endif
#ifdef DIO
    case DIO: // dual I/O
        // Set GPIO as input ( need pull to high )
     
        // End  GPIO
		cycle_cnt = 4;

        for( i= 0; i < cycle_cnt; i++ )
        {
        SET_CLK_LOW();
		
		if(GET_SO())
		 {
			data_buf = data_buf | (0x80 >> (2*i));
		 }
		 
		 if(GET_SI())
		 {
			data_buf = data_buf | (0x40 >> (2*i));
		 }
   
         SET_CLK_HIGH();
        }
        break;
#endif
#ifdef QIO
    case QIO: // quad I/O
        // Set  GPIO as input ( need pull to high )
      
        // End GPIO
		
		cycle_cnt = 2;

        for( i= 0; i < cycle_cnt; i++ )
        {
        SET_CLK_LOW();
		
		if(GET_HOLD())
		 {
			data_buf = data_buf | (0x80 >> (4*i));
		 }
		 
		 if(GET_WP())
		 {
			data_buf = data_buf | (0x40 >> (4*i));
		 }
		 
		 if(GET_SO())
		 {
			data_buf = data_buf | (0x20 >> (4*i));
		 }
		 
		 if(GET_SI())
		 {
			data_buf = data_buf | (0x10 >> (4*i));
		 }
   
         SET_CLK_HIGH();
        }
        break;
#endif

#ifdef DTSIO
    case DTSIO: //  Double transfer rate Single I/O
        // Set GPIO as input ( need pull to high )
      
        // End  GPIO

        cycle_cnt = 4;
        for( i= 0; i < cycle_cnt; i++ )
        {
          SET_CLK_LOW();
		  
		 if(GET_SO())
		 {
			data_buf = data_buf | (0x80 >> (2*i));
		 }
		 
          SET_CLK_HIGH();
		  
		if(GET_SO())
		 {
			data_buf = data_buf | (0x40 >> (2*i));
		 }
           
        }
        break;
#endif
#ifdef DTDIO
    case DTDIO: //  Double transfer rate Dual I/O
        // Set  GPIO as input ( need pull to high )
        
        // End  GPIO

        cycle_cnt = 2;
        for( i= 0; i < cycle_cnt; i++ )
        {
            SET_CLK_LOW();
			if(GET_SO())
			 {
				data_buf = data_buf | (0x80 >> (4*i));
			 }
			 if(GET_SI())
			 {
				data_buf = data_buf | (0x40 >> (4*i));
			 }
            SET_CLK_HIGH();
			if(GET_SO())
			 {
				data_buf = data_buf | (0x20 >> (4*i));
			 }
			 if(GET_SI())
			 {
				data_buf = data_buf | (0x10 >> (4*i));
			 }
            
        }
      
        break;
#endif
#ifdef DTQIO
    case DTQIO: //  DTR qual I/O
        // Set GPIO as input ( need pull to high )
        
        // End GPIO

        SET_CLK_LOW();
		if(GET_HOLD())
		 {
			data_buf = data_buf | 0x80 ;
		 }
		 if(GET_WP())
		 {
			data_buf = data_buf | 0x40;
		 }
		 
		 if(GET_SO())
		 {
			data_buf = data_buf | 0x20 ;
		 }
		 if(GET_SI())
		 {
			data_buf = data_buf | 0x10;
		 }
       
        SET_CLK_HIGH();
		
		if(GET_HOLD())
		 {
			data_buf = data_buf | 0x08 ;
		 }
		 if(GET_WP())
		 {
			data_buf = data_buf | 0x04;
		 }
		 
		 if(GET_SO())
		 {
			data_buf = data_buf | 0x02 ;
		 }
		 if(GET_SI())
		 {
			data_buf = data_buf | 0x01;
		 }
       
        break;
#endif

    default:
        break;

    }

#else
    switch( transfer_type )
    {
#ifdef SIO
    case SIO: // Single I/O
        //--- insert your code here for single IO receive. ---//
        break;
#endif
#ifdef DIO
    case DIO: // Dual I/O
        //--- insert your code here for dual IO receive. ---//
        break;
#endif
#ifdef QIO
    case QIO: // Quad I/O
        //--- insert your code here for qual IO receive. ---//
        break;
#endif

#ifdef DTSIO
    case DTSIO: // Double transfer rate Single I/O
        //--- insert your code here for DT single IO receive. ---//
        break;
#endif
#ifdef DTDIO
    case DTDIO: // Double transfer rate Dual I/O
        //--- insert your code here for DT dual IO receive. ---//
        break;
#endif
#ifdef DTQIO
    case DTQIO: // Double transfer rate Qual I/O
        //--- insert your code here for DT quad IO receive. ---//
#endif
    default:
        break;
    }
#endif  /* End of GetByte */
    return data_buf;
}
#endif


/*
 * Function:       SendFlashAddr
 * Arguments:      flash_address, 32 bit flash memory address
 *                 io_mode, I/O mode to transfer address
 *                 addr_4byte_mode,
 * Description:    Send flash address with 3-byte or 4-byte mode.
 * Return Message: None
 */
void SendFlashAddr( uint32 flash_address, uint8 io_mode, uint8 addr_bytes)
{
  
    if(addr_bytes==4)
	{
		SendByte( (flash_address >> 24), io_mode );
	}
    /* A23-A0 */
	if(addr_bytes>=3)
	{
		SendByte( (flash_address >> 16), io_mode );
	}
	if(addr_bytes>=2)
	{
		SendByte( (flash_address >> 8), io_mode );
	}
    SendByte( flash_address, io_mode );
}

/*Adapter functions*/
/*
 * Function:       FLASH_READ
 * Arguments:      command,write a SIO command byte to flash
 *                 addr,system address to be used
 *                 data_buffer,Pointer to the data buffer containing data to be written
 *                 length, number of bytes to be read             
 * Description:    Flash read operation
 *                 
 *                    
 *                     
 *              
 * Return Value: status of the operation
*/

HAL_STATUS FLASH_READ(uint8 command, uint32 addr, uint8 dummy,uint8 addr_mode,uint8 addr_bytes,uint8 data_mode,uint8 *data_buffer,uint32 length)
{
	HAL_STATUS  status = HAL_OK;
	uint32 data_cycle = 0;
	CS_Low();

	//Send command 
	SendByte(command,SIO);
	
	//Send Address
	if((addr != ADDRESS_NULL) && (addr_mode != IO_NULL))
	{
		SendFlashAddr(addr,addr_mode,addr_bytes);
	}
	
	//Send Dummy cycle
	if(dummy!=0)
	{
		InsertDummyCycle(dummy);
	}
	
	
	//Receive Data
	if(length != 0)
	{
		for(data_cycle = 0;data_cycle < length;data_cycle++)
		{
			*(data_buffer+data_cycle) = GetByte(data_mode);
		}
	}
	
	CS_High();
	return  status;
}


/*
 * Function:       FLASH_WRITE
 * Arguments:      command,write a SIO command byte to flash
 *                 addr,system address to be used
 *                 data_buffer,Pointer to the data buffer containing data to be written
 *                 length, number of bytes to be write             
 * Description:    Flash write operation
 *                 
 *                    
 *                     
 *              
 * Return Value: status of the operation
*/

HAL_STATUS FLASH_WRITE(uint8 command, uint32 addr,uint8 addr_mode,uint8 addr_bytes,uint8 data_mode, uint8 *data_buffer, uint32 length)
{
    HAL_STATUS  status = HAL_OK;
	uint32 data_cycle = 0;
	
	CS_Low();

	//Send command 
	SendByte(command,SIO);
	
	
	//Send Address
	if((addr != ADDRESS_NULL) && (addr_mode != IO_NULL))
	{
		SendFlashAddr(addr,addr_mode,addr_bytes);
	}
	
	// Send Data
	if(length != 0)
	{
		for(data_cycle = 0;data_cycle < length;data_cycle++)
		{
			SendByte(*(data_buffer+data_cycle),data_mode);
		}
	}
	CS_High();
	return status;
}
