# Hex

Find the pentomino puzzle solutions for a 10x6 board.

## Pieces and Board

There are 12 pentomino pieces.

<img src="./pieces.svg">

A solution tiles the board. Here is solution 9356:

<img src="./solution9356.svg">

## Download

Download the code using git clone:

~~~
mkdir -p ~/code/hex
cd ~/code/hex
git clone https://github.com/flenniken/hex.git .
~~~

## Compile

Build the hex program by running the make command.

~~~
make

cc -O3 -c source/hex.c
cc -O3 -c source/cal.c
cc -O3 -c source/asm.c
cc -o hex hex.o cal.o asm.o
~~~

## Run

Run hex to write the solutions to the solutions.out file. It takes
about 15 seconds to run on my computer.

~~~
./hex

....................................
Total time 14.55 sec
See solution.out
~~~

There are 9356 solutions. The last one found is shown below.  It is
the same solution shown in color above. Each piece is numbered 1 to 12
in hexadecimal: 1, 2, 3, 4, 5, 6, 7, 8, 9, a, b, c.

~~~
tail -7 solution.out

6662222444
6869952741
8899555741
8c9bb57731
8ccbaaa731
ccbbaa3331
~~~

## Non-Trival Solutions

A trival solution is created from a known solution by rotation or
reflect of the whole board.

Run trival.py to find the trival and non-trival solutions by post
processing the solutions.out file. There are 2339 non-trival solutions.
Here is the last one:

~~~
python trival.py | tail -7

aaa6657777
1aa6555722
1bb6658882
13bcc88942
13bbcc9942
1333c99444
~~~
