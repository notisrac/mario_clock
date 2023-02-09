#ifndef Gfx_h
#define Gfx_h

#include <TFT_eSPI.h>       // Hardware-specific library

class Gfx {
  private:
    TFT_eSprite* _sprWork;
    TFT_eSprite* _sprScreenBuffer;
    TFT_eSprite* _sprClockFont;
    const unsigned short* _spriteSheet;
    int16_t _spriteSize;
    // number of sprites per row
    int16_t _spriteSheetWidth;
    uint16_t _transparentColor;
    int16_t _fontWidth;
    const unsigned short* _font;

  public:
    Gfx(TFT_eSPI* tft, uint16_t transparentColor) {
      _sprWork = TFT_eSprite(tft);
      _sprScreenBuffer = TFT_eSprite(tft);
      _sprScreenBuffer->createSprite(tft->width(), tft->height());
      _sprClockFont = TFT_eSprite(tft);
      _transparentColor = transparentColor;
    }

    void loadSpriteSheet(const unsigned short* spriteSheet, int16_t spriteSize, int16_t spriteSheetWidth) {
      _spriteSheet = spriteSheet;
      _spriteSize = spriteSize;
      _spriteSheetWidth = spriteSheetWidth / spriteSize;
      _sprWork->createSprite(spriteSize, spriteSize);
    }

    void loadFont(const unsigned short* font, int16_t fontHeight, int16_t fontWidth, int16_t fontCharNo) {
      _fontWidth = fontWidth;
      _font = font;
      _sprClockFont->createSprite(_fontWidth, fontHeight);
    }

    void printChar(int32_t x, int32_t y, int32_t charPos) {
      // load the glyph from the appropriate position in the font
      _sprClockFont->pushImage( -1 * (_fontWidth * charPos), 0, _sprClockFont->width(), _sprClockFont->height(), _font);
      // add it to the screen buffer
      _sprWork->pushToSprite(_sprScreenBuffer, x, y, _transparentColor);
    }
    
    void drawSprite(int32_t x, int32_t y, int16_t spriteNo) {
      _sprWork->pushImage(-1 * (_spriteSize * spriteNo), (spriteNo / _spriteSheetWidth) * _spriteSize, _spriteSize, _spriteSize, _spriteSheet);
      _sprWork->pushToSprite(_sprScreenBuffer, x, y, _transparentColor);
      _sprWork->deleteSprite();
    }

};


#endif