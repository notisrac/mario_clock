#ifndef Object_h
#define Object_h

#include <Gfx.h>

class Object {
  private:
    Gfx * _gfx;
    const unsigned short *_image;
    int _width;
    int _height;
    int _transparent_color;

  public:
    Object(Gfx * gfx, const unsigned short *image, int width, int height, int transparent_color) {
      this->_gfx = gfx;
      this->_image = image;
      this->_width = width;
      this->_height = height;
      this->_transparent_color = height;
    }
  
    virtual void draw(int x, int y) {
      this->_gfx.draw(this->_image, this->_x, this->_y, this->_width, this->_height);
    }


};


#endif