#include "InputController.hpp"
#include <Arduino.h>

InputController::InputController()
  : _joyXAxisPin{ 0 },
    _joyYAxisPin{ 0 },
    _switchedAxes{ false },
    _invertedXAxis{ false },
    _invertedYAxis{ false },
    _throttleTime{ DEFAULT_THROTTLE_TIME } {
}

InputController::InputController(
  uint8_t joyXAxisPin,
  uint8_t joyYAxisPin,
  uint8_t primaryBtnPin,
  uint8_t secondaryBtnPin,
  bool switchedAxes = false,
  bool invertedXAxis = false,
  bool invertedYAxis = false,
  uint32_t throttleTime = DEFAULT_THROTTLE_TIME
)
  : _joyXAxisPin{ joyXAxisPin },
    _joyYAxisPin{ joyYAxisPin },
    _switchedAxes{ switchedAxes },
    _invertedXAxis{ invertedXAxis },
    _invertedYAxis{ invertedYAxis },
    _throttleTime{ throttleTime } {

  _primaryBtnState.pin = primaryBtnPin;
  _secondaryBtnState.pin = secondaryBtnPin;
  pinMode(_joyXAxisPin, INPUT);
  pinMode(_joyYAxisPin, INPUT);
  pinMode(_primaryBtnState.pin, INPUT_PULLUP);
  pinMode(_secondaryBtnState.pin, INPUT_PULLUP);
}

int8_t InputController::getJoyValueOnAxis(uint8_t pin) const {
  uint16_t value = analogRead(pin);

  if (value >= POSITIVE_THRESHOLD) {
    return 1;
  } else if (value <= NEGATIVE_THRESHOLD) {
    return -1;
  } else {
    return 0;
  }
}

Direction InputController::getJoyDirection(bool throttled = false) {
  if (throttled) {
    uint32_t now = millis();

    if (now - _throttleTimer <= _throttleTime) {
      return Direction(0, 0);
    } else {
      _throttleTimer = now;
    }
  }
  int8_t xAxisVal = getJoyValueOnAxis(_joyXAxisPin);
  int8_t yAxisVal = getJoyValueOnAxis(_joyYAxisPin);
  Direction direction = Direction(xAxisVal, yAxisVal);
  return getDirectionWithInversions(direction);
}

Direction InputController::getDirectionWithInversions(Direction direction) const {
  Direction finalDirection = direction;

  if (_switchedAxes) {
    finalDirection.switchAxes();
  }

  if (_invertedXAxis) {
    finalDirection.invertXAxis();
  }

  if (_invertedYAxis) {
    finalDirection.invertYAxis();
  }

  return finalDirection;
}


InputController& InputController::operator=(InputController const& other) {

  if (this == &other) {
    return *this;
  }

  _joyXAxisPin = other._joyXAxisPin;
  _joyYAxisPin = other._joyYAxisPin;
  _primaryBtnState = other._primaryBtnState;
  _secondaryBtnState = other._secondaryBtnState;
  _switchedAxes = other._switchedAxes;
  _invertedXAxis = other._invertedXAxis;
  _invertedYAxis = other._invertedYAxis;

  return *this;
}

void InputController::setThrotthleTime(uint32_t throttleTime) {
  _throttleTime = throttleTime;
}

InputController::DebounceBtnReadingState
InputController::readButtonState(
  InputController::DebounceBtnReadingState currentBtnState,
  void (*onStateChange)(byte),
  uint8_t notifyOn = CHANGE
) {
  InputController::DebounceBtnReadingState updatedState = currentBtnState;
  byte buttonReading = digitalRead(currentBtnState.pin);
  uint32_t now = millis();

  if (buttonReading != currentBtnState.lastReading) {
    updatedState.lastReadingTime = now;
  }

  if (now - updatedState.lastReadingTime > INPUT_DEBOUNCE_TIME) {
    if (buttonReading != currentBtnState.value) {
      updatedState.value = buttonReading;

      if (onStateChange != nullptr) {
        if ((notifyOn == CHANGE) || (notifyOn == RISING && buttonReading == HIGH) || (notifyOn == FALLING && buttonReading == LOW)) {
          onStateChange(buttonReading);
        }
      }
    }
  }

  updatedState.lastReading = buttonReading;
  return updatedState;
}

bool InputController::isPrimaryBtnPressed() {
  _primaryBtnState = readButtonState(_primaryBtnState, nullptr, CHANGE);
  return _primaryBtnState.value == BUTTON_PRESSED;
}

bool InputController::isSecondaryBtnPressed() {
  _secondaryBtnState = readButtonState(_secondaryBtnState, nullptr, CHANGE);
  return _secondaryBtnState.value == BUTTON_PRESSED;
}