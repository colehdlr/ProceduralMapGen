#include <stdlib.h>

#include "types.h"
#include "raymath.h"

Node *createNode() {
  Node *node = malloc(sizeof(Node));
  node->left = NULL;
  node->right = NULL;
  node->back = NULL;
  node->front = NULL;
  return node;
}

Edge getOppositeSide(Edge side) {
  switch (side) {
    case LEFT:
      return RIGHT;
    case RIGHT:
      return LEFT;
    case BACK:
      return FRONT;
    case FRONT:
      return BACK;
    default:
      return NO_EDGE;
  }
}

Vector3 offsetByX(Vector3 position, int x) {
  position.x += x;
  return position;
}

Vector3 offsetByY(Vector3 position, int y) {
  position.y += y;
  return position;
}

Vector3 offsetByZ(Vector3 position, int z) {
  position.z += z;
  return position;
}
