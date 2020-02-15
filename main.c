#include "EEPROMDriver.h"

RCC_OscInitTypeDef hOsc;
RCC_ClkInitTypeDef hClk;
GPIO_InitTypeDef hgpio;
I2C_HandleTypeDef hi2c1;

static void connectHSI2PLL(void);
static void configureSysClk(void);
static void configureI2C(void);


uint8_t tmsg[]="First message";
uint8_t t1msg[]="Second message";
uint8_t rmsg[40];
int main()
{
	EEPROM my4kb;
	HAL_Init();
	HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
	HAL_NVIC_EnableIRQ(SysTick_IRQn);
	connectHSI2PLL();
	configureSysClk();
	SystemCoreClockUpdate();
	configureI2C();
	EEPROM_Init(&my4kb,&hi2c1);
	EEPROM_Write(my4kb,0x200,(uint8_t *)tmsg,strlen((const char *)tmsg)+1);
	EEPROM_Erase(my4kb);
	EEPROM_Write(my4kb,0x200,(uint8_t *)t1msg,strlen((const char *)t1msg)+1);
	EEPROM_Read(my4kb,0x200,(uint8_t*)rmsg,strlen((const char *)t1msg)+1);
	
	while(1);
}

static void configureI2C(void)
{
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	hi2c1.Instance=I2C1;
	hi2c1.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.ClockSpeed=100000;
	hi2c1.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.DutyCycle=I2C_DUTYCYCLE_2;
	hi2c1.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
	hi2c1.Init.OwnAddress1=0x0;
	hi2c1.Init.OwnAddress2=0x0;
	
	
	hgpio.Mode=GPIO_MODE_AF_OD;
	hgpio.Alternate=GPIO_AF4_I2C1;
	hgpio.Pin=GPIO_PIN_8 | GPIO_PIN_9;
	hgpio.Pull=GPIO_PULLUP;
	hgpio.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	
	HAL_GPIO_Init(GPIOB,&hgpio);
	HAL_I2C_Init(&hi2c1);
	
	
}
static void connectHSI2PLL(void)
{
	hOsc.OscillatorType=RCC_OSCILLATORTYPE_HSI;
	hOsc.HSIState=RCC_HSI_ON;
	hOsc.HSICalibrationValue=16;
	hOsc.PLL.PLLState=RCC_PLL_ON;
	hOsc.PLL.PLLSource=RCC_PLLSOURCE_HSI;
	hOsc.PLL.PLLM=16;
	hOsc.PLL.PLLN=336;
	hOsc.PLL.PLLP=2;
	HAL_RCC_OscConfig(&hOsc);
	return;
}
static void configureSysClk(void)
{
	hClk.ClockType=RCC_CLOCKTYPE_SYSCLK;
	hClk.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
	hClk.APB1CLKDivider=8;	//Set apb1 clock to 21Mhz
	hClk.APB2CLKDivider=8;	//Set apb2 clock to 21Mhz
	HAL_RCC_ClockConfig(&hClk,FLASH_LATENCY_5);
	RCC->CFGR |=(0x1800 | 0xC000);
	return;
}
void SysTick_Handler()
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
