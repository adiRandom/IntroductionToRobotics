#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include <stdint.h>
#include "Utils.h"

class Collider {
private:
  BoundingBox _boundingBox;
public:
  Collider(BoundingBox boundingBox);
  bool isTouching(Collider other) const;
  bool isColliding(Collider other) const;
  void updateBoundingBox(BoundingBox boundingBox);
};

#endif