
#define DRAWPATTERN 0
#define PRINTLOCATION 0
#define PRINTBASE 0
#define PRINTPATTERN 0
#define DRAWPIECE 0
#define TOTALMEMORY 0
#define PRINTBITSQUARE 0

#define PRINTSQUAREDETAILS 0
#define DISPLAYSQUARE 0

#include "stdio.h"
#include "stdlib.h"
/* #include "malloc.h" */
#include "string.h"
#include "hex.h"
#include "dontuse.h"

void CalculatePositions(void);
void PrintPattern(PIECE *piece);
void DrawPiece(uint32_t pattern);
void CalculatePatterns(void);
int CountPatterns(int p);
void DoAllPieces(void);
void *mymalloc(uint16_t size);
void DoBaseLocation(BIT64 *p, uint32_t *base, uint8_t *location);
void DisplayPiece(PIECE *piece, int num);
BOOLEAN useit(uint32_t base, uint8_t location);

#include "pieces.h"

typedef struct {
	uint32_t pat:25;
	uint32_t shifts:3;
	uint32_t piece:4;
} PATTERN;


#if 0
		printf("987654321098765432109876543210987654321098765432109876543210\n");

	    "05 11 17 23 29 35 41 47 53 59\n"
	    "04 10 16 22 28 34 40 46 52 58\n"
	    "03 09 15 21 27 33 39 45 51 57\n"
        "02 08 14 20 26 32 38 44 50 56\n"
	    "01 07 13 19 25 31 37 43 49 55\n"
	    "00 06 12 18 24 30 36 42 48 54\n"
#endif

uint32_t Value[] = {
//             7654 3210 9876 5432 1098 7654 3210
0x10,       //                             4
0x8,        //                               3
0x200,      //                       9
0x4,        //								  2
0x100,      //                        8
0x4000,     //				  14
0x2,        //								   1
0x80,       //							 7
0x2000,     //				   13
0x80000,    //			19
0x1,        //                                  0
0x40,       //							 6
0x1000,     //					12
0x40000,	//			 18
0x1000000,	//	 24
};
PATTERN *Pattern;

PIECE *Allpiece[12];   /* 12 pointers, where each is a pointer to an array of
							placements of one piece. */
uint16_t Placements[12]; /* Number of placements for each piece. */
SQUARE *Square[60];
int SquareCount[60];
uint32_t Bitsq[32];

uint16_t Totalmemory;

void CalculateAllpiece(void)
{
#if PRINTPATTERN
	int j;
	int i;
	int count;
#endif

	CalculatePatterns();

	CalculatePositions();

	/* Fill in Allpiece arrays. */
	DoAllPieces();

#if PRINTPATTERN
	for (i = 0; i < 12; i++)
	{
		count = 0;
		printf("Piece %d, count=%d\n", i+1, Placements[i]);
		DrawPiece(Allpiece[i][0].u.base);

		for (j = 0; j < (int)Placements[i]; j++)
		{
			int character;

			printf("%d\n", ++count);
			DisplayPiece(&Allpiece[i][j], i);
			printf("9876 54321098 76543210 98765432 10987654 32109876 54321098 76543210\n");
			PrintPattern(&Allpiece[i][j]);
			printf("\n");
			printf("%d, %d, %lx\n", Allpiece[i][j].location,
				Allpiece[i][j].lowestbit, Allpiece[i][j].u.base);

#if DONTUSE
			character = getch();
			if (character == 'w')
			{
				FILE *file;
				if ((file = fopen("hex.out", "at+")) == NULL)
					exit(1);
				fprintf(file, "%d, %lx\n", Allpiece[i][j].location, Allpiece[i][j].u.base);
				fclose(file);
			}
			else if (character == 's')
				break;
#endif

		}
	}
#endif

	free(Pattern);
}

