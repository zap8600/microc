#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Some of these values won't work in my compiler, since they used wrapping on 16-bit integers, while I'm using 32-bit integers
#define TOK_INT 6388
#define TOK_VOID 76922 // 11386 doesn't work here.
#define TOK_ASM 5631
#define TOK_COMM -4 // 65532 doesn't work here
#define TOK_SEMI 11
#define TOK_LPAREN -8 // 65528 doesn't work here.
#define TOK_RPAREN -7 // 65529 doesn't work here.
#define TOK_START 544362713 // 20697 doesn't work here.
#define TOK_DEREF -6161267 // 64653 doesn't work here.
#define TOK_WHILE_BEGIN 7723522 // 55810 doesn't work here.
#define TOK_IF_BEGIN 6232
#define TOK_BODY_BEGIN 5
#define TOK_BLK_BEGIN 75
#define TOK_BLK_END 77
#define TOK_ASSIGN 13
#define TOK_ADDR -10 // 65526 doesn't work here.
#define TOK_SUB -3 // 65533 doesn't work here.
#define TOK_ADD -5 // 65531 doesn't work here.
#define TOK_DIV -1 // Added by me.
#define TOK_MUL -6 // 65530 doesn't work here.
#define TOK_AND -10 // 65526 doesn't work here.
#define TOK_OR 76
#define TOK_XOR 46
#define TOK_SHL 132
#define TOK_SHR 154
#define TOK_EQ 143
#define TOK_NE -137 // 65399 doesn't work here.
#define TOK_LT 12
#define TOK_GT 14
#define TOK_LE 133
#define TOK_GE 153

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
bool tok_is_call;
int tok_next() {
    int token = 0;
    char lasttwo[2];
    tok_is_num = false;
    tok_is_call = false;

    char ch = getch();
    while(ch <= 32) {
        ch = getch();
    }

    if(ch <= 57) tok_is_num = true;

    while(ch > 32) {
        lasttwo[1] = lasttwo[0];
        lasttwo[0] = ch;

        token = 10 * token + (ch - 48);

        ch = getch();
    }

    if((*(uint16_t*)lasttwo) == 0x2f2f) {
        while(ch != 10) {
            ch = getch();
        }
        return tok_next();
    } else if((*(uint16_t*)lasttwo) == 0x2f2a) {
        int ttoken = 0;
        while(ttoken != 65475) {
            ttoken = tok_next();
        }
        return tok_next();
    }
    if((*(uint16_t*)lasttwo) == 0x2829) {
        tok_is_call = true;
    }

    return token;
}

int compile_unary(const int ttoken) {
    int token = ttoken;
    if(tok_is_num) {
        printf("    mov ax,imm ");
    } else {
        printf("    mov ax,[imm] ");
        token *= 2; // This should change based on the architecture. It is 2 for a 16-bit systen
    }
    printf("%u;\n", token);
    return tok_next();
}

int dest;
int compile_assign(const int ttoken) {
    int token = ttoken;
    dest = token;
    tok_next();
    token = tok_next();
    token = compile_unary(token);
    int op = 0;
    switch(token) {
        case TOK_ADD: op = TOK_ADD; break;
        case TOK_SUB: op = TOK_SUB; break;
        case TOK_EQ: op = TOK_EQ; break;
        case TOK_NE: op = TOK_NE; break;
        default: break; // TODO: Add other operators
    }

    if(op != 0) {
        printf("    push ax;\n");
        token = tok_next();
        token = compile_unary(token);
        printf("    pop cx;\n    xchg ax,cx;\n");
        switch(op) {
            case TOK_ADD: printf("    add ax,cx;\n"); break;
            case TOK_SUB: printf("    sub ax,cx;\n"); break;
            case TOK_EQ: printf("    cmp ax,cx;\n    mov ax,0x00;\n    sete al;\n"); break;
            case TOK_NE: printf("    cmp ax,cx;\n    mov ax,0x00;\n    setne al;\n"); break;
        }
    }

    token = dest;
    printf("    mov [imm],ax ");
    token *= 2;
    printf("%d;\n", token);

    return tok_next();
}

int functionname;
int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [C file]", argv[0]);
        return -1;
    }

    c = fopen(argv[1], "rb");

    int token = 0;
    while(1) {
        token = tok_next();
        if(token != TOK_INT) {
            token = tok_next();
            functionname = token;
            printf("void %d() {\n", functionname);
            tok_next();
            token = tok_next();
            while(token != TOK_BLK_END) {
                token = compile_assign(token);
            }
            printf("    return;\n}\n\n");
            token = functionname;
            if(token == TOK_START) {
                break;
            }
        } else {
            printf("int ");
            token = tok_next();
            printf("%d;\n", token);
            tok_next();
        }
    }

    fclose(c);
    return 0;
}
