#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sandbox.h>

#include <string.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/input.h>

typedef struct _fb_info_t {
  int fd;
  void* bits;
  struct fb_fix_screeninfo fix;
  struct fb_var_screeninfo var;
} fb_info_t;

static int fb_open(fb_info_t* fb, const char* filename) {
  uint32_t size = 0;
  fb->fd = open(filename, O_RDWR);
  if (fb->fd < 0) {
    return -1;
  }

  if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->fix) < 0) goto fail;
  if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->var) < 0) goto fail;

  fb->var.xoffset = 0;
  fb->var.yoffset = 0;
  ioctl(fb->fd, FBIOPAN_DISPLAY, &(fb->var));

  size = fb_size(fb);
#ifdef FTK_FB_NOMMAP
  // uclinux doesn't support MAP_SHARED or MAP_PRIVATE with PROT_WRITE, so no mmap at all is simpler
  fb->bits = fb->fix.smem_start;
#else
  fb->bits = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);
#endif

  if (fb->bits == MAP_FAILED) {
    goto fail;
  }

  memset(fb->bits, 0xff, size);
  return 0;
fail:
  close(fb->fd);

  return -1;
}


static void fb_close(fb_info_t* fb) {
  if (fb != NULL) {
    munmap(fb->bits, fb_size(fb));
    close(fb->fd);
  }
  return;
}

static void fb_sync(fb_info_t* info) {
  int ret = 0;
  int zero = 0;
  ret = ioctl(info->fd, FBIO_WAITFORVSYNC, &zero);
//   log_debug("FBIO_WAITFORVSYNC: %d %d\n", ret, zero);
  return;
}

#define IMAGE_W 800
#define IMAGE_H 480
 
struct sandbox_fb_t {
	int fb_id;
	int width;
	int height;
	unsigned long buffersize;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

	unsigned long int *mmap_start;
};

void * sandbox_sdl_fb_init(const char * title, int width, int height, int fullscreen)
{
	struct sandbox_fb_t * hdl;

	hdl = malloc(sizeof(struct sandbox_fb_t));
	if(!hdl)
	return NULL;

	hdl->fb_id = open("/dev/fb0", O_RDWR);

	if (hdl->fb_id < 0)  
	{  
		printf("Error : Can not open framebuffer device/n");  
		exit(1);
	}  

	if ( ioctl(hdl->fb_id,FBIOGET_FSCREENINFO,&(hdl->finfo)) )  
	{  
		printf("Error reading fixed information/n");  
		exit(2);  
	}  

	if ( ioctl(hdl->fb_id,FBIOGET_VSCREENINFO,&(hdl->vinfo)) )
	{  
		printf("Error reading variable information/n");  
		exit(3);  
	}
	hdl->buffersize = hdl->vinfo.xres * hdl->vinfo.yres * hdl->vinfo.bits_per_pixel / 8;  
	/*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/  
	hdl->mmap_start =(char *) mmap (0, hdl->buffersize, PROT_READ | PROT_WRITE, MAP_SHARED, hdl->fb_id,0);  
	if ((int) hdl->mmap_start == -1)  
	{  
		printf ("Error: failed to map framebuffer device to memory./n");  
		exit (4);  
	}
	return hdl;
}

void sandbox_sdl_fb_exit(void * handle)
{
	struct sandbox_fb_t * hdl = (struct sandbox_fb_t *)handle;
	munmap (hdl->mmap_start, hdl->buffersize); /*解除映射*/  
	close (hdl->fb_id);
	free(hdl);
}

int sandbox_sdl_fb_get_width(void * handle)
{
	return 800;
}

int sandbox_sdl_fb_get_height(void * handle)
{
	return 480;
}

int sandbox_sdl_fb_surface_create(void * handle, struct sandbox_fb_surface_t * surface)
{
	struct sandbox_fb_t * hdl = (struct sandbox_fb_t *)handle;
	surface->width = 800;
	surface->height = 480;
	surface->pitch = hdl->vinfo.bits_per_pixel;
	surface->pixels = malloc(sizeof(uint32_t)*hdl->buffersize);
	return 0;
}

int sandbox_sdl_fb_surface_destroy(void * handle, struct sandbox_fb_surface_t * surface)
{
	struct sandbox_fb_t * hdl = (struct sandbox_fb_t *)handle;
	free(surface->pixels);
	return 0;
}

int sandbox_sdl_fb_surface_present(void * handle, struct sandbox_fb_surface_t * surface)
{
	struct sandbox_fb_t * hdl = (struct sandbox_fb_t *)handle;

	unsigned long int *fbp,*pix;
	char * cur_pix;
	char * cur_pix2;

	int i,j;

	fbp=hdl->mmap_start;
	pix = surface->pixels;
	for(i=0;i<IMAGE_H;i++){
		for(j=0;j<IMAGE_W;j++){
			cur_pix2 = (char *)pix;
			cur_pix=(char *)fbp;
			cur_pix[0]=cur_pix2[0];
			cur_pix[1]=cur_pix2[1];
			cur_pix[2]=cur_pix2[2];
			cur_pix[3]=cur_pix2[3];
			fbp++;
			pix++;
		}
	}
	
	return 0;
}

void sandbox_sdl_fb_set_backlight(void * handle, int brightness)
{
	struct sandbox_fb_t * hdl = (struct sandbox_fb_t *)handle;
	// SDL_SetWindowBrightness(hdl->window, brightness / 1023.0);
}

int sandbox_sdl_fb_get_backlight(void * handle)
{
	// struct sandbox_fb_t * hdl = (struct sandbox_fb_t *)handle;
	// return (int)(SDL_GetWindowBrightness(hdl->window) * 1023);
	return 100;
}
