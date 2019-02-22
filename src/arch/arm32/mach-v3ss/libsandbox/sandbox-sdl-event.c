#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <pthread.h>
#include <sandbox.h>

struct event_callback_t {
	struct {
		void * device;
		void (*down)(void * device, unsigned int key);
		void (*up)(void * device, unsigned int key);
	} key;

	struct {
		void * device;
		void (*down)(void * device, int x, int y, unsigned int button);
		void (*move)(void * device, int x, int y);
		void (*up)(void * device, int x, int y, unsigned int button);
		void (*wheel)(void * device, int dx, int dy);
	} mouse;

	struct {
		void * device;
		void (*begin)(void * device, int x, int y, unsigned int id);
		void (*move)(void * device, int x, int y, unsigned int id);
		void (*end)(void * device, int x, int y, unsigned int id);
	} touch;

	struct {
		void * device;
		void (*left_stick)(void * device, int x, int y);
		void (*right_stick)(void * device, int x, int y);
		void (*left_trigger)(void * device, int v);
		void (*right_trigger)(void * device, int v);
		void (*button_down)(void * device, unsigned int button);
		void (*button_up)(void * device, unsigned int button);
	} joystick;
};
static struct event_callback_t __event_callback = { 0 };

static int handle_event(void * data)
{
	// struct event_callback_t * cb = (struct event_callback_t *)(data);
	// SDL_GameController * gc = NULL;
	// SDL_Event e;
	// int x, y, v;
	// unsigned int button;

	while(0)
	{
		// if(SDL_WaitEvent(&e))
		// {
	    //     switch(e.type)
	    //     {
	    //     case SDL_KEYDOWN:
	    //     	if(cb->key.down)
	    //     		cb->key.down(cb->key.device, keycode_map(e.key.keysym.sym));
	    //     	break;

	    //     case SDL_KEYUP:
	    //     	if(cb->key.up)
	    //     		cb->key.up(cb->key.device, keycode_map(e.key.keysym.sym));
	    //     	break;

	    //     case SDL_MOUSEBUTTONDOWN:
	    //     	switch(e.button.button)
	    //     	{
		// 		case SDL_BUTTON_LEFT:
		// 			button = 0x01;
		// 			break;
		// 		case SDL_BUTTON_MIDDLE:
		// 			button = 0x02;
		// 			break;
		// 		case SDL_BUTTON_RIGHT:
		// 			button = 0x03;
		// 			break;
		// 		case SDL_BUTTON_X1:
		// 			button = 0x04;
		// 			break;
		// 		case SDL_BUTTON_X2:
		// 			button = 0x05;
		// 			break;
		// 		default:
		// 			button = 0x00;
		// 			break;
	    //     	}
		// 		if(cb->mouse.down && (button != 0x00))
		// 			cb->mouse.down(cb->mouse.device, e.button.x, e.button.y, button);
		// 		break;

	    //     case SDL_MOUSEMOTION:
	    //     	if(cb->mouse.move)
	    //     		cb->mouse.move(cb->mouse.device, e.motion.x, e.motion.y);
	    //     	break;

	    //     case SDL_MOUSEBUTTONUP:
	    //     	switch(e.button.button)
	    //     	{
		// 		case SDL_BUTTON_LEFT:
		// 			button = 0x01;
		// 			break;
		// 		case SDL_BUTTON_MIDDLE:
		// 			button = 0x02;
		// 			break;
		// 		case SDL_BUTTON_RIGHT:
		// 			button = 0x03;
		// 			break;
		// 		case SDL_BUTTON_X1:
		// 			button = 0x04;
		// 			break;
		// 		case SDL_BUTTON_X2:
		// 			button = 0x05;
		// 			break;
		// 		default:
		// 			button = 0x00;
		// 			break;
	    //     	}
		// 		if(cb->mouse.up && (button != 0x00))
		// 			cb->mouse.up(cb->mouse.device, e.button.x, e.button.y, button);
		// 		break;

	    //     case SDL_MOUSEWHEEL:
	    //     	if(cb->mouse.wheel)
	    //     		cb->mouse.wheel(cb->mouse.device, e.wheel.x, e.wheel.y);
	    //     	break;
/*
	        case SDL_FINGERDOWN:
				if(cb->touch.begin)
					cb->touch.begin(cb->touch.device, (int)(e.tfinger.x * sandbox_sdl_fb_get_width()), (int)(e.tfinger.y * sandbox_sdl_fb_get_height()), (unsigned int)e.tfinger.fingerId);
	        	break;

	        case SDL_FINGERMOTION:
				if(cb->touch.move)
					cb->touch.move(cb->touch.device, (int)(e.tfinger.x * sandbox_sdl_fb_get_width()), (int)(e.tfinger.y * sandbox_sdl_fb_get_height()), (unsigned int)e.tfinger.fingerId);
	        	break;

	        case SDL_FINGERUP:
				if(cb->touch.end)
					cb->touch.end(cb->touch.device, (int)(e.tfinger.x * sandbox_sdl_fb_get_width()), (int)(e.tfinger.y * sandbox_sdl_fb_get_height()), (unsigned int)e.tfinger.fingerId);
	        	break;
*/
	    //     case SDL_CONTROLLERDEVICEADDED:
	    //     	if(!gc)
	    //     		gc = SDL_GameControllerOpen(e.cdevice.which);
	    //         break;

	    //     case SDL_CONTROLLERDEVICEREMOVED:
	    //     	if(gc)
	    //     	{
	    //     		SDL_GameControllerClose(gc);
	    //     		gc = NULL;
	    //     	}
	    //         break;

	    //     case SDL_CONTROLLERAXISMOTION:
	    //     	switch(e.caxis.axis)
	    //     	{
		// 		case SDL_CONTROLLER_AXIS_LEFTX:
		// 		case SDL_CONTROLLER_AXIS_LEFTY:
		// 			x = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTX);
		// 			y = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTY);
		// 			if(cb->joystick.left_stick)
		// 				cb->joystick.left_stick(cb->joystick.device, x, y);
		// 			break;

		// 		case SDL_CONTROLLER_AXIS_RIGHTX:
		// 		case SDL_CONTROLLER_AXIS_RIGHTY:
		// 			x = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_RIGHTX);
		// 			y = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_RIGHTY);
		// 			if(cb->joystick.right_stick)
		// 				cb->joystick.right_stick(cb->joystick.device, x, y);
		// 			break;

		// 		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		// 			v = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		// 			if(cb->joystick.left_trigger)
		// 				cb->joystick.left_trigger(cb->joystick.device, v);
		// 			break;

		// 		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		// 			v = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		// 			if(cb->joystick.right_trigger)
		// 				cb->joystick.right_trigger(cb->joystick.device, v);
		// 			break;

		// 		default:
		// 			break;
	    //     	}
	    //     	break;

	    //     case SDL_CONTROLLERBUTTONDOWN:
		// 		switch(e.cbutton.button)
		// 		{
		// 		case SDL_CONTROLLER_BUTTON_A:
		// 			button = 0x05;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_B:
		// 			button = 0x06;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_X:
		// 			button = 0x07;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_Y:
		// 			button = 0x08;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_BACK:
		// 			button = 0x09;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_GUIDE:
		// 			button = 0x0b;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_START:
		// 			button = 0x0a;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
		// 			button = 0x0e;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
		// 			button = 0x0f;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		// 			button = 0x0c;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		// 			button = 0x0d;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_DPAD_UP:
		// 			button = 0x01;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		// 			button = 0x02;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		// 			button = 0x03;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		// 			button = 0x04;
		// 			break;
		// 		default:
		// 			button = 0x00;
		// 			break;
		// 		}
		// 		if(cb->joystick.button_down && (button != 0x00))
		// 			cb->joystick.button_down(cb->joystick.device, button);
		// 		break;

	    //     case SDL_CONTROLLERBUTTONUP:
		// 		switch(e.cbutton.button)
		// 		{
		// 		case SDL_CONTROLLER_BUTTON_A:
		// 			button = 0x05;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_B:
		// 			button = 0x06;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_X:
		// 			button = 0x07;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_Y:
		// 			button = 0x08;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_BACK:
		// 			button = 0x09;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_GUIDE:
		// 			button = 0x0b;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_START:
		// 			button = 0x0a;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
		// 			button = 0x0e;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
		// 			button = 0x0f;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		// 			button = 0x0c;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		// 			button = 0x0d;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_DPAD_UP:
		// 			button = 0x01;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		// 			button = 0x02;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		// 			button = 0x03;
		// 			break;
		// 		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		// 			button = 0x04;
		// 			break;
		// 		default:
		// 			button = 0x00;
		// 			break;
		// 		}
		// 		if(cb->joystick.button_up && (button != 0x00))
		// 			cb->joystick.button_up(cb->joystick.device, button);
	    //     	break;

	    //     case SDL_QUIT:
	    //     	sandbox_exit();
	    //     	break;

	    //     default:
	    //     	break;
	    //     }
		// }
	}

	return 0;
}


