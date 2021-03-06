#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "ispvm.h"
#include "gpiolib-fast.h"

volatile uint32_t *mx6gpio;

#define TDI_BANK 	MX6_GPIO_BANK7
#define TDI			1<<12
#define TCK_BANK	MX6_GPIO_BANK7
#define TCK			1<<11
#define TMS_BANK	MX6_GPIO_BANK1
#define TMS			1<<5
#define TDO_BANK	MX6_GPIO_BANK5
#define TDO			1<<19

void init_ts7990(void)
{
	mx6gpio = gpiofast_init();
	assert(mx6gpio != 0);

	mx6gpio[(TCK_BANK + GPGDIR)/4] |= TCK;
	mx6gpio[(TDI_BANK + GPGDIR)/4] |= TDI;
	mx6gpio[(TMS_BANK + GPGDIR)/4] |= TMS;
	mx6gpio[(TDO_BANK + GPGDIR)/4] &= ~(TDO);

	mx6gpio[(TCK_BANK + GPDR)/4] |= TCK;
	mx6gpio[(TDI_BANK + GPDR)/4] |= TDI;
	mx6gpio[(TMS_BANK + GPDR)/4] |= TMS;
}

void restore_ts7990(void)
{
	mx6gpio[(TDI_BANK + GPGDIR)/4] &= ~(TDI);
	mx6gpio[(TCK_BANK + GPGDIR)/4] &= ~(TCK);
	mx6gpio[(TMS_BANK + GPGDIR)/4] &= ~(TMS);
}

int readport_ts7990(void)
{
	return (mx6gpio[(TDO_BANK + GPPSR)/4] & TDO) ? 1 : 0;
}

void writeport_ts7990(int pins, int val)
{
	if(val) {
		if(pins & g_ucPinTDI)
			mx6gpio[(TDI_BANK + GPDR)/4] |= TDI;
		if(pins & g_ucPinTCK)
			mx6gpio[(TCK_BANK + GPDR)/4] |= TCK;
		if(pins & g_ucPinTMS)
			mx6gpio[(TMS_BANK + GPDR)/4] |= TMS;
	} else {
		if(pins & g_ucPinTDI)
			mx6gpio[(TDI_BANK + GPDR)/4] &= ~TDI;
		if(pins & g_ucPinTCK)
			mx6gpio[(TCK_BANK + GPDR)/4] &= ~TCK;
		if(pins & g_ucPinTMS)
			mx6gpio[(TMS_BANK + GPDR)/4] &= ~TMS;
	}
}

void sclock_ts7990()
{
	mx6gpio[(TCK_BANK + GPDR)/4] |= TCK;
	mx6gpio[(TCK_BANK + GPDR)/4] &= ~(TCK);
}
