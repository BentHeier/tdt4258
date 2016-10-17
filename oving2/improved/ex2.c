#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "efm32gg.h"
#include "sounds.h"

// Declaration of peripheral setup functions
void setupGPIO();
void setupTimer(uint16_t period);
void setupDAC();
void setupNVIC();

// Main function
int main(void) {
	// Call the peripheral setup functions.
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	// Enable interrupt handling.
	setupNVIC();

	// Enable sleep on exit.
	*SCR = 2;		

	// Wait for interrupt.
	while(1){
		__asm("wfi");	
	}
	return 0;
}

// Sets up the interrupt controller, enabling interrupts.
void setupNVIC() {
	 // Enable GPIO_EVEN and GPIO_ODD interrupts.
	 *ISER0 |= (1 << 1);
	 *ISER0 |= (1 << 11);
	 // Enable TIMER1 interrupts.
	 *ISER0 |= (1 << 12);
}

/* if other interrupt handlers are needed, use the following names: 
   NMI_Handler
   HardFault_Handler
   MemManage_Handler
   BusFault_Handler
   UsageFault_Handler
   Reserved7_Handler
   Reserved8_Handler
   Reserved9_Handler
   Reserved10_Handler
   SVC_Handler
   DebugMon_Handler
   Reserved13_Handler
   PendSV_Handler
   SysTick_Handler
   DMA_IRQHandler
   GPIO_EVEN_IRQHandler
   TIMER0_IRQHandler
   USART0_RX_IRQHandler
   USART0_TX_IRQHandler
   USB_IRQHandler
   ACMP0_IRQHandler
   ADC0_IRQHandler
   DAC0_IRQHandler
   I2C0_IRQHandler
   I2C1_IRQHandler
   GPIO_ODD_IRQHandler
   TIMER1_IRQHandler
   TIMER2_IRQHandler
   TIMER3_IRQHandler
   USART1_RX_IRQHandler
   USART1_TX_IRQHandler
   LESENSE_IRQHandler
   USART2_RX_IRQHandler
   USART2_TX_IRQHandler
   UART0_RX_IRQHandler
   UART0_TX_IRQHandler
   UART1_RX_IRQHandler
   UART1_TX_IRQHandler
   LEUART0_IRQHandler
   LEUART1_IRQHandler
   LETIMER0_IRQHandler
   PCNT0_IRQHandler
   PCNT1_IRQHandler
   PCNT2_IRQHandler
   RTC_IRQHandler
   BURTC_IRQHandler
   CMU_IRQHandler
   VCMP_IRQHandler
   LCD_IRQHandler
   MSC_IRQHandler
   AES_IRQHandler
   EBI_IRQHandler
   EMU_IRQHandler
*/