void CalculatePatterns(void)
{
	int i;
	int j;
	int shifts;
	int count;
	int piece;
	uint32_t number;

	for (count = 0; Pieces[count][0] != '0'; count++)
		;	// count pieces

	Pattern = mymalloc((count+1)*sizeof(PATTERN));
	if (!Pattern)
		exit(1);

	for (i = 0; i < count; i++)
	{
		number = 0l;
		for (piece = shifts = j = 0; j < 15; j++)
		{
			if (Pieces[i][j] != ' ')
			{
				number += Value[j];
				if (piece == 0)
				{
					if (Pieces[i][j]-'0' <= 9)
						piece = Pieces[i][j]-'0';
					else
						piece = Pieces[i][j]-'A'+10;
				}
				if (shifts == 0)
				{
					if (j == 0)
						shifts = 2;
					else if (j <= 2)
						shifts = 3;
					else if (j <= 5)
						shifts = 4;
					else if (j <= 9)
						shifts = 5;
					else
						shifts = 6;
				}
			}
		}
		Pattern[i].piece = piece;
		Pattern[i].shifts = shifts;
		Pattern[i].pat = number;
	}
	Pattern[i].piece = 0;
	Pattern[i].shifts = 0;
	Pattern[i].pat = 0;
}

void CalculatePositions(void)
{
	int p;
	int piecenum;
	int count;

	/* Allocate Allpiece arrays. */
	p = 0;
	piecenum = 1;
	while (Pattern[p].pat)
	{
		count = 0;
		while (piecenum == (int)Pattern[p].piece)
		{
			count += CountPatterns(p);
			p++;
		}
		if (! (Allpiece[piecenum-1] = mymalloc(count * sizeof(PIECE)) ))
			exit(1);

#if DRAWPATTERN
		printf("patterns for piece %d = %d\n", piecenum, count);
#endif
		Placements[piecenum-1] = count;
		piecenum++;
	}
}

int CountPatterns(int p)
{
	int i;
	int j;
	int k;
	int shifts;
	int count;
	BIT64 pattern;

	pattern.high = 0l;
	pattern.low = Pattern[p].pat;
	shifts = (int)Pattern[p].shifts;

	count = 0;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < shifts; j++)
		{
			count++;
			if (pattern.high & HIGHMAX)
				goto DONE;
			pattern.high <<= 1;
			if (pattern.low & 0x80000000)
				pattern.high |= 1;
			pattern.low <<= 1;
		}
		for (k = 0; k < (6 - shifts); k++)
		{
			if (pattern.high & HIGHMAX)
				goto DONE;
			pattern.high <<= 1;
			if (pattern.low & 0x80000000)
				pattern.high |= 1;
			pattern.low <<= 1;
		}
	}
DONE:
	return count;
}

void DoAllPieces(void)
{
	int i;
	int j;
	int k;
	int shifts;
	int count;
	BIT64 pattern;
	int p;
	int piecenum;
	uint32_t base;
	uint8_t location;
	uint8_t lowest;

	p = 0;
	count = 0;
	piecenum = 1;

	while (Pattern[p].pat)
	{
		pattern.high = 0l;
		pattern.low = Pattern[p].pat;
		shifts = (int)Pattern[p].shifts;

		if (piecenum != (int)Pattern[p].piece)
		{
			piecenum = (int)Pattern[p].piece;
			count = 0;
		}
		p++;

		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < shifts; j++)
			{
				DoBaseLocation(&pattern, &base, &location);
#if 0
				printf("%lu\n", base);
				printf("%d\n", location);
#endif
				if (useit(base, location))
				{
					for (lowest = 0; lowest < 32; lowest++)
					{
						if (base & Bitsq[lowest])
							break;
					}

					Allpiece[piecenum-1][count].u.base = base;
					Allpiece[piecenum-1][count].location = location;
					Allpiece[piecenum-1][count].lowestbit = lowest;
					count++;
				}

#if 0
				if (Allpiece[0][0].location != 0)
				{
					printf("piece %d, count %d\n", piecenum, count);
					exit(1);
				}
#endif
				if (pattern.high & HIGHMAX)
					goto DONE;
				pattern.high <<= 1;
				if (pattern.low & 0x80000000)
					pattern.high |= 1;
				pattern.low <<= 1;
			}
			for (k = 0; k < (6 - shifts); k++)
			{
				if (pattern.high & HIGHMAX)
					goto DONE;
				pattern.high <<= 1;
				if (pattern.low & 0x80000000)
					pattern.high |= 1;
				pattern.low <<= 1;
			}
		}
