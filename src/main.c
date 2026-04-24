#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "main.h"
#include "types.h"
#include "utils.h"

void growWorld(Node *world[WORLD_CACHE_LENGTH][WORLD_CACHE_LENGTH], Node *head, Vector2Int position, int depth, Edge entry) {
  world[position.x][position.y] = head;

  int availableSides = NUM_SIDES;

  Node **sides[NUM_SIDES] = {&head->left, &head->right, &head->back, &head->front};

  if (entry != NO_EDGE) {
    availableSides--;
  }

  if (depth == 0) return;

  do {
    Edge side = rand() % (NUM_SIDES - 1);
    if (side >= entry) side++;

    Vector2Int newPosition = stepInDirection(side, position);

    if (*sides[side] == NULL && world[newPosition.x][newPosition.y] == NULL) {
      *sides[side] = createNode(); 
      switch (getOppositeSide(side)) {
        case LEFT:
          (*sides[side])->left = head;
          break;
        case RIGHT:
          (*sides[side])->right = head;
          break;
        case BACK:
          (*sides[side])->back = head;
          break;
        case FRONT:
          (*sides[side])->front = head;
          break;
        default:
          break;
      }

      growWorld(world, *sides[side], newPosition, depth - 1, getOppositeSide(side));
      availableSides--;
    }
  }
  while (availableSides > 0 && rand() % RAND_LIMIT != 0);
}

void drawRooms(Node *head, Vector3 position, Models *models, Edge entry) {
  // Floor
  DrawModel(*models->floor, position, 1.0f, WHITE);

  // Ceiling
  DrawModel(*models->ceiling, offsetByY(position, ROOM_HEIGHT), 1.0f, WHITE);

  if (head) {
    if (head->left == NULL) {
      models->wall->transform = MatrixRotateXYZ((Vector3){DEG2RAD*-90, 0, DEG2RAD*-90});
      DrawModel(*models->wall, Vector3Add(position, (Vector3){-ROOM_WIDTH/2, ROOM_HEIGHT/2, 0}), 1.0f, WHITE);
    }
    else if (entry != LEFT) {
      drawRooms(head->left, offsetByX(position, -ROOM_WIDTH), models, getOppositeSide(LEFT));
    }

    if (head->right == NULL) {
      models->wall->transform = MatrixRotateXYZ((Vector3){DEG2RAD*-90, 0, DEG2RAD*90});
      DrawModel(*models->wall, Vector3Add(position, (Vector3){ROOM_WIDTH/2, ROOM_HEIGHT/2, 0}), 1.0f, WHITE);
    }
    else if (entry != RIGHT) {
      drawRooms(head->right, offsetByX(position, ROOM_WIDTH), models, getOppositeSide(RIGHT));
    }
    
    if (head->back == NULL) {
      models->wall->transform = MatrixRotateXYZ((Vector3){-DEG2RAD*90, 0, DEG2RAD*-180});
      DrawModel(*models->wall, Vector3Add(position, (Vector3){0, ROOM_HEIGHT/2, -ROOM_WIDTH/2}), 1.0f, WHITE);
    }
    else if (entry != BACK) {
      drawRooms(head->back, offsetByZ(position, -ROOM_WIDTH), models, getOppositeSide(BACK));
    }

    if (head->front == NULL) {
      models->wall->transform = MatrixRotateXYZ((Vector3){DEG2RAD*90, DEG2RAD*180, DEG2RAD*180});
      DrawModel(*models->wall, Vector3Add(position, (Vector3){0, ROOM_HEIGHT/2, ROOM_WIDTH/2}), 1.0f, WHITE);
    }
    else if (entry != FRONT) {
      drawRooms(head->front, offsetByZ(position, ROOM_WIDTH), models, getOppositeSide(FRONT));
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
  camera.position = (Vector3){ 0.0f, 2.0f, 0.0f };
  camera.target   = (Vector3){ 0.0f, 2.0f, 1.0f };
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

  // Create world 
  srand(time(NULL));

  Node *world[WORLD_CACHE_LENGTH][WORLD_CACHE_LENGTH] = {0};
  Node *head = createNode();
  growWorld(world, head, (Vector2Int){WORLD_CACHE_LENGTH/2, WORLD_CACHE_LENGTH/2}, RENDER_DEPTH, NO_EDGE);

  DisableCursor();
  SetTargetFPS(144);

  while (!WindowShouldClose())
  {
      // Update camera
      UpdateCamera(&camera, CAMERA_FIRST_PERSON);

      // Draw
      BeginDrawing();
      ClearBackground(BLACK);

      BeginMode3D(camera);

      drawRooms(head, (Vector3){0, 0, 0}, &models, NO_EDGE);

      EndMode3D();

      DrawFPS(10, 10);
      Vector2Int worldPos = convertPositionToWorld(camera.position);
      DrawText(TextFormat("position: %d %d", worldPos.x , worldPos.y), 10, 50, 20, GREEN);

      EndDrawing();
  }

  CloseWindow();
  return 0;
}
