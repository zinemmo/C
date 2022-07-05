#include "sort.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdlib.h>
#include <string.h>

TEST_GROUP(Sort);

TEST_SETUP(Sort)
{
}

TEST_TEAR_DOWN(Sort)
{
}

TEST(Sort, test_if_array_is_sorted)
{
    // Array desordenado
    int array[] = {1,9,3,6,0,5,2,8,4,7};
    // Pressupondo tamanho correto
    int size = sizeof(array)/sizeof(array[0]);
    int i = 0;
    // Executando a função com o Array desordenado
    sort(array, size);

    // Pressupondo o correto funcionamento da função e verificando a ordenação crescente ou descendente
    if(array[0] > array[1]){
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_GREATER_THAN(array[i+1], array[i]);
        }
    } else if(array[0] < array[1]) {
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_LESS_THAN(array[i+1], array[i]);
        }
    }
}

TEST(Sort, test_if_array_is_sorted_and_with_correct_values)
{
    int array[] = {1,9,3,6,0,5,2,8,4,7};
    // Pressupondo correto funcionamento da função devemos verificar se todos os valores estão corretamente ordenados
    // Tanto em orde crescente como decrescente 
    int arrayAuxAsc[] = {0,1,2,3,4,5,6,7,8,9};
    int arrayAuxDes[] = {9,8,7,6,5,4,3,2,1,0};
    // Pressupondo tamanho correto
    int size = sizeof(array)/sizeof(array[0]);
    int i = 0;

    // Executando função com funcionamento pressupostamente correto
    sort(array, size);

    if(array[0] > array[1]){
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_GREATER_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxDes, size);
    } else if(array[0] < array[1]) {
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_LESS_THAN(array[i+1], array[i]);
        }
        // Se ordenação correta verifica se os valores estão corretos
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxAsc, size);
    }
}

TEST(Sort, test_if_array_is_sorted_and_with_correct_negative_values)
{
    int array[] = {-1,-9,-3,-6,0,-5,-2,-8,-4,-7};
    // Pressupondo correto funcionamento da função devemos verificar se todos os valores estão corretamente ordenados
    // Tanto em orde crescente como decrescente 
    int arrayAuxAsc[] = {0,-1,-2,-3,-4,-5,-6,-7,-8,-9};
    int arrayAuxDes[] = {-9,-8,-7,-6,-5,-4,-3,-2,-1,0};
    // Pressupondo tamanho correto
    int size = sizeof(array)/sizeof(array[0]);
    int i = 0;

    // Executando função com funcionamento pressupostamente correto
    sort(array, size);

    if(array[0] > array[1]){
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_GREATER_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxAsc, size);
    } else if(array[0] < array[1]) {
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_LESS_THAN(array[i+1], array[i]);
        }
        // Se ordenação correta verifica se os valores estão corretos
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxDes, size);
    }
}

TEST(Sort, test_if_array_is_sorted_and_with_correct_values_and_with_size_lesser_than_array_size)
{
    int array[] = {1,9,3,6,0,5,2,8,4,7};
    // Pressupondo correto funcionamento da função devemos verificar se todos os valores estão corretamente ordenados
    // Tanto em orde crescente como decrescente 
    int arrayAuxAsc[] = {0,1,2,3,5,6,8,9};
    int arrayAuxDes[] = {9,8,6,5,3,2,1,0};
    // Pressupondo tamanho correto
    int size = sizeof(array)/sizeof(array[0]) - 2;
    int i = 0;

    // Executando função com funcionamento pressupostamente correto
    sort(array, size);

    if(array[0] > array[1]){
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_GREATER_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxDes, size);
    } else if(array[0] < array[1]) {
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_LESS_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxAsc, size);
    }
}

TEST(Sort, test_if_array_is_null)
{
    // Cria um Array nulo
    int *array = NULL;
    int size = 1;

    sort(array, size);

    // Aguarda que a função não altere o mesmo
    TEST_ASSERT_NULL(array);
}

