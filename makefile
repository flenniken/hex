objects = hex.o cal.o asm.o

# DEBUG
#coptions = -Og -c
coptions = -O3 -c

ifeq ($(OS),Windows_NT)
    SEP := "\\"
else
    SEP := "/"
endif

SRC_DIR = source

hex : $(objects)
	cc -o hex $(objects)

hex.o : source$(SEP)hex.c source$(SEP)hex.h
	cc $(coptions) source$(SEP)hex.c

cal.o : source$(SEP)cal.c source$(SEP)hex.h source$(SEP)pieces.h source$(SEP)dontuse.h
	cc $(coptions) source$(SEP)cal.c

asm.o : source$(SEP)asm.c source$(SEP)hex.h
	cc $(coptions) source$(SEP)asm.c

.PHONY : clean
clean :
	rm -f hex $(objects) solution.out *.pyc t.txt

