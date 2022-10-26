uint8_t const RED_PIN = 9;
uint8_t const GREEN_PIN = 11;
uint8_t const BLUE_PIN = 10;
uint8_t const RED_ANALOG_INPUT_PIN = A0;
uint8_t const GREEN_ANALOG_INPUT_PIN = A1;
uint8_t const BLUE_ANALOG_INPUT_PIN = A2;

uint16_t const ANALOG_INPUT_MIN = 0;
uint16_t const ANALOG_INPUT_MAX = 1023;
uint8_t const LED_MIN = 0;
uint8_t const LED_MAX = 255;

struct Color {
  float red;
  float green;
  float blue;
};

void setLedColor(Color const& color) {
  analogWrite(RED_PIN, color.red);
  analogWrite(GREEN_PIN, color.green);
  analogWrite(BLUE_PIN, color.blue);
}

/**
 * Scale a number `input` from [inputMin, inputMax] to [outputMin, outputMax]
 */
float scale(float input, float inputMin, float inputMax, float outputMin, float outpuMax) {
  return (input - inputMin) * (outpuMax - outputMin) / (inputMax - inputMin) + outputMin;
}


float getColorFromAnalogInput(uint8_t inputPin) {
  uint16_t knobValue = analogRead(inputPin);
  return scale(knobValue, ANALOG_INPUT_MIN, ANALOG_INPUT_MAX, LED_MIN, LED_MAX);
}

Color getLedColor() {
  Color ledColor;
  ledColor.red = getColorFromAnalogInput(RED_ANALOG_INPUT_PIN);
  ledColor.green = getColorFromAnalogInput(GREEN_ANALOG_INPUT_PIN);
  ledColor.blue = getColorFromAnalogInput(BLUE_ANALOG_INPUT_PIN);

  return ledColor;
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);


  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Color ledColor = getLedColor();
  setLedColor(ledColor);
}