DONE:
		Placements[piecenum-1] = count;
	}
}


void PrintPattern(PIECE *piece)
{
	int space;
	int sq;

	space = 4;
	for (sq = 59; sq >= 0; sq--)
	{
		if (IsPieceOnSquare(piece, sq))
			printf("1");
		else
			printf("0");
		space++;
		if ((space % 8) == 0)
			printf(" ");
	}
	printf("\n");
}



BOOLEAN IsPieceOnSquare(PIECE *piece, int square)
{
	int i;
	uint32_t bit;

	i = square - (8 * piece->location);

	if (i < 0 || i >= 32)
		return FALSE;

	bit = Bitsq[i];

	if (bit & piece->u.base)
		return TRUE;

	return FALSE;
}

void CalculateSquare(void)
{
	int i;
	int j;
	int k;
	int sq;
	int count;
	int maxnext;

#if DISPLAYSQUARE
	printf("Calculate square\n");
#endif

	/* Count the pieces on each square.*/
	for (sq = 0; sq < 60; sq++)
	{
		for (i = 0; i < 12; i++)
		{
			for (j = 0; j < (int)Placements[i]; j++)
			{
				/* Check piece to see whether it fits over this square. */
				if (IsPieceOnSquare(&Allpiece[i][j], sq))
					SquareCount[sq]++;
			}
		}
#if COUNTPIECES
		printf("%d pieces on square %d\n", SquareCount[sq], sq);
#endif
	}

	for (sq = 0; sq < 60; sq++)
	{
		Square[sq] = mymalloc(SquareCount[sq]*sizeof(SQUARE));
#if DISPLAYSQUARE
		printf("Square %d is %d bytes\n", sq, SquareCount[sq]*sizeof(SQUARE) );
#endif
		if (!Square[sq])
		{
			printf("Out of memory\n");
			exit(1);
		}
	}

	/* Fill in the squares. */
	for (sq = 0; sq < 60; sq++)
	{
		count = 0;
		for (i = 0; i < 12; i++)
		{
			for (j = 0; j < (int)Placements[i]; j++)
			{
				/* Check piece to see whether it fits over this square. */
				if (IsPieceOnSquare(&Allpiece[i][j], sq))
				{
					Square[sq][count].i = (unsigned char)i;
					Square[sq][count].j = j;
					if ((int)Square[sq][count].i != i ||
						(int)Square[sq][count].j != j)
					{
						printf("error\n");
						exit(1);
					}
					count++;
//					printf("square=%d, i=%d, j=%d\n", sq, i, j);
//					DisplayBoard(Allpiece[i][j], i);
				}
			}
		}
	}

#if DISPLAYSQUARE
	printf("Calculate nexts\n");
#endif

	maxnext = 0;
	for (sq = 0; sq < 60; sq++)
	{
		for (i = 0; i < 12; i++)
		{
			/* count the number of i pieces that fit on this square. */
			for (k = count = 0; count < SquareCount[sq]; count++)
			{
				if (i == (int)Square[sq][count].i)
					k++;
			}
			if (k > maxnext)
				maxnext = k;
			for (count = 0; count < SquareCount[sq]; count++)
			{
				if (i == (int)Square[sq][count].i)
				{
					Square[sq][count].next = k;
					k--;
				}
			}
		}
	}

#if DISPLAYSQUARE
	printf("Max next %d\n", maxnext);
#endif

	for (sq = 0; sq < 60; sq++)
	{
#if DISPLAYSQUARE
		printf("square %d, pieces %d\n", sq, SquareCount[sq]);
#endif

#if PRINTSQUAREDETAILS
		for (count = 0; count < SquareCount[sq]; count++)
		{
			i = Square[sq][count].i;
			j = Square[sq][count].j;
			printf("square= %d, i=%d, j=%d, next=%d", sq, i, j,
				Square[sq][count].next);
			if (IsPieceOnSquare(&Allpiece[i][j].base, sq))
				printf("\n");
			else
			{
				printf(" bad square\n");
				DisplayBoard(Allpiece[i][j], i);
			}
		}
#endif
	}
#if TOTALMEMORY
	printf("Total memory allocated %u\n", Totalmemory);
#endif
}

