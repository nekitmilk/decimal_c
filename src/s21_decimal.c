#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

}

int get_sign(s21_decimal d) {
    return (d.bits[3] >> 31) & 1; // Бит 31: 0 (+) или 1 (-)
}

void set_sign(s21_decimal *d, int sign) {
    d->bits[3] &= ~(1 << 31); // Обнуляем бит 31
    d->bits[3] |= (sign & 1) << 31; // Устанавливаем новый знак
}

int get_scale(const s21_decimal *d) {
    return (d->bits[3] >> 16) & 0xFF; // Биты 16-23 (маска 0xFF)
}

void set_scale(s21_decimal *d, int scale) {
    d->bits[3] &= ~(0xFF << 16); // Обнуляем биты 16-23
    d->bits[3] |= (scale & 0xFF) << 16; // Устанавливаем scale
}


// Добавить проверку, что конвертация прошла успешно и в зависимости от этого делать код возврата
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    memset(dst, 0, sizeof(s21_decimal)); // Обнуляем все биты

    if (src < 0) {
        set_sign(dst, 1);
        src = -src;
    }

    dst->bits[0] = src; // Младшие 32 бита
    set_scale(dst, 0); // Без дробной части

    return 0;
}