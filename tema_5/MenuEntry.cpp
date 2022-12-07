#include "MenuEntry.hpp"
#include <string.h>
#include <Arduino.h>
#include "MemoryFree.h"

MenuEntry::MenuEntry()
  : _id{ 0 }, _selectorPos{
      Point{ 0, 0 }
    } {
}

MenuEntry::MenuEntry(uint16_t id, Point selectorPos)
  : _selectorPos{ selectorPos }, _id{id} {
}

MenuEntry::MenuEntry(MenuEntry const& menuEntry)
  : _selectorPos{ menuEntry._selectorPos }, _id{menuEntry._id} {
}

MenuEntry& MenuEntry::operator=(MenuEntry const& menuEntry) {
  if (&menuEntry == this) {
    return *this;
  }

  _selectorPos = menuEntry._selectorPos;
  _id = menuEntry._id;
  return *this;
}

uint16_t MenuEntry::getId() const {
  return _id;
}
Point MenuEntry::getSelectorPos() const {
  return _selectorPos;
}