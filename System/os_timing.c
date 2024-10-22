/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_timing.c
 * @brief    初始化定时中断、定时查询
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_var.h"

#if OS_TIMINTTOTAL || OS_TIMQRYTOTAL

#define	__EXT_TIMINT(tmid) \
	extern const s_timinthand_ts _CONST_MEM_ u_timinthand_##tmid;

#define	__EXT_TIMQRY(tmid) \
	extern const s_timqryhand_ts _CONST_MEM_ u_timqryhand_##tmid;

#define	__INIT_TIMINT(tmid) \
	s_timint_autoreload[tmid] = u_timinthand_##tmid.hookortask_autoreload & 0x01; \
	s_timint_hookortask[tmid] = u_timinthand_##tmid.hookortask_autoreload & 0x80 ? true : false; \
	s_timint_handle[tmid] = &u_timinthand_##tmid;

#define	__INIT_TIMQRY(tmid) \
	s_timqry_autoreload[tmid] = u_timqryhand_##tmid.hookortask_autoreload & 0x01; \
	s_timqry_hookortask[tmid] = u_timqryhand_##tmid.hookortask_autoreload & 0x80 ? true : false; \
	s_timqry_handle[tmid] = &u_timqryhand_##tmid;

#if OS_TIMINTTOTAL
#if OS_TIMINTTOTAL > 0
__EXT_TIMINT(0)
#if OS_TIMINTTOTAL > 1
__EXT_TIMINT(1)
#if OS_TIMINTTOTAL > 2
__EXT_TIMINT(2)
#if OS_TIMINTTOTAL > 3
__EXT_TIMINT(3)
#if OS_TIMINTTOTAL > 4
__EXT_TIMINT(4)
#if OS_TIMINTTOTAL > 5
__EXT_TIMINT(5)
#if OS_TIMINTTOTAL > 6
__EXT_TIMINT(6)
#if OS_TIMINTTOTAL > 7
__EXT_TIMINT(7)
#if OS_TIMINTTOTAL > 8
__EXT_TIMINT(8)
#if OS_TIMINTTOTAL > 9
__EXT_TIMINT(9)
#if OS_TIMINTTOTAL > 10
__EXT_TIMINT(10)
#if OS_TIMINTTOTAL > 11
__EXT_TIMINT(11)
#if OS_TIMINTTOTAL > 12
__EXT_TIMINT(12)
#if OS_TIMINTTOTAL > 13
__EXT_TIMINT(13)
#if OS_TIMINTTOTAL > 14
__EXT_TIMINT(14)
#if OS_TIMINTTOTAL > 15
__EXT_TIMINT(15)
#if OS_TIMINTTOTAL > 16
__EXT_TIMINT(16)
#if OS_TIMINTTOTAL > 17
__EXT_TIMINT(17)
#if OS_TIMINTTOTAL > 18
__EXT_TIMINT(18)
#if OS_TIMINTTOTAL > 19
__EXT_TIMINT(19)
#if OS_TIMINTTOTAL > 20
__EXT_TIMINT(20)
#if OS_TIMINTTOTAL > 21
__EXT_TIMINT(21)
#if OS_TIMINTTOTAL > 22
__EXT_TIMINT(22)
#if OS_TIMINTTOTAL > 23
__EXT_TIMINT(23)
#if OS_TIMINTTOTAL > 24
__EXT_TIMINT(24)
#if OS_TIMINTTOTAL > 25
__EXT_TIMINT(25)
#if OS_TIMINTTOTAL > 26
__EXT_TIMINT(26)
#if OS_TIMINTTOTAL > 27
__EXT_TIMINT(27)
#if OS_TIMINTTOTAL > 28
__EXT_TIMINT(28)
#if OS_TIMINTTOTAL > 29
__EXT_TIMINT(29)
#if OS_TIMINTTOTAL > 30
__EXT_TIMINT(30)
#if OS_TIMINTTOTAL > 31
__EXT_TIMINT(31)
#if OS_TIMINTTOTAL > 32
__EXT_TIMINT(32)
#if OS_TIMINTTOTAL > 33
__EXT_TIMINT(33)
#if OS_TIMINTTOTAL > 34
__EXT_TIMINT(34)
#if OS_TIMINTTOTAL > 35
__EXT_TIMINT(35)
#if OS_TIMINTTOTAL > 36
__EXT_TIMINT(36)
#if OS_TIMINTTOTAL > 37
__EXT_TIMINT(37)
#if OS_TIMINTTOTAL > 38
__EXT_TIMINT(38)
#if OS_TIMINTTOTAL > 39
__EXT_TIMINT(39)
#if OS_TIMINTTOTAL > 40
__EXT_TIMINT(40)
#if OS_TIMINTTOTAL > 41
__EXT_TIMINT(41)
#if OS_TIMINTTOTAL > 42
__EXT_TIMINT(42)
#if OS_TIMINTTOTAL > 43
__EXT_TIMINT(43)
#if OS_TIMINTTOTAL > 44
__EXT_TIMINT(44)
#if OS_TIMINTTOTAL > 45
__EXT_TIMINT(45)
#if OS_TIMINTTOTAL > 46
__EXT_TIMINT(46)
#if OS_TIMINTTOTAL > 47
__EXT_TIMINT(47)
#if OS_TIMINTTOTAL > 48
__EXT_TIMINT(48)
#if OS_TIMINTTOTAL > 49
__EXT_TIMINT(49)
#if OS_TIMINTTOTAL > 50
__EXT_TIMINT(50)
#if OS_TIMINTTOTAL > 51
__EXT_TIMINT(51)
#if OS_TIMINTTOTAL > 52
__EXT_TIMINT(52)
#if OS_TIMINTTOTAL > 53
__EXT_TIMINT(53)
#if OS_TIMINTTOTAL > 54
__EXT_TIMINT(54)
#if OS_TIMINTTOTAL > 55
__EXT_TIMINT(55)
#if OS_TIMINTTOTAL > 56
__EXT_TIMINT(56)
#if OS_TIMINTTOTAL > 57
__EXT_TIMINT(57)
#if OS_TIMINTTOTAL > 58
__EXT_TIMINT(58)
#if OS_TIMINTTOTAL > 59
__EXT_TIMINT(59)
#if OS_TIMINTTOTAL > 60
__EXT_TIMINT(60)
#if OS_TIMINTTOTAL > 61
__EXT_TIMINT(61)
#if OS_TIMINTTOTAL > 62
__EXT_TIMINT(62)
#if OS_TIMINTTOTAL > 63
__EXT_TIMINT(63)
#if OS_TIMINTTOTAL > 64
__EXT_TIMINT(64)
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#if OS_TIMQRYTOTAL
#if OS_TIMQRYTOTAL > 0
__EXT_TIMQRY(0)
#if OS_TIMQRYTOTAL > 1
__EXT_TIMQRY(1)
#if OS_TIMQRYTOTAL > 2
__EXT_TIMQRY(2)
#if OS_TIMQRYTOTAL > 3
__EXT_TIMQRY(3)
#if OS_TIMQRYTOTAL > 4
__EXT_TIMQRY(4)
#if OS_TIMQRYTOTAL > 5
__EXT_TIMQRY(5)
#if OS_TIMQRYTOTAL > 6
__EXT_TIMQRY(6)
#if OS_TIMQRYTOTAL > 7
__EXT_TIMQRY(7)
#if OS_TIMQRYTOTAL > 8
__EXT_TIMQRY(8)
#if OS_TIMQRYTOTAL > 9
__EXT_TIMQRY(9)
#if OS_TIMQRYTOTAL > 10
__EXT_TIMQRY(10)
#if OS_TIMQRYTOTAL > 11
__EXT_TIMQRY(11)
#if OS_TIMQRYTOTAL > 12
__EXT_TIMQRY(12)
#if OS_TIMQRYTOTAL > 13
__EXT_TIMQRY(13)
#if OS_TIMQRYTOTAL > 14
__EXT_TIMQRY(14)
#if OS_TIMQRYTOTAL > 15
__EXT_TIMQRY(15)
#if OS_TIMQRYTOTAL > 16
__EXT_TIMQRY(16)
#if OS_TIMQRYTOTAL > 17
__EXT_TIMQRY(17)
#if OS_TIMQRYTOTAL > 18
__EXT_TIMQRY(18)
#if OS_TIMQRYTOTAL > 19
__EXT_TIMQRY(19)
#if OS_TIMQRYTOTAL > 20
__EXT_TIMQRY(20)
#if OS_TIMQRYTOTAL > 21
__EXT_TIMQRY(21)
#if OS_TIMQRYTOTAL > 22
__EXT_TIMQRY(22)
#if OS_TIMQRYTOTAL > 23
__EXT_TIMQRY(23)
#if OS_TIMQRYTOTAL > 24
__EXT_TIMQRY(24)
#if OS_TIMQRYTOTAL > 25
__EXT_TIMQRY(25)
#if OS_TIMQRYTOTAL > 26
__EXT_TIMQRY(26)
#if OS_TIMQRYTOTAL > 27
__EXT_TIMQRY(27)
#if OS_TIMQRYTOTAL > 28
__EXT_TIMQRY(28)
#if OS_TIMQRYTOTAL > 29
__EXT_TIMQRY(29)
#if OS_TIMQRYTOTAL > 30
__EXT_TIMQRY(30)
#if OS_TIMQRYTOTAL > 31
__EXT_TIMQRY(31)
#if OS_TIMQRYTOTAL > 32
__EXT_TIMQRY(32)
#if OS_TIMQRYTOTAL > 33
__EXT_TIMQRY(33)
#if OS_TIMQRYTOTAL > 34
__EXT_TIMQRY(34)
#if OS_TIMQRYTOTAL > 35
__EXT_TIMQRY(35)
#if OS_TIMQRYTOTAL > 36
__EXT_TIMQRY(36)
#if OS_TIMQRYTOTAL > 37
__EXT_TIMQRY(37)
#if OS_TIMQRYTOTAL > 38
__EXT_TIMQRY(38)
#if OS_TIMQRYTOTAL > 39
__EXT_TIMQRY(39)
#if OS_TIMQRYTOTAL > 40
__EXT_TIMQRY(40)
#if OS_TIMQRYTOTAL > 41
__EXT_TIMQRY(41)
#if OS_TIMQRYTOTAL > 42
__EXT_TIMQRY(42)
#if OS_TIMQRYTOTAL > 43
__EXT_TIMQRY(43)
#if OS_TIMQRYTOTAL > 44
__EXT_TIMQRY(44)
#if OS_TIMQRYTOTAL > 45
__EXT_TIMQRY(45)
#if OS_TIMQRYTOTAL > 46
__EXT_TIMQRY(46)
#if OS_TIMQRYTOTAL > 47
__EXT_TIMQRY(47)
#if OS_TIMQRYTOTAL > 48
__EXT_TIMQRY(48)
#if OS_TIMQRYTOTAL > 49
__EXT_TIMQRY(49)
#if OS_TIMQRYTOTAL > 50
__EXT_TIMQRY(50)
#if OS_TIMQRYTOTAL > 51
__EXT_TIMQRY(51)
#if OS_TIMQRYTOTAL > 52
__EXT_TIMQRY(52)
#if OS_TIMQRYTOTAL > 53
__EXT_TIMQRY(53)
#if OS_TIMQRYTOTAL > 54
__EXT_TIMQRY(54)
#if OS_TIMQRYTOTAL > 55
__EXT_TIMQRY(55)
#if OS_TIMQRYTOTAL > 56
__EXT_TIMQRY(56)
#if OS_TIMQRYTOTAL > 57
__EXT_TIMQRY(57)
#if OS_TIMQRYTOTAL > 58
__EXT_TIMQRY(58)
#if OS_TIMQRYTOTAL > 59
__EXT_TIMQRY(59)
#if OS_TIMQRYTOTAL > 60
__EXT_TIMQRY(60)
#if OS_TIMQRYTOTAL > 61
__EXT_TIMQRY(61)
#if OS_TIMQRYTOTAL > 62
__EXT_TIMQRY(62)
#if OS_TIMQRYTOTAL > 63
__EXT_TIMQRY(63)
#if OS_TIMQRYTOTAL > 64
__EXT_TIMQRY(64)
#if OS_TIMQRYTOTAL > 65
__EXT_TIMQRY(65)
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

