#include "unity.h"
#include "unity_fixture.h"
#include "../src/identifier.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TEST_GROUP(Identifier);

TEST_SETUP(Identifier)
{
}

TEST_TEAR_DOWN(Identifier)
{
}

//VALID_S TESTS
TEST(Identifier, validS_should_return_0_with_digit)
{
    TEST_ASSERT_EQUAL_INT(0, valid_s(0));
    TEST_ASSERT_EQUAL_INT(0, valid_s(64));
    TEST_ASSERT_EQUAL_INT(0, valid_s(125));
}

TEST(Identifier, validS_should_return_0_with_char_digit)
{
    TEST_ASSERT_EQUAL_INT(0, valid_s('1'));
    TEST_ASSERT_EQUAL_INT(0, valid_s('5'));
    TEST_ASSERT_EQUAL_INT(0, valid_s('9'));
}

TEST(Identifier, validS_should_return_0_with_char_special)
{
    TEST_ASSERT_EQUAL_INT(0, valid_s('/'));
    TEST_ASSERT_EQUAL_INT(0, valid_s('&'));
    TEST_ASSERT_EQUAL_INT(0, valid_s('*'));
}

TEST(Identifier, validS_should_return_1_with_char)
{
    TEST_ASSERT_EQUAL_INT(1, valid_s('A'));
    TEST_ASSERT_EQUAL_INT(1, valid_s('Z'));
    TEST_ASSERT_EQUAL_INT(1, valid_s('a'));
    TEST_ASSERT_EQUAL_INT(1, valid_s('z'));
}

//VALID_F TESTS
TEST(Identifier, validF_should_return_0_with_digit)
{
    TEST_ASSERT_EQUAL_INT(0, valid_f(0));
    TEST_ASSERT_EQUAL_INT(0, valid_f(64));
    TEST_ASSERT_EQUAL_INT(0, valid_f(125));
}

TEST(Identifier, validF_should_return_0_with_char_special)
{
    TEST_ASSERT_EQUAL_INT(0, valid_s('/'));
    TEST_ASSERT_EQUAL_INT(0, valid_s('&'));
    TEST_ASSERT_EQUAL_INT(0, valid_s('*'));
}

TEST(Identifier, validF_should_return_1_with_char_digit)
{
    TEST_ASSERT_EQUAL_INT(1, valid_f('1'));
    TEST_ASSERT_EQUAL_INT(1, valid_f('5'));
    TEST_ASSERT_EQUAL_INT(1, valid_f('9'));
}

TEST(Identifier, validF_should_return_1_with_char)
{
    TEST_ASSERT_EQUAL_INT(1, valid_f('m'));
    TEST_ASSERT_EQUAL_INT(1, valid_f('A'));
    TEST_ASSERT_EQUAL_INT(1, valid_f('Z'));
}

//IDENTIFIER TESTS
TEST(Identifier, identifier_starts_with_break)
{
    char *charTeste = "\neste";
    // char charTeste[10];
    // strcpy(charTeste, "\neste");
    TEST_ASSERT_EQUAL_INT(1, identifier(charTeste, 0));
}

TEST(Identifier, identifier_starts_with_char_digit)
{  
    char *charTeste = "1este";
    // char charTeste[10];
    // strcpy(charTeste, "1este");
    TEST_ASSERT_EQUAL_INT(1, identifier(charTeste, 0));
}

TEST(Identifier, identifier_have_bad_char)
{
    char *charTeste = "tes/t";
    // char charTeste[10];
    // strcpy(charTeste, "tes&t");
    TEST_ASSERT_EQUAL_INT(1, identifier(charTeste, 0));
}

TEST(Identifier, identifier_is_null)
{
    char *charTeste = "";
    // char charTeste[10];
    // strcpy(charTeste, "");
    TEST_ASSERT_EQUAL_INT(1, identifier(charTeste, 0));
}

TEST(Identifier, identifier_is_bigger_than_limit)
{
    char *charTeste = "abcde1234";
    // char charTeste[10];
    // strcpy(charTeste, "abcde1234");
    TEST_ASSERT_EQUAL_INT(1, identifier(charTeste, 0));
}

TEST(Identifier, identifier_starts_with_char)
{
    char *charTeste = "Ae1t3";
    // char charTeste[10];
    // strcpy(charTeste, "Aest");
    TEST_ASSERT_EQUAL_INT(0, identifier(charTeste, 0));
}