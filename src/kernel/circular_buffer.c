#include <stdint.h>
#include <stddef.h>

#include "circular_buffer.h"


/*
 * Pushes a single byte onto a circular buffer.
 * Returns 0 on succes and -1 if buffer is full.
 */
int8_t push(circBuf_t *c, uint8_t data)
{
	size_t next = c->head + 1;
	if (next >= c->maxLen)
		next = 0;

	// Cicular buffer is full
	if (next == c->tail) {
		return -1;
	}

	c->buffer[c->head] = data;
	c->head = next;
	return 0;
}


/*
* Pops a single byte from a circular buffer.
* Returns 0 on succes and -1 if buffer is empty.
*/
int8_t pop(circBuf_t *c, uint8_t *data)
{
	/* if the head isn't ahead of the tail, we don't have any characters */
	if (c->head == c->tail)
		return -1;  /* quit with an error */

	*data = c->buffer[c->tail];
	c->buffer[c->tail] = 0;  /* clear the data (optional) */

	size_t next = c->tail + 1;
	if(next >= c->maxLen)
		next = 0;

	c->tail = next;

	return 0;
}


/*
 * Pushes an array of length 'len' onto a circular buffer.
 * Returns 0 on succes and -1 if buffer is full.
 * Note: Every message gets a header of sizeof(size_t) on the buffer.
 */
int8_t push_item(circBuf_t *c, uint8_t *data, size_t len)
{
	size_t saved_head = c->head;
	uint8_t *blen = (uint8_t *)&len;
	for (uint8_t i = 0; i < sizeof(len); ++i) {
		const int8_t ret = push(c, *(blen + i));
		if (ret == -1) {
			c->head = saved_head;
			return ret;
		}
	}

	for (size_t i = 0; i < len; ++i) {
		const int8_t ret = push(c, *(data + i));
		if (ret == -1) {
			c->head = saved_head;
			return ret;
		}
	}

	return 0;
}


/*
 * Pops an array of length 'len' from a circular buffer.
 * Returns 0 on succes and -1 if buffer is empty.
 */
int8_t pop_item(circBuf_t *c, uint8_t *data, size_t *len)
{
	*len = 0;
	for (size_t i = 0; i < sizeof(size_t); ++i) {
		const int8_t ret = pop(c, (uint8_t *)len + i);
		if (ret == -1) {
			return ret;
		}
	}

	for (size_t i = 0; i < *len; ++i) {
		const int8_t ret = pop(c, data + i);
		if (ret == -1) {
			return ret;
		}
	}

	return 0;
}
