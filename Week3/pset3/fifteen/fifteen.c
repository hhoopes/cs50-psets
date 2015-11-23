/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);
bool inbound(int check);

//track 0 space at all times
int i0 = 0;
int j0 = 0;
 
//store the position of tile next to 0 if it matches the tile the user specifies
int tilei=0;
int tilej=0;
    
   
int main(int argc, string argv[])
{
   
    
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    //set position of 0
        i0=i0+d-1;
        j0=j0+d-1;
        
    //initialize an even board
     if ((d%2)==0)
        {
    
        int special=0;
       
        for (int i= 0; i < d; i++)
            {
            for (int j = 0; j < d; j++)
                {
                
                special = d*d-i*d-j-1;
                if (special == 2)
                    {
                    board[i][j]=1;
                    }
                else if (special == 1)
                    {
                    board[i][j]=2;
                    }
          
                else 
                    {
                    board [i][j] = d*d-i*d-j-1;
                
                    }
                }
            }
        }
    
    //initialize an even board where 2 and 1 are switched
    else 
        {
        for (int i= 0; i < d; i++)
            {
            for (int j = 0; j<d; j++)
                {
                board [i][j] = d*d-i*d-j-1;
                }
            }
        }
    
}


/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // print each square of the board from the array
    for (int i=0; i < d; i++)
        {
       
        for (int j=0; j < d; j++)
            {
            if (board[i][j] == 0)
                {
                printf("  _  ");
                
                }

            else 
                {
                printf("  %d  ", board[i][j]);
                }
            }
         printf("\n");
        }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    //saves if tiles need to be swapped
    int swapneed=0;
    
    // compare position of tile to move with position of 0
   
    //check if the test position is inbounds of the array
    
    {
        if (inbound(i0+1))
        //check up
            {
            if (board[(i0+1)][j0]==tile)
                {
                tilei=i0+1;
                tilej=j0+0;
                swapneed = swapneed +1;
                }
            }
        //check down
        if (inbound(i0-1))
            {
            if (board[(i0-1)][j0]==tile)
                {
                tilei=i0-1;
                tilej=j0+0;
                swapneed = swapneed +1;
                }
            }
        //check right  
        if (inbound(j0+1))   
            {
            if (board[i0][(j0+1)]==tile)
                {
                tilei=i0+0;
                tilej=j0+1;
                swapneed = swapneed +1;
                }
            }
            
        //check left
        if (inbound(j0-1))
            {     
            if (board[i0][(j0-1)]==tile)
                {
                tilei=i0;
                tilej=j0-1;
                swapneed = swapneed +1;
                }
            }
    }
        
    //if tile is not next to an empty    
    if (swapneed == 0)
        return false;
    
    
    //store new position of 0
    else
        {
         //printf("swapping %d, %d, %d, %d,%d\n", i0, j0, tilei, tilej, board[tilei][tilej]);
        board[i0][j0]=board[tilei][tilej];
        board[tilei][tilej]=0;
        i0 = tilei;
        j0 = tilej;
       
       
        return true;
        
        }
}

/**
 * Returns true if the test value is within the boundaries of the puzzle dimensions
 */
 
bool inbound(int check)
{
    //printf("%d\n", check);
    if (0 <= check < d)
        return true;
        
    else return false;
}


/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // for loops to iterate through the array
    int outorder = 0;

    for (int i = 0; i < d; i++)
        {
        for (int j=0; j < d; j++)
            {
            //compare across the row
            if ((j+1) < d)
                {
                if (board[i][j+1]>0)
                    {
                    if (board[i][j]>board[i][j+1])
                        {
                        outorder = outorder +1;
                        }
                    }    
                }      
            
            //compare righthand numbers to the one on the next row        
            else
                {
                //printf("hitting this loop/n");
                if (inbound(i+1))
                    {
                        if (board[i][j] > board[i+1][0])
                            {
                            outorder = outorder +1;
                            }
                    }
                }
            }
        }
    //printf("%d\n", outorder);
    if (outorder == 0)
        return true;
    else return false;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
