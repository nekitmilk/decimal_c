#include "s21_decimal.h"
#include "utils.h"

int calculate_fractional_part(s21_int256 remainder, s21_int256 value_2_256int,
                              s21_int256 *result_256int, int *result_scale);

// 0 - OK; 1 - too big or infinity; 2 - too small or -infinity; somthing wrong
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  status = check_input_decimals(value_1, value_2, result) ? 4 : 0;
  if (!status) {
    int overflow = 0;
    *result = get_zero_dec();
    int sign1 = get_sign(value_1);
    int sign2 = get_sign(value_2);
    int result_sign = sign1;
    int scale1 = get_scale(&value_1);
    int scale2 = get_scale(&value_2);
    int max_scale = scale1 > scale2 ? scale1 : scale2;
    s21_int256 value_1_256int, value_2_256int, result_256int = {0};
    convert_decimal_to_int256(value_1, &value_1_256int);
    convert_decimal_to_int256(value_2, &value_2_256int);
    align_scales_int256(&value_1_256int, &value_2_256int);
    if (sign1 != sign2) {
      if (compare_bits_int256(value_1_256int, value_2_256int)) {
        overflow =
            sub_bits_int256(value_1_256int, value_2_256int, &result_256int);
      } else {
        overflow =
            sub_bits_int256(value_2_256int, value_1_256int, &result_256int);
        result_sign = sign2;
      }
    } else {
      overflow =
          add_bits_int256(value_1_256int, value_2_256int, &result_256int);
    }
    if (!overflow) {
      set_sign_int256(&result_256int, result_sign);
      set_scale_int256(&result_256int, max_scale);
      overflow = convert_int256_to_decimal(result_256int, result);
    }
    if (overflow) {
      status = get_sign_int256(result_256int) ? 2 : 1;
      *result = get_zero_dec();
    }
  }
  return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;

  status = check_input_decimals(value_1, value_2, result) ? 4 : 0;

  if (!status) {
    s21_decimal negative_value_2 = {0};
    s21_negate(value_2, &negative_value_2);
    status = s21_add(value_1, negative_value_2, result);
  }

  return status;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  status = check_input_decimals(value_1, value_2, result) ? 4 : 0;
  if (!status) {
    int overflow = 0;
    *result = get_zero_dec();
    int sign1 = get_sign(value_1);
    int sign2 = get_sign(value_2);
    int result_sign = sign1 != sign2 ? 1 : 0;
    int scale1 = get_scale(&value_1);
    int scale2 = get_scale(&value_2);
    s21_int256 value_1_256int, value_2_256int, result_256int = {0};
    convert_decimal_to_int256(value_1, &value_1_256int);
    convert_decimal_to_int256(value_2, &value_2_256int);
    if (compare_bits_int256(value_1_256int, value_2_256int)) {
      overflow =
          mul_bits_int256(value_1_256int, value_2_256int, &result_256int);
    } else {
      overflow =
          mul_bits_int256(value_2_256int, value_1_256int, &result_256int);
    }
    if (!overflow) {
      int result_scale = scale1 + scale2;
      set_scale_int256(&result_256int, result_scale);
      set_sign_int256(&result_256int, result_sign);
      overflow = convert_int256_to_decimal(result_256int, result);
    }
    if (overflow == 1) {
      status = result_sign ? 2 : 1;
      *result = get_zero_dec();
    } else if (overflow == 2) {
      status = 2;
      *result = get_zero_dec();
    }
  }
  return status;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;

  status = check_input_decimals(value_1, value_2, result) ? 4 : 0;
  if (!status) {
    status = s21_is_equal(value_2, get_zero_dec()) ? 3 : 0;
  }
  if (!status) {
    *result = get_zero_dec();
    int sign1 = get_sign(value_1);
    int sign2 = get_sign(value_2);
    int result_sign = sign1 != sign2 ? 1 : 0;
    s21_int256 value_1_256int, value_2_256int, result_256int = {0};
    convert_decimal_to_int256(value_1, &value_1_256int);
    convert_decimal_to_int256(value_2, &value_2_256int);
    align_scales_int256(&value_1_256int, &value_2_256int);
    s21_int256 remainder = {0};
    division_bits_int256(value_1_256int, value_2_256int, &result_256int,
                         &remainder);
    if (result_256int.bits[4] != 0 || result_256int.bits[5] != 0 ||
        result_256int.bits[6] != 0 || result_256int.bits[7] != 0) {
      status = result_sign ? 2 : 1;
      *result = get_zero_dec();
    } else {
      int result_scale = 0;
      int overflow = 0;
      calculate_fractional_part(remainder, value_2_256int, &result_256int,
                                &result_scale);
      set_scale_int256(&result_256int, result_scale);
      set_sign_int256(&result_256int, result_sign);
      overflow = convert_int256_to_decimal(result_256int, result);
      overflow = is_value_equal_zero(*result) &&
                         !is_value_equal_zero(value_1) && !overflow
                     ? 2
                     : overflow;
      if (overflow == 1) {
        status = result_sign ? 2 : 1;
        *result = get_zero_dec();
      } else if (overflow == 2) {
        status = 2;
        *result = get_zero_dec();
      }
    }
  }
  return status;
}

int calculate_fractional_part(s21_int256 remainder, s21_int256 value_2_256int,
                              s21_int256 *result_256int, int *result_scale) {
  int overflow = 0;
  while (*result_scale < 29 && !equal_zero_int256(remainder) && !overflow) {
    s21_int256 stored_num = get_zero_int256();
    multiply_by_10_int256(&remainder);
    division_bits_int256(remainder, value_2_256int, &stored_num, &remainder);
    s21_int256 tmp_result = *result_256int;
    multiply_by_10_int256(&tmp_result);
    overflow = add_bits_int256(tmp_result, stored_num, &tmp_result);
    if (!overflow) {
      *result_256int = tmp_result;
      *result_scale += 1;
    }
  }

  if (!equal_zero_int256(remainder)) {
    multiply_by_10_int256(result_256int);
    add_bits_int256(*result_256int, get_one_int256(), result_256int);
    *result_scale += 1;
  }

  return overflow;
}