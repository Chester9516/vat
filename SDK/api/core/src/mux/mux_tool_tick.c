#include <mux/mux_define.h>



#if 0

uint32_t mux_clock_get_eclipse(Pmux_clock_tick pclk0, Pmux_clock_tick pclk1)
{
	if(pclk1->_90KHz < pclk0->_90KHz)
	{
		uint32_t tick = (((uint64_t)0x1FFFFFFFF) - pclk0->_90KHz) + pclk1->_90KHz;
		tick *= 300;
		tick += pclk1->_27MHz + (300 - pclk0->_27MHz);
		return (uint32_t)tick;
	}else if(pclk0->_90KHz == pclk1->_90KHz)
	{
		int32_t tick = pclk1->_27MHz - pclk0->_27MHz;
		if(tick < 0)return 0;
		return tick;
	}else
	{
		uint32_t tick = ((pclk1->_90KHz - pclk0->_90KHz) * 300);
		tick +=  (int32_t)(pclk1->_27MHz - pclk0->_27MHz);
		return tick;
	}
}

#endif