#define MAX 200

int fd;

pthread_t thread[2];
pthread_mutex_t mut;
int number=0, i;

void *thread1()
{
    // printf ("thread1 : I'm thread 1, my thread id is %lu\n", pthread_self());
    // for (i = 0; i < MAX; i++) {
    //     printf("thread1 : number = %d_______________\n",number);
    //     pthread_mutex_lock(&mut);
    //     number++;
    //     pthread_mutex_unlock(&mut);
    //     usleep(100000);
    // }
    // printf("thread1 :the main function is waiting for me?\n");
	fd_set			rds;
	int			ret;
	struct input_event	event;
	struct timeval		time;
	
	int x, y, v;
	unsigned int button;

	while ( 1 )
	{
		FD_ZERO( &rds );
		FD_SET( fd, &rds );
		/*调用select检查是否能够从/dev/input/event0设备读取数据*/
		ret = select( fd + 1, &rds, NULL, NULL, NULL );
		if ( ret < 0 )
		{
			perror( "select" );
			return(-1);
		}
		/*能够读取到数据*/
		else if ( FD_ISSET( fd, &rds ) )
		{
			static int x,y;
			static uint8_t flag = 0;
			ret	= read( fd, &event, sizeof(struct input_event) );
			// time	= event.time;
			if(event.type == 1) {
			    if(event.code == 53) {
					y = event.value;
				} else if (event.code == 54)
				{
					x = event.value;
					if(flag == 1) {
						flag = 2;
						//__event_callback.touch.begin(__event_callback.touch.device, (int)(x), (int)(y), (unsigned int)0);
						__event_callback.mouse.down(__event_callback.mouse.device, x, y, 1);
					} else if (flag == 2)
					{
						__event_callback.mouse.move(__event_callback.mouse.device, x, y);
						//__event_callback.touch.move(__event_callback.touch.device, (int)(x), (int)(y), (unsigned int)0);
					}
					
				}
				
				if(event.value == 0) {
			        printf("抬起\n");
					flag = 0;
					__event_callback.mouse.up(__event_callback.mouse.device, x, y, 1);
					//__event_callback.touch.end(__event_callback.touch.device, (int)(x), (int)(y), (unsigned int)0);
			    } else {
			        printf("按下\n");
					flag = 1;
			    }
				// __event_callback.touch.begin(__event_callback.touch.device, (int)(e.tfinger.x * sandbox_sdl_fb_get_width()), (int)(e.tfinger.y * sandbox_sdl_fb_get_height()), (unsigned int)e.tfinger.fingerId);
				// __event_callback.touch.move(__event_callback.touch.device, (int)(e.tfinger.x * sandbox_sdl_fb_get_width()), (int)(e.tfinger.y * sandbox_sdl_fb_get_height()), (unsigned int)e.tfinger.fingerId);
				// __event_callback.touch.end(__event_callback.touch.device, (int)(e.tfinger.x * sandbox_sdl_fb_get_width()), (int)(e.tfinger.y * sandbox_sdl_fb_get_height()), (unsigned int)e.tfinger.fingerId);
			}
		}
	}

    pthread_exit(NULL);
}

