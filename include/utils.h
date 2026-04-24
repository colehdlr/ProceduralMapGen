#include "types.h"
#include "raymath.h"

Node *createNode();

Vector2Int stepInDirection(Edge direction, Vector2Int location);

Edge getOppositeSide(Edge side);

Vector3 offsetByX(Vector3 position, int x);

Vector3 offsetByY(Vector3 position, int y);

Vector3 offsetByZ(Vector3 position, int z);
