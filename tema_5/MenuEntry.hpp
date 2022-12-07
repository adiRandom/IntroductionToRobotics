#ifndef MENU_ENTRY_HPP
#define MENU_ENTRY_HPP
#include "Utils.h"

class MenuEntry {
private:

  uint16_t _id;
  Point _selectorPos;
public:

  MenuEntry();
  MenuEntry(uint16_t id, Point selectorPos);
  MenuEntry(MenuEntry const& menuEntry);
  MenuEntry& operator=(MenuEntry const& menuEntry);

  uint16_t getId() const;
  Point getSelectorPos() const;
};

#endif