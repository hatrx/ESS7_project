#ifndef UTILS_H
#define UTILS_H


#include <stdint.h>


void delay_ms(uint32_t ms);


#define ARR_LEN(arr)    (sizeof(arr) / sizeof(arr[0]))

/* Bit operations
* Does operation on the specified bit
* where a=target, b=bit number to act on
*/
#define BIT_SET(a,b) 		( (a) |= (1<<(b))	)
#define BIT_CLEAR(a,b) 		( (a) &= ~(1<<(b))	)
#define BIT_FLIP(a,b) 		( (a) ^= (1<<(b))	)
#define BIT_CHECK(a,b) 		( (a) & (1<<(b))	)

/* Bit mask operations
* Does operation on the specified target
* according to the mask where
* x=target, y=mask
*/
#define BITMASK_SET(x,y) 	( (x) |= (y)	)
#define BITMASK_CLEAR(x,y) 	( (x) &= (~(y))	)
#define BITMASK_FLIP(x,y) 	( (x) ^= (y)	) // Use: ( (x) = ~(y)	) instead?
#define BITMASK_CHECK(x,y) 	( (x) & (y)		)

/* Check if x is a power of 2 */
#define IS_POW2(x) 				( (x) && !((x) & ((x) - 1))	)

/* check if x and y has opposite sign */
#define HAS_OPPSITE_SIGN(x, y)	( (((x) ^ (y)) < 0) )

/* Extracts the low-order (rightmost) byte of a variable. */
#define LOW_BYTE(w) 		( (uint8_t) ((w) & 0xFF)		)

/* Extracts the high-order (leftmost) byte of a word
* (or the second lowest byte of a larger data type).
*/
#define HIGH_BYTE(w) 		( (uint8_t) ((w) >> 8)			)


#define LOW_NIBBLE(b)		( (b) & 0xF 		)
#define HIGH_NIBBLE(b)		( ((b) >> 4) & 0xF 	)
#define MERGE_NIBBLES(h, l) ( ((h) << 4) | (l) 	)

#define IS_ODD(n)	(((n) & 1) == 1)

#define DEBUG()	do {printf("%s:%d\n", __FUNCTION__, __LINE__);} while(0)


#endif /* UTILS_H */
