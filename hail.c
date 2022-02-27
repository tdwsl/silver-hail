/* Silver Hail - tdwsl 2022 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "sdl.h"
#include "hmap.h"
#include "dirs.h"

#define PI 3.14159
#define SCROLL_SPEED 0.04
#define ENEMY_FOVRANGE 200

struct enemy {
  float x, y, a;
  bool alert;
};

int playerhmap[280000];

struct enemy enemies[400];
int numEnemies;

int map[10000];
int mapW, mapH;

float mapScroll = 0;

float playerX = 400, playerY = 500;
int controlXV = 0, controlYV = 0;
int directionYV = -1, directionXV = 0;

void generateMap() {
  mapW = 20;
  mapH = 220;
  for(int i = 0; i < mapW*mapH; i++)
    map[i] = 0;

  /* reset scroll */
  mapScroll = mapH*32-300;

  int bh = 5;
  for(int y = bh; y < mapH; y += bh+rand()%6) {
    int w = 4 + rand()%((mapW/2)-4);
    int wx = rand()%(mapW-w+1);
    for(int x = wx; x < wx+w; x++)
      map[y*mapW+x] = 1;
  }

  int bn = mapH/4 + rand()%24;
  for(int i = 0; i < bn; i++) {
    int xy = rand()%(mapW*mapH);
    if(!map[xy])
      map[xy] = 3;
  }

  int rn = mapH/3;
  for(int i = 0; i < rn; i++) {
    int w = 5+rand()%3, h = 4+rand()%4;
    int rx = 1+rand()%(mapW-w+1-2), ry = 1+rand()%(mapH-h+1-2);
    int fail = 0;

    for(int x = rx-1; x < rx+w+1; x++)
      for(int y = ry-1; y < ry+h+1; y++)
        if(map[y*mapW+x])
          if(++fail >= 2)
            continue;

    for(int x = rx; x < rx+w; x++)
      for(int y = ry; y < ry+h; y++)
        if(x == rx || y == ry || x == rx+w-1 || y == ry+h-1) {
          if(x == rx+w/2 || y == ry+h/2)
            continue;
          if(map[y*mapW+x]) {
            x = mapW;
            break;
          }
          map[y*mapW+x] = 2;
        }
  }

  /* place enemies */
  numEnemies = 12 + rand()%10;
  for(int i = 0; i < numEnemies; i++) {
    int xy;
    for(;;) {
      xy = 10*mapW + rand()%(mapW*(mapH-20));
      if(!map[xy])
        break;
    }
    map[xy] = -1;
    enemies[i].x = (xy%mapW)*32+20;
    enemies[i].y = (xy/mapW)*32+20 - mapScroll;
    enemies[i].a = ((float)(rand()%12)/12.0)*PI*2;
    enemies[i].alert = false;
  }
  for(int i = 0; i < mapW*mapH; i++)
    if(map[i] == -1)
      map[i] = 0;
}

void drawRect(int x, int y, int w, int h) {
    glVertex2i(x, y);
    glVertex2i(x+w, y+1);
    glVertex2i(x+w, y+h);
    glVertex2i(x, y+h);
  glEnd();
}

void drawCircle(int x, int y, int r, float s) {
  for(float a = 0; a <= PI*2; a += s)
    glVertex2i(x+r*cosf(a), y+r*sinf(a));
  glEnd();
}

void drawShape(const int *arr, int x, int y, float a) {
  int l = *(arr++);
  for(int i = 0; i < l; i++)
    glVertex2i(x + arr[i*2]*cosf(a) + arr[i*2+1]*sinf(a),
        y - (arr[i*2+1]*cosf(a) - arr[i*2]*sinf(a)));
  glEnd();
}

void drawMap() {
  glColor3f(1.0, 1.0, 1.0);

  for(int i = 0; i < mapW*mapH; i++) {
    if(!map[i])
      continue;

    switch(map[i]) {
    case 1:
      glColor3f(0.6, 0.6, 0.6);
      break;
    case 2:
      glColor3f(0.8, 0.8, 0.8);
      break;
    case 3:
      glColor3f(0.6, 0.8, 0.6);
      break;
    }
    glBegin(GL_LINE_LOOP);
    drawRect((i%mapW)*32+1, (i/mapW)*32+1-mapScroll, 32-2, 32-2);
  }
}

