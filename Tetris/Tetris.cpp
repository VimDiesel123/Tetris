#include <stdio.h>
#include <sys/timeb.h>

//NOTE I switched to using system("cls") instead of the escape
// sequences but this is Windows only
#include <stdlib.h> /* for system("cls")*/
#include <windows.h> /* for GetAsyncKeyState */

#define bool char


const int FRAME_RATE = 1000 / 2;
const int BOARD_WIDTH = 12;
const int BOARD_HEIGHT = 21;
char board[BOARD_HEIGHT][BOARD_WIDTH];

int score = 0;


typedef struct
{
    int xPos;
    int yPos;
    int shape[4][4];
} Tetranimo;

//TODO "Empty Space above the square causes it to spawn in one line
//lower.
Tetranimo Square =
{
    0,
    0,
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
    },
};
Tetranimo L =
{
    0,
    0,
    {
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
    },
};

/* NOTE: Representing Player actions with an enum for now, 
 * may need to change to something more complicated in the future if this sucks
 */
typedef enum playerAction {
    IDLE, // TODO: Have an IDLE action right now if the player is not doing anything this 'game-tick'. Stupid? Change this? 
    MOVE_LEFT,
    MOVE_RIGHT,
    ROTATE,
    FORCE_DOWN,
    QUIT
} PlayerAction;

void initialize();
void update(bool *, PlayerAction);
void draw();
void teardown();


//TODO Right now I'm using a global ActivePiece and passing it to
//functions by pointer. This might need to change later or maybe just
//permanently draw the piece onto the board once it becomes inactive.
static Tetranimo ActivePiece;
void spawnPiece(Tetranimo* piece);
void movePieceLeft(Tetranimo* piece);

int main() {
    /* STUFF WE NEED:
        - 10 X 20 BOARD (DONE)
        - TETROMINOS
        - SCORE
        - TETRICALES FALL AT SPEED
        - ROTATE WITH ARROW KEYS
        - FALL INSANTLY WITH SPACE
    */

    bool game_over = false;

    struct timeb start, end;
    ftime(&start);
    int time_diff;
    ftime(&start);


    initialize();

    //TODO hard-coded for the moment.
    spawnPiece(&L);
    ActivePiece = L;

    
    PlayerAction playerAction = IDLE;
    while (!game_over) {
        //NOTE AFAIK C's Standard Libraries input is all buffered so I
        // couldn't figure out a way to break the game loop in real
        // time. This looks evil because GetAsyncKeyState returns a
        // short whose most significant integer is set, hence the
        // bit-shifting to check if that's the case.
        if (int input = GetAsyncKeyState(VK_ESCAPE) & (1 << 15) != 0)
            playerAction = QUIT;
        if (int input = GetAsyncKeyState(VK_LEFT) & (1 << 15) != 0)
            playerAction = MOVE_LEFT;
        
        
        ftime(&end);
        time_diff = (int)1000 * (end.time - start.time) + (end.millitm - start.millitm);
        if (time_diff > FRAME_RATE) {
            update(&game_over, playerAction);
            draw();
            ftime(&start);
            playerAction = IDLE;
        }

  


    }

    teardown();


}

void initialize() {
    system("cls");

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (j == 0 || j == BOARD_WIDTH - 1) {
                board[i][j] = '|';
            }
            else if (i == BOARD_HEIGHT - 1) {
                board[i][j] = '_';
            }
            else {
                board[i][j] = '.';
            }
        }
    }
}

void update(bool *game_over, PlayerAction playerAction) {
    switch (playerAction) {
    case QUIT: {
        *game_over = true;
        break;
        }
    case MOVE_LEFT: {
        movePieceLeft(&ActivePiece);
        break;
    }
    }

}

void draw() {
    system("cls");
    printf("SCORE:%d\n\n", score);
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }

}

void teardown() {
    system("cls");
    printf("GAME_OVER!");

}

/* spawnPiece: spawns a piece centered at the top of the board. It can be any shaped piece */
void spawnPiece(Tetranimo* piece)
{
    int mid_x = (BOARD_WIDTH / 2) - 2;
    int i, j;

    piece->xPos = mid_x;
    piece->yPos = 0;


    for (i = piece->yPos; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (piece->shape[i][j] == 1)
                board[i][piece->xPos + j] = '#';
        }

    }
}

/* movePieceLeft: moves a piece to the left*/
void movePieceLeft(Tetranimo* piece)
{
    //TODO remove later, for debugging
    static int test = 1;

    //NOTE initialize to wipe the board essentially
    initialize();

    int i, j;
    piece->xPos--;
    for (i = piece->yPos; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (piece->shape[i][j] == 1)
                board[i][piece->xPos + j] = '#';
        }

    }
    printf("piece moved %d times\n", test);
    test++;
}
