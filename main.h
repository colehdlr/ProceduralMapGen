#define ROOM_WIDTH 8
#define ROOM_HEIGHT 4
#define RAND_LIMIT 3
#define DOORS_ARRAY_SIZE 4

enum Edge {
  LEFT,
  RIGHT,
  BACK,
  FRONT,
  NO_EDGE
};

struct Node {
  enum Edge doors[DOORS_ARRAY_SIZE];
  int doorLen;
  struct Node *left;
  struct Node *right;
  struct Node *front;
  struct Node *back;
};

void growTree(struct Node *head, int amount, enum Edge entry);
