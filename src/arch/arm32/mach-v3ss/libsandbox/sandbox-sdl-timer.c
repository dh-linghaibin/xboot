#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// #include <SDL.h>
#include <sandbox.h>

#include <sys/time.h>
#include <unistd.h>

struct timer_callback_data_t {
	void (*cb)(void *);
	void * data;
};

void sandbox_sdl_timer_init(void)
{
}

void sandbox_sdl_timer_exit(void)
{
}

static uint32_t timer_callback(uint32_t interval, void * param)
{
	struct timer_callback_data_t * tcd = (struct timer_callback_data_t *)(param);

	((void (*)(void *))tcd->cb)(tcd->data);
	return 0;
}

void sandbox_sdl_timer_set_next(int delay, void (*cb)(void *), void * data)
{
	static struct timer_callback_data_t tcd;

	tcd.cb = cb;
	tcd.data = data;
	// printf("sandbox_sdl_timer_set_next");
	// SDL_AddTimer(delay, timer_callback, &tcd);
}

uint64_t sandbox_get_time_counter(void)
{
	struct timeval tv;
    gettimeofday(&tv,NULL);
	return tv.tv_sec*1000 + tv.tv_usec/1000;//SDL_GetPerformanceCounter();
}

uint64_t sandbox_get_time_frequency(void)
{
	return 1000;//SDL_GetPerformanceFrequency();
}
