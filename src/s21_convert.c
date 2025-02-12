#include "s21_decimal.h"
#include <math.h>

/*
Я абсолютно не уверен, когда-нибудь проверим
*/
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    int result = 0;
    if (src == INFINITY) { result = 1; }
    else if (src == -INFINITY) { result = 2; }
    else if (src != src) { result = 3; }
    else {
        int *bit = (int*)&src;
        int sign = *bit >> 31 & 1;
        int scale = (*bit >> 23) - 127;
        int mantissa = (scale == -127) ? 
                (*bit && 0x7FFFFF) : (*bit && 0x7FFFFF) | 0x1000000;
        for (; mantissa & 1 == 0; mantissa >>= 1) {}
        s21_from_int_to_decimal(mantissa * (sign ? -1 : 1), dst);
        if (src != (int)src) {
            s21_decimal sc;
            s21_from_int_to_decimal((int)pow(2, scale), &sc);
            s21_div(*dst, sc, dst);
        }
    }
    return result;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int result = 0;
    s21_decimal min, max;
    s21_from_int_to_decimal(__INT_MAX__, &max);
    s21_from_int_to_decimal(-__INT_MAX__, &min);
    if (s21_is_greater(src, max) || s21_is_less(src, min)) { result = 1; }
    else {
        *dst = (int)(pow(10, src.bits[3] >> 16 & 0x00FF) * src.bits[2]);
    }
    return result;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int result = 0;
    *dst = (src.bits[2] / pow(10, src.bits[3] >> 16 & 0x00FF));
    return result;
}

