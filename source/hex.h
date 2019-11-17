typedef uint32_t ULONG;
typedef uint16_t USHORT;
typedef unsigned char UCHAR;

#define HIGHMAX 0x8000000
#define HIGHDONE 0xFFFFFFF


typedef struct {
	UCHAR b0;
	UCHAR b1;
	UCHAR b2;
	UCHAR b3;
	UCHAR b4;
	UCHAR b5;
	UCHAR b6;
	UCHAR b7;
} BIT64BYTES;

typedef UCHAR BB[8];

typedef struct {
	UCHAR b0;
	UCHAR b1;
	UCHAR b2;
	UCHAR b3;
} BASE;

//typedef struct {
//	ULONG base : 25;
//	ULONG shift : 7;
//} PIECE;

typedef struct {
	union {
		ULONG base;
		UCHAR bb[4];
		struct {
			UCHAR b0;
			UCHAR b1;
			UCHAR b2;
			UCHAR b3;
		}b;
	}u;
	UCHAR location : 3;
	UCHAR lowestbit : 5;
} PIECE;

typedef struct {
	ULONG low;
	ULONG high;
} BIT64;

typedef struct {
	unsigned char i;
	unsigned int j:10;
	unsigned int next:6;
} SQUARE;

typedef int BOOLEAN;
#define TRUE 1
#define FALSE 0

void CalculateAllpiece(void);
void CalculateSquare(void);
void shift1(void);
void CalculateBitsq(void);
BOOLEAN IsPieceOnSquare(PIECE *piece, int square);
void DisplayPiece(PIECE *piece, int num);
void PrintPattern(PIECE *piece);
BOOLEAN placepiece(PIECE *piece);

#define BIT64TEST(a, b) (a.low & b.low || a.high & b.high)
#define BIT64XOR(result, a, b) \
	result.high = a.high ^ b.high;\
	result.low = a.low ^ b.low
#define BIT64OR(result, a, b) \
	result.high = a.high | b.high;\
	result.low = a.low | b.low
#define BIT64NOTAND(a, b) \
	a.high &= ~(b.high);\
	a.low &= ~(b.low)

/* This is a bit field that represents the board, one bit per square. Top
four bits are unused. */
typedef struct {
	union {
		UCHAR bb[8];
		struct {
			ULONG low;
			ULONG high;
		}hl;
	}u;
} BOARD;

