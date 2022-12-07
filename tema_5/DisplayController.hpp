#ifndef DISPLAY_CONTROLLER_HPP
#define DISPLAY_CONTROLLER_HPP

#include "LedControl.h"
#include <stdint.h>
#include "Utils.h"
#include "List.cpp"

uint8_t const DISPLAY_SIZE = 8;
uint8_t const DISPLAY_WIDTH = 8;
uint8_t const DISPLAY_HEIGHT = 8;
uint8_t const DISPLAY_COUNT = 1;

class DisplayController {
private:
  uint8_t _dataPin;
  uint8_t _loadPin;
  uint8_t _clkPin;
  bool _state[DISPLAY_HEIGHT][DISPLAY_WIDTH];
  bool _nextFrame[DISPLAY_HEIGHT][DISPLAY_WIDTH];

  LedControl _lc;

  struct PixelCoords {
    uint8_t row;
    uint8_t column;
    uint8_t display;
  };

  PixelCoords resolvePixel(uint8_t row, uint8_t column) const;
  void initDisplay();
  void emptyNextFrame();


public:
  DisplayController();
  DisplayController(
    uint8_t dataPin,
    uint8_t loadPin,
    uint8_t clkPin
  );
  DisplayController(DisplayController const& other);

  DisplayController& operator=(DisplayController const& other);

  void draw();
  void setPixel(Pixel pixel);
  void setPixels(Pixel pixels[], uint32_t length);
  void setPixels(List<Pixel> pixels);
  void commitNextFrame();
  void initStateAndNextFrame(DisplayController* initValue);
};

#endif