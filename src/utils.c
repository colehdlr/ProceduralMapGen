#include <stdlib.h>

#include "types.h"

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
