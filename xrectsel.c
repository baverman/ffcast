/* xrectsel.c

Copyright (C) 2009 lolilolicon <lolilolicon@gmail.com>
License: GPL v3

Description: you drag a rectangle area on screen with mouse,
             and it prints geometry in the format of wxh+x+y
Compile: gcc -Wall -Wextra -o xrectsel xrectsel.c -lX11

Thanks to:
HashBox, see:
  http://bbs.archlinux.org/viewtopic.php?id=85378
main.c from scrot:
  the early version was ripped and modified from:
  scrot_sel_and_grab_image(void)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

static char self[] = "xrectsel";

int main(void)
{
  Display *dpy;
  XEvent ev;

  GC sel_gc;
  XGCValues sel_gv;

  Window rootwin;

  int done = 0, btn_pressed = 0;
  int x, y;
  int start_x, start_y, width, height;

  dpy = XOpenDisplay(NULL);
  if (!dpy) {
    fprintf(stderr, "%s: Could not open display %s", self, getenv("DISPLAY"));
  }

  Cursor cursor;
  cursor = XCreateFontCursor(dpy, XC_left_ptr);

  rootwin = DefaultRootWindow(dpy);

  XGrabButton(dpy, 1, 0, rootwin, True, ButtonPressMask,
              GrabModeSync, GrabModeAsync, None, cursor);

  /* Grab pointer for these events */
  XGrabPointer(dpy, rootwin, True, PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
               GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime);
  /* Grab keyboard */
  XGrabKeyboard(dpy, rootwin, False, GrabModeAsync, GrabModeAsync, CurrentTime);

  sel_gv.function = GXinvert;
  sel_gv.subwindow_mode = IncludeInferiors;
  sel_gv.line_width = 1;
  sel_gc = XCreateGC(dpy, rootwin, GCFunction | GCSubwindowMode | GCLineWidth, &sel_gv);

  for (;;) {
    XNextEvent(dpy, &ev);
    switch (ev.type) {
      case ButtonPress:
        btn_pressed = 1;
        x = start_x = ev.xbutton.x_root;
        y = start_y = ev.xbutton.y_root;
        width = height = 0;
        break;
      case MotionNotify:
        /* Draw only if button is pressed */
        if (btn_pressed) {
          /* Re-draw last Rectangle to clear it */
          XDrawRectangle(dpy, rootwin, sel_gc, x, y, width, height);

          width = ev.xbutton.x_root - start_x;
          height = ev.xbutton.y_root - start_y;

          /* Cursor moves backwards, (x,y) always is the northwest pole */
          if (width < 0) {
              width = 0 - width;
              x = start_x - width;
          } else {
              x = start_x;
          }
          if (height < 0) {
              height = 0 - height;
              y = start_y - height;
          } else {
              y = start_y;
          }
          /* Draw Rectangle */
          XDrawRectangle(dpy, rootwin, sel_gc, x, y, width, height);
          XFlush(dpy);
        }
        break;
      case ButtonRelease:
        done = 1;
        break;
      case KeyPress:
        fprintf(stderr, "%s: Keyboard pressed, aborting\n", self);
        done = 2;
        break;
      case KeyRelease:
        break;
      default:
        break;
    }
    if (done)
      break;
  }

  /* Re-draw last Rectangle to clear it */
  XDrawRectangle(dpy, rootwin, sel_gc, x, y, width, height);
  XFlush(dpy);

  XUngrabButton(dpy, 1, 0, rootwin);
  XUngrabPointer(dpy, CurrentTime);
  XUngrabKeyboard(dpy, CurrentTime);
  XFreeCursor(dpy, cursor);
  XFreeGC(dpy, sel_gc);
  XSync(dpy, 1);

  XCloseDisplay(dpy);

  if (done < 2) {
    printf("%ix%i+%i+%i\n", width, height, x, y);
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}
