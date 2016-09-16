# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

ex1.bin : ex1.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1.elf : ex1.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

ex1.o : ex1.s
	${AS} ${ASFLAGS} $< -o $@

interrupt.bin : interrupt.elf
	${OBJCOPY} -j .text -O binary $< $@

interrupt.elf : interrupt.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

interrupt.o : interrupt.s
	${AS} ${ASFLAGS} $< -o $@

small_test.bin : small_test.elf
	${OBJCOPY} -j .text -O binary $< $@

small_test.elf : small_test.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

small_test.o : small_test.s
	${AS} ${ASFLAGS} $< -o $@

polling.bin : polling.elf
	${OBJCOPY} -j .text -O binary $< $@

polling.elf : polling.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

polling.o : polling.s
	${AS} ${ASFLAGS} $< -o $@

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1.bin" -r

.PHONY : upload_polling
upload_polling:
	-eACommander.sh -r --address 0x00000000 -f "polling.bin" -r

.PHONY : upload_test
upload_test:
	-eACommander.sh -r --address 0x00000000 -f "small_test.bin" -r

.PHONY : upload_interrupt
upload_interrupt:
	-eACommander.sh -r --address 0x00000000 -f "interrupt.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
