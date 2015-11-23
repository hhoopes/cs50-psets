//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// space between bricks
#define brickpadding 2

// brick dimensions
#define brickh 10
#define brickw 36

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// paddle dimensions
#define paddleh 15
#define paddlew 75

// paddle height from screen top
#define paddledelta 550

// message height for win/lose
#define MESSAGEHEIGHT 450

// brick starting height from screen top
#define brickdelta 50

// velocity multiplier
#define velmultiplier 3

// yvelocity
#define YVELOCITY 4.0

// max velocity so it doesn't go crazy
#define MAXVEL 5.0

// add a speed multiplier on when the ball hits the paddle corner
#define CORNERSPEED 1.2


// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
GLabel initLifeboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
void updateLifeboard(GWindow window, GLabel lifelabel, int lives);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{ 
    
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // add circle to window
    //add(window, paddle);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    
    //instantiate lifeboard, listing number of lives left
    GLabel lifelabel = initLifeboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;    
    
    // initial ball speed
    double xvelocity = drand48()*velmultiplier;
    double yvelocity = YVELOCITY;
    
    // keep playing until game over
    //mouse click to drop ball
    waitForClick();
        
    while (lives > 0 && bricks > 0)
    {        
        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
        while (event != NULL)
            {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
                {
                //paddle follows x cursor
                double x = getX(event) - getWidth(paddle) / 2;
              
                setLocation(paddle, x, paddledelta);
                }
             // check for mouse event
            event = getNextEvent(MOUSE_EVENT);
            }
        
            pause(10);
            
        move(ball, xvelocity, yvelocity);
            
        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
            {            
            //dealing with unusual collisions that it can't escape
            setLocation(ball, WIDTH-getWidth(ball), getY(ball)); 
            xvelocity = -xvelocity;
            }

        // bounce off left edge of window
       
        if (getX(ball) <= 0)
            {
             //dealing with unusual collisions that it can't escape
            setLocation(ball, 0, getY(ball)); 
            xvelocity = -xvelocity;      
            }
        
        // ball drops past the paddle
        if (getY(ball) >= paddledelta)
            {
            lives = lives -1;
            updateLifeboard(window, lifelabel, lives);
        
            // reset the ball
            xvelocity = drand48()*velmultiplier;
            yvelocity = YVELOCITY;
            removeGWindow(window, ball);
            ball = initBall(window);
    
            // mouse click to drop ball
            waitForClick();
              
            }
            
        //bounce ball off top
        if (getY(ball) <=0)
            {
            yvelocity = -yvelocity;
            }
        
        //collisions with objects
        GObject object = detectCollision(window, ball);
        
        if (object != NULL)
            {
            //paddle interactions
            if (object == paddle)
                {                               
                //Speed bonus changing the angle for hitting the ball with the edge of the paddle
                if ((((getX(paddle) <= getX(ball)+2*RADIUS) && 
                    (getX(ball) < (getX(paddle)+.1*getWidth(paddle)))) ||
                    ((getX(paddle) >= getX(ball)) &&
                    (getX(ball) > (getX(paddle)-.1*getWidth(paddle))))))
                    {
                    if (xvelocity < MAXVEL)
                        {                      
                        xvelocity = xvelocity*CORNERSPEED;
                        yvelocity = -yvelocity;                                                                  
                        }
                    else 
                        {
                        yvelocity = -yvelocity;
                        }
                    }   
                    
                //normal paddle behavior
                else 
                    {                                      
                    yvelocity = -yvelocity;
                    }   
                }
                
            else if (strcmp(getType(object), "GRect") == 0)
                {
                removeGWindow(window, object);
                bricks = bricks -1;
                points = points +1;
                yvelocity = -yvelocity;
                updateScoreboard(window, label, points);
                }
            }
    }

    if (lives == 0)
        {
        GLabel loselabel = newGLabel("GAME OVER!");
        setFont(loselabel, "SansSerif-35");
      
        double x = (getWidth(window) - getWidth(loselabel)) / 2;
        double y = MESSAGEHEIGHT;
        setLocation(loselabel, x, y);
        add(window, loselabel);
        }
    
    if (points == 50)
     {
        GLabel winlabel = newGLabel("CONGRATULATIONS!");
        setFont(winlabel, "SansSerif-35");
      
        double x = (getWidth(window) - getWidth(winlabel)) / 2;
        double y = MESSAGEHEIGHT;
        setLocation(winlabel, x, y);
        add(window, winlabel);
        }
    
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // setting up bricks
    for (int i=0; COLS > i > 0; i++)
        {
        for (int j=0; ROWS > j > 0; j++)
            {            
            GRect bricks = newGRect(i*(brickw + 2*brickpadding) + brickpadding, brickdelta + j*(brickh+2*brickpadding) + brickpadding, brickw, brickh);
            setFilled(bricks, true);            
            if (j==0) (setColor(bricks, "2A5C19"));
            if (j==1) (setColor(bricks, "489C2C"));
            if (j==2) (setColor(bricks, "5BC738"));
            if (j==3) (setColor(bricks, "82ED5F"));
            if (j==4) (setColor(bricks, "AAF291"));
            add(window, bricks);            
            }
        }                 
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // black ball
      GOval ball = newGOval(WIDTH/2-RADIUS, HEIGHT/2-RADIUS, RADIUS*2, RADIUS*2);
      setFilled(ball, true);
      setColor(ball, "BLACK");
      add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // green paddle
      GRect paddle = newGRect(WIDTH/2-paddlew/2, paddledelta, paddlew, paddleh);
      setFilled(paddle, true);
      setColor(paddle, "GREEN");
      add(window, paddle);
      return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
      GLabel label = newGLabel("0");
      setFont(label, "SansSerif-22");
     
      
      double x = (getWidth(window) - getWidth(label)) / 2;
      double y = (getHeight(window) - getHeight(label)) / 2;
      setLocation(label, x, y);
      add(window, label);
      return label;
    
}

//Label for number of lives
GLabel initLifeboard(GWindow window)
{
      GLabel lifelabel = newGLabel("3 lives");
      setFont(lifelabel, "SansSerif-22");
      double x = (getWidth(window) - getWidth(lifelabel)- brickpadding
      setLocation(lifelabel, x, y);
      add(window, lifelabel);
      return lifelabel;

}
/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

void updateLifeboard(GWindow window, GLabel lifelabel, int lives)
{
    // update label
    char l[12];
    sprintf(l, "%i lives", lives);
    setLabel(lifelabel, l);

    // label in top right corner
    double x = (getWidth(window) - getWidth(lifelabel));
    double y = (0 + getHeight(lifelabel));
    setLocation(lifelabel, x, y);
}    
    
/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
