#include <stdio.h>
#include <stdint.h>

#define TOK_INT 6388
#define TOK_VOID 11386
#define TOK_ASM 5631
#define TOK_COMM 65532
#define TOK_SEMI 11
#define TOK_LPAREN 65528
#define TOK_RPAREN 65529
#define TOK_START 20697
#define TOK_DEREF 64653
#define TOK_WHILE_BEGIN 55810
#define TOK_IF_BEGIN 6232
#define TOK_BODY_BEGIN 5
#define TOK_BLK_BEGIN 75
#define TOK_BLK_END 77
#define TOK_ASSIGN 13
#define TOK_ADDR 65526
#define TOK_SUB 65533
#define TOK_ADD 65531
#define TOK_MUL 65530
#define TOK_AND 65526
#define TOK_OR 76
#define TOK_XOR 46
#define TOK_SHL 132
#define TOK_SHR 154
#define TOK_EQ 143
#define TOK_NE 65399
#define TOK_LT 12
#define TOK_GT 14
#define TOK_LE 133
#define TOK_GE 153

/*
This `atoi` implementation is from [the `sectorc` page](https://xorvoid.com/sectorc.html).
*/

int atoi(const char) {
    int n = 0;
    while(1) {
        char c = *s++;
        if(!c) break;
        n = 10 * n + (c - '0');
    }
    return n;
}

uint16_t codegenoffset; // Set to 0 by `.bss` segment.
// TODO: I need a symbol buffer and a codegen output buffer

int main() {
    //
}
