int n1;
int n2;
int n;
int amt;
void _start() {
    asm 85;
    asm 137;
    asm 229;
    asm 131;
    asm 236;
    asm 4;

    amt = 4;
    n1 = 1;
    while( amt != 0 ){
        n = n1 + n2;
        n2 = n1;
        n1 = n;
        amt = amt - 1;
    }
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