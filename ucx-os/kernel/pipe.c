/* file:          pipe.c
 * description:   pipe implementation
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

static int32_t ispowerof2(uint32_t x)
{
	return x && !(x & (x - 1));
}

static uint32_t nextpowerof2(uint32_t x)
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	
	return x;
}

struct pipe_s *ucx_pipe_create(uint16_t size)
{
	struct pipe_s *pipe;
	
	if (size < 2)
		size = 2;
	
	if (!ispowerof2(size))
		size = nextpowerof2(size);
		
	pipe = (struct pipe_s *)malloc(sizeof(struct pipe_s));
	
	if (!pipe)
		return 0;
	
	pipe->mask = size - 1;
	pipe->data = (char *)malloc(size);
	if (!pipe->data) {
		free(pipe);
		return 0;
	}
	pipe->head = 0;
	pipe->tail = 0;
	pipe->size = 0;
	
	return pipe;
}

int32_t ucx_pipe_destroy(struct pipe_s *pipe)
{
	if (!pipe->data)
		return -1;
	
	pipe->mask = 0;
	free(pipe->data);
	free(pipe);
	
	return 0;
}

void ucx_pipe_flush(struct pipe_s *pipe)
{
	ucx_critical_enter();
	pipe->head = 0;
	pipe->tail = 0;
	pipe->size = 0;
	ucx_critical_leave();
}

int32_t ucx_pipe_size(struct pipe_s *pipe)
{
	return pipe->size;
}

static int32_t ucx_pipe_get(struct pipe_s *pipe)
{
	int32_t head;

	if (pipe->head == pipe->tail)
		return -1;

	head = pipe->head;
	pipe->head = (pipe->head + 1) & pipe->mask;
	pipe->size--;

	return pipe->data[head];
}

static int32_t ucx_pipe_put(struct pipe_s *pipe, char data)
{
	int32_t tail;

	tail = (pipe->tail + 1) & pipe->mask;
	if (tail == pipe->head)
		return -1;

	pipe->data[pipe->tail] = data;
	pipe->tail = tail;
	pipe->size++;

	return 0;
}

/* this routine is blocking and must be called inside a task. */
int32_t ucx_pipe_read(struct pipe_s *pipe, char *data, uint16_t size)
{
	uint16_t i = 0;
	int32_t byte;
	
	while (i < size) {
		ucx_critical_enter();
		byte = ucx_pipe_get(pipe);
		ucx_critical_leave();
		if (byte == -1) {
			_delay_ms(1);
			continue;
		}
		data[i] = byte;
		i++;
	}
	
	return i;
	
}

/* this routine is blocking and must be called inside a task. */
int32_t ucx_pipe_write(struct pipe_s *pipe, char *data, uint16_t size)
{
	uint16_t i = 0;
	int32_t res;
	
	while (i < size) {
		ucx_critical_enter();
		res = ucx_pipe_put(pipe, data[i]);
		ucx_critical_leave();
		if (res == -1) {
			_delay_ms(1);
			continue;
		}
		i++;
	}

	return i;
}