void s_init_timing(void)
{
	s_u8_t i;
	#if OS_TIMINTTOTAL
	for(i = 0; i < OS_TIMINTTOTAL; i++){
		s_timint_loader[i] = 0;
		s_timint_reload[i] = 0;
	}
	#if OS_TIMINTTOTAL > 0
	__INIT_TIMINT(0)
	#if OS_TIMINTTOTAL > 1
	__INIT_TIMINT(1)
	#if OS_TIMINTTOTAL > 2
	__INIT_TIMINT(2)
	#if OS_TIMINTTOTAL > 3
	__INIT_TIMINT(3)
	#if OS_TIMINTTOTAL > 4
	__INIT_TIMINT(4)
	#if OS_TIMINTTOTAL > 5
	__INIT_TIMINT(5)
	#if OS_TIMINTTOTAL > 6
	__INIT_TIMINT(6)
	#if OS_TIMINTTOTAL > 7
	__INIT_TIMINT(7)
	#if OS_TIMINTTOTAL > 8
	__INIT_TIMINT(8)
	#if OS_TIMINTTOTAL > 9
	__INIT_TIMINT(9)
	#if OS_TIMINTTOTAL > 10
	__INIT_TIMINT(10)
	#if OS_TIMINTTOTAL > 11
	__INIT_TIMINT(11)
	#if OS_TIMINTTOTAL > 12
	__INIT_TIMINT(12)
	#if OS_TIMINTTOTAL > 13
	__INIT_TIMINT(13)
	#if OS_TIMINTTOTAL > 14
	__INIT_TIMINT(14)
	#if OS_TIMINTTOTAL > 15
	__INIT_TIMINT(15)
	#if OS_TIMINTTOTAL > 16
	__INIT_TIMINT(16)
	#if OS_TIMINTTOTAL > 17
	__INIT_TIMINT(17)
	#if OS_TIMINTTOTAL > 18
	__INIT_TIMINT(18)
	#if OS_TIMINTTOTAL > 19
	__INIT_TIMINT(19)
	#if OS_TIMINTTOTAL > 20
	__INIT_TIMINT(20)
	#if OS_TIMINTTOTAL > 21
	__INIT_TIMINT(21)
	#if OS_TIMINTTOTAL > 22
	__INIT_TIMINT(22)
	#if OS_TIMINTTOTAL > 23
	__INIT_TIMINT(23)
	#if OS_TIMINTTOTAL > 24
	__INIT_TIMINT(24)
	#if OS_TIMINTTOTAL > 25
	__INIT_TIMINT(25)
	#if OS_TIMINTTOTAL > 26
	__INIT_TIMINT(26)
	#if OS_TIMINTTOTAL > 27
	__INIT_TIMINT(27)
	#if OS_TIMINTTOTAL > 28
	__INIT_TIMINT(28)
	#if OS_TIMINTTOTAL > 29
	__INIT_TIMINT(29)
	#if OS_TIMINTTOTAL > 30
	__INIT_TIMINT(30)
	#if OS_TIMINTTOTAL > 31
	__INIT_TIMINT(31)
	#if OS_TIMINTTOTAL > 32
	__INIT_TIMINT(32)
	#if OS_TIMINTTOTAL > 33
	__INIT_TIMINT(33)
	#if OS_TIMINTTOTAL > 34
	__INIT_TIMINT(34)
	#if OS_TIMINTTOTAL > 35
	__INIT_TIMINT(35)
	#if OS_TIMINTTOTAL > 36
	__INIT_TIMINT(36)
	#if OS_TIMINTTOTAL > 37
	__INIT_TIMINT(37)
	#if OS_TIMINTTOTAL > 38
	__INIT_TIMINT(38)
	#if OS_TIMINTTOTAL > 39
	__INIT_TIMINT(39)
	#if OS_TIMINTTOTAL > 40
	__INIT_TIMINT(40)
	#if OS_TIMINTTOTAL > 41
	__INIT_TIMINT(41)
	#if OS_TIMINTTOTAL > 42
	__INIT_TIMINT(42)
	#if OS_TIMINTTOTAL > 43
	__INIT_TIMINT(43)
	#if OS_TIMINTTOTAL > 44
	__INIT_TIMINT(44)
	#if OS_TIMINTTOTAL > 45
	__INIT_TIMINT(45)
	#if OS_TIMINTTOTAL > 46
	__INIT_TIMINT(46)
	#if OS_TIMINTTOTAL > 47
	__INIT_TIMINT(47)
	#if OS_TIMINTTOTAL > 48
	__INIT_TIMINT(48)
	#if OS_TIMINTTOTAL > 49
	__INIT_TIMINT(49)
	#if OS_TIMINTTOTAL > 50
	__INIT_TIMINT(50)
	#if OS_TIMINTTOTAL > 51
	__INIT_TIMINT(51)
	#if OS_TIMINTTOTAL > 52
	__INIT_TIMINT(52)
	#if OS_TIMINTTOTAL > 53
	__INIT_TIMINT(53)
	#if OS_TIMINTTOTAL > 54
	__INIT_TIMINT(54)
	#if OS_TIMINTTOTAL > 55
	__INIT_TIMINT(55)
	#if OS_TIMINTTOTAL > 56
	__INIT_TIMINT(56)
	#if OS_TIMINTTOTAL > 57
	__INIT_TIMINT(57)
	#if OS_TIMINTTOTAL > 58
	__INIT_TIMINT(58)
	#if OS_TIMINTTOTAL > 59
	__INIT_TIMINT(59)
	#if OS_TIMINTTOTAL > 60
	__INIT_TIMINT(60)
	#if OS_TIMINTTOTAL > 61
	__INIT_TIMINT(61)
	#if OS_TIMINTTOTAL > 62
	__INIT_TIMINT(62)
	#if OS_TIMINTTOTAL > 63
	__INIT_TIMINT(63)
	#if OS_TIMINTTOTAL > 64
	__INIT_TIMINT(64)
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	
	#if OS_TIMQRYTOTAL
	for(i = 0; i < OS_TIMQRYTOTAL; i++){
		s_timqry_loader[i] = (s_timqry_t)(SYSCFG_USERTIMQRYINIT - 1);
		s_timqry_reload[i] = (s_timqry_t)(SYSCFG_USERTIMQRYINIT - 1);
	}
	#if OS_TIMQRYTOTAL > 0
	__INIT_TIMQRY(0)
	#if OS_TIMQRYTOTAL > 1
	__INIT_TIMQRY(1)
	#if OS_TIMQRYTOTAL > 2
	__INIT_TIMQRY(2)
	#if OS_TIMQRYTOTAL > 3
	__INIT_TIMQRY(3)
	#if OS_TIMQRYTOTAL > 4
	__INIT_TIMQRY(4)
	#if OS_TIMQRYTOTAL > 5
	__INIT_TIMQRY(5)
	#if OS_TIMQRYTOTAL > 6
	__INIT_TIMQRY(6)
	#if OS_TIMQRYTOTAL > 7
	__INIT_TIMQRY(7)
	#if OS_TIMQRYTOTAL > 8
	__INIT_TIMQRY(8)
	#if OS_TIMQRYTOTAL > 9
	__INIT_TIMQRY(9)
	#if OS_TIMQRYTOTAL > 10
	__INIT_TIMQRY(10)
	#if OS_TIMQRYTOTAL > 11
	__INIT_TIMQRY(11)
	#if OS_TIMQRYTOTAL > 12
	__INIT_TIMQRY(12)
	#if OS_TIMQRYTOTAL > 13
	__INIT_TIMQRY(13)
	#if OS_TIMQRYTOTAL > 14
	__INIT_TIMQRY(14)
	#if OS_TIMQRYTOTAL > 15
	__INIT_TIMQRY(15)
	#if OS_TIMQRYTOTAL > 16
	__INIT_TIMQRY(16)
	#if OS_TIMQRYTOTAL > 17
	__INIT_TIMQRY(17)
	#if OS_TIMQRYTOTAL > 18
	__INIT_TIMQRY(18)
	#if OS_TIMQRYTOTAL > 19
	__INIT_TIMQRY(19)
	#if OS_TIMQRYTOTAL > 20
	__INIT_TIMQRY(20)
	#if OS_TIMQRYTOTAL > 21
	__INIT_TIMQRY(21)
	#if OS_TIMQRYTOTAL > 22
	__INIT_TIMQRY(22)
	#if OS_TIMQRYTOTAL > 23
	__INIT_TIMQRY(23)
	#if OS_TIMQRYTOTAL > 24
	__INIT_TIMQRY(24)
	#if OS_TIMQRYTOTAL > 25
	__INIT_TIMQRY(25)
	#if OS_TIMQRYTOTAL > 26
	__INIT_TIMQRY(26)
	#if OS_TIMQRYTOTAL > 27
	__INIT_TIMQRY(27)
	#if OS_TIMQRYTOTAL > 28
	__INIT_TIMQRY(28)
	#if OS_TIMQRYTOTAL > 29
	__INIT_TIMQRY(29)
	#if OS_TIMQRYTOTAL > 30
	__INIT_TIMQRY(30)
	#if OS_TIMQRYTOTAL > 31
	__INIT_TIMQRY(31)
	#if OS_TIMQRYTOTAL > 32
	__INIT_TIMQRY(32)
	#if OS_TIMQRYTOTAL > 33
	__INIT_TIMQRY(33)
	#if OS_TIMQRYTOTAL > 34
	__INIT_TIMQRY(34)
	#if OS_TIMQRYTOTAL > 35
	__INIT_TIMQRY(35)
	#if OS_TIMQRYTOTAL > 36
	__INIT_TIMQRY(36)
	#if OS_TIMQRYTOTAL > 37
	__INIT_TIMQRY(37)
	#if OS_TIMQRYTOTAL > 38
	__INIT_TIMQRY(38)
	#if OS_TIMQRYTOTAL > 39
	__INIT_TIMQRY(39)
	#if OS_TIMQRYTOTAL > 40
	__INIT_TIMQRY(40)
	#if OS_TIMQRYTOTAL > 41
	__INIT_TIMQRY(41)
	#if OS_TIMQRYTOTAL > 42
	__INIT_TIMQRY(42)
	#if OS_TIMQRYTOTAL > 43
	__INIT_TIMQRY(43)
	#if OS_TIMQRYTOTAL > 44
	__INIT_TIMQRY(44)
	#if OS_TIMQRYTOTAL > 45
	__INIT_TIMQRY(45)
	#if OS_TIMQRYTOTAL > 46
	__INIT_TIMQRY(46)
	#if OS_TIMQRYTOTAL > 47
	__INIT_TIMQRY(47)
	#if OS_TIMQRYTOTAL > 48
	__INIT_TIMQRY(48)
	#if OS_TIMQRYTOTAL > 49
	__INIT_TIMQRY(49)
	#if OS_TIMQRYTOTAL > 50
	__INIT_TIMQRY(50)
	#if OS_TIMQRYTOTAL > 51
	__INIT_TIMQRY(51)
	#if OS_TIMQRYTOTAL > 52
	__INIT_TIMQRY(52)
	#if OS_TIMQRYTOTAL > 53
	__INIT_TIMQRY(53)
	#if OS_TIMQRYTOTAL > 54
	__INIT_TIMQRY(54)
	#if OS_TIMQRYTOTAL > 55
	__INIT_TIMQRY(55)
	#if OS_TIMQRYTOTAL > 56
	__INIT_TIMQRY(56)
	#if OS_TIMQRYTOTAL > 57
	__INIT_TIMQRY(57)
	#if OS_TIMQRYTOTAL > 58
	__INIT_TIMQRY(58)
	#if OS_TIMQRYTOTAL > 59
	__INIT_TIMQRY(59)
	#if OS_TIMQRYTOTAL > 60
	__INIT_TIMQRY(60)
	#if OS_TIMQRYTOTAL > 61
	__INIT_TIMQRY(61)
	#if OS_TIMQRYTOTAL > 62
	__INIT_TIMQRY(62)
	#if OS_TIMQRYTOTAL > 63
	__INIT_TIMQRY(63)
	#if OS_TIMQRYTOTAL > 64
	__INIT_TIMQRY(64)
	#if OS_TIMQRYTOTAL > 65
	__INIT_TIMQRY(65)
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#if SYSCFG_DEBUGGING == __ENABLED__
	#if SYSCFG_USERTIMQRYINIT - 1
	  s_timqry_loader[OS_TMID_DEBUGHOOK]
	= s_timqry_reload[OS_TMID_DEBUGHOOK]
	= 0;
	#endif
	  s_timqry_loader[OS_TMID_TASKMGR]
	= s_timqry_reload[OS_TMID_TASKMGR]
	= (1000UL * SYSCFG_TASKMGRUPSPEED) / SYSCFG_SYSTICKCYCLE;
	#endif
	#endif
}

#endif
