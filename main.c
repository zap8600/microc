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
bool tok_is_call;
int tok_next() {
    int token = 0;
    char lasttwo[0];
    tok_is_num = false;
    tok_is_call = false;

    char ch = getch();
    while(ch <= 32) {
        ch = getch();
        if(ch == EOF) return ch;
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

void compile_stmts(const int ttoken);

void control_flow_block() {
    int token = tok_next();
    compile_expr(token);

    printf("test ax,ax\n");
    printf("je ");
    printf("placeholder\n");

    // Save forward patch location
    token = tok_next();
    compile_stmts(token);
    // Restore forward patch location
}

void patch_fwd() {
    // Compute forward jump
}

void patch_back(int loopstartloc) {
    printf("jmp ");
    printf("placeholder\n");
    patch_fwd();
}

void compile_stmts(const int ttoken) {
    int token = ttoken;
    while(1) {
        if(token == TOK_BLK_END) {
            return;
        }

        if(!tok_is_call) {
            if(token != TOK_ASM) {
                if(token != TOK_IF_BEGIN) {
                    if(token != TOK_WHILE_BEGIN) {
                        compile_assign();
                    } else {
                        // Save loop start location
                        control_flow_block();
                        patch_back();
                        token = tok_next();
                    }
                } else {
                    control_flow_block();
                    patch_fwd();
                    token = tok_next();
                }
            } else {
                token = tok_next();
                printf("%d\n", token);
                tok_next();
                token = tok_next();
            }
        } else {
            printf("call ");
            // Get target from symbol table
            printf("placeholder\n");

            tok_next();
            token = tok_next();
        }
    }
}

void compile_function() {
    int token = tok_next();
    // Save function name in a symbol table
    tok_next();
    token = tok_next();
    compile_stmts(token);

    printf("ret\n");
}

void compile() {
    int token = 0;
    while(token != EOF) {
        token = tok_next();
        if(token != TOK_INT) {
            compile_function();
            if(token == TOK_START) return;
        } else {
            tok_next();
            tok_next();
        }
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [C file]", argv[0]);
        return -1;
    }

    c = fopen(argv[1], "rb");

    printf("  .globl main\nmain:\n");

    //compile();
    int token = 0;
    while(token != TOK_SEMI) {
        token = tok_next();
        printf("%d\n", token);
    }

    fclose(c);
    return 0;
}
