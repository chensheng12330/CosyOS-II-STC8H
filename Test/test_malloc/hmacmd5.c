/**************************************************************************//**
 * @file     hmacmd5.c
 * @brief    hmac-md5 算法
 * @author   迟凯峰
 * @version  V1.0.1
 * @date     2024.06.25
 ******************************************************************************/

#include "hmacmd5.h"
#if TEST_OBJECT == TEST_MALLOC

static uint8_t _MALLOC_MEM_ *PADDING;

/*
 * uint8_t 转 16进制字符串
 * caps：0为小写字母，1为大写字母
 */
static char *u8str16(char *str, uint8_t var, bool caps)
{
	str[0] = (var >> 4) + '0';
	str[1] = (var & 0x0F) + '0';
	if(str[0] > '9') str[0] += caps ? 0x07 : 0x27;
	if(str[1] > '9') str[1] += caps ? 0x07 : 0x27;
	str[2] = '\0';
	return str;
}

static void MD5Init(MD5_CTX *context)
{
	context->count[0] = 0;
	context->count[1] = 0;
	context->state[0] = 0x67452301;
	context->state[1] = 0xEFCDAB89;
	context->state[2] = 0x98BADCFE;
	context->state[3] = 0x10325476;
}

static void MD5Decode(uint32_t *output, uint8_t *input, uint8_t len)
{
	uint8_t i = 0;
	uint8_t j = 0;
	while(j < len){
		output[i] = ((uint32_t)input[j + 0] <<  0)
		          | ((uint32_t)input[j + 1] <<  8)
		          | ((uint32_t)input[j + 2] << 16)
		          | ((uint32_t)input[j + 3] << 24);
		i++;
		j += 4;
	}
}

static void MD5Encode(uint8_t *output, uint32_t *input, uint8_t len)
{
	uint8_t i = 0;
	uint8_t j = 0;
	while(j < len){
		output[j + 0] = (input[i] >>  0) & 0xFF;
		output[j + 1] = (input[i] >>  8) & 0xFF;
		output[j + 2] = (input[i] >> 16) & 0xFF;
		output[j + 3] = (input[i] >> 24) & 0xFF;
		i++;
		j += 4;
	}
}

static bool MD5Transform(uint32_t *state, uint8_t *block)
{
	volatile uint32_t a = state[0];
	volatile uint32_t b = state[1];
	volatile uint32_t c = state[2];
	volatile uint32_t d = state[3];
	uint32_t _MALLOC_MEM_ *x = (uint32_t _MALLOC_MEM_ *)uMalloc(256);
	if(!x) return false;
	
	MD5Decode(x, block, 64);
	FF(a, b, c, d, x[ 0],  7, 0xd76aa478); /* 1 */
	FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */
	FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */
	FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[ 4],  7, 0xf57c0faf); /* 5 */
	FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[ 8],  7, 0x698098d8); /* 9 */
	FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12],  7, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */
	/* Round 2 */
	GG(a, b, c, d, x[ 1],  5, 0xf61e2562); /* 17 */
	GG(d, a, b, c, x[ 6],  9, 0xc040b340); /* 18 */
	GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[ 5],  5, 0xd62f105d); /* 21 */
	GG(d, a, b, c, x[10],  9, 0x02441453); /* 22 */
	GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[ 9],  5, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, x[14],  9, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13],  5, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[ 2],  9, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */
	/* Round 3 */
	HH(a, b, c, d, x[ 5],  4, 0xfffa3942); /* 33 */
	HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[ 1],  4, 0xa4beea44); /* 37 */
	HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13],  4, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[ 6], 23, 0x04881d05); /* 44 */
	HH(a, b, c, d, x[ 9],  4, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */
	/* Round 4 */
	II(a, b, c, d, x[ 0],  6, 0xf4292244); /* 49 */
	II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12],  6, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[ 8],  6, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[ 4],  6, 0xf7537e82); /* 61 */
	II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	uFree(x);
	return true;
}

static bool MD5Update(MD5_CTX *context, uint8_t *input, uint8_t inputlen)
{
	uint8_t i;
	uint8_t index = (context->count[0] >> 3) & 0x3F;
	uint8_t partlen = 64 - index;
	context->count[0] += (uint32_t)inputlen << 3;
	if(context->count[0] < ((uint32_t)inputlen << 3)) context->count[1]++;
	context->count[1] += (uint32_t)inputlen >> 29;
	if(inputlen >= partlen){
		memcpy(&context->buffer[index], input, partlen);
		if(!MD5Transform(context->state, context->buffer)) return false;
		for(i = partlen; i + 64 <= inputlen; i += 64){
			if(!MD5Transform(context->state, &input[i])) return false;
		}
		index = 0;
	}
	else i = 0;
	memcpy(&context->buffer[index], &input[i], inputlen - i);
	return true;
}

