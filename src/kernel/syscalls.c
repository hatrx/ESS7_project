#include <stm32f4xx.h>
#include <sys/stat.h>
#include <unistd.h>

#include "drivers/uart.h"

#define STACK_POINTER (__get_MSP())
#define UART_AUTO_ECHO


int _close(int file)
{
	UNUSED(file);
	return -1;
}


int _fstat(int file, struct stat *st)
{
	UNUSED(file);
	st->st_mode = S_IFCHR;
	return 0;
}


int _isatty(int file)
{
	switch(file) {
	case 1: // stdio
	case 2: // stderr
		return 1;

	default: return 0;
	}
}


int _lseek(int file, int ptr, int dir)
{
	UNUSED(file); UNUSED(ptr); UNUSED(dir);
	return 0;
}


int _open(const char *name, int flags, int mode)
{
	UNUSED(name); UNUSED(flags); UNUSED(mode);
	return -1;
}


int _read(int file, char *ptr, int len)
{
	UNUSED(file);
	int i;
	for (i = 0; i < len; i++) {
		ptr[i] = _uart_getc();
#ifdef UART_AUTO_ECHO
		_uart_putc(ptr[i]);
#endif
		/* Return partial buffer if we get EOL */
		if ('\r' == ptr[i]) {
			++i;
			ptr[i] = '\n';
			return i;
		}
	}

	ptr[i-1] = '\r';
	ptr[i] = '\n';
	return  i;
}


caddr_t _sbrk(int incr)
{
	extern char _end;		/* Defined by the linker */
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &_end;
	}
	prev_heap_end = heap_end;
	if ((unsigned)(heap_end + incr) > STACK_POINTER) {
		/* Heap and stack collision */
		return (caddr_t)0;
	}

	heap_end += incr;
	return (caddr_t) prev_heap_end;
}


int _write(int file, char *ptr, int len)
{
	if (!isatty(file)) {
		return BSP_UARTx_transmit((uint8_t*)ptr, len) == 0 ? len : 0;
	}

	int todo;
	for (todo = 0; todo < len; todo++) {
		const char c = *ptr++;
		if (c == '\n') BSP_UARTx_transmit((uint8_t*)&(char){'\r'}, 1);
		if (BSP_UARTx_transmit((uint8_t*)&c, 1) != 0) break;
	}

	return todo;
}


void _init(void) {}
