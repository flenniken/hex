objects = hex.o cal.o asm.o
coptions = -g -c

hex : $(objects)
	cc -o hex $(objects)

hex.o : hex.c hex.h
	cc $(coptions) hex.c

cal.o : cal.c hex.h pieces.h dontuse.h
	cc $(coptions) cal.c

asm.o : asm.c hex.h
	cc $(coptions) asm.c

.PHONY : clean
clean :
	rm -f hex $(objects) solution.out

