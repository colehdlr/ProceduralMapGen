#define ROOM_WIDTH 8
#define ROOM_HEIGHT 4
#define RAND_LIMIT 2
#define DOORS_ARRAY_SIZE 4
#define WALLS_ARRAY_SIZE 4

enum Edge {
  LEFT,
  RIGHT,
  BACK,
  FRONT,
  NO_EDGE
};

struct Node {
  enum Edge walls[WALLS_ARRAY_SIZE];
  struct Node *left;
  struct Node *right;
  struct Node *front;
  struct Node *back;
};

void growTree(struct Node *head, int amount, enum Edge entry);
