/* file:          ucx.c
 * description:   UCX/OS kernel
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct kcb_s kernel_state;
struct kcb_s *kcb_p = &kernel_state;
int clockCounter = 1;

/* kernel auxiliary functions */

static void krnl_guard_check(void)
{
	uint32_t check = 0x33333333;
	
	if (*kcb_p->tcb_p->guard_addr != check) {
		ucx_hexdump((void *)kcb_p->tcb_p->guard_addr, kcb_p->tcb_p->guard_sz);
		printf("\n*** HALT - task %d, guard %08x (%d) check failed\n", kcb_p->tcb_p->id,
			(size_t)kcb_p->tcb_p->guard_addr, (size_t)kcb_p->tcb_p->guard_sz);
		for (;;);
	}
		
}

static void krnl_delay_update(void)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
	for (;;	tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->state == TASK_BLOCKED && tcb_ptr->delay > 0) {
			tcb_ptr->delay--;
			if (tcb_ptr->delay == 0)
				tcb_ptr->state = TASK_READY;
		}
		if (tcb_ptr->tcb_next == kcb_p->tcb_first) break;
	}
}

static void krnl_sched_init(int32_t preemptive)
{
	kcb_p->tcb_p = kcb_p->tcb_first;
	if (preemptive) {
		_timer_enable();
	}
	(*kcb_p->tcb_p->task)();
}


/* task scheduler and dispatcher */

uint16_t krnl_schedule(void)
{
	if (kcb_p->tcb_p->state == TASK_RUNNING) {
		printf("Task Running %d\n", kcb_p->tcb_p->id);
		kcb_p->tcb_p->state = TASK_READY;
	}

	do {
		do {
			kcb_p->tcb_p = kcb_p->tcb_p->tcb_next;
		} while (kcb_p->tcb_p->state != TASK_READY);
	} while (--kcb_p->tcb_p->priority & 0xff);
	kcb_p->tcb_p->priority |= (kcb_p->tcb_p->priority >> 8) & 0xff;
	kcb_p->tcb_p->state = TASK_RUNNING;
	kcb_p->ctx_switches++;
	
	return kcb_p->tcb_p->id;
}

int16_t krnl_schedule_edf(void)
{
	struct tcb_s *tcb_aux;
	tcb_aux->deadlineCounter = __INT16_MAX__;
	int currentRunningTaskId;
	bool cycledThroughList = false;
	// Duvida se temos que rodar pra sempre ou só dentro do MMC
	// Se for pra sempre, como controlamos o reinício das tasks periódicas;

	// Definir como solucionar casos de borda, como não execução após fim do período

	if (kcb_p->tcb_p->state == TASK_RUNNING) {
		printf("Task Running %d\n", kcb_p->tcb_p->id);
		if(kcb_p->tcb_p->capacityCounter > 0) {
			kcb_p->tcb_p->capacityCounter--;
			printf("Capacity of running task %d\n", kcb_p->tcb_p->capacityCounter);
		}

		kcb_p->tcb_p->state = TASK_READY;
		currentRunningTaskId = kcb_p->tcb_p->id;
	}

	do {
		if(kcb_p->tcb_p->isStr) {
			kcb_p->tcb_p->deadlineCounter--; 
			if(kcb_p->tcb_p->deadlineCounter < tcb_aux->deadlineCounter && kcb_p->tcb_p->capacityCounter > 0) {
				tcb_aux = kcb_p->tcb_p;
			}
			
			// Reseta a tarefa com capacidade zerada e em um novo período
			if(kcb_p->tcb_p->capacityCounter == 0 && kcb_p->tcb_p->period == clockCounter) {
				printf("Task %d reseted\n", kcb_p->tcb_p->id);
				kcb_p->tcb_p->capacityCounter = kcb_p->tcb_p->capacity;
				kcb_p->tcb_p->deadlineCounter = kcb_p->tcb_p->deadline;
				kcb_p->tcb_p->state = TASK_READY;
			}

			// Temos que resetar as que passam do periodo necessitando de computações ainda
			
			printf("CC - %u DC - %d P %u - ID %u\n", kcb_p->tcb_p->capacityCounter, kcb_p->tcb_p->deadlineCounter, kcb_p->tcb_p->period, kcb_p->tcb_p->id);
		}
		kcb_p->tcb_p = kcb_p->tcb_p->tcb_next;
		
		if(currentRunningTaskId == kcb_p->tcb_p->id) {
			cycledThroughList = true;
		}

	} while (cycledThroughList == false);
	
	if(tcb_aux->deadlineCounter != __INT16_MAX__) {
		printf("Task EDF scheduled %d - %d\n", tcb_aux->id, tcb_aux);
	}

	if(tcb_aux->deadlineCounter == __INT16_MAX__) {
		return -1;
	}

	kcb_p->tcb_p = tcb_aux;
	kcb_p->tcb_p->priority |= (kcb_p->tcb_p->priority >> 8) & 0xff;
	kcb_p->tcb_p->state = TASK_RUNNING;
	kcb_p->ctx_switches++;
	

	if(kcb_p->tcb_p->isStr) {
		return kcb_p->tcb_p->id;
	}

	return -1;
}

