#include "s21_decimal.h"

// 0 - OK; 1 - too big or infinity; 2 - too small or -infinity
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int status = 0;

    memset(result, 0, sizeof(s21_decimal));
    int sign1 = get_sign(value_1);
    int sign2 = get_sign(value_2);
    
    // Выравнивание масштабов
    align_scales(&value_1, &value_2);

    int overflow = 0;

    // обработка, когда одно число положительное, другое отрицательное 
    if (sign1 != sign2) {
        if (compare_bits(value_1, value_2)) {
            overflow = sub_bits(value_1, value_2, result);
            set_sign(result, sign1);
        }
        else {
            overflow = sub_bits(value_2, value_1, result);
            set_sign(result, sign2);
        }
    }
    else {
        overflow = add_bits(&value_1, &value_2, result);
        set_sign(result, sign1);
    }

    if (overflow) {
        status = overflow;
        memset(result, 0, sizeof(s21_decimal));
    }
    else {
        // Установка общего масштаба и знака
        set_scale(result, get_scale(&value_1));
        
    }

    return status;
    
}

int get_sign(s21_decimal d) {
    return (d.bits[3] >> 31) & 1; // Бит 31: 0 (+) или 1 (-)
}

void set_sign(s21_decimal *d, int sign) {
    d->bits[3] &= ~(1 << 31); // Обнуляем бит 31
    d->bits[3] |= (sign & 1) << 31; // Устанавливаем новый знак
}

// Получить степень из bits[3]
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