static bool MD5Final(MD5_CTX *context, uint8_t *digest)
{
	uint8_t bits[8];
	uint8_t index = (context->count[0] >> 3) & 0x3F;
	uint8_t padlen = (index < 56) ? (56 - index) : (120 - index);
	MD5Encode(bits, context->count, 8);
	if(!MD5Update(context, PADDING, padlen)) return false;
	if(!MD5Update(context, bits, 8)) return false;
	MD5Encode(digest, context->state, 16);
	return true;
}

char *hmac_md5(char *str, uint8_t *text, uint8_t tlen, uint8_t *key, uint8_t klen)
{
	uint8_t i;
	uint8_t _MALLOC_MEM_ *out;
	uint8_t _MALLOC_MEM_ *tempString16;
	uint8_t _MALLOC_MEM_ *OneEnding;
	uint8_t _MALLOC_MEM_ *TwoEnding;
	uint8_t _MALLOC_MEM_ *ThreeEnding;
	uint8_t _MALLOC_MEM_ *FourEnding;
	uint8_t _MALLOC_MEM_ *FiveEnding;
	uint8_t _MALLOC_MEM_ *SixEnding;
	MD5_CTX	_MALLOC_MEM_ *md5;
	char temp[3];
	char ipad = 0x36;
	char opad = 0x5c;
	
	uCreateMempool;
	if(!uInitMempool(650)) return NULL;
	out          = (uint8_t _MALLOC_MEM_ *)uTalloc(LENGTH_MD5_RESULT);                if(!out) goto __FAULT;
	tempString16 = (uint8_t _MALLOC_MEM_ *)uTalloc(LENGTH_MD5_RESULT);                if(!tempString16) goto __FAULT;
	OneEnding    = (uint8_t _MALLOC_MEM_ *)uTalloc(LENGTH_BLOCK);                     if(!OneEnding) goto __FAULT;
	TwoEnding    = (uint8_t _MALLOC_MEM_ *)uTalloc(LENGTH_BLOCK);                     if(!TwoEnding) goto __FAULT;
	ThreeEnding  = (uint8_t _MALLOC_MEM_ *)uTalloc(LENGTH_BLOCK + tlen);              if(!ThreeEnding) goto __FAULT;
	FourEnding   = (uint8_t _MALLOC_MEM_ *)uTalloc(LENGTH_MD5_RESULT);                if(!FourEnding) goto __FAULT;
	FiveEnding   = (uint8_t _MALLOC_MEM_ *)uTalloc(LENGTH_BLOCK);                     if(!FiveEnding) goto __FAULT;
	SixEnding    = (uint8_t _MALLOC_MEM_ *)uTalloc(LENGTH_BLOCK + LENGTH_MD5_RESULT); if(!SixEnding) goto __FAULT;
	PADDING      = (uint8_t _MALLOC_MEM_ *)uTalloc(64);                               if(!PADDING) goto __FAULT;
	md5          = (MD5_CTX _MALLOC_MEM_ *)uTalloc(sizeof(MD5_CTX));                  if(!md5) goto __FAULT;
	
	memset(PADDING, 0, 64);
	PADDING[0] = 0x80;
	for(i = 0; i < LENGTH_BLOCK; i++) OneEnding[i] = 0;
	if(klen > LENGTH_BLOCK){
		MD5Init(md5);
		if(!MD5Update(md5, key, klen)) goto __FAULT;
		if(!MD5Final(md5, tempString16)) goto __FAULT;
		for(i = 0; i < LENGTH_MD5_RESULT; i++) OneEnding[i] = tempString16[i];
	}
	else{
		for(i = 0; i < klen; i++) OneEnding[i] = key[i];
	}
	for(i = 0; i < LENGTH_BLOCK; i++) TwoEnding[i] = OneEnding[i] ^ ipad;
	for(i = 0; i < LENGTH_BLOCK; i++) ThreeEnding[i] = TwoEnding[i];
	for(; i < tlen + LENGTH_BLOCK; i++) ThreeEnding[i] = text[i - LENGTH_BLOCK];
	MD5Init(md5);
	if(!MD5Update(md5, ThreeEnding, (uint8_t)(LENGTH_BLOCK + tlen))) goto __FAULT;
	if(!MD5Final(md5, FourEnding)) goto __FAULT;
	for(i = 0; i < LENGTH_BLOCK; i++) FiveEnding[i] = OneEnding[i] ^ opad;
	for(i = 0; i < LENGTH_BLOCK; i++) SixEnding[i] = FiveEnding[i];
	for(; i < LENGTH_BLOCK + LENGTH_MD5_RESULT; i++) SixEnding[i] = FourEnding[i - LENGTH_BLOCK];
	MD5Init(md5);
	if(!MD5Update(md5, SixEnding, LENGTH_BLOCK + LENGTH_MD5_RESULT)) goto __FAULT;
	if(!MD5Final(md5, out)) goto __FAULT;
	str[0] = '\0';
	for(i = 0; i < LENGTH_MD5_RESULT; i++) strcat(str, u8str16(temp, out[i], 0));
	uFreeMempool;
	return str;
	
__FAULT:
	uFreeMempool;
	return NULL;
}

#endif
