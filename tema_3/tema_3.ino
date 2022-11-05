uint8_t const DISPLAY_PIN_A = 4;
uint8_t const DISPLAY_PIN_B = 5;
uint8_t const DISPLAY_PIN_C = 6;
uint8_t const DISPLAY_PIN_D = 7;
uint8_t const DISPLAY_PIN_E = 8;
uint8_t const DISPLAY_PIN_F = 9;
uint8_t const DISPLAY_PIN_G = 10;
uint8_t const DISPLAY_PIN_DP = 11;
uint8_t const DISPLAY_PINS_SIZE = 8;
uint8_t const DISPLAY_PINS[DISPLAY_PINS_SIZE] = {
  DISPLAY_PIN_A,
  DISPLAY_PIN_B,
  DISPLAY_PIN_C,
  DISPLAY_PIN_D,
  DISPLAY_PIN_E,
  DISPLAY_PIN_F,
  DISPLAY_PIN_G,
  DISPLAY_PIN_DP
};

// The first dimension is the display led pin index
// The second dimension is the led pin index that shold be selected if we move into the direction indicated by the second index from the pin indicated by the first index
// The indexing of the second dimension coresponds to the Direction enum
// -1 indicates we can't select any display led in that direction
int8_t const LED_MOVE_GRAPH[DISPLAY_PINS_SIZE][4] = {
  { -1, 3, 5, 1 },
  { -1, 2, 0, -1 },
  { 1, -1, 3, 7 },
  { 6, -1, 4, 2 },
  { 5, -1, -1, 3 },
  { -1, 4, -1, 0 },
  { 0, 3, -1, -1 },
  { -1, -1, 2, -1 },
};

uint8_t const JOY_X_AXIS_PIN = A0;
uint8_t const JOY_Y_AXIS_PIN = A1;
uint8_t const JOY_BTN_PIN = 12;

// The thresholds of the joystick from which we consider the input as being on the positive semi-axis or on the negative
uint16_t const POSITIVE_THRESHOLD = 700;
uint16_t const NEGATIVE_THRESHOLD = 300;

uint16_t const BLINK_INTERVAL = 500;

// True if the X axis should be interpreted as the Y axis and vice versa
// False otherwise
bool invertedAxes = true;
bool shouldReadJoyDir = true;
// When we disable joy reading, we keep track of the axis we are currently reading.
// When that axis gets into the neutral position, we re-enable reading
int8_t activeJoyAxisPin = -1;

uint8_t currentLedPinIndex = 0;
bool ledState[DISPLAY_PINS_SIZE] = { LOW };
bool shouldCurrentLedBlink = true;
uint8_t blinkState = LOW;
uint32_t lastBlinkTime = 0;
bool canCurrentLedMove = true;

enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  NONE
};

void pinSetup() {
  for (uint8_t i = 0; i < DISPLAY_PINS_SIZE; i++) {
    pinMode(DISPLAY_PINS[i], OUTPUT);
  }

  pinMode(JOY_X_AXIS_PIN, INPUT);
  pinMode(JOY_Y_AXIS_PIN, INPUT);
  pinMode(JOY_BTN_PIN, INPUT_PULLUP);
}

void setup() {
  pinSetup();

  Serial.begin(9600);
}

// @Returns 1 for the positive semi-axis,
// -1 for the negative one,
// 0 if neutral
// and a negative integer != -1 if we can't read from the joystick now
int8_t getJoyValueOnAxis(uint8_t pin) {
  uint16_t value = analogRead(pin);

  if (
    value < POSITIVE_THRESHOLD
    && value > NEGATIVE_THRESHOLD
    && activeJoyAxisPin == pin
  ) {
    // Active axis became neutral
    shouldReadJoyDir = true;
    activeJoyAxisPin = -1;
    return 0;
  } else if (value >= POSITIVE_THRESHOLD && shouldReadJoyDir) {
    // Reading a new axis on the positive side
    shouldReadJoyDir = false;
    activeJoyAxisPin = pin;
    return 1;
  } else if (value <= NEGATIVE_THRESHOLD && shouldReadJoyDir) {
    // Reading a new axis on the negative side
    shouldReadJoyDir = false;
    activeJoyAxisPin = pin;
    return -1;
  } else if (shouldReadJoyDir == false) {
    // Can't read joystick right now
    return -2;
  } else {
    // All axis are on neutral
    return 0;
  }
}

Direction getJoyDirection() {

  // We first read the X axis then the Y axis
  // If we register input on the X axis first, we return a direction on that axis
  // When we get a reading on an axis, we block further reading until the joystick gets back to the neutral position first
  int8_t xAxisVal = getJoyValueOnAxis(JOY_X_AXIS_PIN);

  if (xAxisVal == 1) {
    return invertedAxes ? Direction::UP : Direction::RIGHT;
  } else if (xAxisVal == -1) {
    return invertedAxes ? Direction::DOWN : Direction::LEFT;
  }

  // We haven't returned yet so we can process the Y axis
  int8_t yAxisVal = getJoyValueOnAxis(JOY_Y_AXIS_PIN);

  if (yAxisVal == 1) {
    return invertedAxes ? Direction::RIGHT : Direction::UP;
  } else if (yAxisVal == -1) {
    return invertedAxes ? Direction::LEFT : Direction::DOWN;
  }

  return Direction::NONE;
}

void blinkLed() {
  uint32_t now = millis();

  if (now - lastBlinkTime >= BLINK_INTERVAL) {
    blinkState = !blinkState;
    lastBlinkTime = now;
  }
}

void showLeds() {
  for (uint8_t i = 0; i < DISPLAY_PINS_SIZE; i++) {
    if (i == currentLedPinIndex) {
      if (shouldCurrentLedBlink) {
        blinkLed();
        digitalWrite(DISPLAY_PINS[i], blinkState);
      }
    } else {
      digitalWrite(DISPLAY_PINS[i], ledState[i]);
    }
  }
}

void moveCurrentLed(Direction dir) {
  int8_t nextLedPinIndex = LED_MOVE_GRAPH[currentLedPinIndex][dir];

  if (nextLedPinIndex != -1) {
    currentLedPinIndex = nextLedPinIndex;
  }
}

void handleInput() {
  Direction dir = getJoyDirection();

  if (dir == Direction::NONE) {
    return;
  }
  if (canCurrentLedMove) {
    moveCurrentLed(dir);
  }
}

void loop() {
  showLeds();
  handleInput();
}