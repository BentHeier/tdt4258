.syntax unified

.include "efm32gg.s"

/////////////////////////////////////////////////////////////////////////////
//
// Exception vector table
// This table contains addresses for all exception handlers
//
/////////////////////////////////////////////////////////////////////////////

.section .vectors

	.long   stack_top               /* Top of Stack                 */
	.long   _reset                  /* Reset Handler                */
	.long   dummy_handler           /* NMI Handler                  */
	.long   dummy_handler           /* Hard Fault Handler           */
	.long   dummy_handler           /* MPU Fault Handler            */
	.long   dummy_handler           /* Bus Fault Handler            */
	.long   dummy_handler           /* Usage Fault Handler          */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* SVCall Handler               */
	.long   dummy_handler           /* Debug Monitor Handler        */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* PendSV Handler               */
	.long   dummy_handler           /* SysTick Handler              */

	/* External Interrupts */
	.long   dummy_handler
	.long   gpio_handler            /* GPIO even handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   gpio_handler            /* GPIO odd handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler

.section .text

/////////////////////////////////////////////////////////////////////////////
//
// Reset handler
// The CPU will start executing here after a reset
//
/////////////////////////////////////////////////////////////////////////////

	.globl  _reset
	.type   _reset, %function
	.thumb_func
_reset: 
	B     en_gpio_clk

// Enables the clock for the GPIO-controller in order to use it
en_gpio_clk:
	LDR   R1, cmu_base_addr
	
	// Loads the current value for the register into R2:
	LDR   R2, [r1, #CMU_HFPERCLKEN0]
	
	// Sets GPIO-bit to 1 by bit-shift and OR:
	MOV   R3, #1
	LSL   R3, R3, #CMU_HFPERCLKEN0_GPIO
	ORR   R2, R2, R3
	
	// Stores the result:
	STR   R2, [R1, #CMU_HFPERCLKEN0]
	
// Initializes the LEDs	
init_leds:	
	LDR   R1, gpio_pa_base_addr
	
	MOV   R3, #0x2
	STR   R3, [R1, #GPIO_CTRL]
	MOV   R3, #0x55555555
	STR   R3, [R1, #GPIO_MODEH]
	MOV   R3, #0xaaaaaaaa
	
	STR   R3, [R1, #GPIO_DOUT]
	MOV   R4, #0xFF
	MOV   R7, #0

// Initalizes the buttons	
init_btns:
	LDR   R2, gpio_pc_base_addr
	
	// Set GPIO mode to input
	MOV   R3, #0x33333333
	STR   R3, [R2, #GPIO_MODEL]
	
	// Activate internal pull-up resistors (the buttons dont have any)
	MOV   R3, #0xFF
	STR   R3, [R2, #GPIO_DOUT]

init_interrupt:
	LDR   R6, gpio_base_addr
        MOV   R3, #0x22222222
	STR   R3, [R6, #GPIO_EXTIPSELL]
	MOV   R3, #0xff
	STR   R3, [R6, #GPIO_EXTIFALL]
	STR   R3, [R6, #GPIO_IEN]
	
	LDR   R6, iser0_addr
	LDR   R3, =0x802
	STR   R3, [R6]	
	
	LDR   R6, scr_addr
	MOV   R3, #6
	STR   R3, [R6]
	WFI
	B     .

	


	
cmu_base_addr:
	.long CMU_BASE

gpio_pa_base_addr:
	.long GPIO_PA_BASE

gpio_pc_base_addr:
	.long GPIO_PC_BASE

gpio_base_addr:
	.long GPIO_BASE

iser0_addr:
	.long ISER0

scr_addr:
	.long SCR


/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

    .thumb_func
gpio_handler:  
	ldr R3, [R1, #GPIO_DOUT]
	mvn R3, R3 
	str R3, [R1, #GPIO_DOUT]
	ldr R6, gpio_base_addr
	ldr R3, [R6, #GPIO_IF]
	str R3, [R6, #GPIO_IFC]

        // Reads current button state
	LDR   R3, [R2, #GPIO_DIN]
	
	// Inverts the values (1 meaning pressed equals less headache)
	MVN   R3, R3
	
	// Finds the diff between last and current state
	EOR   R4, R4, R3
	
	// Filters on pressed-state (we only respond to btn-down)
	AND   R4, R4, R3
	
check_left:
	// Filter out all but button 1
	AND   R5, R4, #1
	
	// If BTN1 changed AND is pressed, update leds, else branch
	CBZ   R5, check_right
	
	// Subtract 1 from the current led-position
	SUB   R7, R7, #1
	
	// Check if we got outside range (R7 < 0)
	CMP   R7, #-1
	IT    EQ
	// Set back to 0 if we did
	MOVEQ R7, #0
	
check_right:
	// Filter out all but button 3
	AND   R5, R4, #0x4
	
	// If changed AND pressed, update leds, else branch
	CBZ   R5, end_of_loop
	
	// Add 1 to current led-position
	ADD   R7, R7, #1
	
	// Check if we got outside range (R7 > 7)
	CMP   R7, #8
	IT    EQ
	// Set back to 7 if we did
	MOVEQ R7, #7
	
end_of_loop:
	// Sets R5s R7th bit to 1, rest to 0
	MOV   R5, #1
	LSL   R5, R7
	
	// Invert it (LEDs are active-low)
	MVN   R5, R5
	
	// Shift them 8 bits (LEDs start at bit 8)
	LSL   R5, #8
	
	// Update the LED values
	STR   R5, [R1, #GPIO_DOUT]
	
	// Store current button state for comparing when looping
	MOV   R4, R3
	
	BX    lr
	

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:  
	b .  // do nothing

