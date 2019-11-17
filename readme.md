# hex

Find the solutions to a Pentomino puzzle on a 10x6 board.

Here are the pieces:

![Alt text](./pieces.svg)
<img src="./pieces.svg">

Run hex to see the solutions.

```
cd source

make

cc -g -c asm.c
cc -o hex hex.o cal.o asm.o

./hex

....................................Total time 26.00 sec
See solution.out

tail solution.out

Found solution 9356
elapsed time 26.00 sec
6662222444
6869952741
8899555741
8c9bb57731
8ccbaaa731
ccbbaa3331
```
