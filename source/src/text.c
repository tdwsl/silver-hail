#include <GL/gl.h>
#include "text.h"

enum {
  HI=1,
  LEFT_HI=2,
  LEFT_LO=4,
  MID=8,
  LO=16,
  RIGHT_HI=32,
  RIGHT_LO=64,
  HI_L=128,
  HI_R=256,
  MID_L=512,
  MID_R=1024,
  LO_L=2048,
  LO_R=4096,
  MID_HI=8192,
  MID_LO=16384,
};

unsigned int alphabet[] = {
  HI | LEFT_HI | LEFT_LO | MID | RIGHT_HI | RIGHT_LO,		// a
  LEFT_HI|LEFT_LO|HI_L|MID_L|MID_R|MID_HI|RIGHT_LO|LO,		// b
  LEFT_HI | LEFT_LO | HI | LO,					// c
  LEFT_HI | LEFT_LO | HI_L | LO | RIGHT_LO | MID_HI | MID_R,	// d
  LEFT_HI | LEFT_LO | HI | LO | MID,				// e
  LEFT_HI | LEFT_LO | HI | MID,					// f
  LEFT_HI | LEFT_LO | HI | LO | RIGHT_LO | MID_R,		// g
  LEFT_HI | LEFT_LO | MID | RIGHT_HI | RIGHT_LO,		// h
  MID_HI | MID_LO,						// i
  MID_HI | MID_LO | HI_L | HI_R | LO_L | LEFT_LO,		// j
  LEFT_HI | LEFT_LO | MID_L | MID_R | RIGHT_LO | HI_R | MID_HI,	// k
  LEFT_HI | LEFT_LO | LO,					// l
  LEFT_HI|LEFT_LO|HI_L|HI_R|MID_HI|MID_LO|RIGHT_HI|RIGHT_LO,	// m
  LEFT_HI|LEFT_LO|HI_L|MID_HI|MID_LO|LO_R|RIGHT_HI|RIGHT_LO,	// n
  LEFT_HI | LEFT_LO | HI | LO | RIGHT_HI | RIGHT_LO,		// o
  LEFT_HI | LEFT_LO | HI | MID | RIGHT_HI,			// p
  LEFT_HI | HI | MID | RIGHT_HI | RIGHT_LO,			// q
  LEFT_HI | LEFT_LO | HI_L | MID_L | MID_R | MID_HI | RIGHT_LO,	// r
  LEFT_HI | RIGHT_LO | HI | MID | LO,				// s
  MID_HI | MID_LO | HI_L | HI_R,				// t
  LEFT_HI | LEFT_LO | LO | RIGHT_HI | RIGHT_LO,			// u
  LEFT_HI | LEFT_LO | RIGHT_HI | MID_R | MID_LO | LO_L,		// v
  LEFT_HI|LEFT_LO|LO_L|LO_R|MID_HI|MID_LO|RIGHT_HI|RIGHT_LO,	// w
  LEFT_LO | RIGHT_LO | MID_L | MID_R | MID_HI | RIGHT_HI,	// x
  LEFT_HI | RIGHT_HI | MID_L | MID_R | MID_LO,			// y
  LEFT_LO | RIGHT_HI | HI | MID | LO,				// z
};

unsigned int numbers[] = {
  LEFT_HI | LEFT_LO | HI | LO | RIGHT_HI | RIGHT_LO,		// 0
  RIGHT_HI | RIGHT_LO,						// 1
  HI | RIGHT_HI | MID | LEFT_LO | LO,				// 2
  RIGHT_HI | RIGHT_LO | HI | MID | LO,				// 3
  LEFT_HI | MID | RIGHT_HI | RIGHT_LO,				// 4
  HI | LEFT_HI | MID | RIGHT_LO | LO,				// 5
  HI | LEFT_HI | MID | RIGHT_LO | LO | LEFT_LO,			// 6
  HI | RIGHT_HI | RIGHT_LO,					// 7
  HI | RIGHT_HI | LEFT_HI | MID | LEFT_LO | RIGHT_LO | LO,	// 8
  HI | RIGHT_HI | LEFT_HI | MID | RIGHT_LO,			// 9
};

