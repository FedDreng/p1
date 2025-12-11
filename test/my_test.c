#include "mtest.h"

TEST_CASE(my_first_test_case, {
  int i = 42;
  CHECK_EQ_INT(42, i);
})

TEST_CASE(my_other_test_case, {
  double d = 3.14159;
  CHECK_EQ_DOUBLE(3.14159, d, 0.0001);
})

MAIN_RUN_TESTS(my_first_test_case, my_other_test_case)
