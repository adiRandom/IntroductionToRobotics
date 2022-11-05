uint8_t const BUTTON_PIN = 8;
uint8_t const CAR_RED_LED_PIN = 9;
uint8_t const CAR_YELLOW_LED_PIN = 10;
uint8_t const CAR_GREEN_LED_PIN = 11;
uint8_t const CROSSWALK_RED_LED_PIN = 12;
uint8_t const CROSSWALK_GREEN_LED_PIN = 13;
uint8_t const BUZZER_PIN = 3;
uint8_t const BUZZER_TONE = 1600;

uint8_t const INPUT_DEBOUNCE_TIME = 50;
uint32_t const GREEN_TO_YELLO_CAR_LIGHT_STATE_DELAY = 8 * 1000;
uint32_t const YELLOW_TO_RED_CAR_LIGHT_STATE_DELAY = 3 * 1000;
uint32_t const RED_TO_READY_CAR_LIGHT_STATE_DELAY = 8 * 1000;
uint32_t const READY_TO_GREEN_CAR_LIGHT_STATE_DELAY = 4 * 1000;
uint32_t const CROSSWALK_GREEN_LED_BLINK_INTERVAL = 300;
uint32_t const BUZZER_REGULAR_INTERVAl = 300;
uint32_t const BUZZER_FAST_INTERVAl = 150;


byte lastButtonReadingState = HIGH;
byte buttonState = HIGH;
uint32_t lastButtonReadingDebounce = 0;
int32_t stateChangeTimer = -1;
uint8_t activeCarLedPin = -1;
uint8_t activeCrosswalkLedPin = -1;
uint32_t lastBlinkTime = 0;
byte crosswalkGreenLedState = HIGH;
bool isBuzzing = false;
uint32_t lastBuzzTime = 0;

enum CarLightState {
  GREEN,
  YELLOW,
  RED,
  // The state when the crosswalk light is blinking
  READY
};

// Using the state of the car lights we can also determine the state of the crosswalk lighs
CarLightState carLightState = CarLightState::GREEN;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(CAR_RED_LED_PIN, OUTPUT);
  pinMode(CAR_YELLOW_LED_PIN, OUTPUT);
  pinMode(CAR_GREEN_LED_PIN, OUTPUT);
  pinMode(CROSSWALK_RED_LED_PIN, OUTPUT);
  pinMode(CROSSWALK_GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void readButtonState(void (*onStateChange)(byte), uint8_t notifyOn = CHANGE) {
  byte buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonReadingState) {
    lastButtonReadingDebounce = millis();
  }

  if (millis() - lastButtonReadingDebounce > INPUT_DEBOUNCE_TIME) {
    if (buttonReading != buttonState) {
      buttonState = buttonReading;

      if ((notifyOn == CHANGE) || (notifyOn == RISING && buttonState == HIGH) || (notifyOn == FALLING && buttonState == LOW)) {
        onStateChange(buttonState);
      }
    }
  }

  lastButtonReadingState = buttonReading;
}

void startTransitionToSecondState() {
  if (stateChangeTimer != -1) {
    // Timer already started
    return;
  }

  if (carLightState != CarLightState::GREEN) {
    // The button triggers an action only in the first state
    return;
  }

  stateChangeTimer = millis();
}

void handleStateChange() {
  if (stateChangeTimer == -1) {
    return;
  }

  uint32_t now = millis();

  switch (carLightState) {
    case CarLightState::GREEN:
      {
        if (now - stateChangeTimer >= GREEN_TO_YELLO_CAR_LIGHT_STATE_DELAY) {
          carLightState = CarLightState::YELLOW;
          stateChangeTimer = now;
        }

        return;
      }

    case CarLightState::YELLOW:
      {
        if (now - stateChangeTimer >= YELLOW_TO_RED_CAR_LIGHT_STATE_DELAY) {
          carLightState = CarLightState::RED;
          stateChangeTimer = now;
        }

        return;
      }

    case CarLightState::RED:
      {
        if (now - stateChangeTimer >= RED_TO_READY_CAR_LIGHT_STATE_DELAY) {
          carLightState = CarLightState::READY;
          stateChangeTimer = now;
        }

        return;
      }

    case CarLightState::READY:
      {
        if (now - stateChangeTimer >= READY_TO_GREEN_CAR_LIGHT_STATE_DELAY) {
          carLightState = CarLightState::GREEN;
          // Transition stops after this
          stateChangeTimer = -1;
        }

        return;
      }

    default:
      {
        return;
      }
  }
}