void drawVbar_p(float x, float y, float w, float h) {
  glVertex2i(x+w/2, y);
  glVertex2i(x+w, y+w/2);
  glVertex2i(x+w, y+h-w/2);
  glVertex2i(x+w/2, y+h);
  glVertex2i(x, y+h-w/2);
  glVertex2i(x, y+w/2);

  glEnd();
}

void drawHbar_p(float x, float y, float w, float h) {
  glVertex2i(x, y+h/2);
  glVertex2i(x+h/2, y);
  glVertex2i(x+w-h/2, y);
  glVertex2i(x+w, y+h/2);
  glVertex2i(x+w-h/2, y+h);
  glVertex2i(x+h/2, y+h);

  /*glVertex2i(x, y+h/2);
  glVertex2i(x+h/2, y+h);
  glVertex2i(x+w-h/2, y+h);
  glVertex2i(x+w, y+h/2);
  glVertex2i(x+w+h/2, y);
  glVertex2i(x+h/2, y);*/

  glEnd();
}

void drawBar(float x, float y, float w, float h, float a) {
  glColor4f(0.7, 0.7, 0.9, 0.4*a);
  glBegin(GL_POLYGON);
  if(w < h)
    drawVbar_p(x, y, w, h);
  else
    drawHbar_p(x, y, w, h);

  glColor4f(0.8, 0.8, 1.0, 1.0*a);
  glBegin(GL_LINE_LOOP);
  if(w < h)
    drawVbar_p(x, y, w, h);
  else
    drawHbar_p(x, y, w, h);
}

void drawChar(char c, float x, float y, float w, float h, float a) {
  unsigned int sym;
  if(c >= 'A' && c <= 'Z')
    sym = alphabet[c-'A'];
  else if(c >= 'a' && c <= 'z')
    sym = alphabet[c-'a'];
  else if(c >= '0' && c <= '9')
    sym = numbers[c-'0'];
  else
    return;

  if(sym & HI)
    drawBar(x+w*0.15, y, w*0.7, h*0.1, a);
  if(sym & MID)
    drawBar(x+w*0.15, y+h*0.45, w*0.7, h*0.1, a);
  if(sym & LO)
    drawBar(x+w*0.15, y+h*0.9, w*0.7, h*0.1, a);

  if(sym & LEFT_HI)
    drawBar(x, y+h*0.1, w*0.2, h*0.4, a);
  if(sym & MID_HI)
    drawBar(x+w*0.4, y+h*0.1, w*0.2, h*0.4, a);
  if(sym & RIGHT_HI)
    drawBar(x+w*0.8, y+h*0.1, w*0.2, h*0.4, a);

  if(sym & LEFT_LO)
    drawBar(x, y+h*0.5, w*0.2, h*0.4, a);
  if(sym & MID_LO)
    drawBar(x+w*0.4, y+h*0.5, w*0.2, h*0.4, a);
  if(sym & RIGHT_LO)
    drawBar(x+w*0.8, y+h*0.5, w*0.2, h*0.4, a);

  if(sym & HI_L)
    drawBar(x+w*0.1, y, w*0.3, h*0.1, a);
  if(sym & MID_L)
    drawBar(x+w*0.1, y+h*0.45, w*0.3, h*0.1, a);
  if(sym & LO_L)
    drawBar(x+w*0.1, y+h*0.9, w*0.3, h*0.1, a);

  if(sym & HI_R)
    drawBar(x+w*0.5, y, w*0.3, h*0.1, a);
  if(sym & MID_R)
    drawBar(x+w*0.5, y+h*0.45, w*0.3, h*0.1, a);
  if(sym & LO_R)
    drawBar(x+w*0.5, y+h*0.9, w*0.3, h*0.1, a);
}

void drawText(const char *text, float x, float y,
    float cw, float ch, float a)
{
  for(const char *c = text; *c; c++) {
    drawChar(*c, x, y, cw*0.8, ch*0.8, a);
    x += cw;
  }
}