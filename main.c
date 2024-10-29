#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

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

#define TOK_RET 7205622

FILE* c;

bool semi;
int getch() {
    if(semi) {
        semi = false;
        return 59;
    } else {
        int ch = getc(c);
        if(ch == 59) {
            semi = true;
            ch = 0;
        }
        return ch;
    }
}

bool tok_is_num;
int tok_next() {
    int token = 0;
    tok_is_num = false;

    char ch = getch();
    while(ch <= 32) {
        ch = getch();
        if(ch == EOF) return ch;
    }

    if(ch <= 57) tok_is_num = true;

    while(ch > 32) {
        token = 10 * token + (ch - 48);

        ch = getch();
    }

    return token;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [C file]", argv[0]);
        return -1;
    }

    //c = fopen(argv[1], "rb");

    printf("  .globl main\nmain:\n");
    int token = 0;
    while(token != EOF) {
        token = tok_next();
        if(tok_is_num) {
            //
        }
    }

    //fclose(c);
    return 0;
}
