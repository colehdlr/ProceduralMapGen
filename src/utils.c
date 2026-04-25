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

Vector2Int stepInDirection(Edge direction, Vector2Int location) {
  switch (direction) {
    case LEFT:
      location.x--;
      return location;
    case RIGHT:
      location.x++;
      return location;
    case BACK:
      location.y--;
      return location;
    case FRONT:
      location.y++;
      return location;
    default:
      return (Vector2Int){0, 0};
  }
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

Vector2Int convertPositionToWorld(Vector3 position) {
  return (Vector2Int){
    round(position.x/ROOM_WIDTH) + WORLD_CACHE_LENGTH/2,
    round(position.z/ROOM_WIDTH) + WORLD_CACHE_LENGTH/2
  };
}

bool sameVector2Int(Vector2Int a, Vector2Int b) {
  return a.x == b.x && a.y == b.y;
}
