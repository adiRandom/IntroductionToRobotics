//DS= [D]ata [S]torage - data
//STCP= [ST]orage [C]lock [P]in latch
//SHCP= [SH]ift register [C]lock [P]in clock

uint8_t const LATCH_PIN = 11;  // STCP to 12 on Shift Register
uint8_t const CLOCK_PIN = 10;  // SHCP to 11 on Shift Register
uint8_t const DATA_PIN = 12;   // DS to 14 on Shift Register

uint8_t const DISPLAY_D1_PIN = 7;
uint8_t const DISPLAY_D2_PIN = 6;
uint8_t const DISPLAY_D3_PIN = 5;
uint8_t const DISPLAY_D4_PIN = 4;

uint8_t const REG_SIZE = 8;

uint8_t const DISPLAYS[] = {
  DISPLAY_D4_PIN, DISPLAY_D3_PIN, DISPLAY_D2_PIN, DISPLAY_D1_PIN
};
uint8_t const DISPLAY_COUNT = 4;
uint8_t const ENCODINGS_LENGTH = 16;


uint8_t const ENCODINGS[ENCODINGS_LENGTH] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110   // F
};


uint8_t const JOY_X_AXIS_PIN = A0;
uint8_t const JOY_Y_AXIS_PIN = A1;
uint8_t const JOY_BTN_PIN = 2;

// The thresholds of the joystick from which we consider the input as being on the positive semi-axis or on the negative
uint16_t const POSITIVE_THRESHOLD = 700;
uint16_t const NEGATIVE_THRESHOLD = 300;
uint8_t const INPUT_DEBOUNCE_TIME = 50;
uint32_t const LONG_PRESS_TIME = 1500;
uint16_t const BLINK_INTERVAL = 500;

// True if the X axis should be interpreted as the Y axis and vice versa
// False otherwise
bool INVERTED_AXES = true;
// The value emitted by the button when pressed
uint8_t BUTTON_PRESSED = LOW;

bool shouldReadJoyDir = true;
// When we disable joy reading, we keep track of the axis we are currently reading.
// When that axis gets into the neutral position, we re-enable reading
int8_t activeJoyAxisPin = -1;

uint8_t currentDisplayIndex = 0;
uint8_t displayState[DISPLAY_COUNT] = { 0, 0, 0, 0 };
bool canCycleDisplays = true;

byte lastButtonReadingState = HIGH;
byte joyBtnState = HIGH;
uint32_t lastButtonReadingDebounce = 0;
uint32_t joyBtnPressTime = 0;

bool shouldDpBlink = true;
uint8_t dpBlinkState = HIGH;
uint32_t lastBlinkTime = 0;

enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  NONE
};

void pinSetup() {
  // put your setup code here, to run once:
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  for (int i = 0; i < DISPLAY_COUNT; i++) {
    pinMode(DISPLAYS[i], OUTPUT);
  }
  pinMode(JOY_X_AXIS_PIN, INPUT);
  pinMode(JOY_Y_AXIS_PIN, INPUT);
  pinMode(JOY_BTN_PIN, INPUT_PULLUP);
}

void resetDisplay() {
  for (uint8_t i = 0; i < DISPLAY_COUNT; i++) {
    displayState[i] = 0;
  }

  currentDisplayIndex = DISPLAY_COUNT - 1;
  shouldDpBlink = true;
  canCycleDisplays = true;
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
    return INVERTED_AXES ? Direction::UP : Direction::RIGHT;
  } else if (xAxisVal == -1) {
    return INVERTED_AXES ? Direction::DOWN : Direction::LEFT;
  }

  // We haven't returned yet so we can process the Y axis
  int8_t yAxisVal = getJoyValueOnAxis(JOY_Y_AXIS_PIN);

  if (yAxisVal == 1) {
    return INVERTED_AXES ? Direction::RIGHT : Direction::UP;
  } else if (yAxisVal == -1) {
    return INVERTED_AXES ? Direction::LEFT : Direction::DOWN;
  }

  return Direction::NONE;
}

uint8_t getEncodingWithDp(uint8_t encoding, uint8_t displayIndex) {
  if (displayIndex == 0) {
  }
  return displayIndex != currentDisplayIndex ? encoding : (encoding | dpBlinkState);
}

