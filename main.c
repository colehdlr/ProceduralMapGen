#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "main.h"

struct Node *createNode() {
    struct Node *node = malloc(sizeof(struct Node));
    node->doorsLen = 0;
    node->wallsLen = 0;
    node->left = NULL;
    node->right = NULL;
    node->back = NULL;
    node->front = NULL;
    return node;
}

void addWallToNode(struct Node *head, enum Edge side) {
  head->walls[head->wallsLen] = side;
  head->wallsLen++;
}

void addDoorToNode(struct Node *head, enum Edge side) {
  head->doors[head->doorsLen] = side;
  head->doorsLen++;
}

void maybeGrowTreeToSide(struct Node *head, struct Node **sideRef, enum Edge side, enum Edge entry, int amount) {
  if (side != entry) {
    if (rand() % RAND_LIMIT == 1) {
      *sideRef = createNode();
      addDoorToNode(head, side);
      growTree(*sideRef, amount - 1, side);
    }
    else {
      addWallToNode(head, side);
    }
  }
}

void growTree(struct Node *head, int amount, enum Edge entry) {
  addDoorToNode(head, entry);

  if (amount > 0) {
    maybeGrowTreeToSide(head, &head->left, LEFT, entry, amount);
    maybeGrowTreeToSide(head, &head->right, RIGHT, entry, amount);
    maybeGrowTreeToSide(head, &head->back, BACK, entry, amount);
    maybeGrowTreeToSide(head, &head->front, FRONT, entry, amount);
  }
  else {
    // Leaf node
    // TODO: Add walls to all sides other than entry
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

void drawRooms(struct Node *head, Vector3 position) {
  // Floor
  DrawPlane(position, (Vector2){ ROOM_WIDTH, ROOM_WIDTH }, RED);
  // Ceiling
  DrawCube(offsetByY(position, ROOM_HEIGHT), ROOM_WIDTH, 0.01f, ROOM_WIDTH, GRAY);
  // Edges
  // TODO: Why so many walls, and no doors? FIX
  for (int i = 0; i < head->wallsLen; i++) {
    switch (head->walls[i]) {
      case LEFT:
        DrawCube(Vector3Add(position, (Vector3){-ROOM_WIDTH/2, ROOM_HEIGHT/2, 0}), 0.01f, ROOM_HEIGHT, ROOM_WIDTH, YELLOW);
        break;
      case RIGHT:
        DrawCube(Vector3Add(position, (Vector3){ROOM_WIDTH/2, ROOM_HEIGHT/2, 0}), 0.01f, ROOM_HEIGHT, ROOM_WIDTH, GREEN);
        break;
      default:
        break;
    }
  }
  for (int i = 0; i < head->doorsLen; i++) {
    // When we add doored walls we can extract the logic
    // from above to avoid duplication.
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

  struct Node *head = createNode();
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
