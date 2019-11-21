
#define HIGHMAX 0x8000000
#define HIGHDONE 0xFFFFFFF


typedef struct {
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
    uint8_t b5;
    uint8_t b6;
    uint8_t b7;
} BIT64BYTES;

typedef uint8_t BB[8];

typedef struct {
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
} BASE;

//typedef struct {
//  uint32_t base : 25;
//  uint32_t shift : 7;
//} PIECE;

typedef struct {
    union {
        uint32_t base;
        uint8_t bb[4];
        struct {
            uint8_t b0;
            uint8_t b1;
            uint8_t b2;
            uint8_t b3;
        }b;
    }u;
    uint8_t location : 3;
    uint8_t lowestbit : 5;
} PIECE;

typedef struct {
    uint32_t low;
    uint32_t high;
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
        uint8_t bb[8];
        struct {
            uint32_t low;
            uint32_t high;
        }hl;
    }u;
} BOARD;
