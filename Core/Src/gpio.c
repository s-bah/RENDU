/*
 * gpio.c - GPIO functions, only for Nucleo-STM32L476 board
 */
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "gpio.h"

#define LED_PORT GPIOA
#define LED_PIN LL_GPIO_PIN_5
#define BUT_PORT GPIOC
#define BUT_PIN LL_GPIO_PIN_13

void GPIO_init(void)
{
// PORT A
LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_GPIOA );
// Green LED (user LED) - PA5
LL_GPIO_SetPinMode(       LED_PORT, LED_PIN, LL_GPIO_MODE_OUTPUT );
LL_GPIO_SetPinOutputType( LED_PORT, LED_PIN, LL_GPIO_OUTPUT_PUSHPULL );

// PORT C
LL_AHB2_GRP1_EnableClock( LL_AHB2_GRP1_PERIPH_GPIOC );
// Blue button - PC13
LL_GPIO_SetPinMode( BUT_PORT, BUT_PIN, LL_GPIO_MODE_INPUT );
}


void LED_GREEN( int val )
{
if	( val )
	LL_GPIO_SetOutputPin(   LED_PORT, LED_PIN );
else	LL_GPIO_ResetOutputPin( LED_PORT, LED_PIN );
}

int BLUE_BUTTON()
{
return ( !LL_GPIO_IsInputPinSet( BUT_PORT, BUT_PIN ) );
}





