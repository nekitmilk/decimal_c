#include "s21_decimal.h"

/*
Я абсолютно не уверен, когда-нибудь проверим

нужно подумать над округлением
*/
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    int error_code = 0;
    if (src == INFINITY || src == -INFINITY || src != src) { error_code = 1; }
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
    return error_code;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int error_code = 0;
    s21_decimal min, max;
    s21_from_int_to_decimal(INT_MAX, &max);
    s21_from_int_to_decimal(INT_MIN, &min);
    if (s21_is_greater(src, max) || s21_is_less(src, min)) { error_code = 1; }
    else {
        /*
        change for cases where bits[0] and bits[1] != 0
        */
        *dst = (int)(pow(10, src.bits[3] >> 16 & 0x00FF) * src.bits[2]);
    }
    return error_code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int error_code = 0;
    s21_decimal sigma, sigma_negate, zero;
    s21_from_int_to_decimal(0, &zero);
    sigma.bits[2] = 1;
    sigma.bits[3] = 0x001c0000; // наглая ложь, нужно переделавать
    s21_negate(sigma, &sigma_negate);
    if (s21_is_greater(src, sigma_negate)
        && s21_is_less(src, sigma)
        && !s21_is_equal(src, zero)) {
        *dst = 0;
        error_code = 1;
    } else {
        *dst = (src.bits[2] / pow(10, src.bits[3] >> 16 & 0x00FF));
    }
    return error_code;
}

