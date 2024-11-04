#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t tmp1 = 74741;
    FILE* test = fopen("./testbin", "wb");
    fwrite(&tmp1, 4, 1, test);
    uint32_t tmp2;
    fread(&tmp2, 4, 1, test);
    printf("%u\n", tmp2);
    fclose(test);
}
