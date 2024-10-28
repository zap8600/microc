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

FILE* c;

bool semicolon;
char getch() {
    char ch;
    if(semicolon) {
        semicolon = false;
        ch = 59;
    }
    int ch = getc(c); // TODO: Expand subset, as this is incompatible with this subset of C, meaning the compiler can't compile itself.
    if(ch == 59) {
        semicolon = true;
        ch = 0;
    }
    return (char)ch;
}

bool tok_is_num;
bool tok_is_call;

int tok_next() {
    char ch = getch();
    while((ch = getch()) == 32) {}

    int token = 0;
    char lasttwo[2];
    lasttwo[0] = 0;
    lasttwo[1] = 0;

    if(ch <= 57) {
        tok_is_num = true;
    }

    while(ch != 32) {
        lasttwo[1] = lasttwo[0];
        lasttwo[0] = ch;

        token = 10 * token + (ch - 48);

        ch = getch();
    }

    if(*(uint16_t*)lasttwo == 0x2f2f) {
        while(ch != 10) {
            ch = getch();
        }
        return tok_next();
    } else if(*(uint16_t*)lasttwo == 0x2f2a) {
        while(*(uint16_t*)lasttwo != 65475) {
            *(uint16_t*)lasttwo = tok_next();
        }
        return tok_next();
    }
    if(*(uint16_t*)lasttwo == 0x2829) {
        trailing_parens = true;
    }

    return token;
}

int* symboltable;
int codegenoffet;

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [C file]", argv[0]);
        return -1;
    }

    c = fopen(argv[1], "rb");
    symboltable = (int*)malloc(1 * 1024 * 1024);

    int token = 0;
    /*printf("  .globl main\nmain:\n");
    while(1) {
        token = tok_next();
        if(token != TOK_INT) {

        }
    }
    */
    printf("%d\n", tok_next());

    fclose(c);
    free(symboltable);
    return 0;
}
