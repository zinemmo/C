#include <ucx.h>

void task2(void)
{
	ucx_task_init();

	while (1) {
		printf("*\n");
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
	ucx_task_add_periodic(task0, 1, 4, 4, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task1, 4, 5, 4, DEFAULT_GUARD_SIZE);
	ucx_task_add(task2, DEFAULT_GUARD_SIZE);

	return 1;
}
