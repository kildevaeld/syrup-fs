#include <unity.h>

extern void test_fs_is_req();


int main() {
  UNITY_BEGIN();

  RUN_TEST(test_fs_is_req);
  

  return UNITY_END();
}