#ifndef INPUT_CONTROLLER_HPP
#define INPUT_CONTROLLER_HPP

#include <stdint.h>
#include "Direction.hpp"
#include <Arduino.h>

// The thresholds of the joystick from which we consider the input as being on the positive semi-axis or on the negative
uint16_t const POSITIVE_THRESHOLD = 700;
uint16_t const NEGATIVE_THRESHOLD = 300;
uint8_t const INPUT_DEBOUNCE_TIME = 50;
uint32_t const DEFAULT_THROTTLE_TIME = 100;

// The value emitted by the button when pressed
byte const BUTTON_PRESSED = LOW;

class InputController {
private:
  uint8_t _joyXAxisPin;
  uint8_t _joyYAxisPin;
  bool _switchedAxes;
  bool _invertedXAxis;
  bool _invertedYAxis;
  uint32_t _throttleTime;
  uint32_t _throttleTimer = millis();

  struct DebounceBtnReadingState {
    byte lastReading = HIGH;
    byte value = HIGH;
    uint32_t lastReadingTime = 0;
    uint8_t pin = 0;
  };

  DebounceBtnReadingState _primaryBtnState;
  DebounceBtnReadingState _secondaryBtnState;

  // @Returns 1 for the positive semi-axis,
  // -1 for the negative one,
  // 0 if neutral
  int8_t getJoyValueOnAxis(uint8_t pin) const;
  /**
   * Handle all the inverted axes and get the correct direction
   */
  Direction getDirectionWithInversions(Direction direction) const;
  /**
   * Read the button at currentBtnState.pin debounced. 
   * The reading will be passed with a cb on RISING, FALLING or CHANGE
   * Returns the updated BtnReadingState
   */
  DebounceBtnReadingState readButtonState(
    DebounceBtnReadingState currentBtnState,
    void (*onStateChange)(byte),
    uint8_t notifyOn = CHANGE
  );

public:
  InputController();
  InputController(
    uint8_t joyXAxisPin,
    uint8_t joyYAxisPin,
    uint8_t primaryBtnPin,
    uint8_t secondaryBtnPin,
    bool switchedAxis,
    bool invertedXAxis,
    bool invertedYAxis,
    uint32_t throttleTime = DEFAULT_THROTTLE_TIME
  );

  InputController& operator=(InputController const& other);

  Direction getJoyDirection(bool throttled);
  bool isPrimaryBtnPressed();
  bool isSecondaryBtnPressed();
  void setThrotthleTime(uint32_t throttleTime);
};

#endif