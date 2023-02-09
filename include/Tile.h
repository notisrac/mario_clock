#ifndef Tile_h
#define Tile_h

#include <Gfx.h>
#include <Sprite.h>

class Tile : public Sprite {
  private:
    bool _isBlocking;

  public:
    Tile(Gfx *gfx, int x, int y, int w, int h, int spriteNo, bool isBlocking) : Sprite(gfx, x, y, w, h, spriteNo) {
      _isBlocking = isBlocking;
    }
  };


#endif