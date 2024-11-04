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

const uint8_t elfheader[64] = {0x7F, 'E', 'L', 'F', 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x3E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x38, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t textheader[56] = {0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t dataheader[56] = {0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t movqinst[2] = {0x48, 0xb8};
const uint8_t rbxsetupinst[2] = {0x48, 0xbb};
const uint8_t getvarinst[3] = {0x48, 0x8b, 0x03};
const uint8_t savraxinst = 0x50;
const uint8_t returncxinst = 0x59;
const uint8_t swapinst[2] = {0x48, 0x91};
const uint8_t savevarinst[3] = {0x48, 0x89, 0x03};
const uint8_t returninst = 0xc3;

const uint8_t addinst[3] = {0x48, 0x01, 0xc8};
const uint8_t subinst[3] = {0x48, 0x29, 0xc8};

FILE* c;
FILE* datatmp;
FILE* texttmp;
FILE* out;

uint64_t dataamt; // Holds how many variables we currently have.

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
uint64_t tok_next() {
    uint64_t token = 0;
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
// 00000000 00000000 00000000 00000000 00000000 00000000 11111111 00000000
uint64_t byterev64(const uint64_t bytes) {
    return ((bytes << 56) | ((bytes & 0xff00) << 40) | ((bytes & 0xff0000) << 24) | ((bytes & 0xff000000) << 8) | ((bytes & 0xff00000000) >> 8) | ((bytes & 0xff0000000000) >> 24) | ((bytes & 0xff00000000000000) >> 56));
}

uint64_t compile_unary(const uint64_t ttoken) {
    uint64_t token = ttoken;
    if(tok_is_num) {
        printf("    mov rax,imm ");
        fwrite(movqinst, 2, 1, texttmp);
        uint64_t tmp = byterev64(token);
        fwrite(&tmp, 8, 1, texttmp);
    } else {
        printf("    mov rbx,imm\n    mov rax,[rbx] ");
        fseek(datatmp, 0, SEEK_SET);
        uint64_t i;
        while(i != (dataamt + 1)) {
            uint64_t var;
            fread(&var, 8, 1, datatmp);
            if(var == token) break;
            i++;
        }
        if(i > dataamt) { // Sucks to suck
            fprintf(stderr, "Error: variable hasn't been defined yet!\n");
            fclose(c);
            fclose(texttmp);
            fclose(datatmp);
            fclose(out);
            exit(-1);
        }
        fseek(datatmp, 0, SEEK_END);
        fwrite(rbxsetupinst, 2, 1, texttmp);
        uint64_t tmp = byterev64(0xA4800408 + i); // Need to add .data address to the token
        fwrite(&tmp, 8, 1, texttmp);
        fwrite(getvarinst, 3, 1, texttmp);
    }
    printf("%u;\n", token);
    return tok_next();
}

uint64_t dest;
uint64_t compile_assign(const uint64_t ttoken) {
    uint64_t token = ttoken;
    dest = token;
    tok_next();
    token = tok_next();
    token = compile_unary(token);
    uint64_t op = 0;
    switch(token) {
        case TOK_ADD: op = TOK_ADD; break;
        case TOK_SUB: op = TOK_SUB; break;
        default: break; // TODO: Add other operators
    }

    if(op != 0) {
        printf("    push rax;\n");
        fwrite(&savraxinst, 1, 1, texttmp);
        token = tok_next();
        token = compile_unary(token);
        printf("    pop cx;\n    xchg ax,cx;\n");
        fwrite(&returncxinst, 1, 1, texttmp);
        fwrite(swapinst, 2, 1, texttmp);
        switch(op) {
            case TOK_ADD: printf("    add rax,rcx;\n"); fwrite(&addinst, 3, 1, texttmp); break;
            case TOK_SUB: printf("    sub rax,rcx;\n"); fwrite(&subinst, 3, 1, texttmp); break;
        }
    }

    token = dest;
    printf("    mov rbx,imm\n    mov [rbx],rax ");
    fseek(datatmp, 0, SEEK_SET);
    uint64_t i;
    while(i <= dataamt) {
        uint64_t var;
        fread(&var, 8, 1, datatmp);
        if(var == token) break;
        i++;
    }
    if(i > dataamt) { // Sucks to suck
        fprintf(stderr, "Error: variable hasn't been defined yet!\n");
        fclose(c);
        fclose(texttmp);
        fclose(datatmp);
        fclose(out);
        exit(-1);
    }
    fseek(datatmp, 0, SEEK_END);
    fwrite(rbxsetupinst, 2, 1, texttmp);
    uint64_t tmp = byterev64(0xA4800408 + i);
    fwrite(&tmp, 8, 1, texttmp);
    fwrite(&savevarinst, 3, 1, texttmp);
    printf("%u;\n", i);

    return tok_next();
}

uint64_t functionname;
int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [C file] [Output executable]\n", argv[0]);
        return -1;
    }

    c = fopen(argv[1], "rb");
    datatmp = fopen("./datatmp", "wb+"); // I'm using files for these to use as little RAM right now. I'd like this to be usable on an embedded processor.
    texttmp = fopen("./texttmp", "wb");
    out = fopen(argv[2], "wb");

    /* At the moment, the stack pointer only needs 8 bytes for rax when doing operations on variables.
    It'll need more sixe for local variables in the future, but for now we can keep code size down.
    Besides, the generated code from this will be insanely unoptimized. Setting rbx to an address is 10 bytes.
    I know there are optimizations I could make, but right now, I just want the basics. */

    // TODO: Write a prologue for the stack pointer to be set up.

    uint64_t token = 0;
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
            fwrite(&returninst, 1, 1, texttmp);
            token = functionname;
            if(token == TOK_START) {
                break;
            }
        } else {
            printf("int ");
            token = tok_next();
            printf("%d;\n", token);
            fwrite(token, 8, 1, datatmp);
            dataamt += 1;
            tok_next();
        }
    }

    fwrite(elfheader, 64, 1, out);
    fwrite(textheader, 56, 1, out);
    fwrite(dataheader, 56, 1, out);

    fseek(out, 25, SEEK_SET);
    uint64_t entry = 0x80800408;

    fclose(c);
    fclose(datatmp);
    fclose(texttmp);
    fclose(out);
    return 0;
}
