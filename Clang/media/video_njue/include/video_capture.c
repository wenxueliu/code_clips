/*
 * video_capture.c
 *
 *  Created on: Mar 20, 2013
 *      Author: yang
 *  Modify by : Dec 15, 2014
 *      Author: wenxueliu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#include "sdf.h"
#include "video_capture.h"

//open camera
void open_camera(struct camera *cam)
{
	struct stat st;

	//query the file is exist
	if (-1 == stat(cam->device_name, &st))
	{
        printf("Cann't identify %s : %s", cam->device_name, strerror(errno));
		exit(EXIT_FAILURE);
	}

	//test the file is a character device
	if (!S_ISCHR(st.st_mode))
	{
		printf("%s is not a device file\n", cam->device_name);
		exit(EXIT_FAILURE);
	}

	//open the camera device
	cam->camera_fd = open(cam->device_name, O_RDWR, 0);
	//test whether open successfully or not
	if (-1 == cam->camera_fd)
	{
        printf("open %s error : %s", cam->device_name, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

//close camera
void close_camera(struct camera *cam)
{
	//close the camera device and test the result
	if (-1 == close(cam->camera_fd))
	{
		printf("close device failure: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}


//initialize the camera device
int init_camera(struct camera *cam)
{
	struct v4l2_capability 	*cap = &(cam->v4l2_cap);
	struct v4l2_format 		*fmt = &(cam->v4l2_fmt);
	struct v4l2_crop 		*crop = &(cam->crop);
	unsigned int min;

	//get the video device capability
	if (-1 == ioctl(cam->camera_fd, VIDIOC_QUERYCAP, cap))
	{
	    printf("query cap failure: %s\n", strerror(errno));
        return 1;
	}

	//judge the device whether is a video capture device or not
	if (!(cap->capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{
		printf("%s is not a video capture device\n", cam->device_name);
        return 2;
	}

	//test the device support video streaming or not
	if (!(cap->capabilities & V4L2_CAP_STREAMING))
	{
		printf("%s is not support streaming\n", cam->device_name);
        return 3;
	}

#ifdef DEBUG_CAM
	printf("\nVIDOOC_QUERYCAP\n");
	printf("the camera driver is %s\n", cap->driver);
	printf("the camera card is %s\n", cap->card);
	printf("the camera bus info is %s\n", cap->bus_info);
	printf("the version is %d\n", cap->version);
#endif

	ZERO(*fmt);

	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt->fmt.pix.width = cam->width;
	fmt->fmt.pix.height = cam->height;
	fmt->fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt->fmt.pix.field = V4L2_FIELD_INTERLACED;

	//set the video format
	if (-1 == ioctl(cam->camera_fd, VIDIOC_S_FMT, fmt))
	{
		printf("set video format error: %s\n", strerror(errno));
        return 4;
	}

	//set image size
	cam->image_size = fmt->fmt.pix.width * fmt->fmt.pix.height * 2;

	min = fmt->fmt.pix.width * 2;
	if (fmt->fmt.pix.bytesperline < min)
	{
		fmt->fmt.pix.bytesperline = min;
	}

	min = fmt->fmt.pix.bytesperline * fmt->fmt.pix.height;
	if(fmt->fmt.pix.sizeimage < min)
	{
		fmt->fmt.pix.sizeimage = min;
	}

    cam->bytesperrow = fmt->fmt.pix.bytesperline;

	//initialize the memory map from kernel space to user space
	return alloc_mem_for_cam(cam);
}

int alloc_mem_for_camera(struct camera *cam)
{
    return init_mmap(cam);
}

//map the driver memory from kernel space to user space
int init_mmap(struct camera *cam)
{
	struct v4l2_requestbuffers req;
	int 	i = 0;

	ZERO(req);

	req.count = FPS;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	//get capture memory
	if (-1 == ioctl(cam->camera_fd, VIDIOC_REQBUFS, &req))
	{
		printf("get memory failure: %s\n", strerror(errno));
        return 1;
	}

	if (req.count < 2)
	{
		printf("FPS too low \n");
        return 1;
	}

	cam->buffers = calloc(req.count, sizeof(*(cam->buffers)));

	if (cam->buffers == NULL)
	{
		printf("calloc memory error %s\n", strerror(errno));
        return 1;
	}

	for (i = 0; i < req.count; ++i)
	{
		struct v4l2_buffer buf;

		ZERO(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == ioctl(cam->camera_fd, VIDIOC_QUERYBUF, &buf))
		{
			printf("ioctl request VIDIOC_QUERYBUF :%s \n", strerror(errno));
            free(cam->buffers);
            return 1;
		}

		cam->buffers[i].length = buf.length;
		cam->buffers[i].start = mmap(NULL,
				buf.length, PROT_READ|PROT_WRITE,
				MAP_SHARED, cam->camera_fd, buf.m.offset);

		//test memory map is useful
		if (MAP_FAILED == cam->buffers[i].start)
		{
			printf("map memory failure: %s\n", strerror(errno));
            return 1;
		}
	}
    return 0;
}

void free_mem_for_cammera(struct camera *cam, size_t len)
{
	int i = 0;

	for (i = 0; i < len; ++i)
	{
		if (-1 == munmap(cam->buffers[i].start, cam->buffers[i].length))
		{
			printf("free the mapped memory error: %s\n", strerror(errno));
		}
	}

	free(cam->buffers);
	cam->buffers = NULL;
}

//free the camera
void uninit_camera(struct camera *cam)
{
    free_mem_for_cammera(cam, FPS);
}


//set and get video capture FPS
void set_capture_fps(struct camera *cam, int *fps)
{
	struct v4l2_streamparm setfps={0};

	setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	setfps.parm.capture.timeperframe.numerator = 1;
	setfps.parm.capture.timeperframe.denominator = *fps;

	if (-1 == ioctl(cam->camera_fd, VIDIOC_S_PARM, &setfps))
	{
		printf("ioctl request VIDIOC_S_PARM error: %s\n", strerror(errno));
		return;
	}

	*fps = (int)setfps.parm.capture.timeperframe.denominator;

	printf("now the video FPS is %d\n", *fps);
}


//start capture the picture from camera
int start_capturing(struct camera *cam)
{
	int i = 0;
	enum v4l2_buf_type type;

	for (i = 0; i < FPS; ++i)
	{
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == ioctl(cam->camera_fd, VIDIOC_QBUF, &buf))
		{
			printf("ioctl request VIDIOC_QBUF: %s\n", strerror(errno));
            return 1;
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	//start capture the video
	if (-1 == ioctl(cam->camera_fd, VIDIOC_STREAMON, &type))
	{
		printf("ioctl request VIDIOC_STREAMON : %s\n", strerror(errno));
        return 1;
	}
    return 0;
}


//stop capture the picture
void stop_capturing(struct camera *cam)
{
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VBI_CAPTURE;

	//stop capturing picture
	if (-1 == ioctl(cam->camera_fd, VIDIOC_STREAMOFF, &type))
	{
		printf("stop capture the video device error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}



//read a frame from the opened camera device
int read_frame_from_camera(struct camera *cam, uint8 *frame_buf, int *frame_length)
{
	struct v4l2_buffer buf;
	CLEAR(buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == ioctl(cam->camera_fd, VIDIOC_DQBUF, &buf))
	{
		switch(errno)
		{
		case EAGAIN:
			return 0;
		case EIO:
		default:
			printf("read frame from camera error : %s\n", strerror(errno));
			return 1;
		}
	}

	memcpy(frame_buf, cam->buffers[buf.index].start, cam->buffers[buf.index].length);
	*frame_length = cam->buffers[buf.index].length;

	if (-1 == ioctl(cam->camera_fd, VIDIOC_QBUF, &buf))
	{
		printf("read frame frame camera put buf into the frame queue error");
		return BASICERROR;
	}

	return 0;
}


//initialize video driver for linux system
void v4l2_init(struct camera *cam)
{
	int fps=15;

	open_camera(cam);

	//test
	set_capture_fps(cam, &fps);

	init_camera(cam);
	start_capturing(cam);
}


//close the video driver for linux system
void v4l2_close(struct camera *cam)
{
	stop_capturing(cam);
	uninit_camera(cam);
	close_camera(cam);

	free(cam);
	cam = NULL;
}


