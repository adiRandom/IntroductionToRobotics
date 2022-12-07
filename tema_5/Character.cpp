#include "Character.hpp"


Character::Character()
  : _origin{ Point{ 0, 0 } },
    _collider{ Collider(getBoundingBox()) },
    _orientation{ Orientation::RIGHT },
    _state{ State::IDLE } {
}

Character::Character(Point initialPosition, Orientation orientation = Orientation::RIGHT)
  : _origin{ initialPosition },
    _collider{ Collider(getBoundingBox()) },
    _orientation{ orientation },
    _state{ State::IDLE } {
}

List<Pixel> Character::getPixels(int32_t displayHeight) const {
  List<Pixel> result;
  switch (_state) {
    case State::IDLE:
      {
        if (_orientation == Orientation::RIGHT) {
          result.push(Point{ _origin.getX(), _origin.getY() - 1 }.toPixel(displayHeight));
          result.push(_origin.toPixel(displayHeight));
          result.push(Point{ _origin.getX(), _origin.getY() + 1 }.toPixel(displayHeight));
          result.push(Point{ _origin.getX() + 1, _origin.getY() }.toPixel(displayHeight));
        }
        break;
      }
    case State::PUNCHIG:
      {
        if (_orientation == Orientation::RIGHT) {
          result.push(Point{ _origin.getX(), _origin.getY() - 1 }.toPixel(displayHeight));
          result.push(_origin.toPixel(displayHeight));
          result.push(Point{ _origin.getX(), _origin.getY() + 1 }.toPixel(displayHeight));
          result.push(Point{ _origin.getX() + 1, _origin.getY() }.toPixel(displayHeight));
          result.push(Point{ _origin.getX() + 2, _origin.getY() }.toPixel(displayHeight));
        }
        break;
      }
    default:
      {
        break;
      }
  }
  return result;
}

BoundingBox Character::getBoundingBox() {
  switch (_state) {
    case Character::State::IDLE:
      {
        if (_orientation == Orientation::RIGHT) {
          return BoundingBox{
            Point{ _origin.getX(), _origin.getY() + 1 },
            Point{ _origin.getX() + 1, _origin.getY() - 1 },
          };
        } else {
          return BoundingBox{
            Point{ _origin.getX() - 1, _origin.getY() + 1 },
            Point{ _origin.getX(), _origin.getY() - 1 },
          };
        }
      }
    default:
      {
        return BoundingBox{
          Point{ 0, 0 },
          Point{ 0, 0 }
        };
      }
  }
}

void Character::moveRight() {
  // TODO: Handle colission
  _origin.updateX(1);
}

void Character::moveLeft() {
  // TODO: Handle colission
  _origin.updateX(-1);
}

bool Character::punch() {
  switch (_state) {
    case State::PUNCHIG:
    case State::CROUCHED_PUNCHING:
    case State::JUMPING:
      {
        return false;
      }
    case State::CROUCHED:
      {
        _state = State::CROUCHED_PUNCHING;
        break;
      }
    default:
      {
        _state = State::PUNCHIG;
        break;
      }
  }
  _punchingTimer = millis();
  return true;
}

bool Character::runAnimations() {
  uint32_t now = millis();

  switch (_state) {
    case State::PUNCHIG:
      {
        if (now - _punchingTimer > PUNCH_ANIMATION_TIME) {
          _state = State::IDLE;
          return true;
        }
      }
    case State::CROUCHED_PUNCHING:
      {
        if (now - _punchingTimer > PUNCH_ANIMATION_TIME) {
          _state = State::CROUCHED;
          return true;
        }
      }
    default:
      {
        return false;
      }
  }
}