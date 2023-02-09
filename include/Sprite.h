#ifndef Sprite_h
#define Sprite_h

#include <Gfx.h>

class Sprite {
  public:
    Gfx *gfx;
    int xPos;
    int yPos;
    int width;
    int height;
    int spriteNo;

  public:
    Sprite(Gfx *gfx, int x, int y, int w, int h, int spriteNumber) {
      gfx = gfx;
      spriteNo = spriteNumber;
      xPos = x;
      yPos = y;
      width = w;
      height = h;
    }
  
    virtual void draw() {
      gfx->drawSprite(xPos, yPos, spriteNo);
    }
};


#endif