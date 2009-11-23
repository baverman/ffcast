/* xrectsel.c

Copyright (C) 2009 lolilolicon <lolilolicon@gmail.com>
Code ripped and modified from:
main.c from scrot Copyright (C) 1999,2000 Tom Gilbert.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies of the Software and its documentation and acknowledgment shall be
given in the documentation and software packages that this Software was
used.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#include<stdio.h>
#include<stdlib.h>
#include<X11/Xlib.h>
#include<X11/cursorfont.h>

int main(void)
{
  int rx = 0, ry = 0, rw = 0, rh = 0;
  int rect_x = 0, rect_y = 0, rect_w = 0, rect_h = 0;
  int btn_pressed = 0, done = 0;

  XEvent ev;
  Display *disp = XOpenDisplay(NULL);

  if(!disp)
    return EXIT_FAILURE;

  Screen *scr = NULL;
  scr = ScreenOfDisplay(disp, DefaultScreen(disp));

  Window root = 0;
  root = RootWindow(disp, XScreenNumberOfScreen(scr));

  Cursor cursor, cursor2;
  cursor = XCreateFontCursor(disp, XC_left_ptr);
  cursor2 = XCreateFontCursor(disp, XC_lr_angle);

  XGCValues gcval;
  gcval.foreground = XWhitePixel(disp, 0);
  gcval.function = GXxor;
  gcval.background = XBlackPixel(disp, 0);
  gcval.plane_mask = gcval.background ^ gcval.foreground;
  gcval.subwindow_mode = IncludeInferiors;

  GC gc;
  gc = XCreateGC(disp, root,
                 GCFunction | GCForeground | GCBackground | GCSubwindowMode,
                 &gcval);

  /* this XGrab* stuff makes XPending true ? */
  if ((XGrabPointer
       (disp, root, False,
        ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
        GrabModeAsync, root, cursor, CurrentTime) != GrabSuccess))
    printf("couldn't grab pointer:");

  if ((XGrabKeyboard
       (disp, root, False, GrabModeAsync, GrabModeAsync,
        CurrentTime) != GrabSuccess))
    printf("couldn't grab keyboard:");

  while (!done) {
    while (!done && XPending(disp)) {
      XNextEvent(disp, &ev);
      switch (ev.type) {
        case MotionNotify:
        /* this case is purely for drawing rect on screen */
          if (btn_pressed) {
            if (rect_w) {
              /* re-draw the last rect to clear it */
              XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
            } else {
              /* Change the cursor to show we're selecting a region */
              XChangeActivePointerGrab(disp,
                                       ButtonMotionMask | ButtonReleaseMask,
                                       cursor2, CurrentTime);
            }
            rect_x = rx;
            rect_y = ry;
            rect_w = ev.xmotion.x - rect_x;
            rect_h = ev.xmotion.y - rect_y;

            if (rect_w < 0) {
              rect_x += rect_w;
              rect_w = 0 - rect_w;
            }
            if (rect_h < 0) {
              rect_y += rect_h;
              rect_h = 0 - rect_h;
            }
            /* draw rectangle */
            XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
            XFlush(disp);
          }
          break;
        case ButtonPress:
          btn_pressed = 1;
          rx = ev.xbutton.x;
          ry = ev.xbutton.y;
          break;
        case ButtonRelease:
          done = 1;
          break;
      }
    }
  }
  /* clear the drawn rectangle */
  if (rect_w) {
    XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
    XFlush(disp);
  }
  rw = ev.xbutton.x - rx;
  rh = ev.xbutton.y - ry;
  /* cursor moves backwards */
  if (rw < 0) {
    rx += rw;
    rw = 0 - rw;
  }
  if (rh < 0) {
    ry += rh;
    rh = 0 - rh;
  }
/*
  printf("rx %d\n", rx);
  printf("ry %d\n", ry);
  printf("rw %d\n", rw);
  printf("rh %d\n", rh);
*/
  XCloseDisplay(disp);

  printf("%dx%d+%d+%d\n",rw,rh,rx,ry);

  return EXIT_SUCCESS;
}

