#ifndef RENDERER_H_
#define RENDERER_H_

#include <TFT_eSPI.h>       // Hardware-specific library
#include "../BitFlag.h"


class Renderer
{
public:
	Renderer(int width, int height, int scale, const char* windowTitle, unsigned short backgroundColor, unsigned short transparentColor);
	~Renderer();

	void setPixel(int x, int y, int color);
	void drawSprite(int x, int y, int width, int height, unsigned int* pixels);
	void renderScreen();
	void fillBackground(unsigned short color);
	BitFlag* pollEvents();
	const int getWidth() const { return _width;	}
	const int getHeight() const { return _height; }
	void debugWrite(const char* text);

	void close();

private:
	TFT_eSPI _tft = TFT_eSPI();  // Invoke custom library
	TFT_eSprite _spr_pixels = TFT_eSprite(&_tft);  // Declare Sprite object "spr" with pointer to "tft" object
	int _width;
	int _height;
	int _scale;
	unsigned short* _pixels;
	unsigned short _backgroundColor;
	unsigned short _transparentColor;
	BitFlag _eventsBitFlag;
};

#endif