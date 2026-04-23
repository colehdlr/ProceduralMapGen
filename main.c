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

typedef struct Models {
  Model *ceiling;
  Model *floor;
  Model *wall;
} Models;

void drawRooms(Node *head, Vector3 position, Models *models) {
  // Floor
  DrawModel(*models->floor, position, 1.0f, WHITE);

  // Ceiling
  DrawModel(*models->ceiling, offsetByY(position, ROOM_HEIGHT), 1.0f, WHITE);

  // Edges
  for (int i = LEFT; i <= FRONT; i++) {
    if (head->walls[i] == 1) {
      switch (i) {
        case LEFT:
          models->wall->transform = MatrixRotateXYZ((Vector3){DEG2RAD*-90, 0, DEG2RAD*-90});
          DrawModel(*models->wall, Vector3Add(position, (Vector3){-ROOM_WIDTH/2, ROOM_HEIGHT/2, 0}), 1.0f, WHITE);
          break;
        case RIGHT:
          models->wall->transform = MatrixRotateXYZ((Vector3){DEG2RAD*-90, 0, DEG2RAD*90});
          DrawModel(*models->wall, Vector3Add(position, (Vector3){ROOM_WIDTH/2, ROOM_HEIGHT/2, 0}), 1.0f, WHITE);
          break;
        case BACK:
          models->wall->transform = MatrixRotateXYZ((Vector3){-DEG2RAD*90, 0, DEG2RAD*-180});
          DrawModel(*models->wall, Vector3Add(position, (Vector3){0, ROOM_HEIGHT/2, -ROOM_WIDTH/2}), 1.0f, WHITE);
          break;
        case FRONT:
          models->wall->transform = MatrixRotateXYZ((Vector3){DEG2RAD*90, DEG2RAD*180, DEG2RAD*180});
          DrawModel(*models->wall, Vector3Add(position, (Vector3){0, ROOM_HEIGHT/2, ROOM_WIDTH/2}), 1.0f, WHITE);
          break;
      }
    }
  }

  if (head) {
    if (head->left) {
      drawRooms(head->left, offsetByX(position, -ROOM_WIDTH), models);
    }
    if (head->right) {
      drawRooms(head->right, offsetByX(position, ROOM_WIDTH), models);
    }
    if (head->back) {
      drawRooms(head->back, offsetByZ(position, -ROOM_WIDTH), models);
    }
    if (head->front) {
      drawRooms(head->front, offsetByZ(position, ROOM_WIDTH), models);
    }
  }
}

int main(void)
{
  const int screenWidth = 1400;
  const int screenHeight = 900;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_VSYNC_HINT);

  InitWindow(screenWidth, screenHeight, "Procedural Generation Demo");

  // Camera setup
  Camera3D camera = { 0 };
  camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
  camera.target   = (Vector3){ 0.0f, 2.0f, 0.0f };
  camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
  camera.fovy     =  60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // Load textures
  Image floorImage = LoadImage("assets/floor.jpg");
  Image ceilingImage = LoadImage("assets/ceiling.jpg");
  Image wallImage = LoadImage("assets/wall.jpg");
  Texture2D floorTexture = LoadTextureFromImage(floorImage);
  Texture2D ceilingTexture = LoadTextureFromImage(ceilingImage);
  Texture2D wallTexture = LoadTextureFromImage(wallImage);
  UnloadImage(floorImage);
  UnloadImage(ceilingImage);
  UnloadImage(wallImage);

  // Load models
  Models models;

  Model ceiling = LoadModelFromMesh(GenMeshPlane(ROOM_WIDTH, ROOM_WIDTH, 1, 1));
  ceiling.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ceilingTexture;
  ceiling.transform = MatrixRotateX(DEG2RAD*180);
  models.ceiling = &ceiling;

  Model floor = LoadModelFromMesh(GenMeshPlane(ROOM_WIDTH, ROOM_WIDTH, 1, 1));
  floor.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = floorTexture;
  models.floor = &floor;

  Model wall = LoadModelFromMesh(GenMeshPlane(ROOM_WIDTH, ROOM_HEIGHT, 1, 1));
  wall.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;
  models.wall = &wall;

  // Create tree
  srand(time(NULL));

  Node *head = createNode();
  growTree(head, 3, NO_EDGE);

  int cameraMode = CAMERA_FIRST_PERSON;

  DisableCursor();
  SetTargetFPS(144);

  while (!WindowShouldClose())
  {
      // Update camera
      UpdateCamera(&camera, cameraMode);

      // Draw
      BeginDrawing();
      ClearBackground(GREEN);

      BeginMode3D(camera);

      drawRooms(head, (Vector3){0, 0, 5}, &models);

      EndMode3D();

      DrawFPS(10, 10);

      EndDrawing();
  }

  CloseWindow();
  return 0;
}
