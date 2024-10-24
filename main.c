#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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

int atoi(const char *s) {
    int n = 0;
    while(1) {
        char c = *s++;
        if(!c) break;
        n = 10 * n + (c - '0');
    }
    return n;
}

bool tokisnum = false;
bool tokiscalltrailingparen = false;

uint8_t ch;
uint16_t scratch; // `ax`.
uint16_t currenttoken; // `bx`.
uint16_t lasttwochars; // `cx`.
uint16_t codegenoffset; // `di`.

void tok_next() {
toknext:
    // TODO: Get character.
    ch = (uint8_t)(scratch & 0x00ff);
    if(ch <= 32) {
        goto toknext;
    }

    currenttoken = 0;
    lasttwochars = 0;

    if(ch <= 57) {
        tokisnum = true;
    }

nextch:
    if(ch <= 32) {
        goto done;
    }

done:
    scratch = lasttwochars;

    if(scratch == 0x2f2f) {
        goto comment_double_slash;
    } else if (scratch == 0x2f2a) {
        goto comment_multi_line;
    }

    if(scratch == 0x2829) {
        tokiscalltrailingparen = true;
    }

    scratch = currenttoken;
    return;

comment_double_slash:
    // TODO: Get character.
    while(ch != 10) {
        // TODO: Get character.
    }
    goto toknext;

comment_multi_line:
    tok_next();
    while(scratch != 65475) {
        tok_next();
    }
    goto toknext;
}

int main() {
    codegenoffset = 0;
    goto toknext;
}
