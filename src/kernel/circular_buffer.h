#ifndef CIRC_BUF_H
#define CIRC_BUF_H


#include <stdint.h>
#include <stddef.h>


typedef struct {
	uint8_t *buffer;
	size_t head;
	size_t tail;
	const size_t maxLen;
} circBuf_t;

#define CIRCBUF_DEF(x,y) uint8_t x##_space[y]; circBuf_t x = { x##_space,0,0,y}


int8_t push_item(circBuf_t *c, uint8_t *data, size_t len);
int8_t pop_item(circBuf_t *c, uint8_t *data, size_t *len);


#endif /* CIRC_BUF_H */
