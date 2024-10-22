/**************************************************************************//**
 * @file     hmacmd5.h
 * @brief    hmac-md5 算法
 * @author   迟凯峰
 * @version  V1.0.1
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __HMACMD5_H
#define __HMACMD5_H

/*
 * 用户定义
 */

#define	text_LEN     69
#define	sign_LEN     32
#define LENGTH_BLOCK 64
#include "..\Test\test.h"
#define bool uint8_t



/*
 * 系统定义
 */

char *hmac_md5(char *str, uint8_t *text, uint8_t tlen, uint8_t *key, uint8_t klen);

typedef struct
{
	uint32_t count[2];
	uint32_t state[4];
	uint8_t  buffer[64];
}MD5_CTX;

#define LENGTH_MD5_RESULT  (sign_LEN / 2)

#define F(x, y, z)         ((x & y) | (~x & z))
#define G(x, y, z)         ((x & z) | (y & ~z))
#define H(x, y, z)         (x ^ y ^ z)
#define I(x, y, z)         (y ^ (x | ~z))

#define ROTATE_LEFT(x, n)  ((x << n) | (x >> (32 - n)))

#define FF(a, b, c, d, x, s, ac) \
do{ \
	a += F(b, c, d) + x + ac; \
	a = ROTATE_LEFT(a, s); \
	a += b; \
}while(0)

#define GG(a, b, c, d, x, s, ac) \
do{ \
	a += G(b, c, d) + x + ac; \
	a = ROTATE_LEFT(a, s); \
	a += b; \
}while(0)

#define HH(a, b, c, d, x, s, ac) \
do{ \
	a += H(b, c, d) + x + ac; \
	a = ROTATE_LEFT(a, s); \
	a += b; \
}while(0)

#define II(a, b, c, d, x, s, ac) \
do{ \
	a += I(b, c, d) + x + ac; \
	a = ROTATE_LEFT(a, s); \
	a += b; \
}while(0)



#endif
