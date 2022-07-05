#include <ucx.h>

void task5(void)
{
	ucx_task_init();

	while (1) {
		printf("*\n");
		ucx_task_wfi();
	}
}

void task4(void)
{
	ucx_task_init();

	while (1) {
		printf("E\n");
		ucx_task_wfi();
	}
}

void task3(void)
{
	ucx_task_init();

	while (1) {
		printf("D\n");
		ucx_task_wfi();
	}
}

void task2(void)
{
	ucx_task_init();

	while (1) {
		printf("C\n");
		ucx_task_wfi();
	}
}

void task1(void)
{
	ucx_task_init();

	while (1) {
		printf("B\n");
		ucx_task_wfi();
	}
}

void task0(void)
{
	ucx_task_init();

	while (1) {
		printf("A\n");
		ucx_task_wfi();
	}
}

int32_t app_main(void)
{
	ucx_task_add_periodic(task4, 1, 20, 11, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task3, 2, 15, 14, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task2, 2, 20, 15, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task1, 1, 10, 6, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task0, 3, 15, 12, DEFAULT_GUARD_SIZE);
	ucx_task_add(task5, DEFAULT_GUARD_SIZE);
	
	return 1;
}
