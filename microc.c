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
#define TOK_MODULO -11 // Added by me.
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

//const uint8_t elfheader[64] = {0x7F, 'E', 'L', 'F', 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x3E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x38, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//const uint8_t textheader[56] = {0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//const uint8_t dataheader[56] = {0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t elfheader[52] = {0x7F, 'E', 'L', 'F', //
0x01, 0x01, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x02, 0x00, 0x03, 0x00,
0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x34, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x34, 0x00, 0x20, 0x00,
0x02, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00
};
const uint8_t textheader[32] = {0x01, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
const uint8_t dataheader[32] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00};

const uint8_t movinst = 0xb8;
const uint8_t ebxsetupinst = 0xbb;
const uint8_t getvarinst[2] = {0x8b, 0x03};
const uint8_t saveaxinst = 0x50;
const uint8_t returnecxinst = 0x59;
const uint8_t swapinst = 0x91;
const uint8_t savevarinst[2] = {0x89, 0x03};
const uint8_t returninst = 0xc3;
const uint8_t addtoebxinst[2] = {0x81, 0xc3};
const uint8_t subtoebxinst[2] = {0x81, 0xeb};
const uint8_t prologue[6] = {0x55, 0x89, 0xe5, 0x83, 0xec, 0x04};
const uint8_t condjumpinst[8] = {0x85, 0xc0, 0x0f, 0x84, 0x00, 0x00, 0x00, 0x00};
const uint8_t compinst[8] = {0x39, 0xc8, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x0f};
const uint8_t terminateinst[12] = {0xb8, 0x01, 0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00, 0x00, 0xcd, 0x80};
const uint8_t jmpinst = 0xe9;
const uint8_t prepderefinst[2] = {0x8b, 0x33};
const uint8_t getderefinst[2] = {0x8b, 0x06};
const uint8_t getaddressinst[2] = {0x8d, 0x03};
const uint8_t setderefinst[2] = {0x89, 0x06};
const uint8_t callinst = 0xe8;

const uint8_t addinst[2] = {0x01, 0xc8};
const uint8_t subinst[2] = {0x29, 0xc8};
const uint8_t mulinst[2] = {0xf7, 0xe1};
const uint8_t divinst[4] = {0x32, 0xd2, 0xf7, 0xf1};
const uint8_t moduloinst[6] = {0x32, 0xd2, 0xf7, 0xf1, 0x89, 0xd0};
const uint8_t andinst[2] = {0x21, 0xc8};
const uint8_t orinst[2] = {0x09, 0xc8};
const uint8_t xorinst[2] = {0x31, 0xc8};
const uint8_t shlinst[2] = {0xd3, 0xe0};
const uint8_t shrinst[2] = {0xd3, 0xe8};
const uint8_t eqinst[2] = {0x94, 0xc0};
const uint8_t neinst[2] = {0x95, 0xc0};
const uint8_t ltinst[2] = {0x9c, 0xc0};
const uint8_t gtinst[2] = {0x9f, 0xc0};
const uint8_t leinst[2] = {0x9e, 0xc0};
const uint8_t geinst[2] = {0x9d, 0xc0};

const uint8_t padding;

FILE* c;
FILE* out;

uint32_t symbolamount;
uint32_t* symboltable;
uint32_t varamt;

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
uint32_t tok_next() {
    uint32_t token = 0;
    uint16_t lasttwo;
    tok_is_num = false;
    tok_is_call = false;

    char ch = getch();
    while(ch <= 32) {
        ch = getch();
    }

    if(ch <= 57) tok_is_num = true;

    while(ch > 32) {
        lasttwo = (lasttwo << 8) | (uint16_t)ch;

        token = 10 * token + (ch - 48);

        ch = getch();
    }

    if(lasttwo == 0x2829) {
        tok_is_call = true;
    }

    return token;
}

// 00000000 00000000 11111111 00000000
uint32_t byterev32(const uint32_t bytes) {
    return ((bytes << 24) | ((bytes & 0xff00) << 8) | ((bytes & 0xff0000) >> 8) | ((bytes & 0xff000000) >> 24));
}

