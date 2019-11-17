objects = hex.o cal.o asm.o

# DEBUG
#coptions = -Og -c

coptions = -O3 -c

hex : $(objects)
	cc -o hex $(objects)

hex.o : source/hex.c source/hex.h
	cc $(coptions) source/hex.c

cal.o : source/cal.c source/hex.h source/pieces.h source/dontuse.h
	cc $(coptions) source/cal.c

asm.o : source/asm.c source/hex.h
	cc $(coptions) source/asm.c

.PHONY : clean
clean :
	rm -f hex $(objects) solution.out