void krnl_dispatcher(void)
{
	if (!setjmp(kcb_p->tcb_p->context)) {
		krnl_delay_update();
		krnl_guard_check();
		printf("------ %d ------\n", clockCounter);
		clockCounter++;
		int id = krnl_schedule_edf();
		if(id < 0) {
			krnl_schedule();
		}
		_interrupt_tick();
		longjmp(kcb_p->tcb_p->context, 1);
	}
}


/* task management API */

int32_t ucx_task_add(void *task, uint16_t guard_size)
{
	struct tcb_s *tcb_last = kcb_p->tcb_p;
	
	kcb_p->tcb_p = (struct tcb_s *)malloc(sizeof(struct tcb_s));
	if (kcb_p->tcb_first == 0)
		kcb_p->tcb_first = kcb_p->tcb_p;

	if (!kcb_p->tcb_p)
		return -1;

	if (tcb_last)
		tcb_last->tcb_next = kcb_p->tcb_p;
	kcb_p->tcb_p->tcb_next = kcb_p->tcb_first;
	kcb_p->tcb_p->task = task;
	kcb_p->tcb_p->delay = 0;
	kcb_p->tcb_p->guard_sz = guard_size;
	kcb_p->tcb_p->id = kcb_p->id++;
	kcb_p->tcb_p->state = TASK_STOPPED;
	kcb_p->tcb_p->priority = TASK_NORMAL_PRIO;
	kcb_p->tcb_p->isStr = false;
	
	return 0;
}

int32_t ucx_task_add_periodic(void *task, uint16_t capacity, uint16_t period, uint16_t deadline, uint16_t guard_size)
{
	struct tcb_s *tcb_last = kcb_p->tcb_p;
	
	kcb_p->tcb_p = (struct tcb_s *)malloc(sizeof(struct tcb_s));
	if (kcb_p->tcb_first == 0)
		kcb_p->tcb_first = kcb_p->tcb_p;

	if (!kcb_p->tcb_p)
		return -1;

	if (tcb_last)
		tcb_last->tcb_next = kcb_p->tcb_p;
	kcb_p->tcb_p->tcb_next = kcb_p->tcb_first;
	kcb_p->tcb_p->task = task;
	kcb_p->tcb_p->delay = 0;
	kcb_p->tcb_p->guard_sz = guard_size;
	kcb_p->tcb_p->id = kcb_p->id++;
	kcb_p->tcb_p->state = TASK_STOPPED;
	kcb_p->tcb_p->priority = TASK_NORMAL_PRIO;
	kcb_p->tcb_p->period = period;
	kcb_p->tcb_p->capacity = capacity;
	kcb_p->tcb_p->deadline = deadline;
	kcb_p->tcb_p->isStr = true;
	kcb_p->tcb_p->deadlineCounter = deadline;
	kcb_p->tcb_p->capacityCounter = capacity;
	
	return 0;
}

