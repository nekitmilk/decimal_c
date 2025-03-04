#include "test_runner.h"

static void run_tests(void);
static int run_test_suite(Suite* (*suite_func)(void));

int main(void) {
  run_tests();
  return 0;
}

static void run_tests(void) {
  int failed = 0;
  // failed += run_test_suite(comparison_suite);
  // failed += run_test_suite(convert_suite);
  failed += run_test_suite(arithmetic_suite);
  // failed += run_test_suite(other_functions_suite);
  //
  // тут делаем  failed += run_test_suite(название вашего suite)
  //
  failed += run_test_suite(add_suite0);
  failed += run_test_suite(add_suite1);
  failed += run_test_suite(add_suite2);
  failed += run_test_suite(add_suite3);
  failed += run_test_suite(add_suite4);
  failed += run_test_suite(add_suite5);
  failed += run_test_suite(add_suite6);
  failed += run_test_suite(add_suite7);
  failed += run_test_suite(add_suite8);
  failed += run_test_suite(add_suite9);

  if (!failed) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("%d %s FAILED\n", failed, failed > 1 ? "TESTS" : "TEST");
  }
}

static int run_test_suite(Suite* (*suite_func)(void)) {
  Suite* s = suite_func();
  SRunner* runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed;
}