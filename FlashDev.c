/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 - 2018 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        11. December 2019
 * $Revision:    V1.2.1
 *
 * Project:      Flash Device Description for STMicroelectronics STM32F4xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.2.1
 *    Corrected STM32F42xxx_43xxx_OPT Algorithm
 *  Version 1.2.0
 *    Added STM32F4xx_1536 Algorithm
 *  Version 1.1.0
 *    Added STM32F4xx_1024dual Algorithm
 *  Version 1.0.2
 *    Added more Option Byte Algorithm
 *  Version 1.0.1
 *    Added STM32F411xx Option Byte Algorithm
 *  Version 1.0.0
 *    Initial release
 */

#include "FlashOS.h"        // FlashOS Structures

struct FlashDevice const FlashDevice  =  {
	FLASH_DRV_VERS,             // Driver Version, do not modify!
	"STM32V6_NAND_FLASH",    // Device Name (128kB)
	EXT8BIT,                     // Device Type
	DEV_ADDR,                 // Device Start Address
	0x08000000,                 // Device Size in Bytes (128MB)
	2048,                       // Programming Page Size
	0,                          // Reserved, must be 0
	0xFF,                       // Initial Content of Erased Memory
	6000,                        // Program Page Timeout 6000 mSec
	6000,                       // Erase Sector Timeout 6000 mSec
	// Specify Size and Address of Sectors
	0x20000, 0x000000,          // Sector Size  128kB (1024 Sectors)
	SECTOR_END
};