/*
 * First following lines of code are absurd at best. Stack marks are
 * used by krnl_guard_check() to detect stack overflows on guard space.
 * It is up to the user to define sufficient stack guard space (considering
 * local thread allocation of the stack for recursion and context
 * saving). Stack allocated for data before ucx_task_init() (generally
 * most stack used by a task) is not verified.
 * We also need the safety pig, just in case.
                         _ 
 _._ _..._ .-',     _.._(`)) 
'-. `     '  /-._.-'    ',/ 
   )         \            '. 
  / _    _    |             \ 
 |  a    a    /              | 
 \   .-.                     ;   
  '-('' ).-'       ,'       ; 
     '-;           |      .' 
        \           \    / 
        | 7  .__  _.-\   \ 
        | |  |  ``/  /`  / 
       /,_|  |   /,_/   / 
          /,_/      '`-' 
*/
void ucx_task_init(void)
{
	char guard[kcb_p->tcb_p->guard_sz];
	
	memset(guard, 0x69, kcb_p->tcb_p->guard_sz);
	memset(guard, 0x33, 4);
	memset((guard) + kcb_p->tcb_p->guard_sz - 4, 0x33, 4);
	kcb_p->tcb_p->guard_addr = (uint32_t *)guard;
	printf("task %d, guard: %08x - %08x\n", kcb_p->tcb_p->id, (size_t)kcb_p->tcb_p->guard_addr,
		(size_t)kcb_p->tcb_p->guard_addr + kcb_p->tcb_p->guard_sz);
	
	if (!setjmp(kcb_p->tcb_p->context)) {
		kcb_p->tcb_p->state = TASK_READY;
		if (kcb_p->tcb_p->tcb_next == kcb_p->tcb_first) {
			kcb_p->tcb_p->state = TASK_RUNNING;
		} else {
			kcb_p->tcb_p = kcb_p->tcb_p->tcb_next;
			kcb_p->tcb_p->state = TASK_RUNNING;
			(*kcb_p->tcb_p->task)();
		}
	}
	_ei(1);
}

void ucx_task_yield()
{
	if (!setjmp(kcb_p->tcb_p->context)) {
		krnl_delay_update();		/* TODO: check if we need to run a delay update on yields. maybe only on a non-preemtive execution? */ 
		krnl_guard_check();
		krnl_schedule();
		longjmp(kcb_p->tcb_p->context, 1);
	}
}

void ucx_task_delay(uint16_t ticks)
{
	kcb_p->tcb_p->delay = ticks;
	kcb_p->tcb_p->state = TASK_BLOCKED;
	ucx_task_yield();
}

int32_t ucx_task_suspend(uint16_t id)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			ucx_critical_enter();
			if (tcb_ptr->state == TASK_READY || tcb_ptr->state == TASK_RUNNING) {
				tcb_ptr->state = TASK_SUSPENDED;
				ucx_critical_leave();
				break;
			} else {
				ucx_critical_leave();
				return -1;
			}
		}
		if (tcb_ptr->tcb_next == kcb_p->tcb_first)
			return -1;
	}
	if (kcb_p->tcb_p->id == id)
		ucx_task_yield();
	
	return 0;
}

int32_t ucx_task_resume(uint16_t id)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			ucx_critical_enter();
			if (tcb_ptr->state == TASK_SUSPENDED) {
				tcb_ptr->state = TASK_READY;
				ucx_critical_leave();
				break;
			} else {
				ucx_critical_leave();
				return -1;
			}
		}	
		if (tcb_ptr->tcb_next == kcb_p->tcb_first)
			return -1;
	}
	if (kcb_p->tcb_p->id == id)
		ucx_task_yield();
	
	return 0;
}

int32_t ucx_task_priority(uint16_t id, uint16_t priority)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;

	switch (priority) {
	case TASK_CRIT_PRIO:
	case TASK_HIGH_PRIO:
	case TASK_NORMAL_PRIO:
	case TASK_LOW_PRIO:
	case TASK_IDLE_PRIO:
		break;
	default:
		return -1;
	}
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			tcb_ptr->priority = priority;
			break;
		}
		if (tcb_ptr->tcb_next == kcb_p->tcb_first)
			return -1;
	}
	
	return 0;
}

uint16_t ucx_task_id()
{
	return kcb_p->tcb_p->id;
}

void ucx_task_wfi()
{
	volatile uint32_t s;
	
	s = kcb_p->ctx_switches;
	while (s == kcb_p->ctx_switches);
}

uint16_t ucx_task_count()
{
	return kcb_p->id + 1;
}

void ucx_critical_enter()
{
	_timer_disable();
}

void ucx_critical_leave()
{
	_timer_enable();
}


/* main() function, called from the C runtime */

int32_t main(void)
{
	int32_t pr;
	
	_hardware_init();
	
	kcb_p->tcb_p = 0;
	kcb_p->tcb_first = 0;
	kcb_p->ctx_switches = 0;
	kcb_p->id = 0;
	
	printf("UCX/OS boot on %s\n", __ARCH__);
#ifndef UCX_OS_HEAP_SIZE
	ucx_heap_init((size_t *)&_heap_start, (size_t)&_heap_size);
	printf("heap_init(), %d bytes free\n", (size_t)&_heap_size);
#else
	ucx_heap_init((size_t *)&_heap, UCX_OS_HEAP_SIZE);
	printf("heap_init(), %d bytes free\n", UCX_OS_HEAP_SIZE);
#endif
	pr = app_main();
	krnl_sched_init(pr);
	
	return 0;
}
