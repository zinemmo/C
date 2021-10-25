#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Identifier)
{
    RUN_TEST_CASE(Identifier, validS_should_return_0_with_digit);
    RUN_TEST_CASE(Identifier, validS_should_return_0_with_char_digit);
    RUN_TEST_CASE(Identifier, validS_should_return_0_with_char_special);
    RUN_TEST_CASE(Identifier, validS_should_return_1_with_char);
    RUN_TEST_CASE(Identifier, validF_should_return_0_with_digit);
    RUN_TEST_CASE(Identifier, validF_should_return_0_with_char_special);
    RUN_TEST_CASE(Identifier, validF_should_return_1_with_char_digit);
    RUN_TEST_CASE(Identifier, validF_should_return_1_with_char);
    RUN_TEST_CASE(Identifier, identifier_starts_with_break);
    RUN_TEST_CASE(Identifier, identifier_starts_with_char_digit);
    RUN_TEST_CASE(Identifier, identifier_have_bad_char);
    RUN_TEST_CASE(Identifier, identifier_is_null);
    RUN_TEST_CASE(Identifier, identifier_is_bigger_than_limit);
    RUN_TEST_CASE(Identifier, identifier_starts_with_char);
}