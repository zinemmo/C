#include <ucx.h>

void task4(void)
{
	ucx_task_init();

	while (1) {
		printf("");
	}
}

void task3(void)
{
	ucx_task_init();

	while (1) {
		printf("");
	}
}

void task2(void)
{
	ucx_task_init();

	while (1) {
		printf("");
	}
}

void task1(void)
{
	ucx_task_init();

	while (1) {
		printf("");
	}
}

void task0(void)
{
	ucx_task_init();

	while (1) {
		printf("");
	}
}

int32_t app_main(void)
{
	ucx_task_add(task4, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task3, 10, 50, 40, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task2, 5, 22, 15, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task1, 3, 5, 4, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task0, 1, 4, 4, DEFAULT_GUARD_SIZE);

	ucx_task_priority(0, TASK_HIGH_PRIO);
	ucx_task_priority(0, TASK_HIGH_PRIO);
	
	return 1;
}
