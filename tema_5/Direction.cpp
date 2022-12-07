#include "Direction.hpp"

Direction::Direction(int8_t xAxis, int8_t yAxis) {
  switch (xAxis) {
    case 1:
      {
        _direction = _direction | Direction::RIGHT;
        break;
      }
    case -1:
      {
        _direction = _direction | Direction::LEFT;
        break;
      }
    default:
      {
        break;
      }
  }

  switch (yAxis) {
    case 1:
      {
        _direction = _direction | Direction::UP;
        break;
      }
    case -1:
      {
        _direction = _direction | Direction::DOWN;
        break;
      }
    default:
      {
        break;
      }
  }
}

bool Direction::isLeft() const {
  return _direction & Direction::LEFT;
}

bool Direction::isRight() const {
  return _direction & Direction::RIGHT;
}

bool Direction::isUp() const {
  return _direction & Direction::UP;
}

bool Direction::isDown() const {
  return _direction & Direction::DOWN;
}

/**
 * Up -> Right
 * Donw -> Left 
 * And vice-versa
 */
void Direction::switchAxes() {
  bool isDirUp = isUp();
  bool isDirDown = isDown();

  _direction = _direction >> 2;
  if (isDirUp) {
    _direction = _direction | RIGHT;
  }

  if (isDirDown) {
    _direction = _direction | LEFT;
  }
}

void Direction::invertXAxis() {
  if (!isLeft() && !isRight()) {
    return;
  }
  _direction = _direction ^ INVERT_X_AXIS_MASK;
}

void Direction::invertYAxis() {
  if (!isUp() && !isDown()) {
    return;
  }
  _direction = _direction ^ INVERT_Y_AXIS_MASK;
}

uint8_t Direction::getDirection() {
  return _direction;
}