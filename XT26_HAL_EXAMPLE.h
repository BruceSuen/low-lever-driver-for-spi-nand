
/* XT26_HAL_EXAMPLE.h -Hardware Abstraction Layer example Code head file */
 
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

#ifndef    __XT26_HAL_EXAMPLE_H__
#define    __XT26_HAL_EXAMPLE_H__
#include "XT26_TYPE.h"
#include "XT26_DEF.h"
#include "XT26_CMD.h"


#define GPIO_SPI
//#define SPI_CONTROLLER



typedef enum {
    HAL_OK,
    HAL_FAIL,
}HAL_STATUS;


#ifdef GPIO_SPI




#else // USE SPI CONTROLLER
	


#endif
void Initial_Spi(void);
HAL_STATUS FLASH_READ(uint8 command, uint32 addr, uint8 dummy,uint8 addr_mode, uint8 addr_bytes,uint8 data_mode,uint8 *data_buffer,uint32 length);
HAL_STATUS FLASH_WRITE(uint8 command, uint32 addr,uint8 addr_mode,uint8 addr_bytes,uint8 data_mode, uint8 *data_buffer, uint32 length);


#endif