uint8_t alignbytes(const uint8_t bbytes) {
    uint8_t bytes = bbytes;
    if((bytes % 4) != 0) {
        while((bytes % 4) != 0) {
            bytes += 1;
        }
    }
    return bytes;
}

uint32_t getnewpos(const uint32_t bytes) {
    // return (alignbytes(bytes) << 24) | (0x800408);
    return 0x08048000 + bytes;
}

uint32_t findvar(const uint32_t token) {
    uint32_t i = 0;
    while(i < symbolamount) {
        uint32_t var = symboltable[i * 2];
        if(var == token) break;
        i++;
    }
    if(i >= symbolamount) { // Sucks to suck
        fprintf(stderr, "Error: variable hasn't been defined yet!: %d\n", token);
        fclose(c);
        fclose(out);
        free(symboltable);
        exit(1);
    }
    return symboltable[(i * 2) + 1];
}

uint32_t findfunction(const uint32_t token) {
    uint32_t i = 0;
    while(i < symbolamount) {
        uint32_t function = symboltable[i * 2];
        if(function == token) break;
        i++;
    }
    if(i >= symbolamount) { // Sucks to suck
        fprintf(stderr, "Error: function hasn't been defined yet!: %d\n", token);
        fclose(c);
        fclose(out);
        free(symboltable);
        exit(1);
    }
    return symboltable[(i * 2) + 1];
}

void compile_expr(const uint32_t ttoken);

uint32_t compile_unary(const uint32_t ttoken) {
    uint32_t token = ttoken;
    if(token != TOK_DEREF) {
        if(token != TOK_LPAREN) {
            if(token != TOK_ADDR) {
                if(tok_is_num) {
                    fwrite(&movinst, 1, 1, out);
                    fwrite(&token, 4, 1, out);
                } else {
                    uint32_t i = findvar(token);
                    fwrite(addtoebxinst, 2, 1, out);
                    uint32_t tmp = i * 4; // Need to add .data address to the token
                    fwrite(&tmp, 4, 1, out);
                    fwrite(getvarinst, 2, 1, out);
                    fwrite(subtoebxinst, 2, 1, out);
                    fwrite(&tmp, 4, 1, out);
                }
            } else {
                token = tok_next();
                uint32_t i = findvar(token);
                fwrite(addtoebxinst, 2, 1, out);
                uint32_t tmp = i * 4;
                fwrite(&tmp, 4, 1, out);
                fwrite(getaddressinst, 2, 1, out);
                fwrite(subtoebxinst, 2, 1, out);
                fwrite(&tmp, 4, 1, out);
            }
        } else {
            token = tok_next();
            compile_expr(token);
        }
    } else {
        token = tok_next();
        uint32_t i = findvar(token);

        fwrite(addtoebxinst, 2, 1, out);
        uint32_t tmp = i * 4;
        fwrite(&tmp, 4, 1, out);
        fwrite(prepderefinst, 2, 1, out);
        fwrite(subtoebxinst, 2, 1, out);
        fwrite(&tmp, 4, 1, out);
        fwrite(getderefinst, 2, 1, out);
    }
    return tok_next();
}

