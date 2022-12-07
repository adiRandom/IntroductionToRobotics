#include "LCDController.hpp"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "MemoryFree.h"

LCDController::LCDController()
  : _rsPin{ 0 },
    _enablePin{ 0 },
    _d4Pin{ 0 },
    _d5Pin{ 0 },
    _d6Pin{ 0 },
    _d7Pin{ 0 },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ 0 },
    _height{ 0 },
    _contrastPin{ 0 } {
}

LCDController::LCDController(
  uint8_t rsPin,
  uint8_t enablePin,
  uint8_t d4Pin,
  uint8_t d5Pin,
  uint8_t d6Pin,
  uint8_t d7Pin,
  uint8_t height,
  uint8_t width,
  uint8_t contrastPin
)
  : _rsPin{ rsPin },
    _enablePin{ enablePin },
    _d4Pin{ d4Pin },
    _d5Pin{ d5Pin },
    _d6Pin{ d6Pin },
    _d7Pin{ d7Pin },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ width },
    _height{ height },
    _contrastPin{ contrastPin } {
  initDisplay();
}

LCDController::LCDController(LCDController const& other)
  : _rsPin{ other._rsPin },
    _enablePin{ other._enablePin },
    _d4Pin{ other._d4Pin },
    _d5Pin{ other._d5Pin },
    _d6Pin{ other._d6Pin },
    _d7Pin{ other._d7Pin },
    _lcd{ LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin) },
    _introTimer{ 0 },
    _width{ other._width },
    _height{ other._height },
    _contrastPin{ other._contrastPin },
    _selectedEntry{ other._selectedEntry } {

  initDisplay();
}

LCDController& LCDController::operator=(LCDController const& other) {
  if (&other == this) {
    return *this;
  }
  _rsPin = other._rsPin;
  _enablePin = other._enablePin;
  _d4Pin = other._d4Pin;
  _d5Pin = other._d5Pin;
  _d6Pin = other._d6Pin;
  _d7Pin = other._d7Pin;
  _lcd = LiquidCrystal(_rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin);
  _introTimer = 0;
  _width = other._width;
  _height = other._height;
  _contrastPin = other._contrastPin;
  _selectedEntry = other._selectedEntry;

  initDisplay();

  return *this;
}

void LCDController::displayCurrentState(char const introMessage[INTRO_MESSAGE_SIZE]) {
  displaySelector();

  if (_state == _lastState && _state != State::INTRO) {
    return;
  }
  switch (_state) {
    case State::INTRO:
      {
        showIntro(introMessage);
        break;
      }
    case State::MENU:
      {
        showMainMenu();
        break;
      }
    case State::ABOUT:
      {
        showAbout(0);
        break;
      }
    case State::GAME:
      {
        showGame();
        break;
      }
    case State::HELP:
      {
        showHelp(0);
        break;
      }
    default:
      {
        break;
      }
  }

  // The screen was cleared due to the change of state
  // Redisplay the selector
  displaySelector();
}

bool LCDController::isShowingGame() const {
  return _state == State::GAME;
}

void LCDController::displaySelector() {
  if (_selectedEntry == nullptr || _lastSelectedEntry == _selectedEntry || _selectedEntry->getSelectorPos().getX() == -1) {
    return;
  }

  _lcd.setCursor(
    _lastSelectedEntry->getSelectorPos().getX(),
    _lastSelectedEntry->getSelectorPos().getY()
  );

  _lcd.print(" ");

  _lcd.setCursor(
    _selectedEntry->getSelectorPos().getX(),
    _selectedEntry->getSelectorPos().getY()
  );
  _lcd.print(">");

  _lastSelectedEntry = _selectedEntry;
}

char const* LCDController::getMainMenuEntryName(uint16_t id) const {
  switch (id) {
    case MAIN_MENU_PLAY_ID:
      {
        return "PLAY";
      }
    case MAIN_MENU_SETTINGS_ID:
      {
        return "SETTINGS";
      }
    case MAIN_MENU_ABOUT_ID:
      {
        return "ABOUT";
      }
    case MAIN_MENU_HELP_ID:
      {
        return "HELP";
      }
    default:
      {
        return "";
      }
  }
}

