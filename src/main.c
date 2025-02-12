#include <stdio.h>

#include "s21_decimal.h"
#include "s21_print_decimal.h"

void print_dec(s21_decimal d);

int main(int argc, const char* argv[]) {
  s21_decimal d = {0};
  s21_decimal a = {0};
  s21_decimal result = {0};

  a.bits[0] = 0xFFFFFFFF;
  a.bits[1] = 0xFFFFFFFF;
  // a.bits[3] = 0x80000000;
  //  a.bits[1] = 0xFFFFFFFF;
  //  a.bits[2] = 0x8000000;
  d.bits[0] = 0xFFFFFFFF;
  // d.bits[1] = 0xFFFFFFFF;
  // d.bits[2] = 0xFFFFF;

  print_dec(a);
  printf("\n");
  print_dec(d);
  printf("\n");
  // if (add_bits(&a, &d, &result) == 0) {
  if (s21_sub(d, a, &result) == 0) {
    print_dec(result);
  } else {
    printf("too match");
  }

  // set_scale(&d, 20);
  // set_sign(&d, 1);

  // s21_from_int_to_decimal(99999, &d);

  // d.bits[0] = 0xFFFFFFFF;
  // d.bits[2] = 0xFFFFFFFF;
  // d.bits[2] = 0xFFFFFFFF;
  //  d.bits[0] = 0x354;
  // d.bits[1] = 1;

  // print_dec(d);
  // printf("\n");
  //  output_dec_bin(d);
  //  printf("\n");
  //  if (multiply_by_8(&d) == 0)
  //  {
  //      output_dec_bin(d);
  //      printf("\n");
  //      //print_dec(d);
  //  }
  //  else {
  //      printf("too match\n");
  //  }

  return 0;
}

void print_dec(s21_decimal d) {
  printf("%.8x %.8x %.8x %x", d.bits[2], d.bits[1], d.bits[0], d.bits[3]);
}