void CalculateBitsq(void)
{
	int i;
	uint32_t bit;

	bit = 1;
	for (i = 0; i < 32; i++)
	{
		Bitsq[i] = bit;
#if PRINTBITSQUARE
		printf("Bitsq[%d] %lx\n", i, Bitsq[i]);
#endif
		bit <<= 1;
	}
}

void *mymalloc(uint16_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (ptr)
		Totalmemory += size;
	return ptr;
}

/*
Display the one piece on the board.
*/
void DisplayPiece(PIECE *piece, int num)
{
	int i, j;
	int sq;

	for (j = 5; j >= 0; j--)
	{
		for (i = 0; i < 10; i++)
		{
			sq = j + i * 6;
			if (IsPieceOnSquare(piece, sq))
			{
				if (num < 9)
					printf("%d", num+1);
				else
					printf("%c", 'a' + num + 1 - 10);
			}
			else
				printf("x");
		}
		printf("\n");
	}
}

BOOLEAN useit(uint32_t base, uint8_t location)
{
	int i;

	for (i = 0; Dontuse[i].u.base != 0; i++)
	{
		if (location == Dontuse[i].location && base == Dontuse[i].u.base)
			return FALSE;
	}
	return TRUE;
}

void DoBaseLocation(BIT64 *p, uint32_t *base, uint8_t *location)
{
	uint8_t loc;
	uint8_t bb[8];
	BASE ba;

	if (p->low & 0xff)
		loc = 0;
	else if (p->low & 0xff00)
		loc = 1;
	else if (p->low & 0xff0000)
		loc = 2;
	else if (p->low & 0xff000000)
		loc = 3;
	else if (p->high & 0xff)
		loc = 4;
	else if (p->high & 0xff00)
		loc = 5;
	else if (p->high & 0xff0000)
		loc = 6;
	else if (p->high & 0xff000000)
		loc = 7;

#if PRINTLOCATION
	printf("%d\n", loc);
#endif

	memcpy(bb, p, 8);

#if PRINTBASE
	printf("%d, %d, %d, %d, %d, %d, %d, %d\n",
		bb[7], bb[6], bb[5], bb[4], bb[3], bb[2], bb[1], bb[0]);
#endif

	ba.b0 = bb[loc+0];
	if (loc+1 <= 7)
		ba.b1 = bb[loc+1];
	else
		ba.b1 = 0;
	if (loc+2 <= 7)
		ba.b2 = bb[loc+2];
	else
		ba.b2 = 0;
	if (loc+3 <= 7)
		ba.b3 = bb[loc+3];
	else
		ba.b3 = 0;

	*base = *(uint32_t *)&ba;
	*location = loc;
}

void DrawPiece(uint32_t pattern)
{
	int i, j;
	uint32_t jbit, bit;

	jbit = 16;
	for (j = 0; j < 6; j++)
	{
		bit = jbit;
		for (i = 0; i < 10; i++)
		{
			if (bit & pattern)
				printf("p");
			else
				printf(" ");
			bit <<= 6;
		}
		printf("\n");
		jbit >>= 1;
	}
}