char const* LCDController::getHelpMenuEntryName(uint16_t id) const {
  switch (id) {
    case HELP_MOVE_LINE_ID:
      {
        return "Move and crouch";
      }
    case HELP_JOYSTICK_LINE_ID:
      {
        return "With joystick";
      }
    case HELP_ATK_LINE_ID:
      {
        return "Hit - select btn";
      }
    case HELP_DEF_LINE_ID:
      {
        return "Block - back btn";
      }
    default:
      {
        return "";
      }
  }
}

char const* LCDController::getAboutMenuEntryName(uint16_t id) const {
  switch (id) {
    case ABOUT_GAME_NAME_ID:
      {
        return "MATRIX FIGHTER";
      }
    case ABOUT_CREATOR_NAME_ID:
      {
        return "Adrian Pascu";
      }
    case ABOUT_CREATOR_GITHUB_USERNAME_ID:
      {
        return "@adiRandom";
      }
    default:
      {
        return "";
      }
  }
}

void LCDController::initDisplay() {
  pinMode(_contrastPin, OUTPUT);
  _lcd.begin(_width, _height);
  analogWrite(_contrastPin, DEFAULT_CONTRAST);
}

void LCDController::moveSelector(Direction direction) {
  switch (_state) {
    case State::INTRO:
      {
        return;
      }
    case State::MENU:
      {
        moveMainMenuSelector(direction);
        break;
      }
    case State::ABOUT:
      {
        moveAboutMenuSelector(direction);
        break;
      }
    case State::HELP:
      {
        moveHelpMenuSelector(direction);
      }
    default:
      {
        break;
      }
  }
}

void LCDController::showIntro(char const introMessage[INTRO_MESSAGE_SIZE]) {
  if (_introTimer == 0) {
    _lcd.print(introMessage);
    _introTimer = millis();
  } else {
    if (millis() - _introTimer > INTRO_SHOW_TIME) {
      _state = State::MENU;
    }
  }
}

void LCDController::showMainMenu() {
  _lcd.clear();

  for (int i = 0; i < MAIN_MENU_SIZE; i++) {
    MenuEntry entry = _mainMenuEntries[i];
    _lcd.setCursor(entry.getSelectorPos().getX() + 1, entry.getSelectorPos().getY());
    _lcd.print(getMainMenuEntryName(entry.getId()));
  }
  _lastState = State::MENU;
  _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_ID];
}

void LCDController::moveMainMenuSelector(Direction direction) {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case MAIN_MENU_PLAY_ID:
      {
        if (direction.isRight()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_SETTINGS_ID];
        } else if (direction.isDown()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_ABOUT_ID];
        }
        break;
      }
    case MAIN_MENU_SETTINGS_ID:
      {
        if (direction.isLeft()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_ID];
        } else if (direction.isDown()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_HELP_ID];
        }
        break;
      }
    case MAIN_MENU_ABOUT_ID:
      {
        if (direction.isRight()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_HELP_ID];
        } else if (direction.isUp()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_PLAY_ID];
        }
        break;
      }
    case MAIN_MENU_HELP_ID:
      {
        if (direction.isLeft()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_ABOUT_ID];
        } else if (direction.isUp()) {
          _selectedEntry = &_mainMenuEntries[MAIN_MENU_SETTINGS_ID];
        }
        break;
      }
    default:
      {
        break;
      }
  }
}

void LCDController::select() {
  switch (_state) {
    case State::MENU:
      {
        selectInMainMenu();
        break;
      }
    default:
      {
        return;
      }
  }
}

void LCDController::selectInMainMenu() {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case MAIN_MENU_ABOUT_ID:
      {
        _state = State::ABOUT;
        break;
      }
    case MAIN_MENU_HELP_ID:
      {
        _state = State::HELP;
        break;
      }
    case MAIN_MENU_PLAY_ID:
      {
        _state = State::GAME;
        break;
      }
    default:
      {
        return;
      }
  }
}

