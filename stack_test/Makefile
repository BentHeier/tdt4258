# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

small_test.bin : small_test.elf
	${OBJCOPY} -j .text -O binary $< $@

small_test.elf : small_test.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

small_test.o : small_test.s
	${AS} ${ASFLAGS} $< -o $@

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "small_test.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
