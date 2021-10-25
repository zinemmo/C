#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Sort)
{
    RUN_TEST_CASE(Sort, test_if_array_is_sorted);
    RUN_TEST_CASE(Sort, test_if_array_is_sorted_and_with_correct_values);
    RUN_TEST_CASE(Sort, test_if_array_is_sorted_and_with_correct_negative_values);
    RUN_TEST_CASE(Sort, test_if_array_is_null);
    RUN_TEST_CASE(Sort, test_if_sorted_size_is_size);
    RUN_TEST_CASE(Sort, test_if_sorted_size_is_bigger);
    RUN_TEST_CASE(Sort, test_if_size_zero_dont_sort);
    RUN_TEST_CASE(Sort, test_if_size_less_than_zero);
    RUN_TEST_CASE(Sort, test_if_array_is_sorted_and_with_correct_values_and_with_size_lesser_than_array_size);
    RUN_TEST_CASE(Sort, test_if_size_float_works);
    RUN_TEST_CASE(Sort, test_if_array_float_works);
}