// void *thread2()
// {
//     printf ("thread2 : I'm thread 2, my thread id is %lu\n", pthread_self());
//     for (i = 0; i < MAX; i++) {
//         printf("thread2 : number = %d\n",number);
//         pthread_mutex_lock(&mut);
//         number++;
//         pthread_mutex_unlock(&mut);
//         usleep(200000);
//     }
//     printf("thread2 :the main function is waiting for me? \n");
//     pthread_exit(NULL);
// }

void thread_create(void)
{
    int temp;
    printf("thread address is %p...%p\n",thread, &thread);
    memset(thread, 0, sizeof(thread));          //initialize

    /*Create thread*/
    if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0)  //return thread id to thread[0]
        printf("Thread 1 fail to create!\n");
    else
        printf("Thread 1 created!\n");
    // if((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0)  //return thread id to thread[1]
    //     printf("Thread 2 fail to create!");
    // else
    //     printf("Thread 2 created!\n");
}

/* to block main() to prevent that main() exits too early
 * so pthread_join(thread[1],NULL); can be commented out without big difference */
void thread_wait(void)
{
    /*Wait thread end*/
    if(thread[0] != 0) {                	//if thread id is effective
        pthread_join(thread[0],NULL);  		//will be blocked here untill the target thread terminates
        printf("Thread 1 completed\n");
    }
    printf("between pthread_join(), if print something, where will it be shown?\n");
    //if(thread[1] != 0) {                	//if thread id is effective
    //    pthread_join(thread[1],NULL);   	//will be blocked here untill the target thread terminates
    //    printf("Thread 2 completed.\n");
    //}
    //printf("after pthread_join(), if print something, where will it be shown?\n");
}