TEST(Sort, test_if_sorted_size_is_size){
	int array[] = {1,9,3,6,0,5,2,8,4,7};
    int arrayAuxAsc[] = {0,1,2,3,4,5,6,7,8,9};
    int arrayAuxDes[] = {9,8,7,6,5,4,3,2,1,0};
	int size = sizeof(array)/sizeof(array[0]);

	sort(array, size);


    TEST_ASSERT_EQUAL((sizeof(array)/sizeof(array[0])), size);

    int i;
    if(array[0] > array[1]){
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_GREATER_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxDes, size);
    } else if(array[0] < array[1]) {
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_LESS_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxAsc, size);
    }
}


TEST(Sort, test_if_sorted_size_is_bigger){
	int array[] = {1,9,3,6,0,5,2,8,4,7};
    int arrayAuxAsc[] = {0,1,2,3,4,5,6,7,8,9};
    int arrayAuxDes[] = {9,8,7,6,5,4,3,2,1,0};
	int size = sizeof(array)/sizeof(array[0]) + 1;

	sort(array, size);


    TEST_ASSERT_EQUAL((sizeof(array)/sizeof(array[0])), size);

    int i;
    if(array[0] > array[1]){
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_GREATER_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxDes, size);
    } else if(array[0] < array[1]) {
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_LESS_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxAsc, size);
    }
}

TEST(Sort, test_if_size_zero_dont_sort){
	int array[] = {1,9,3,6,0,5,2,8,4,7};
	int size = sizeof(array)/sizeof(array[0]);
    // Cria vetor auxiliar para ser comparado se ambos forem iguais mais tarde
    int auxArray[size];
	
    memcpy(auxArray, array, sizeof(array));
	
	sort(array, 0);

	TEST_ASSERT_EQUAL_INT_ARRAY(&array, &auxArray, size);
}

TEST(Sort, test_if_size_less_than_zero){
    int array[] = {1,9,3,6,0,5,2,8,4,7};
    int size = sizeof(array)/sizeof(array[0]);
    // Cria vetor auxiliar para ser comparado se ambos forem iguais mais tarde
    int auxArray[size];
    memcpy(auxArray, array, sizeof(array));

    sort(array, -2);

    TEST_ASSERT_EQUAL_INT_ARRAY(&array, &auxArray, size);
}

TEST(Sort, test_if_size_float_works){
    int array[] = {1,9,3,6,0,5,2,8,4,7};
    int arrayAuxAsc[] = {0,1,2,3,4,5,6,7,8,9};
    int arrayAuxDes[] = {9,8,7,6,5,4,3,2,1,0};
    int size = sizeof(array)/sizeof(array[0]);
    int i = 0;

	float testSize = size + 0.4687;
    
    sort(array, testSize);

    if(array[0] > array[1]){
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_GREATER_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxDes, size);
    } else if(array[0] < array[1]) {
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_LESS_THAN(array[i+1], array[i]);
        }
        // Se ordenação correta verifica se os valores estão corretos
        TEST_ASSERT_EQUAL_INT_ARRAY(&array, &arrayAuxAsc, size);
    }
}

TEST(Sort, test_if_array_float_works){
    float array[] = {1.1,9.2,3.4,6.3,0.1,5.2,2.9,8.1,4.2,7.9};
    float arrayAuxAsc[] = {0.1,1.1,2.9,3.4,4.2,5.2,6.3,7.9,8.1,9.2};
    float arrayAuxDes[] = {9.2,8.1,7.9,6.3,5.2,4.2,3.4,2.9,1.1,0.1};
    int size = sizeof(array)/sizeof(array[0]);
    int i = 0;
    
    sort(array, size);

    if(array[0] > array[1]){
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_GREATER_THAN(array[i+1], array[i]);
        }
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(&array, &arrayAuxDes, size);
    } else if(array[0] < array[1]) {
        for(i = 0; i < size - 1; i++){
            // Validando se a ordenação está correta
            TEST_ASSERT_LESS_THAN(array[i+1], array[i]);
        }
        // Se ordenação correta verifica se os valores estão corretos
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(&array, &arrayAuxAsc, size);
    }
}
