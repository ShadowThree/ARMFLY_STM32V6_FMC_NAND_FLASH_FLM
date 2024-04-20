#include "stm32f4_fmc.h"
#include <string.h>

int SystemClock_Config(void);
static int NAND_Init(void);
static void MX_GPIO_Init(void);

NAND_HandleTypeDef hnand1;

int Init_fmc(void)
{
  int result = 0;
  SystemInit(); 
  //HAL_Init();
  result = SystemClock_Config();
  if (result == 1)
    return result;
	
	MX_GPIO_Init();
	result = NAND_Init();
  if (result) {
    return 1;
	}
	
	result = HAL_NAND_Reset(&hnand1);
	if (result) {
    return 1;
	}
	
//	NAND_IDTypeDef nand_id = {0};
//	result = HAL_NAND_Read_ID(&hnand1, &nand_id);
//	if(result) {
//		return 1;
//	}
	
	// read ID (0xADF1001D for H27U1G8F2BTR)    // 0xADDC9095 for H27U4G8F2ETR
//	if(((nand_id.Maker_Id != 0xAD) || (nand_id.Device_Id != 0xF1) || (nand_id.Third_Id != 0x00) || (nand_id.Fourth_Id != 0x1D)) &&
//		 ((nand_id.Maker_Id != 0xAD) || (nand_id.Device_Id != 0xDC) || (nand_id.Third_Id != 0x90) || (nand_id.Fourth_Id != 0x95))) {
//			 return 1;
//		 }

  return 0;  
}

FMC_NAND_PCC_TimingTypeDef ComSpaceTiming;
FMC_NAND_PCC_TimingTypeDef AttSpaceTiming;
static int NAND_Init(void)
{
  memset(&hnand1, 0, sizeof(hnand1));
	memset(&ComSpaceTiming, 0, sizeof(ComSpaceTiming));
	memset(&AttSpaceTiming, 0, sizeof(AttSpaceTiming));
	
	HAL_NAND_DeInit(&hnand1);

  hnand1.Instance = FMC_NAND_DEVICE;
  /* hnand1.Init */
  hnand1.Init.NandBank = FMC_NAND_BANK2;
  hnand1.Init.Waitfeature = FMC_NAND_PCC_WAIT_FEATURE_DISABLE;
  hnand1.Init.MemoryDataWidth = FMC_NAND_PCC_MEM_BUS_WIDTH_8;
  hnand1.Init.EccComputation = FMC_NAND_ECC_ENABLE;
  hnand1.Init.ECCPageSize = FMC_NAND_ECC_PAGE_SIZE_2048BYTE;
  hnand1.Init.TCLRSetupTime = 0;
  hnand1.Init.TARSetupTime = 0;
  /* hnand1.Config */
  hnand1.Config.PageSize = 2048;
  hnand1.Config.SpareAreaSize = 64;
  hnand1.Config.BlockSize = 64;
  hnand1.Config.BlockNbr = 1024;
  hnand1.Config.PlaneNbr = 1;
  hnand1.Config.PlaneSize = 1024;
  hnand1.Config.ExtraCommandEnable = ENABLE;
  /* ComSpaceTiming */
  ComSpaceTiming.SetupTime = 252;
  ComSpaceTiming.WaitSetupTime = 252;
  ComSpaceTiming.HoldSetupTime = 252;
  ComSpaceTiming.HiZSetupTime = 252;
  /* AttSpaceTiming */
  AttSpaceTiming.SetupTime = 252;
  AttSpaceTiming.WaitSetupTime = 252;
  AttSpaceTiming.HoldSetupTime = 252;
  AttSpaceTiming.HiZSetupTime = 252;

  if (HAL_NAND_Init(&hnand1, &ComSpaceTiming, &AttSpaceTiming) != HAL_OK)
  {
    return 1;
  }
	return 0;
}

static uint32_t FMC_Initialized = 0;
static void HAL_FMC_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct ={0};
  if (FMC_Initialized) {
    return;
  }
  FMC_Initialized = 1;

  __HAL_RCC_FMC_CLK_ENABLE();

  /** FMC GPIO Configuration
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PD11   ------> FMC_CLE
  PD12   ------> FMC_ALE
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NCE2
  */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void HAL_NAND_MspInit(NAND_HandleTypeDef* hnand)
{
  HAL_FMC_MspInit();
}

static uint32_t FMC_DeInitialized = 0;
static void HAL_FMC_MspDeInit(void)
{
  if (FMC_DeInitialized) {
    return;
  }
  FMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();

  /** FMC GPIO Configuration
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PD11   ------> FMC_CLE
  PD12   ------> FMC_ALE
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NCE2
  */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_7);
}

void HAL_NAND_MspDeInit(NAND_HandleTypeDef* hnand)
{
  HAL_FMC_MspDeInit();
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

int SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return 1;
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    return 1;
  }
	return 0;
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	return HAL_OK;
}

uint32_t HAL_GetTick (void) { 
  static uint32_t ticks = 0U; 
         uint32_t i; 
 
  /* If Kernel is not running wait approximately 1 ms then increment  
     and return auxiliary tick counter value */ 
  for (i = (SystemCoreClock >> 14U); i > 0U; i--) { 
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); 
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); 
  } 
  return ++ticks; 
}

void HAL_Delay(uint32_t Delay) 
{ 
  uint32_t tickstart = HAL_GetTick(); 
  uint32_t wait = Delay; 
 
  /* Add a period to guaranty minimum wait */ 
  if (wait < HAL_MAX_DELAY) 
  { 
    wait += (uint32_t)(1U); 
  } 
 
  while((HAL_GetTick() - tickstart) < wait) 
  { 
    __NOP(); 
  } 
}
