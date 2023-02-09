#ifndef Gfx_h
#define Gfx_h

#include <TFT_eSPI.h>       // Hardware-specific library

class Gfx {
  private:
    TFT_eSprite *_spr_work;
    TFT_eSprite *_spr_screen_buffer;

  public:
    Gfx(TFT_eSPI *tft) {
      this->_spr_work = TFT_eSprite(&tft);
      this->_spr_screen_buffer = TFT_eSprite(&tft);
    }
    
    void draw(const unsigned short *image, int x, int y, int width, int height, int transparent_color) {
      spr_work.createSprite(width, height);
      spr_work.setSwapBytes(true);
      spr_work.pushImage(0, 0, width, height, image);
      spr_work.pushToSprite(&this->_spr_screen_buffer, x, y, transparent_color);
      spr_work.deleteSprite();
    }

};


#endif