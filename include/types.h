#pragma once

#include "constants.h"
#include "raylib.h"

typedef enum Edge {
  LEFT,
  RIGHT,
  BACK,
  FRONT,
  NO_EDGE
} Edge;

typedef struct Vector2Int {
  int x;
  int y;
} Vector2Int;

typedef struct Node {
  enum Edge walls[NUM_SIDES];
  struct Node *left;
  struct Node *right;
  struct Node *front;
  struct Node *back;
} Node;

typedef struct Models {
  Model *ceiling;
  Model *floor;
  Model *wall;
} Models;
