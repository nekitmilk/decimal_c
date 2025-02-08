#include <stdio.h>
#include "s21_decimal.h"

void print_dec(s21_decimal d);

int main(int argc, const char * argv[]) {
    
    s21_decimal d = {0};

    // set_scale(&d, 20);
    // set_sign(&d, 1);

    s21_from_int_to_decimal(-12345, &d);

    print_dec(d);

    return 0;
}

void print_dec(s21_decimal d) {
    printf("%x %x %x %x", d.bits[0], d.bits[1], d.bits[2], d.bits[3]);
}