void LCDController::moveAboutMenuSelector(Direction direction) {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case ABOUT_GAME_NAME_ID:
      {
        if (direction.isDown()) {
          showAbout(ABOUT_CREATOR_NAME_ID);
          _selectedEntry = &_aboutEntries[ABOUT_CREATOR_NAME_ID];
        }
        break;
      }
    case ABOUT_CREATOR_NAME_ID:
      {
        if (direction.isDown()) {
          _selectedEntry = &_aboutEntries[ABOUT_CREATOR_GITHUB_USERNAME_ID];
        } else if (direction.isUp()) {
          showAbout(ABOUT_GAME_NAME_ID);
          _selectedEntry = &_aboutEntries[ABOUT_GAME_NAME_ID];
        }
        break;
      }
    case ABOUT_CREATOR_GITHUB_USERNAME_ID:
      {
        if (direction.isUp()) {
          _selectedEntry = &_aboutEntries[ABOUT_CREATOR_NAME_ID];
        }
        break;
      }
    default:
      {
        return "";
      }
  }
}

void LCDController::moveHelpMenuSelector(Direction direction) {
  if (_selectedEntry == nullptr) {
    return;
  }

  switch (_selectedEntry->getId()) {
    case HELP_MOVE_LINE_ID:
      {
        if (direction.isDown()) {
          showHelp(HELP_JOYSTICK_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_JOYSTICK_LINE_ID];
        }
        break;
      }
    case HELP_JOYSTICK_LINE_ID:
      {
        if (direction.isDown()) {
          showHelp(HELP_ATK_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_ATK_LINE_ID];
        } else if (direction.isUp()) {
          showAbout(HELP_MOVE_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_MOVE_LINE_ID];
        }
        break;
      }
    case HELP_ATK_LINE_ID:
      {
        if (direction.isDown()) {
          showHelp(HELP_DEF_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_DEF_LINE_ID];
        } else if (direction.isUp()) {
          showAbout(HELP_JOYSTICK_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_JOYSTICK_LINE_ID];
        }
        break;
      }
    case HELP_DEF_LINE_ID:
      {
        if (direction.isUp()) {
          showHelp(HELP_ATK_LINE_ID);
          _selectedEntry = &_helpMenuEntries[HELP_ATK_LINE_ID];
        }
        break;
      }
    default:
      {
        return "";
      }
  }
}

void LCDController::showAbout(uint16_t topEntryIndex) {
  _lcd.clear();

  for (int i = 0; i < 2; i++) {
    MenuEntry entry = _aboutEntries[topEntryIndex + i];
    _lcd.setCursor(
      entry.getSelectorPos().getX() + 1,
      i
    );
    _lcd.print(getAboutMenuEntryName(entry.getId()));
  }
  _lastState = State::ABOUT;
  _selectedEntry = &_aboutEntries[ABOUT_GAME_NAME_ID];
}

void LCDController::showGame() {
  _lcd.clear();
  _lastState = State::GAME;
}

void LCDController::back() {
  switch (_state) {
    case State::ABOUT:
      {
        _state = State::MENU;
        break;
      }
    case State::HELP:
      {
        _state = State::MENU;
        _lcd.noAutoscroll();
        break;
      }
    default:
      {
        break;
      }
  }
}

void LCDController::showHelp(uint8_t topEntryIndex) {
  _lcd.clear();

  for (int i = 0; i < 2; i++) {
    MenuEntry entry = _helpMenuEntries[topEntryIndex + i];
    _lcd.setCursor(
      entry.getSelectorPos().getX() + 1,
      i
    );
    _lcd.print(getHelpMenuEntryName(entry.getId()));
  }
  _lastState = State::HELP;
  _selectedEntry = &_helpMenuEntries[HELP_MOVE_LINE_ID];
}