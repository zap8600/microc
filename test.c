int tmp1;
int tmp2;
int tmp3;
int tmp4;
int tmp5;
void _start() {
    asm 85;
    asm 137;
    asm 229;
    asm 131;
    asm 236;
    asm 4;

    tmp1 = 3;
    tmp2 = 2;
    tmp3 = tmp1 + tmp2;
    if( tmp3 == 5 ){
        tmp4 = 1;
    }
    if( tmp3 != 2 ){
        tmp5 = 4;
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
}