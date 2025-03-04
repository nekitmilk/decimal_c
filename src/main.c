#include <stdio.h>

#include "s21_decimal.h"
#include "s21_print_decimal.h"
#include "utils.h"

// gcc main.c s21_decimal.c s21_comparison.c utils.c

void print_dec(s21_decimal d);
// void tieshagr_bankers_rounding_v2(s21_decimal *num, int target_scale, int chet);

int main(int argc, const char* argv[]) {
    s21_decimal value_1 = {{0, 0xFFFFFFFF, 0, 0}}; // 18446744069414584320
    s21_decimal value_2 = {{0x1, 0, 0, 0}}; // 1 * 10^-27
    set_scale(&value_2, 27);
    set_sign(&value_2, 1);
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}}; // 18446744069414584319,999...

    s21_decimal expected_1 = {{0, 0xFFFFFFFF, 0, 0}}; // 18446744069414584319,999...
    // set_sign(&expected_1, 1);
    int expected_err = 0;

    int err_1 = s21_add(value_1, value_2, &result_1);
    printf("\n                 ");
    print_dec(value_1);
    printf("\n                 ");
    print_dec(value_2);
    printf("\nreal result:     ");

    // for (int i = 0; i < 8; i++)
    // {
    //   div_by_10(&result_1);
    // }
    
    print_dec(result_1);
    printf("\nerr code: %d\n", err_1);
    printf("\nexpected result: ");
    print_dec(expected_1);

    s21_decimal bank = {{5, 0, 0, 0}};
    printf("\n                 ");
    // bank.bits[0] = 0xFFFFFFFF;
    // bank.bits[1] = 0xFFFFFFFF;
    // bank.bits[2] = 0xFFFFFFFF;
    // 123 - 0,5 = 122
    // 124 - 0,5 = 124
    set_scale(&bank, 1);
    set_sign(&bank, 1);
    print_dec(bank);
    tieshagr_bankers_rounding_v2(&bank, 0, 1);

    printf("\n                 ");
    print_dec(bank);
    
    
//18446744069414584320,000000000
//18446744069414584320

  return 0;
}

// 123 - 0,5 = 122
// 122 - 0,5 = 122
// 124 - 0,5 = 124
// 125 - 0,5 = 124

void print_dec(s21_decimal d) {
  printf("%.8x %.8x %.8x %x", d.bits[2], d.bits[1], d.bits[0], d.bits[3]);
}

// Банковское округление до определенного знака
// void tieshagr_bankers_rounding_v2(s21_decimal *num, int target_scale, int nechet) {
//     int current_scale = get_scale(num);
//     if (current_scale <= target_scale) return; // Округление не требуется

//     // Вычисляем разницу в масштабах
//     int scale_diff = current_scale - target_scale;

//     // Вычисляем 10^scale_diff
//     unsigned int divisor = 1;
//     for (int i = 0; i < scale_diff; i++) {
//         divisor *= 10;
//     }

//     // Получаем мантиссу
//     unsigned int mantissa[3] = {num->bits[0], num->bits[1], num->bits[2]};

//     // Вычисляем остаток
//     unsigned int remainder = 0;
//     for (int i = 2; i >= 0; i--) {
//         unsigned long temp = ((unsigned long)remainder << 32) | mantissa[i];
//         mantissa[i] = (unsigned int)(temp / divisor);
//         remainder = (unsigned int)(temp % divisor);
//     }

//     // Проверяем, нужно ли округлять
//     unsigned int half_divisor = divisor / 2;
//     if (remainder > half_divisor || (remainder == half_divisor && (mantissa[0] & 1)) || nechet) {
//         // Округляем вверх
//         unsigned int carry = 1;
//         for (int i = 0; i < 3 && carry; i++) {
//             unsigned long sum = (unsigned long)mantissa[i] + carry;
//             mantissa[i] = (unsigned int)(sum & 0xFFFFFFFF);
//             carry = (unsigned int)(sum >> 32);
//         }
//     }
    

//     // Обновляем мантиссу и масштаб
//     num->bits[0] = mantissa[0];
//     num->bits[1] = mantissa[1];
//     num->bits[2] = mantissa[2];
//     set_scale(num, target_scale);
// }