#include "FlashOS.h"
#include <stdint.h>
#include "stm32f4_fmc.h"

#define PAGE_SIZE		(2048)
#define BLOCK_SIZE	(PAGE_SIZE * 64)

int Init (unsigned long adr, unsigned long clk, unsigned long fnc)
{
  return (Init_fmc());
}

int UnInit (unsigned long fnc)
{
  return (0);
}

int EraseChip (void)
{	
	return 0;
}

int EraseSector (unsigned long adr)
{
	if(adr != DEV_ADDR) {
		return 1;
	}
	
	adr -= DEV_ADDR;
	NAND_AddressTypeDef nand_addr = {.Page = 0, .Block = adr / BLOCK_SIZE, .Plane = 0};
	if(HAL_OK != HAL_NAND_Erase_Block(&hnand1, &nand_addr)) {
		return 1;
	}
	return 0;
}

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
	if(adr != DEV_ADDR) {
		return 1;
	}
	
	adr -= DEV_ADDR;
	NAND_AddressTypeDef nand_addr = {.Page = adr % BLOCK_SIZE / PAGE_SIZE, .Block = adr / BLOCK_SIZE, .Plane = 0};
	if(HAL_OK != HAL_NAND_Write_Page_8b(&hnand1, &nand_addr, buf, 1)) {
		return 1;
	}
	return 0;
}

unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf){
	
	int result = 0;
	static uint8_t read_buf[PAGE_SIZE] = {0};
	
	result = Init_fmc();
	if (result != 0) {
		return 1;
	}
	
	adr -= DEV_ADDR;
	NAND_AddressTypeDef nand_addr = {.Page = adr % BLOCK_SIZE / PAGE_SIZE, .Block = adr / BLOCK_SIZE, .Plane = 0};
	if(HAL_OK != HAL_NAND_Read_Page_8b(&hnand1, &nand_addr, read_buf, 1)) {
		return 2;
	}
	
	for(uint16_t i = 0; i < PAGE_SIZE; i++) {
		if(read_buf[i] != buf[i]) {
			return (adr + i + DEV_ADDR);
		}
	}
	
	return (adr + sz + DEV_ADDR);
}