void drawPlayer() {
  static const int playerShape[] = {
    3,
    -10, 10,
    10, 10,
    0, -15,
  };

  float a = atan2(directionYV, directionXV)-PI/2;

  glColor4f(0.8, 0.8, 0.5, 0.5);
  glBegin(GL_POLYGON);
  drawShape(playerShape, playerX, playerY, a);

  glColor3f(1.0, 1.0, 0.5);
  glBegin(GL_LINE_LOOP);
  drawShape(playerShape, playerX, playerY, a);
}

void drawEnemy(struct enemy *e) {
  static const int eshape1[] = {
    4,
    -12, 12,
    12, 12,
    3, -8,
    -3, -8,
  };
  static const int fovshape1[] = {
    2,
    -60, -ENEMY_FOVRANGE,
    60, -ENEMY_FOVRANGE,
  };

  float a = e->a-PI/2;

  glColor4f(0.3, 0.4, 0.8, 0.4);
  glBegin(GL_POLYGON);
  drawShape(eshape1, e->x, e->y, a);

  glColor3f(0.3, 0.3, 1.0);
  glBegin(GL_LINES);
  drawShape(eshape1, e->x, e->y, a);

  /* draw fov */
  if(e->alert)
    glColor4f(1.0, 0, 0, 0.6);
  else
    glColor4f(0.8, 0.4, 0, 0.6);
  glBegin(GL_POLYGON);
  glVertex2i(e->x+cosf(e->a)*10, e->y+sinf(e->a)*10);
  glColor4f(0.0, 0, 0, 0.0);
  drawShape(fovshape1, e->x, e->y, a);
}

void drawEnemies() {
  for(int i = 0; i < numEnemies; i++)
    drawEnemy(&enemies[i]);
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  drawMap();
  drawEnemies();
  drawPlayer();

  SDL_GL_SwapWindow(window);
}

void scroll(float s) {
  if(mapScroll < 0)
    return;
  mapScroll -= s;
  playerY += s;
  /* scroll enemies */
  for(int i = 0; i < numEnemies; i++)
    enemies[i].y += s;
}

bool mapXYBlocks(int x, int y) {
  int tx = (x+00)/32, ty = (y+0.0)/32;
  if(ty < 0 || ty >= mapH)
    return false;
  if(tx < 0 || tx >= mapW)
    return true;
  return map[ty*mapW+tx];
}

void moveXY(float *px, float *py, float xm, float ym) {
  float dx = *px + xm,  dy = *py + ym;

  if(dx < 0 || dx >= mapW*32)
    return;

  if(mapXYBlocks(dx, dy+mapScroll))
    return;

  if(px != &playerX)
    for(int i = 0; i < numEnemies; i++) {
      if(&enemies[i].x == px)
        continue;
      if(pow(enemies[i].x-dx, 2) + pow(enemies[i].y-dy, 2) < 10*10)
        return;
    }

  *px = dx;
  *py = dy;
}

void followXY(float *px, float *py, float speed) {
  int tx = (*px)/32.0;
  int ty = ((*py)+mapScroll)/32.0;
  if(tx < 0)
    tx = 0;
  if(ty < 0)
    ty = 0;
  if(tx >= mapW)
    tx = mapW-1;
  if(ty >= mapH)
    ty = mapH-1;

  if(playerhmap[ty*mapW+tx] <= 0)
    return;

  float a;
  for(int d = 0; d < 5; d++) {
    if(d == 4)
      return;

    int x = tx+dirs[d*2], y = ty+dirs[d*2+1];
    if(x < 0 || y < 0 || x >= mapW || y >= mapH)
      continue;
    if(playerhmap[y*mapW+x] == -1)
      continue;

    if(playerhmap[y*mapW+x] == playerhmap[ty*mapW+tx]-1) {
      a = (PI/2)*d - PI/2;
      break;
    }
  }

  moveXY(px, py, 0, sinf(a)*speed);
  moveXY(px, py, cosf(a)*speed, 0);
}

bool lineOfFire(int x1, int y1, int x2, int y2) {
  int d = pow(y2-y1, 2) + pow(x2-x1, 2);
  float a = atan2(y2-y1, x2-x1);

  for(int m = 10; m*m < d; m++) {
    int x = x1+cosf(a)*m, y = y1+sinf(a)*m+mapScroll;
    if(mapXYBlocks(x, y))
      return false;
    if(m > ENEMY_FOVRANGE*1.5)
      return false;
  }

  return true;
}

