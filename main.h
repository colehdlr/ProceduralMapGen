#define ROOM_WIDTH 16
#define ROOM_HEIGHT 4
#define RAND_LIMIT 2
#define WALLS_ARRAY_SIZE 4

typedef enum Edge {
  LEFT,
  RIGHT,
  BACK,
  FRONT,
  NO_EDGE
} Edge;

typedef struct Node {
  enum Edge walls[WALLS_ARRAY_SIZE];
  struct Node *left;
  struct Node *right;
  struct Node *front;
  struct Node *back;
} Node;

void growTree(struct Node *head, int amount, enum Edge entry);