void compile_expr(const uint32_t ttoken) {
    int token = ttoken;
    token = compile_unary(token);
    uint32_t op = 0;
    switch(token) {
        case TOK_ADD: op = TOK_ADD; break;
        case TOK_SUB: op = TOK_SUB; break;
        case TOK_MUL: op = TOK_MUL; break;
        case TOK_DIV: op = TOK_DIV; break;
        case TOK_MODULO: op = TOK_MODULO; break;
        case TOK_AND: op = TOK_AND; break;
        case TOK_OR: op = TOK_OR; break;
        case TOK_XOR: op = TOK_XOR; break;
        case TOK_SHL: op = TOK_SHL; break;
        case TOK_SHR: op = TOK_SHR; break;
        case TOK_EQ: op = TOK_EQ; break;
        case TOK_NE: op = TOK_NE; break;
        case TOK_LT: op = TOK_LT; break;
        case TOK_GT: op = TOK_GT; break;
        case TOK_LE: op = TOK_LE; break;
        case TOK_GE: op = TOK_GE; break;
        default: break; // TODO: Add other operators
    }

    if(op != 0) {
        fwrite(&saveaxinst, 1, 1, out);
        token = tok_next();
        token = compile_unary(token);
        fwrite(&returnecxinst, 1, 1, out);
        fwrite(&swapinst, 1, 1, out);
        switch(op) {
            case TOK_ADD: fwrite(addinst, 2, 1, out); break;
            case TOK_SUB: fwrite(subinst, 2, 1, out); break;
            case TOK_MUL: fwrite(mulinst, 2, 1, out); break;
            case TOK_DIV: fwrite(divinst, 4, 1, out); break;
            case TOK_MODULO: fwrite(moduloinst, 6, 1, out); break;
            case TOK_AND: fwrite(andinst, 2, 1, out); break;
            case TOK_OR: fwrite(orinst, 2, 1, out); break;
            case TOK_XOR: fwrite(xorinst, 2, 1, out); break;
            case TOK_SHL: fwrite(shlinst, 2, 1, out); break;
            case TOK_SHR: fwrite(shrinst, 2, 1, out); break;
            case TOK_EQ: fwrite(compinst, 8, 1, out); fwrite(eqinst, 2, 1, out); break;
            case TOK_NE: fwrite(compinst, 8, 1, out); fwrite(neinst, 2, 1, out); break;
            case TOK_LT: fwrite(compinst, 8, 1, out); fwrite(ltinst, 2, 1, out);
            case TOK_GT: fwrite(compinst, 8, 1, out); fwrite(gtinst, 2, 1, out);
            case TOK_LE: fwrite(compinst, 8, 1, out); fwrite(leinst, 2, 1, out); break;
            case TOK_GE: fwrite(compinst, 8, 1, out); fwrite(geinst, 2, 1, out); break;
        }
    }
}

uint32_t dest;
uint32_t compile_assign(const uint32_t ttoken) {
    uint32_t token = ttoken;
    if(token != TOK_DEREF) {
        dest = token;
        tok_next();
        token = tok_next();
        compile_expr(token);
        token = dest;
        uint32_t i = findvar(token);
        fwrite(addtoebxinst, 2, 1, out);
        uint32_t tmp = i * 4;
        fwrite(&tmp, 4, 1, out);
        fwrite(savevarinst, 2, 1, out);
        fwrite(subtoebxinst, 2, 1, out);
        fwrite(&tmp, 4, 1, out);
    } else {
        token = tok_next();
        dest = token;
        tok_next();
        token = tok_next();
        compile_expr(token);

        token = dest;
        uint32_t i = findvar(token);

        fwrite(addtoebxinst, 2, 1, out);
        uint32_t tmp = i * 4;
        fwrite(&tmp, 4, 1, out);
        fwrite(prepderefinst, 2, 1, out);
        fwrite(subtoebxinst, 2, 1, out);
        fwrite(&tmp, 4, 1, out);
        fwrite(setderefinst, 2, 1, out);
    }

    return tok_next();
}

void patch_fwd(const uint32_t patchloc) {
    uint32_t tmp = ftell(out) - patchloc;
    fseek(out, (patchloc - 4), SEEK_SET);
    fwrite(&tmp, 4, 1, out);
    fseek(out, 0, SEEK_END);
}

void patch_back(const uint32_t loopstart, const uint32_t patchloc) {
    fwrite(&jmpinst, 1, 1, out);
    uint32_t tmp = loopstart - ftell(out);
    tmp -= 4;
    fwrite(&tmp, 4, 1, out);

    patch_fwd(patchloc);
}

void compile_stmt(const uint32_t ttoken);

uint32_t control_flow_block() {
    uint32_t token = tok_next();
    compile_expr(token);
    
    fwrite(condjumpinst, 8, 1, out);
    
    uint32_t jumppos = ftell(out);
    token = tok_next();
    compile_stmt(token);

    return jumppos;
}