void updatePlayer(int diff) {
  moveXY(&playerX, &playerY, controlXV*0.2*diff, 0);
  moveXY(&playerX, &playerY, 0, controlYV*0.2*diff);
  /*playerX += controlXV*0.2*diff;
  playerY += controlYV*0.2*diff;*/

  if(playerX < 0)
    playerX = 0;
  if(playerX > 640)
    playerX = 640-1;
  if(playerY < 0)
    playerY = 0;
  if(playerY > 480)
    playerY = 480-1;

  if(playerY < 100)
    scroll(100-playerY);

  /* generate map for following player */

  for(int i = 0; i < mapW*mapH; i++)
    playerhmap[i] = 1;

  for(int i = 0; i < mapW*mapH; i++)
    playerhmap[i] = map[i];

  int x = (playerX/32.0);
  int y = ((playerY+mapScroll)/32.0);
  if(x < 0)
    x = 0;
  if(y < 0)
    y = 0;
  if(x >= mapW)
    x = mapW-1;
  if(y >= mapH)
    y = mapH-1;

  generateHeatmap(playerhmap, mapW, mapH, x, y);
}

void updateEnemy(struct enemy *e, int diff) {
  float ox = e->x, oy = e->y;
  float speed = 0.06;

  if(e->alert)
  {
    if(lineOfFire(e->x, e->y, playerX, playerY)) {
      e->a = atan2(playerY-e->y, playerX-e->x);
    }
    else {
      speed *= 2;
      followXY(&e->x, &e->y, speed*diff);
      if(e->x != ox || e->y != oy)
        e->a = atan2(e->y-oy, e->x-ox);
    }
  }
  else {
    moveXY(&e->x, &e->y, cosf(e->a)*speed*diff, sinf(e->a)*speed*diff);

    if(e->x == ox || e->y == oy) {
      e->a += PI/8;
      if(e->a > PI*2)
        e->a -= PI*2;
    }
  }

  /* check if enemy can see player */

  float pa = atan2(playerY-e->y, playerX-e->x);
  float ad = pa-e->a;
  if(ad < 0)
    ad += PI*2;
  if(ad > PI*2)
    ad -= PI*2;

  float pd = pow(playerY-e->y, 2) + pow(playerX-e->x, 2);

  if((ad < PI/5 || ad > PI*2-PI/5) && pd < ENEMY_FOVRANGE*ENEMY_FOVRANGE)
  {
    int m;
    for(m = 10; m < ENEMY_FOVRANGE; m++) {
      int x = e->x+cosf(pa)*m;
      int y = e->y+sinf(pa)*m + mapScroll;
      if(mapXYBlocks(x, y))
        break;
    }

    if(m*m >= pd)
      e->alert = true;
  }
}

void updateEnemies(int diff) {
  for(int i = 0; i < numEnemies; i++)
    updateEnemy(&enemies[i], diff);

  /* cull out of range */
  for(int i = 0; i < numEnemies; i++)
    if(enemies[i].y > 640+100)
      enemies[i--] = enemies[--numEnemies];
}

void update(int diff) {
  /* update scroll */
  scroll(SCROLL_SPEED*diff);

  updatePlayer(diff);
  updateEnemies(diff);
}

int main() {
  initSDL();
  srand(time(NULL));
  generateMap();

  int lastUpdate = SDL_GetTicks();

  bool quit = false;
  while(!quit) {
    SDL_Event ev;
    while(SDL_PollEvent(&ev))
      switch(ev.type) {
      case SDL_QUIT:
        quit = true;
        break;

      /* controls */
      case SDL_KEYDOWN:
        switch(ev.key.keysym.sym) {
        case SDLK_UP:
          controlYV = -1;
          break;
        case SDLK_DOWN:
          controlYV = 1;
          break;
        case SDLK_LEFT:
          controlXV = -1;
          break;
        case SDLK_RIGHT:
          controlXV = 1;
          break;
        }
        break;
      case SDL_KEYUP:
        switch(ev.key.keysym.sym) {
        case SDLK_UP:
          if(controlYV < 0)
            controlYV = 0;
          break;
        case SDLK_DOWN:
          if(controlYV > 0)
            controlYV = 0;
          break;
        case SDLK_LEFT:
          if(controlXV < 0)
            controlXV = 0;
          break;
        case SDLK_RIGHT:
          if(controlXV > 0)
            controlXV = 0;
          break;
        }
        break;
      }

    if(controlXV || controlYV) {
      directionXV = controlXV;
      directionYV = controlYV;
    }

    draw();

    int currentTime = SDL_GetTicks();
    update(currentTime-lastUpdate);
    lastUpdate = currentTime;
  }

  endSDL();
  return 0;
}