void handleCarLedState(CarLightState carLightState) {

  switch (carLightState) {
    case CarLightState::RED:
    case CarLightState::READY:
      {
        if (activeCarLedPin != CAR_RED_LED_PIN) {
          digitalWrite(activeCarLedPin, LOW);

          activeCarLedPin = CAR_RED_LED_PIN;
          digitalWrite(activeCarLedPin, HIGH);
        }
        return;
      }

    case CarLightState::YELLOW:
      {
        if (activeCarLedPin != CAR_YELLOW_LED_PIN) {
          digitalWrite(activeCarLedPin, LOW);

          activeCarLedPin = CAR_YELLOW_LED_PIN;
          digitalWrite(activeCarLedPin, HIGH);
        }
        return;
      }

    case CarLightState::GREEN:
      {
        if (activeCarLedPin != CAR_GREEN_LED_PIN) {
          digitalWrite(activeCarLedPin, LOW);

          activeCarLedPin = CAR_GREEN_LED_PIN;
          digitalWrite(activeCarLedPin, HIGH);
        }
        return;
      }
  }
}

void handleCrosswalkLedState(CarLightState carLightState) {

  switch (carLightState) {
    case CarLightState::RED:
      {
        if (activeCrosswalkLedPin != CROSSWALK_GREEN_LED_PIN) {
          digitalWrite(activeCrosswalkLedPin, LOW);

          activeCrosswalkLedPin = CROSSWALK_GREEN_LED_PIN;
          crosswalkGreenLedState = HIGH;
          digitalWrite(activeCrosswalkLedPin, crosswalkGreenLedState);
        }
        return;
      }

    case CarLightState::YELLOW:
    case CarLightState::GREEN:
      {
        if (activeCrosswalkLedPin != CROSSWALK_RED_LED_PIN) {
          digitalWrite(activeCrosswalkLedPin, LOW);

          activeCrosswalkLedPin = CROSSWALK_RED_LED_PIN;
          digitalWrite(activeCrosswalkLedPin, HIGH);
        }
        return;
      }

    case CarLightState::READY:
      {
        // We know the green led is already the active led
        uint32_t now = millis();

        if (now - lastBlinkTime >= CROSSWALK_GREEN_LED_BLINK_INTERVAL) {
          crosswalkGreenLedState = !crosswalkGreenLedState;
          lastBlinkTime = now;
          digitalWrite(activeCrosswalkLedPin, crosswalkGreenLedState);
        }

        return;
      }
  }
}

void handleBuzzerTone(CarLightState carLightState) {
  switch (carLightState) {
    case CarLightState::GREEN:
    case CarLightState::YELLOW:
      {
        if (isBuzzing) {
          noTone(BUZZER_PIN);
          isBuzzing = false;
          return;
        }
      }
    case CarLightState::RED:
      {
        uint32_t now = millis();
        if (now - lastBuzzTime < BUZZER_REGULAR_INTERVAl) {
          return;
        }

        if (!isBuzzing) {
          tone(BUZZER_PIN, BUZZER_TONE);
          isBuzzing = true;
        } else {
          noTone(BUZZER_PIN);
          isBuzzing = false;
        }

        lastBuzzTime = now;
        return;
      }

    case CarLightState::READY:
      {
        uint32_t now = millis();
        if (now - lastBuzzTime < BUZZER_FAST_INTERVAl) {
          return;
        }

        if (!isBuzzing) {
          tone(BUZZER_PIN, BUZZER_TONE);
          isBuzzing = true;
        } else {
          noTone(BUZZER_PIN);
          isBuzzing = false;
        }

        lastBuzzTime = now;
        return;
      }
  }
}

void loop() {
  readButtonState(startTransitionToSecondState, RISING);
  handleCarLedState(carLightState);
  handleCrosswalkLedState(carLightState);
  handleBuzzerTone(carLightState);
  handleStateChange();
}