void blinkDp() {
  uint32_t now = millis();

  if (now - lastBlinkTime >= BLINK_INTERVAL) {
    dpBlinkState = !dpBlinkState;
    lastBlinkTime = now;
  }
}


void readButtonState(void (*onStateChange)(byte, bool), uint8_t notifyOn = CHANGE) {
  byte buttonReading = digitalRead(JOY_BTN_PIN);
  uint32_t now = millis();

  if (buttonReading != lastButtonReadingState) {
    lastButtonReadingDebounce = now;
  }

  if (now - lastButtonReadingDebounce > INPUT_DEBOUNCE_TIME) {
    if (buttonReading != joyBtnState) {
      joyBtnState = buttonReading;
      bool isLongPress = false;

      if (joyBtnState == BUTTON_PRESSED) {
        // Start timer for long press
        joyBtnPressTime = now;
      } else {
        if (now - joyBtnPressTime >= LONG_PRESS_TIME) {
          isLongPress = true;
        }

        joyBtnPressTime = 0;
      }

      if (
        (notifyOn == CHANGE)
        || (notifyOn == RISING && joyBtnState == HIGH)
        || (notifyOn == FALLING && joyBtnState == LOW)
      ) {
        onStateChange(joyBtnState, isLongPress);
      }
    }
  }

  lastButtonReadingState = buttonReading;
}

void onButtonStateChange(byte state, bool isLongPress) {
  if (isLongPress) {
    resetDisplay();
    return;
  }

  canCycleDisplays = !canCycleDisplays;
  shouldDpBlink = !shouldDpBlink;

  if (!shouldDpBlink) {
    dpBlinkState = HIGH;
  }
}

void cycleDisplays(Direction dir) {
  switch (dir) {
    case Direction::LEFT:
      {
        currentDisplayIndex = currentDisplayIndex == 0 ? DISPLAY_COUNT - 1 : currentDisplayIndex - 1;
        break;
      }
    case Direction::RIGHT:
      {
        currentDisplayIndex = (currentDisplayIndex + 1) % DISPLAY_COUNT;
        break;
      }
    default:
      {
        break;
      }
  }
}

void updateDigit(Direction dir) {
  switch (dir) {
    case Direction::UP:
      {
        displayState[currentDisplayIndex] = (displayState[currentDisplayIndex] + 1) % ENCODINGS_LENGTH;
        break;
      }
    case Direction::DOWN:
      {
        displayState[currentDisplayIndex] = displayState[currentDisplayIndex] == 0 ? ENCODINGS_LENGTH - 1 : displayState[currentDisplayIndex] - 1;
        break;
      }
    default:
      {
        break;
      }
  }
}

void handleInput() {
  Direction dir = getJoyDirection();
  readButtonState(onButtonStateChange, RISING);

  if (dir == Direction::NONE) {
    return;
  }
  if (canCycleDisplays) {
    cycleDisplays(dir);
  } else {
    updateDigit(dir);
  }
}

void writeReg(uint8_t encoding) {
  // ST_CP LOW to keep LEDs from changing while reading serial data
  digitalWrite(LATCH_PIN, LOW);
  // Shift out the bits
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, encoding);
  /* ST_CP on Rising to move the data from shift register
     * to storage register, making the bits available for output.
     */
  digitalWrite(LATCH_PIN, HIGH);
}

void setDisplay(uint8_t displayIndex, uint8_t encoding) {
  for (int i = 0; i < DISPLAY_COUNT; i++) {
    digitalWrite(DISPLAYS[i], HIGH);  // turn off all the displays
  }

  writeReg(encoding);
  digitalWrite(DISPLAYS[displayIndex], LOW);  // activate the display you want to use
}

void printDisplays() {
  if (shouldDpBlink) {
    blinkDp();
  }
  for (int i = 0; i < DISPLAY_COUNT; i++) {
    setDisplay(i, getEncodingWithDp(ENCODINGS[displayState[i]], i));
    delay(5);
  }
}

void setup() {
  pinSetup();
  Serial.begin(9600);
}

void loop() {
  handleInput();
  printDisplays();
}