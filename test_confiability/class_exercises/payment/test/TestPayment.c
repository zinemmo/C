#include "payment.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdlib.h>
#include <string.h>

TEST_GROUP(Payment);

TEST_SETUP(Payment)
{
}

TEST_TEAR_DOWN(Payment)
{
}

TEST(Payment, Test_Inferior_Limit)
{
	TEST_ASSERT_EQUAL(0, payment(0.01, "VIP"));
}

TEST(Payment, Test_Superior_Limit)
{
	TEST_ASSERT_EQUAL(0, payment(99999, "VIP"));
}

TEST(Payment, Test_Immediatly_Inferior){
	TEST_ASSERT_EQUAL(1, payment(0, "VIP"));
}

TEST(Payment, Test_Immediatly_Superior){
	TEST_ASSERT_EQUAL(1, payment(99999.01, "VIP"));
}

TEST(Payment, Test_First_Interior_Value){
	TEST_ASSERT_EQUAL(0, payment(1, "VIP"));
}

TEST(Payment, Test_Second_Interior_Value){
	TEST_ASSERT_EQUAL(0, payment(45000, "VIP"));
}

TEST(Payment, Test_More_Than_Two_Decimals){
	TEST_ASSERT_EQUAL(1, payment(5000.156, "VIP"));
}

TEST(Payment, Test_If_Value_Is_Not_Number){
	TEST_ASSERT_EQUAL(1, payment('a', "VIP"));
}

TEST(Payment, Test_Status_Regular){
	TEST_ASSERT_EQUAL(0, payment(45000.15, "regular"));
}

TEST(Payment, Test_Status_Estudante){
	TEST_ASSERT_EQUAL(0, payment(45000.15, "estudante"));
}

TEST(Payment, Test_Status_Aposentado){
	TEST_ASSERT_EQUAL(0, payment(45000.15, "aposentado"));
}

TEST(Payment, Test_Status_VIP){
	TEST_ASSERT_EQUAL(0, payment(45000.15, "VIP"));
}

TEST(Payment, Test_Char_Array_Greater_Than_Fifteen){
	TEST_ASSERT_EQUAL(2, payment(1000.60, "abcdefghijklmnopqrs"));
}

TEST(Payment, Test_Null_Char_Array){
	char* status = NULL;
	TEST_ASSERT_EQUAL(2, payment(45000.15, status));
}

TEST(Payment, Test_If_Status_Not_In_List){
	TEST_ASSERT_EQUAL(0, payment(45000.15, "estudantes"));
}

TEST(Payment, Test_Int_Array){
	int array[] = {1, 2, 3};
	TEST_ASSERT_EQUAL(2, payment(45000.15, array));
}

TEST(Payment, Test_Int){
	TEST_ASSERT_EQUAL(2, payment(45000.15, 4));
}
