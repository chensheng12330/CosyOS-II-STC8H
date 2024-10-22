#include "..\System\os_link.h"

#if MCUCFG_ISA != __ARM__

void led_task_1(void)
{
	uDelay_ms(100);
	P27 = 0;
	uDelay_ms(100);
	P26 = 0;
	uDelay_ms(100);
	P25 = 0;
	uDelay_ms(100);
	P24 = 0;
	uDelay_ms(100);
	P23 = 0;
	uDelay_ms(100);
	P22 = 0;
	uDelay_ms(100);
	P21 = 0;
	uDelay_ms(100);
	P20 = 0;
	
	uDelay_ms(100);
	P35 = 1;
	uDelay_ms(100);
	P34 = 1;
	uDelay_ms(100);
	P33 = 1;
	uDelay_ms(100);
	P32 = 1;
	
	uDelay_ms(100);
	P27 = 1;
	uDelay_ms(100);
	P26 = 1;
	uDelay_ms(100);
	P25 = 1;
	uDelay_ms(100);
	P24 = 1;
	uDelay_ms(100);
	P23 = 1;
	uDelay_ms(100);
	P22 = 1;
	uDelay_ms(100);
	P21 = 1;
	uDelay_ms(100);
	P20 = 1;
}

void led_task_2(void)
{
	uDelay_ms(100);
	P35 = 0;
	uDelay_ms(100);
	P34 = 0;
	uDelay_ms(100);
	P33 = 0;
	uDelay_ms(100);
	P32 = 0;
}

#else

void led_task_1(void)
{
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
}

void led_task_2(void)
{
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
	uDelay_ms(100);
	__NOP();
}

#endif
