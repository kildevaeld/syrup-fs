#include <syrup/fs.h>
#include <unity.h>

void test_fs_is_req() {
  TEST_ASSERT_EQUAL(true, sy_file_is_dir("./src"));
  TEST_ASSERT_EQUAL(true, sy_file_is_regfile("./Makefile"));
}