

#include "s21_decimal.h"
#include "utils.h"

typedef enum {
  IS_LESS,
  IS_LESS_OR_EQUAL,
  IS_GREATER,
  IS_GREATER_OR_EQUAL,
  IS_EQUAL,
  IS_NOT_EQUAL
} types_of_comparison;

static int compare_values(s21_decimal value_1, s21_decimal value_2,
                          types_of_comparison type);

static int is_less_compare(s21_decimal value_1, s21_decimal value_2,
                           types_of_comparison type);

static int is_greater_compare(s21_decimal value_1, s21_decimal value_2,
                              types_of_comparison type);

static int fast_sign_check(s21_decimal value_1, s21_decimal value_2,
                           types_of_comparison type, int* result);

static int is_equal_compare(s21_decimal value_1, s21_decimal value_2,
                            types_of_comparison type);

static void invert_result(int* result);

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int need_to_continue = fast_sign_check(value_1, value_2, IS_LESS, &result);

  if (need_to_continue) {
    equalize_scales(&value_1, &value_2);
    result = compare_values(value_1, value_2, IS_LESS);
  }

  return result;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int need_to_continue =
      fast_sign_check(value_1, value_2, IS_LESS_OR_EQUAL, &result);

  if (need_to_continue) {
    equalize_scales(&value_1, &value_2);
    result = compare_values(value_1, value_2, IS_LESS_OR_EQUAL);
  }
  return result;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int need_to_continue = fast_sign_check(value_1, value_2, IS_GREATER, &result);

  if (need_to_continue) {
    equalize_scales(&value_1, &value_2);
    result = compare_values(value_1, value_2, IS_GREATER);
  }
  return result;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int need_to_continue =
      fast_sign_check(value_1, value_2, IS_GREATER_OR_EQUAL, &result);

  if (need_to_continue) {
    equalize_scales(&value_1, &value_2);
    result = compare_values(value_1, value_2, IS_GREATER_OR_EQUAL);
  }

  return result;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int need_to_continue = fast_sign_check(value_1, value_2, IS_EQUAL, &result);
  if (need_to_continue) {
    equalize_scales(&value_1, &value_2);
    result = compare_values(value_1, value_2, IS_EQUAL);
  }
  return result;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int need_to_continue =
      fast_sign_check(value_1, value_2, IS_NOT_EQUAL, &result);
  if (need_to_continue) {
    equalize_scales(&value_1, &value_2);
    result = compare_values(value_1, value_2, IS_NOT_EQUAL);
  }
  return result;
}

static int fast_sign_check(s21_decimal value_1, s21_decimal value_2,
                           types_of_comparison type, int* result) {
  int need_to_continue = 1;
  if (is_negative(value_1) && !is_negative(value_2)) {
    if (type == IS_LESS || type == IS_LESS_OR_EQUAL || type == IS_NOT_EQUAL) {
      *result = 1;
      need_to_continue = 0;
    } else if (type == IS_GREATER || type == IS_GREATER_OR_EQUAL ||
               type == IS_EQUAL) {
      *result = 0;
      need_to_continue = 0;
    }
  } else if (!is_negative(value_1) && is_negative(value_2)) {
    if (type == IS_LESS || type == IS_LESS_OR_EQUAL || type == IS_EQUAL) {
      *result = 0;
      need_to_continue = 0;
    } else if (type == IS_GREATER || type == IS_GREATER_OR_EQUAL ||
               type == IS_NOT_EQUAL) {
      *result = 1;
      need_to_continue = 0;
    }
  }
  return need_to_continue;
}

static int compare_values(s21_decimal value_1, s21_decimal value_2,
                          types_of_comparison type) {
  int result = -1;
  switch (type) {
    case IS_LESS:
    case IS_LESS_OR_EQUAL:
      result = is_less_compare(value_1, value_2, type);
      break;
    case IS_GREATER:
    case IS_GREATER_OR_EQUAL:
      result = is_greater_compare(value_1, value_2, type);
      break;
    case IS_EQUAL:
    case IS_NOT_EQUAL:
      result = is_equal_compare(value_1, value_2, type);
      break;
  }
  return result;
}

static int is_less_compare(s21_decimal value_1, s21_decimal value_2,
                           types_of_comparison type) {
  int result = -1;
  int is_values_equal = 0;
  if (value_1.bits[2] < value_2.bits[2]) {
    result = 1;
  } else if (value_1.bits[2] > value_2.bits[2]) {
    result = 0;
  } else if (value_1.bits[1] < value_2.bits[1]) {
    result = 1;
  } else if (value_1.bits[1] > value_2.bits[1]) {
    result = 0;
  } else if (value_1.bits[0] < value_2.bits[0]) {
    result = 1;
  } else if (value_1.bits[0] > value_2.bits[0]) {
    result = 0;
  } else {
    result = type == IS_LESS ? 0 : 1;
    is_values_equal = 1;
  }
  if (is_negative(value_1) && is_negative(value_2) && !is_values_equal)
    invert_result(&result);
  return result;
}

static int is_greater_compare(s21_decimal value_1, s21_decimal value_2,
                              types_of_comparison type) {
  int result = -1;
  int is_values_equal = 0;
  if (value_1.bits[2] > value_2.bits[2]) {
    result = 1;
  } else if (value_1.bits[2] < value_2.bits[2]) {
    result = 0;
  } else if (value_1.bits[1] > value_2.bits[1]) {
    result = 1;
  } else if (value_1.bits[1] < value_2.bits[1]) {
    result = 0;
  } else if (value_1.bits[0] > value_2.bits[0]) {
    result = 1;
  } else if (value_1.bits[0] < value_2.bits[0]) {
    result = 0;
  } else {
    result = type == IS_GREATER ? 0 : 1;
    is_values_equal = 1;
  }
  if (is_negative(value_1) && is_negative(value_2) && !is_values_equal)
    invert_result(&result);
  return result;
}

static int is_equal_compare(s21_decimal value_1, s21_decimal value_2,
                            types_of_comparison type) {
  int result = 1;
  for (int i = 0; i < 3; ++i) {
    if (value_1.bits[i] != value_2.bits[i]) {
      result = 0;
    }
  }

  if (type == IS_NOT_EQUAL) {
    invert_result(&result);
  }
  return result;
}

int is_negative(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

static void invert_result(int* result) {
  if (*result == 0) {
    *result = 1;
  } else if (*result == 1) {
    *result = 0;
  }
}