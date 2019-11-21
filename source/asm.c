
#include <stdint.h>
#include "hex.h"

#define USEASMREMOVEPIECE 0

extern BOARD Board;

/* remove piece */
void RemovePiece(PIECE *piece)
{
    /* xor the piece off. */
#if USEASMREMOVEPIECE
_asm {
    push si
    mov si, [ebp+4] ; piece

    mov bl, [esi+4] ; loc
    and bx, 7

    lodsw               ; piece->b0, piece->b1
    xor word ptr Board[ebx], ax

    lodsw
    xor word ptr Board[ebx+2], ax

    pop si
}
#else
    Board.u.bb[piece->location+0] ^= piece->u.b.b0;
    Board.u.bb[piece->location+1] ^= piece->u.b.b1;
    Board.u.bb[piece->location+2] ^= piece->u.b.b2;
    Board.u.bb[piece->location+3] ^= piece->u.b.b3;
#endif

}

#if USEASMREMOVEPIECE
BOOLEAN placepiece(PIECE *piece)
{
//  uint8_t b;
//  uint8_t xor;
//  BOARD board;
//  int i;

_asm
{
    push si
    push di

    mov si, WORD PTR [ebp+4]        ; piece
    mov edi, offset Board
    mov bl, byte ptr [esi+4]
    and bx, 7
    add di, bx

    lodsw                   ; mov ax, first piece
    mov dx, ax
    xor ax, [edi]
    mov cx, ax
    and ax, dx
    cmp ax, dx
    jnz donefalse

    lodsw
    mov dx, ax
    xor ax, [edi+2]
    mov bx, ax
    and ax, dx
    cmp ax, dx
    jnz donefalse

    mov ax, cx
    stosw
    mov ax, bx
    stosw

    mov ax, 1
    jmp short done

donefalse:
    sub ax, ax

done:
    pop di
    pop si
}
}
#else

BOOLEAN placepiece(PIECE *piece)
{
    uint8_t b;
    uint8_t xor;
    BOARD board;
//  int i;

    board = Board;

#if 0
    for (i = 0; i < 4; i++)
    {
        b = piece->u.bb[i];
        if (b)
        {
            xor = b ^ Board.u.bb[piece->location+i];
            b = board.u.bb[piece->location+i] = b | Board.u.bb[piece->location+i];
            if (xor != b)
                return FALSE;
        }
    }
#endif

    b = piece->u.bb[0];
    if (b)
    {
        xor = board.u.bb[piece->location+0] = b ^ Board.u.bb[piece->location+0];
        if ((xor & b) != b)
            return FALSE;
    }
    b = piece->u.bb[1];
    if (b)
    {
        xor = board.u.bb[piece->location+1] = b ^ Board.u.bb[piece->location+1];
        if ((xor & b) != b)
            return FALSE;
    }
    b = piece->u.bb[2];
    if (b)
    {
        xor = board.u.bb[piece->location+2] = b ^ Board.u.bb[piece->location+2];
        if ((xor & b) != b)
            return FALSE;
    }
    b = piece->u.bb[3];
    if (b)
    {
        xor = board.u.bb[piece->location+3] = b ^ Board.u.bb[piece->location+3];
        if ((xor & b) != b)
            return FALSE;
    }

    Board = board;

    return TRUE;
}

#endif
