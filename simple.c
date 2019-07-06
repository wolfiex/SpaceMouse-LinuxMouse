/*
This script is free to use for non profit applicaitons. It is an adaption of the simple test from spacenav to add mouse functionality. 
If it is used the current header must be included, unchanged and reference given to Daniel Ellis - wolfiex (github) or @DanEllisScience (twitter)


Author : Dan Ellis
/*

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <spnav.h>
#include <X11/extensions/XTest.h>//button

#include <unistd.h>//sleep

void sig(int s)
{
	spnav_close();
	exit(0);
}

int main(void)
{

#if defined(BUILD_X11)
	Display *dpy;
	Window win;
	unsigned long bpix;
#endif

	spnav_event sev;

	signal(SIGINT, sig);

#if defined(BUILD_X11)

	if(!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "failed to connect to the X server\n");
		return 1;
	}
	bpix = BlackPixel(dpy, DefaultScreen(dpy));
	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 1, 1, 0, bpix, bpix);

	/* This actually registers our window with the driver for receiving
	 * motion/button events through the 3dxsrv-compatible X11 protocol.
	 */
	if(spnav_x11_open(dpy, win) == -1) {
		fprintf(stderr, "failed to connect to the space navigator daemon\n");
		return 1;
	}

#elif defined(BUILD_AF_UNIX)
	if(spnav_open()==-1) {
	  	fprintf(stderr, "failed to connect to the space navigator daemon\n");
		return 1;
	}
#else
#error Unknown build type!
#endif


			Display *dpymove;
			Window root_window;
			Window blah;
			int win_x;
			int win_y;
			int div = 10;
			int scroll = 100;
			int click = -170;
			int rt,dir;
			unsigned int mask; //<--three
			dpymove = XOpenDisplay(0);
			root_window = XRootWindow(dpymove, 0);
			XSelectInput(dpymove, root_window, KeyReleaseMask);




//x11 windows causes errors - temrinal atom


	/* spnav_wait_event() and spnav_poll_event(), will silently ignore any non-spnav X11 events.
	 *
	 * If you need to handle other X11 events you will have to use a regular XNextEvent() loop,
	 * and pass any ClientMessage events to spnav_x11_event, which will return the event type or
	 * zero if it's not an spnav event (see spnav.h).
	 */
	while(spnav_wait_event(&sev)) {
		if(sev.type == SPNAV_EVENT_MOTION) {
			printf("got motion event: t(%d, %d, %d) ", sev.motion.x/div, sev.motion.y/div, sev.motion.z);
			printf("r(%d, %d, %d)\n", sev.motion.rx, sev.motion.ry, sev.motion.rz);
//XWarpPointer(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x, dest_y)

//////// IF ROTATE
rt = abs(sev.motion.ry);
printf("rt %d",rt);
if (rt/scroll){
						dir = (sev.motion.ry<0)? 5 : 4 ;
						printf("dir %d",dir);
						//for (int i = 1; i <= rt; i++)
			    //{
						XTestFakeButtonEvent(dpymove, dir, True, CurrentTime);
						XTestFakeButtonEvent(dpymove, dir, False, CurrentTime);

					//};

					usleep((355-rt)*3000);
					//XSync(dpymove,True);
					spnav_remove_events(SPNAV_EVENT_MOTION);

	}
else if (sev.motion.y < click)
{
	printf("click %d %d",sev.motion.y,click);
	printf("dir %d",dir);
	//for (int i = 1; i <= rt; i++)
		//{
	XTestFakeButtonEvent(dpymove, 1, True, CurrentTime);
	XTestFakeButtonEvent(dpymove, 1, False, CurrentTime);

//};

usleep(200000);
//XSync(dpymove,True);
spnav_remove_events(SPNAV_EVENT_MOTION);
}

else {

XQueryPointer(dpymove, DefaultRootWindow(dpymove), &root_window, &blah, &win_x, &win_y, &win_x, &win_y, &mask); //<--four

printf("Mouse coordinates (X: %d, Y: %d)\n", win_x, win_y);

			XWarpPointer(dpymove, None, root_window,None,None,None,None,win_x + sev.motion.x/div, win_y - sev.motion.z/div);


}
//install libxtst-dev
/*
1 = left button
2 = middle button (pressing the scroll wheel)
3 = right button
4 = turn scroll wheel up
5 = turn scroll wheel down
6 = push scroll wheel left
7 = push scroll wheel right
8 = 4th button (aka browser backward button)
9 = 5th button (aka browser forward button)
*/



			XFlush(dpymove); // Flushes the output buffer, therefore updates the cursor's position. Thanks to Achernar.


		} else {	/* SPNAV_EVENT_BUTTON */
			printf("got button %s event b(%d)\n", sev.button.press ? "press" : "release", sev.button.bnum);

			XTestFakeButtonEvent(dpymove, sev.button.bnum, sev.button.press, 0);

		}
	}

	spnav_close();
	return 0;
}
//https://stackoverflow.com/questions/2433447/how-to-set-mouse-cursor-position-in-c-on-linux
