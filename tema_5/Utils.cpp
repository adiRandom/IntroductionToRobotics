#include "Utils.h"

Point::Point(int16_t x, int16_t y)
  : _x{ x },
    _y{ y } {
}

int16_t Point::getX() const {
  return _x;
}

int16_t Point::getY() const {
  return _y;
}

Pixel Point::toPixel(int32_t displayHeight) const {
  // We use the bottom left corner as (0,0) and the top left as (0, displayHeight - 1)
  // So we need to resolve this
  return Pixel{ _x, displayHeight - _y - 1, true };
}

void Point::updateX(int16_t delta) {
  _x += delta;
}

void Point::updateY(int16_t delta) {
  _y += delta;
}
