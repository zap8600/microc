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

FILE* c;

int semicolon;
int ch;

void getch() {
    if(semicolon) {
        semicolon = 0;
        ch = 59;
    }
    ch = getc(c); // TODO: Expand subset, as this is incompatible with this subset of C, meaning the compiler can't compile itself.
    if(ch == 59) {
        semicolon = 1;
        ch = 0;
    }
}

int token;
int ch_1;
int ch_0;
int tok_is_num;
int tok_is_call;
int trailing_parens;

void tok_next() {
    getch();
    if(ch == 32) {
        while(ch == 32) {
            getch();
        }
    }

    token = 0;

    if(ch <= 57) {
        tok_is_num = 1;
    }

    while(ch != 32) {
        ch_1 = ch_0;
        ch_0 = ch;

        token = token * 10;
        ch = ch - 48;
        token = token + ch;

        getch();
    }

    ch = ch_0;
    if(ch == 12079) {
        //
    }
    if(ch == 12074) {
        /**/
    }
    if(ch == 10281) {
        trailing_parens = 1;
    }

    ch = token;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [C file]\n", argv[0]);
    }

    c = fopen(argv[1], "rb");
}
