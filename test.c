/*
Makes a good test to see if comments are supported by my tokenizer too.
tmp1 should have no offset.
tmp2 should have an offset of 8.
tmp3 should have an offset of 16.
tmp4 should have an offset of 24.
tmp5 should have an offset of 32.
*/
int tmp1;
int tmp2;
int tmp3;
int tmp4;
int tmp5;
void _start() {
    tmp1 = 1;
    tmp2 = 4;
    tmp3 = tmp2 + tmp1;
    tmp4 = 1 + tmp4;
    tmp5 = tmp4 - 1;
}