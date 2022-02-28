#include <stdbool.h>
#include <stdio.h>
#include "hmap.h"
#include "dirs.h"

void generateHeatmap(int *hmap, int w, int h, int x, int y) {
  for(int i = 0; i < w*h; i++)
    hmap[i] = ((bool)hmap[i])*-1;

  hmap[y*w+x] = 1;
  bool stuck = false;

  for(int i = 1; !stuck; i++) {
    stuck = true;

    for(int j = 0; j < w*h; j++) {
      if(hmap[j] != i)
        continue;

      stuck = false;

      for(int d = 0; d < 4; d++) {
        int x = j%w+dirs[d*2];
        int y = j/w+dirs[d*2+1];
        if(x < 0 || y < 0 || x >= w || y >= h)
          continue;

        if(!hmap[y*w+x])
          hmap[y*w+x] = i+1;
      }
    }
  }
}