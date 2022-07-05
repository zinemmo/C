#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Payment)
{
    RUN_TEST_CASE(Payment, Test_Inferior_Limit);
    RUN_TEST_CASE(Payment, Test_Superior_Limit);
    RUN_TEST_CASE(Payment, Test_Immediatly_Inferior);
    RUN_TEST_CASE(Payment, Test_Immediatly_Superior);
    RUN_TEST_CASE(Payment, Test_First_Interior_Value);
    RUN_TEST_CASE(Payment, Test_Second_Interior_Value);
    RUN_TEST_CASE(Payment, Test_More_Than_Two_Decimals);
    RUN_TEST_CASE(Payment, Test_If_Value_Is_Not_Number);
    RUN_TEST_CASE(Payment, Test_Status_Regular);
    RUN_TEST_CASE(Payment, Test_Status_Estudante);
    RUN_TEST_CASE(Payment, Test_Status_Aposentado);
    RUN_TEST_CASE(Payment, Test_Status_VIP);
    RUN_TEST_CASE(Payment, Test_Char_Array_Greater_Than_Fifteen);
    RUN_TEST_CASE(Payment, Test_Null_Char_Array);
    RUN_TEST_CASE(Payment, Test_If_Status_Not_In_List);
    RUN_TEST_CASE(Payment, Test_Int_Array);
    RUN_TEST_CASE(Payment, Test_Int);
}
