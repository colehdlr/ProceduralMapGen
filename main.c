#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "main.h"

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

void maybeGrowTreeToSide(Node *head, Node **sideRef, Edge side, Edge entry, int amount) {
  if (side != entry) {
    if (rand() % RAND_LIMIT == 1) {
      *sideRef = createNode();
      growTree(*sideRef, amount - 1, getOppositeSide(side));
    }
    else {
      head->walls[side] = 1;
    }
  }
}

void growTree(Node *head, int amount, Edge entry) {
  if (amount > 0) {
    maybeGrowTreeToSide(head, &head->left, LEFT, entry, amount);
    maybeGrowTreeToSide(head, &head->right, RIGHT, entry, amount);
    maybeGrowTreeToSide(head, &head->back, BACK, entry, amount);
    maybeGrowTreeToSide(head, &head->front, FRONT, entry, amount);
  }
  else {
    // Leaf node
    for (int i = LEFT; i <= FRONT; i++) {
      if (i != entry) {
        head->walls[i] = 1;
      }
    }
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

void drawRooms(Node *head, Vector3 position) {
  // Floor
  DrawPlane(position, (Vector2){ ROOM_WIDTH, ROOM_WIDTH }, RED);
  // Ceiling
  DrawCube(offsetByY(position, ROOM_HEIGHT), ROOM_WIDTH, 0.01f, ROOM_WIDTH, GRAY);
  // Edges
  for (int i = LEFT; i <= FRONT; i++) {
    if (head->walls[i] == 1) {
      switch (i) {
        case LEFT:
          DrawCube(Vector3Add(position, (Vector3){-ROOM_WIDTH/2, ROOM_HEIGHT/2, 0}), WALL_THICKNESS, ROOM_HEIGHT, WALL_WIDTH, YELLOW);
          break;
        case RIGHT:
          DrawCube(Vector3Add(position, (Vector3){ROOM_WIDTH/2, ROOM_HEIGHT/2, 0}), WALL_THICKNESS, ROOM_HEIGHT, WALL_WIDTH, GREEN);
          break;
        case BACK:
          DrawCube(Vector3Add(position, (Vector3){0, ROOM_HEIGHT/2, -ROOM_WIDTH/2}), WALL_WIDTH, ROOM_HEIGHT, WALL_THICKNESS, BLUE);
          break;
        case FRONT:
          DrawCube(Vector3Add(position, (Vector3){0, ROOM_HEIGHT/2, ROOM_WIDTH/2}), WALL_WIDTH, ROOM_HEIGHT, WALL_THICKNESS, BLUE);
          break;
      }
    }
  }

  if (head) {
    if (head->left) {
      drawRooms(head->left, offsetByX(position, -ROOM_WIDTH));
    }
    if (head->right) {
      drawRooms(head->right, offsetByX(position, ROOM_WIDTH));
    }
    if (head->back) {
      drawRooms(head->back, offsetByZ(position, -ROOM_WIDTH));
    }
    if (head->front) {
      drawRooms(head->front, offsetByZ(position, ROOM_WIDTH));
    }
  }
}

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  SetConfigFlags(FLAG_MSAA_4X_HINT);

  InitWindow(screenWidth, screenHeight, "Procedural Generation Demo");

  // Camera setup
  Camera3D camera = { 0 };
  camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
  camera.target   = (Vector3){ 0.0f, 2.0f, 0.0f };
  camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
  camera.fovy     = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // Create tree
  srand(time(NULL));

  Node *head = createNode();
  growTree(head, 3, NO_EDGE);

  int cameraMode = CAMERA_FIRST_PERSON;

  DisableCursor();
  SetTargetFPS(80);

  while (!WindowShouldClose())
  {
      // Update camera
      UpdateCamera(&camera, cameraMode);

      // Draw
      BeginDrawing();
      ClearBackground(RAYWHITE);

      BeginMode3D(camera);

      drawRooms(head, (Vector3){0, 0, 5});

      EndMode3D();

      DrawFPS(10, 10);

      EndDrawing();
  }

  CloseWindow();
  return 0;
}
