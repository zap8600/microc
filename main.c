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

bool tokisnum = false;
bool tokiscalltrailingparen = false;

uint8_t ch;
uint16_t scratch; // `ax`.
uint16_t scratchsave;
uint16_t currenttoken; // `bx`.
uint16_t currenttokensave; 
uint16_t lasttwochars; // `cx`.
uint16_t tablescan; // `si`.
uint8_t *symboltable; // `ds`.
uint16_t codegenoffset; // `di`.
uint16_t codegenoffsetsave;
uint8_t *codegenbuffer; // `es`.

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
    while(1) {
        if(ch <= 32) {
            break;
        }

        lasttwochars = (lasttwochars << 8) | ((uint16_t)scratch & 0xff);

        currenttoken = scratch * 10;
        scratch = scratch - 48;
        currenttoken = currenttoken + scratch;

        // TODO: Get character.
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

void tok_next2() {
    tok_next();
    goto toknext;
}

void control_block_flow() {
    compile_stmts_tok_next();

    // TODO: Emit `test ax,ax` instruction.
    codegenbuffer[codegenoffset] = (uint8_t)(scratch >> 8);
    codegenbuffer[codegenoffset + 1] = (uint8_t)(scratch & 0x00ff);
    codegenoffset = codegenoffset + 2;
    // TODO: Emit jump if equal instruction.
    codegenbuffer[codegenoffset] = (uint8_t)(scratch >> 8);
    codegenbuffer[codegenoffset + 1] = (uint8_t)(scratch & 0x00ff);
    codegenoffset = codegenoffset + 2;
    codegenbuffer[codegenoffset] = (uint8_t)(scratch >> 8);
    codegenbuffer[codegenoffset + 1] = (uint8_t)(scratch & 0x00ff);
    codegenoffset = codegenoffset + 2;

    codegenoffsetsave = codegenoffset;
    compile_stmts_tok_next();
    tablescan = codegenoffsetsave;
}

void compile_assign() {
    if(scratch != TOK_DEREF) {
        goto not_deref_store;
    }
    tok_next();
    save_var_and_compile_expr();
}

void compile_stmts() {
compilestmts:
    scratch = currenttoken;
    if(scratch == TOK_BLK_END) {
        return;
    }

    if(!tokiscalltrailingparen) {
        goto not_call;
    }

    // TODO: Emit call instruction to scratch;
    codegenbuffer[codegenoffset] = (uint8_t)(scratch & 0x00ff);
    codegenoffset = codegenoffset + 1;

    scratch = symboltable[currenttoken];
    scratch = scratch - codegenoffset;
    scratch = scratch - 2;
    codegenbuffer[codegenoffset] = (uint8_t)(scratch >> 8);
    codegenbuffer[codegenoffset + 1] = (uint8_t)(scratch & 0x00ff);
    codegenoffset = codegenoffset + 2;

    goto compilestmtstoknext2;

not_call:
    if(scratch != TOK_ASM) {
        goto not_asm;
    }
    tok_next();
    codegenbuffer[codegenoffset] = (uint8_t)(scratch & 0x00ff);
    codegenoffset = codegenoffset + 1;
    goto compilestmtstoknext2;

not_asm:
    if(scratch != TOK_IF_BEGIN) {
        goto not_if;
    }
    control_block_flow();
    goto patch_fwd;

not_if:
    if(scratch != TOK_WHILE_BEGIN) {
        goto not_while;
    }
    codegenoffsetsave = codegenoffset;
    control_block_flow();
    goto patch_back;

not_while:
    compile_assign();
    goto compilestmts;

patch_back:
    // TODO: Emit jump instruction.
    codegenbuffer[codegenoffset] = (uint8_t)(scratch & 0x00ff);
    codegenoffset = codegenoffset + 1;
    scratch = codegenoffsetsave; // I think this is right.
    scratch = scratch - codegenoffset;
    scratch = scratch - 2;
    codegenbuffer[codegenoffset] = (uint8_t)(scratch >> 8);
    codegenbuffer[codegenoffset + 1] = (uint8_t)(scratch & 0x00ff);
    codegenoffset = codegenoffset + 2;

patch_fwd:
    scratch = codegenoffset;
    scratch = scratch - tablescan;
    codegenbuffer[tablescan - 2] = (uint8_t)(scratch >> 8);
    codegenbuffer[tablescan - 1] = (uint8_t)(scratch & 0x00ff);
    goto compilestmtstoknext;

compilestmtsend:
    return;
}

void compile_stmts_tok_next() {
compilestmtstoknext:
    tok_next();
    goto compilesmts;
}

void compile_stmts_tok_next2() {
compilestmtstoknext2:
    tok_next();
    goto compilestmtstoknext;
}

int main() {
    codegenoffset = 0;

compile:
    while(1) {
        tok_next();

        if(scratch != TOK_INT) {
            break;
        }

        tok_next2();
    }

compile_function:
    tok_next();
    currenttokensave = currenttoken;
    symboltable[currenttoken] = codegenoffset;
    compile_stmts_tok_next2();
}