void sandbox_sdl_event_init(void)
{
	fd_set rds;
	int ret;
	struct input_event	event;
	struct timeval time;
	fd = open( "/dev/input/event2", O_RDONLY );
	if ( fd < 0 )
	{
		perror( "/dev/input/event2" );
		return(-1);
	}

	/*init mutex*/
    pthread_mutex_init(&mut,NULL);
    printf("Main function,creating thread...\n");

    thread_create();
    printf("Main function,waiting for thread end....\n");

    //thread_wait();
}

void sandbox_sdl_event_exit(void)
{
	// fclose();
	// if(__event)
	// 	SDL_WaitThread(__event, NULL);
}

void sandbox_sdl_event_set_key_callback(void * device,
		void (*down)(void * device, unsigned int key),
		void (*up)(void * device, unsigned int key))
{
	__event_callback.key.device = device;
	__event_callback.key.down = down;
	__event_callback.key.up = up;
}

void sandbox_sdl_event_set_mouse_callback(void * device,
		void (*down)(void * device, int x, int y, unsigned int button),
		void (*move)(void * device, int x, int y),
		void (*up)(void * device, int x, int y, unsigned int button),
		void (*wheel)(void * device, int dx, int dy))
{
	__event_callback.mouse.device = device;
	__event_callback.mouse.down = down;
	__event_callback.mouse.move = move;
	__event_callback.mouse.up = up;
	__event_callback.mouse.wheel = wheel;
}

void sandbox_sdl_event_set_touch_callback(void * device,
		void (*begin)(void * device, int x, int y, unsigned int id),
		void (*move)(void * device, int x, int y, unsigned int id),
		void (*end)(void * device, int x, int y, unsigned int id))
{
	__event_callback.touch.device = device;
	__event_callback.touch.begin = begin;
	__event_callback.touch.move = move;
	__event_callback.touch.end = end;
}

void sandbox_sdl_event_set_joystick_callback(void * device,
		void (*left_stick)(void * device, int x, int y),
		void (*right_stick)(void * device, int x, int y),
		void (*left_trigger)(void * device, int v),
		void (*right_trigger)(void * device, int v),
		void (*button_down)(void * device, unsigned int button),
		void (*button_up)(void * device, unsigned int button))
{
	__event_callback.joystick.device = device;
	__event_callback.joystick.left_stick = left_stick;
	__event_callback.joystick.right_stick = right_stick;
	__event_callback.joystick.left_trigger = left_trigger;
	__event_callback.joystick.right_trigger = right_trigger;
	__event_callback.joystick.button_down = button_down;
	__event_callback.joystick.button_up = button_up;
}
