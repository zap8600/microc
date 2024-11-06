int tmp1;
int tmp2;
void _start() {
    asm 187;
    asm 0;
    asm 0;
    asm 0;
    asm 0;
    
    asm 85;
    asm 137;
    asm 229;
    asm 131;
    asm 236;
    asm 4;

    tmp1 = 3;
    tmp2 = & tmp1;
    *(int*) tmp2 = *(int*) tmp2 + 2;
}

    asm 184;
    asm 1;
    asm 0;
    asm 0;
    asm 0;
    asm 187;
    asm 0;
    asm 0;
    asm 0;
    asm 0;
    asm 205;
    asm 128;