void compile_stmt(const uint32_t ttoken) {
    uint32_t token = ttoken;
    while(token != TOK_BLK_END) {
        if(tok_is_call) {
            fwrite(&callinst, 1, 1, out);
            uint32_t i = findfunction(token);
            uint32_t tmp = i - ftell(out);
            tmp -= 4;
            fwrite(&tmp, 4, 1, out);
            tok_next();
            token = tok_next();
        } else {
            if(token != TOK_ASM) {
                if(token != TOK_IF_BEGIN) {
                    if(token != TOK_WHILE_BEGIN) {
                        token = compile_assign(token);
                    } else {
                        uint32_t loopstart = ftell(out);
                        uint32_t jumppos = control_flow_block();
                        patch_back(loopstart, jumppos);
                        token = tok_next();
                    }
                } else {
                    uint32_t jumppos = control_flow_block();
                
                    patch_fwd(jumppos);
                    token = tok_next();
                }
            } else {
                token = tok_next();
                fwrite(&token, 1, 1, out);
                tok_next();
                token = tok_next();
            }
        }
    }
}

uint32_t functionname;
int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [C file] [Output executable]\n", argv[0]);
        return -1;
    }

    c = fopen(argv[1], "rb");
    out = fopen(argv[2], "wb");

    symboltable = (uint32_t*)malloc(8);

    /* At the moment, the stack pointer only needs 4 bytes for eax when doing operations on variables.
    It'll need more sixe for local variables in the future, but for now we can keep code size down.
    Besides, the generated code from this will be insanely unoptimized. Setting rbx to an address is 10 bytes.
    I know there are optimizations I could make, but right now, I just want the basics. */

    //fwrite(prologue, 6, 1, out);

    fwrite(elfheader, 52, 1, out);
    fwrite(textheader, 32, 1, out);
    fwrite(dataheader, 32, 1, out);

    for(uint32_t i = 0; i < 12; i++) {
        fwrite(&padding, 1, 1, out);
    }



    uint32_t token = 0;
    while(1) {
        token = tok_next();
        if(token != TOK_INT) {
            token = tok_next();
            functionname = token;

            symbolamount += 1;
            symboltable = (uint32_t*)realloc(symboltable, symbolamount * 8);
            symboltable[(symbolamount * 2) - 2] = functionname;
            symboltable[(symbolamount * 2) - 1] = ftell(out);

            tok_next();
            token = tok_next();
            compile_stmt(token);
            //printf("    return;\n}\n\n");
            fwrite(&returninst, 1, 1, out);
            token = functionname;
            if(token == TOK_START) {
                //write(terminateinst, 12, 1, out);
                break;
            }
        } else {
            //printf("int ");
            token = tok_next();
            //printf("%d;\n", token);
            symbolamount += 1;
            varamt += 1;
            symboltable = (uint32_t*)realloc(symboltable, symbolamount * 8);
            symboltable[(symbolamount * 2) - 2] = token;
            symboltable[(symbolamount * 2) - 1] = varamt - 1;
            tok_next();
        }
    }

    //printf("At 0x%x in ELF file.\n", ftell(out));
    //printf("At 0x%x in ELF file.\n", ftell(out));

    uint32_t textlen = (ftell(out) - 0x80);

    uint32_t datapos = ftell(out);
    if((datapos % 4) != 0) {
        while((datapos % 4) != 0) {
            fwrite(&padding, 1, 1, out);
            datapos = ftell(out);
        }
    }

    uint32_t startloc = findfunction(TOK_START);
    fseek(out, 24, SEEK_SET);
    uint32_t tmp = getnewpos(startloc);
    fwrite(&tmp, 4, 1, out);
    fseek(out, (startloc + 1), SEEK_SET);
    tmp = getnewpos(datapos);
    fwrite(&tmp, 4, 1, out);
    fseek(out, (52 + 32 + 4), SEEK_SET);
    fwrite(&datapos, 4, 1, out);
    fwrite(&tmp, 4, 1, out);

    fseek(out, (52 + 16), SEEK_SET);
    fwrite(&textlen, 4, 1, out);
    fwrite(&textlen, 4, 1, out);
    fseek(out, (52 + 32 + 16), SEEK_SET);
    uint32_t datalen = varamt * 4;
    fwrite(&datalen, 4, 1, out);
    fwrite(&datalen, 4, 1, out);

    fseek(out, datapos, SEEK_SET);
    for(uint32_t i = 0; i < (4 * varamt); i++) {
        fwrite(&padding, 1, 1, out);
    }

    fclose(c);
    fclose(out);
    free(symboltable);
    return 0;
}
