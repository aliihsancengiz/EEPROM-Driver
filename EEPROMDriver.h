#ifndef __EEPROM__
#define __EEPROM__
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEVADRESS 0xA0		//A0,A1,A2 tied the ground
#define MAXSIZE   0x1000	//4096 Bytes

struct epprom
{
	I2C_HandleTypeDef heeprom;
	uint16_t Totalsize;
	uint16_t DevAdress;
};
typedef struct epprom EEPROM; 
void EEPROM_Init(EEPROM * teep,I2C_HandleTypeDef * heeprom)
{
	teep->DevAdress=DEVADRESS;
	teep->Totalsize=MAXSIZE;
	teep->heeprom=*heeprom;
	
}
HAL_StatusTypeDef EEPROM_Write(EEPROM tepp, uint16_t MemAddress,uint8_t * pdata,uint8_t len)
{
	HAL_StatusTypeDef returnValue;
	uint8_t *data;
 
	data = (uint8_t*)malloc(sizeof(uint8_t)*(len+2));
	data[0] = (uint8_t) ((MemAddress & 0xFF00) >> 8);
	data[1] = (uint8_t) (MemAddress & 0xFF);
	memcpy(data+2, pdata, len);
	returnValue = HAL_I2C_Master_Transmit(&tepp.heeprom,tepp.DevAdress, data, len + 2, HAL_MAX_DELAY);
	if(returnValue != HAL_OK)
    return returnValue;
	free(data);
	while(HAL_I2C_Master_Transmit(&tepp.heeprom,tepp.DevAdress, 0, 0, HAL_MAX_DELAY) != HAL_OK)
	{
		
	}
  return HAL_OK;
}
HAL_StatusTypeDef EEPROM_Read(EEPROM tepp,uint16_t MemAddress, uint8_t *pData, uint16_t len)
{
    HAL_StatusTypeDef returnValue;
    uint8_t addr[2];
    addr[0] = (uint8_t) ((MemAddress & 0xFF00) >> 8);
    addr[1] = (uint8_t) (MemAddress & 0xFF);
    returnValue = HAL_I2C_Master_Transmit(&tepp.heeprom, tepp.DevAdress, addr, 2, HAL_MAX_DELAY);
    if(returnValue != HAL_OK)
    return returnValue;
    returnValue = HAL_I2C_Master_Receive(&tepp.heeprom,tepp.DevAdress, pData, len, HAL_MAX_DELAY);
    return returnValue;
}
void EEPROM_Erase(EEPROM teep)
{
	EEPROM_Write(teep,0,0,teep.Totalsize);
}
#endif
