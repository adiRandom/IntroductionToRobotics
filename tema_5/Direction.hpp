#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <stdint.h>

class Direction {
private:
  uint8_t _direction = 0;

  static uint8_t const INVERT_X_AXIS_MASK = 0b1100;
  static uint8_t const INVERT_Y_AXIS_MASK = 0b0011;

public:
  static uint8_t const UP = 0b0001;
  static uint8_t const DOWN = 0b0010;
  static uint8_t const LEFT = 0b0100;
  static uint8_t const RIGHT = 0b1000;
  static uint8_t const UP_LEFT = 0b0101;
  static uint8_t const UP_RIGHT = 0b1001;
  static uint8_t const DOWN_LEFT = 0b0110;
  static uint8_t const DOWN_RIGHT = 0b1010;
  static uint8_t const NONE = 0b0000;

  /**
    * Each arg takes 0,1,-1 as its value representing the neutral, positive and negative part of the axis
    */
  Direction(int8_t xAxis, int8_t yAxis);

  bool isLeft() const;
  bool isRight() const;
  bool isUp() const;
  bool isDown() const;

  void switchAxes();
  void invertXAxis();
  void invertYAxis();

  uint8_t getDirection();
};

#endif