# Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

interrupt.bin : interrupt.elf
	${OBJCOPY} -j .text -O binary $< $@

interrupt.elf : interrupt.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

interrupt.o : interrupt.s
	${AS} ${ASFLAGS} $< -o $@

polling.bin : polling.elf
	${OBJCOPY} -j .text -O binary $< $@

polling.elf : polling.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

polling.o : polling.s
	${AS} ${ASFLAGS} $< -o $@

.PHONY : upload_polling
upload_polling:
	-eACommander.sh -r --address 0x00000000 -f "polling.bin" -r

.PHONY : upload_interrupt
upload_interrupt:
	-eACommander.sh -r --address 0x00000000 -f "interrupt.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
