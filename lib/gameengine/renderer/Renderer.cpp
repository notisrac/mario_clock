#include "Renderer.h"
#include "../BitFlag.h"

#include <stdexcept>

Renderer::Renderer(int width, int height, int scale, const char* windowTitle, unsigned short backgroundColor, unsigned short transparentColor)
{
    _width = width;
    _height = height;
    _scale = scale;
    _backgroundColor = backgroundColor;
    _transparentColor = transparentColor;

    _tft.init();
    _tft.setRotation(1);	// landscape
    //_tft.fillScreen(_transparentColor);
    _spr_pixels.createSprite(_width, _height);
    _spr_pixels.createSprite(10, 10);
    _spr_pixels.setSwapBytes(true);
}

Renderer::~Renderer()
{
}

void Renderer::setPixel(int x, int y, int color)
{
    if (x >= _width || y >= _height || x < 0 || y < 0)
    {
        return;
    }

    if (color == _transparentColor)
    { // transparent color
        return;
    }
    _spr_pixels.drawPixel(x, y, color);

}

void Renderer::drawSprite(int x, int y, int width, int height, unsigned int* pixels)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            setPixel(x + i, y + j, pixels[j * width + i]);
        }
    }
}

void Renderer::renderScreen()
{
    _spr_pixels.pushSprite(0, 0);
    // reset the buffer after it has been output to the display
    fillBackground(_backgroundColor);
}

void Renderer::fillBackground(unsigned short color)
{
    _spr_pixels.fillSprite(color);
}

BitFlag* Renderer::pollEvents()
{
    // while (SDL_PollEvent(&_event) != NULL)
    // {
    //     switch (_event.type)
    //     {
    //         case SDL_QUIT:
    //             _eventsBitFlag.SetFlag(EventTypes::Quit);
    //             break;
    //         case SDL_KEYDOWN:
    //             switch (_event.key.keysym.sym)
    //             {
    //             case SDLK_LEFT:
    //                 _eventsBitFlag.SetFlag(EventTypes::ButtonLeft);
    //                 break;
    //             case SDLK_RIGHT:
    //                 _eventsBitFlag.SetFlag(EventTypes::ButtonRight);
    //                 break;
    //             case SDLK_UP:
    //                 _eventsBitFlag.SetFlag(EventTypes::ButtonUp);
    //                 break;
    //             case SDLK_DOWN:
    //                 _eventsBitFlag.SetFlag(EventTypes::ButtonDown);
    //                 break;
    //             case SDLK_RETURN:
    //                 _eventsBitFlag.SetFlag(EventTypes::ButtonReturn);
    //                 break;
    //             case SDLK_SPACE:
    //                 _eventsBitFlag.SetFlag(EventTypes::ButtonSpace);
    //                 break;
    //             }
    //             break;
    //         case SDL_KEYUP:
    //             switch (_event.key.keysym.sym)
    //             {
    //             case SDLK_LEFT:
    //                 _eventsBitFlag.UnsetFlag(EventTypes::ButtonLeft);
    //                 break;
    //             case SDLK_RIGHT:
    //                 _eventsBitFlag.UnsetFlag(EventTypes::ButtonRight);
    //                 break;
    //             case SDLK_UP:
    //                 _eventsBitFlag.UnsetFlag(EventTypes::ButtonUp);
    //                 break;
    //             case SDLK_DOWN:
    //                 _eventsBitFlag.UnsetFlag(EventTypes::ButtonDown);
    //                 break;
    //             case SDLK_RETURN:
    //                 _eventsBitFlag.UnsetFlag(EventTypes::ButtonReturn);
    //                 break;
    //             case SDLK_SPACE:
    //                 _eventsBitFlag.UnsetFlag(EventTypes::ButtonSpace);
    //                 break;
    //             }
    //             break;
    //     }
    // }

    return &_eventsBitFlag;
}

void Renderer::debugWrite(const char* text)
{
    // TODO fix this
    // SDL_Surface* surf = TTF_RenderText_Blended(_font, text, _overlayColor);
    // _overlayTexture = SDL_CreateTextureFromSurface(_renderer, surf);
    // SDL_FreeSurface(surf);
}

void Renderer::close()
{
    // SDL_DestroyTexture(_texture);
    // SDL_DestroyRenderer(_renderer);
    // SDL_DestroyWindow(_window);
    // SDL_Quit();
}
