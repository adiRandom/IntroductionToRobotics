#include "DisplayController.hpp"
#include "Character.hpp"
#include "Utils.h"
#include "InputController.hpp"
#include "GameManager.hpp"
#include "LCDController.hpp"

uint8_t const DISPLAY_DATA_PIN = 12;
uint8_t const DISPLAY_CLK_PIN = 11;
uint8_t const DISPLAY_LOAD_PIN = 10;
uint8_t const HORIZONTAL_DISPLAY_COUNT = 1;
uint8_t const PLAYER1_JOYSTICK_X_PIN = A0;
uint8_t const PLAYER1_JOYSTICK_Y_PIN = A1;
uint8_t const PLAYER1_PRIMARY_BTN_PIN = 13;
uint8_t const PLAYER1_SECONDARY_BTN_PIN = A2;
uint8_t const LCD_RS_PIN = 9;
uint8_t const LCD_ENABLE_PIN = 8;
uint8_t const LCD_D4_PIN = 7;
uint8_t const LCD_D5_PIN = 6;
uint8_t const LCD_D6_PIN = 5;
uint8_t const LCD_D7_PIN = 4;
uint8_t const LCD_WIDTH = 16;
uint8_t const LCD_HEIGHT = 2;
uint8_t const LCD_CONTRAST_PIN = 3;
char const* GREETING = "HELLO BRAWLER!";

DisplayController displayController;
InputController inputController(
  PLAYER1_JOYSTICK_X_PIN,
  PLAYER1_JOYSTICK_Y_PIN,
  PLAYER1_PRIMARY_BTN_PIN,
  PLAYER1_SECONDARY_BTN_PIN,
  true,
  false,
  true
);
Character player1(Point{ 0, 1 });
GameManager gameManager;
LCDController lcdController(
  LCD_RS_PIN,
  LCD_ENABLE_PIN,
  LCD_D4_PIN,
  LCD_D5_PIN,
  LCD_D6_PIN,
  LCD_D7_PIN,
  LCD_HEIGHT,
  LCD_WIDTH,
  LCD_CONTRAST_PIN
);


void setup() {
  Serial.begin(9600);

  displayController = DisplayController(
    DISPLAY_DATA_PIN,
    DISPLAY_LOAD_PIN,
    DISPLAY_CLK_PIN
  );


  gameManager = GameManager(displayController, player1, inputController, lcdController);
}

void loop() {
  gameManager.handleInput();
  gameManager.getNextFrame();
  gameManager.getLCDState(GREETING);
}