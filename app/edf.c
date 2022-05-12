#include <ucx.h>

void task3(void)
{
	ucx_task_init();

	while (1) {
		printf("*\n");
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
	ucx_task_add(task3, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task1, 2, 15, 12, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task2, 3, 10, 9, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task0, 5, 15, 8, DEFAULT_GUARD_SIZE);

	// A última task está sendo executada primeiro
	// start UCX/OS, preemptive mode
	return 1;
}
