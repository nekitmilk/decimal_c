#define MAKE_SUITE(nameoftest)

#include "test.h"

static void run_tests(void);
static int run_test_suite(Suite* (*suite_func)(void));

int main(void) {
  run_tests();
  return 0;
}

static void run_tests(void) {
  int failed = 0;
  // failed += run_test_suite(comparison_suite);
  //
  // тут делаем  failed += run_test_suite(название вашего suite)
  //

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
