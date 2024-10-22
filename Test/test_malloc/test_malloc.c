/**************************************************************************//**
 * @item     CosyOS-II 内核服务测试
 * @file     test_malloc.c
 * @brief    动态内存测试
 * @detail   通过 hmac-md5 算法的实施，实现动态内存分配相关服务的测试。
 * @author   迟凯峰
 * @version  V1.1.0
 * @date     2024.06.25
 ******************************************************************************/

#include "hmacmd5.h"
#if TEST_OBJECT == TEST_MALLOC

/* 在阿里云上创建产品和设备并获取三元组 */
#define	productKey       "a1AnuD7s00k"                      // 创建产品成功后会生成产品key
#define deviceName       "863293050329280"                  // 用模组IMEI号做为设备名来创建设备
#define deviceSecret     "618fd4511f9704fd58d29e83e26d42bd" // 创建设备成功后会生成设备密钥

#define	clientId         deviceName
#define	deviceName_LEN   15
#define	deviceSecret_LEN 32

void start_hook(void)
{
	char _MALLOC_MEM_ *text;
	char _MALLOC_MEM_ *sign;
	text = (char _MALLOC_MEM_ *)uMalloc(text_LEN + 1);
	if(!text){
		printf("uMalloc is failed\r\n");
		goto __END;
	}
	sign = (char _MALLOC_MEM_ *)uMalloc(sign_LEN + 1);
	if(!sign){
		printf("uMalloc is failed\r\n");
		goto __END;
	}
	
	text[0] = '\0';
	sign[0] = '\0';
	
	/* 打印标题 */
	#define title \
		\x43\x6F\x73\x79\x4F\x53\x2D\x49\x49 \
		\xB6\xAF\xCC\xAC\xC4\xDA\xB4\xE6\xB7\xD6\xC5\xE4 \
		\xCF\xE0\xB9\xD8\xB7\xFE\xCE\xF1\xB2\xE2\xCA\xD4\r\n
	printf(sDefStr(title));
	
	/*
	 * text（明文）
	 * 格式：clientId-clientId-deviceName-deviceName-productKey-productKey
	 * 示例：clientId863293050329280deviceName863293050329280productKeya1AnuD7s00k
	 */
	{
		strcat(
			strcat(
				strcat(
					strcat(
						strcat(
							strcpy(text, "clientId"),
						clientId),
					"deviceName"),
				deviceName),
			"productKey"),
		productKey);
	}
	
	/* hmac_md5 */
	if(hmac_md5(sign, (u8 *)text , text_LEN, (u8 *)deviceSecret, deviceSecret_LEN)){
		printf("sign: %s\r\n", sign);
		uDelay_s(1);
	}
	else{
		printf("hmac-md5 is failed\r\n");
	}
	
__END:
	uFree(sign);
	uFree(text);
	while(1);
}

#endif
