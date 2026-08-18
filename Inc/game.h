// Ball is 2x2 Pixels
typedef struct {
	int x1, x2;
	int y1, y2;
	int subX;
	int subY;
	int xV;
	int yV;
} Ball;

// Paddles are 2x8 Pixels
typedef struct {
	int x1, x2;
	int y1, y2;
	int xADD;
	int yADD;
} Paddle;

// Define functions
void Ball_Init(void);
void Paddle_Init(void);
void Score_Update(void);
