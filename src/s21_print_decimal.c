#include "s21_print_decimal.h"

int main() {
    s21_decimal decimal = {{0}};
    decimal.bits[0] = 3;
    decimal.bits[1] = 1;
    decimal.bits[2] = (int)pow(2, 32) - 1;
    decimal.bits[3] = 0x00000000;
    output_dec_bin(decimal);
    printf("\n");
    output_dec_hex(decimal);
    return 0;
}

void output_dec_hex(s21_decimal decimal) {
    int sign = decimal.bits[3] & 0x80000000;
    int point = (int)pow(10, decimal.bits[3] >> 16 & 0x00FF);
    printf("sign: %c\n", (sign ? '-' : '+'));
    printf("point: %d\n", point);
    if (sign) { printf("-"); };

    char buff[100] = {0};
    if (decimal.bits[0]) {
        sprintf(buff, "%x%08x%08x\n", decimal.bits[0], decimal.bits[1], decimal.bits[2]);
    }
    else if (decimal.bits[1]) {
        sprintf(buff, "%x%08x\n", decimal.bits[1], decimal.bits[2]);
    }
    else {
        sprintf(buff, "%x\n", decimal.bits[2]);
    }
    printf("%s", buff);
}

void output_dec_bin(s21_decimal decimal) {
    for (int i = 0; i < 3; output_bin(decimal.bits[i++])) {}
}

void output_bin(int bin) {
    char out[33] = {0};
    for (int i = 0; i < 32; i++) {
        out[31 - i] = bin & 1 ? '1' : '0';
        bin = bin >> 1;
    }
    printf("%s", out);
}