//=================================================================
// The main program file.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// External libs
#include <stdlib.h>

// Project includes
#include "globals.h"
#include "hardware.h"
#include "city_landscape_public.h"
#include "missile_public.h"
#include "player_public.h"

#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y-(LANDSCAPE_HEIGHT+MAX_BUILDING_HEIGHT))

//Important globals
PLAYER activePlayer;
DLinkedList* missileList; 
int num_city_g; 
int curr_score; //This reflects the nummber of missiles the player has destroyed
int topScore = 0; 
int levels;
int playerHP; //the player has 3 lives  
int missileExplodeRad;
int level_track = 1;

// function prototypes
void gameStart(void);
void mainMenu(void);
void levelSelect(void);
void endGame(void);
void set_random_seed(Timer*);
int city_landscape_update(void);
int was_player_hit(void);
void missile_contact(void); 
void display_topScore();
void explode_animation(int,int,int,int);
void level_set(int);
void detect_level_up(int);

int main()
{ 
    mainMenu();
}
/**
   mainMenu() function is reached first whenever the game is started or restarted. 
   The user has 3 options and the program will wait for the user input. 
   The user can provide the program with the inputs by pushing the buttons. 
   Each pushed button will make the program to jump to a corresponding function.
*/
void mainMenu()
{  
    GameInputs inputs; //declare a "GameInputs" variable to get the user input 
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
     
    level_set(2); //Default level whenever the game is restarted
    level_track = 1;
    
    int standby = 1;    
    while(standby) //wait for the user input
    {
        inputs = read_inputs(); //read user input
        uLCD.locate(2,0);
        uLCD.printf("Missile Command");
        uLCD.locate(5,2);
        uLCD.printf("Main Menu");
        uLCD.locate(3,5);
        uLCD.printf("B1-Start Game");
        uLCD.locate(2,7);
        uLCD.printf("B2-Select Level");
        uLCD.locate(3,9);
        uLCD.printf("B3-Top Score");
        uLCD.locate(4,11);
        uLCD.printf("Have fun :D");
        
        //Conditions to keep track of which button is pushed by the user
        if(inputs.b3) //the upper button is pushed
        {
            uLCD.cls();
            standby = 0;    
            gameStart();   
        }else if(inputs.b2) //the middile button is pushed
        {
            uLCD.cls();
            standby = 0;
            levelSelect();   
        }else if(inputs.b1) //the lower button is pushed
        {
            uLCD.cls();
            standby = 0;
            display_topScore();   
        }
    }
}   
    
/**
  Whenever the user pushes the upper button, the program will jump to this void function. 
  This function will reset any essential variables and initialize the hardware to prepare the game.
  The game will run continuously as long as the while loop is not broken.  
  The player movement, player info, city info, and score are all kept track of by this function. 
*/
void gameStart()
{
    // First things first: initialize hardware
    GameInputs inputs; 
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    
    // Checks if the player gets hit
    int check_hit = 0;
    
    curr_score = 0; //reset the current score to 0 every new attempt
    playerHP = 3;   //reset number of player lives
    num_city_g = 4; //reset the number of cities to 4
     
    // Game state variables
    int num_remain_city; // number of cities currently on the landscape
    
    // Timer to measure game update speed (secondarily used to generate random seed)
    Timer t;
    int dt; // delta time
    set_random_seed(&t); // Already implemented.
    
    //Initialization functions
    city_landscape_init(num_city_g);
    missile_init();
    player_init();        
    
    while(1)  
    {
        activePlayer = player_get_info(); //retrieve info of the player
        missileList = get_missile_list(); //retrieve info of the list of missiles       
        
        t.start();
        
        // Display the current level on the top-left corner of the screen
        uLCD.locate(3,0);
        uLCD.printf("%d",levels);
        
        // Display the current score on the top-right corner of the screen
        uLCD.locate(15,0);   
        uLCD.printf("%d",curr_score);
        
        // Display the number of player lives on the middile of the screen
        uLCD.locate(9,0);   
        uLCD.printf("%d",playerHP);
        
        inputs = read_inputs(); //continuously keeps track of which push button is pushed by the user
        
        // Generate new missiles and draw all active missiles at current 
        // positions.
        missile_generator();
        
        // Force level advance
        if(inputs.b1 && inputs.b3) //if button 1 and 3 are both pushed
        {
            if(levels < 3)     //if the current level is smaller than 3   
            {
                levels = levels + 1; //increment the level by 1
                level_set(levels);   
            }
        }
        
        // The movement and actions of the player        
        if(inputs.ax < -0.5 || inputs.b3)
        {
            player_moveRight();
        }else if(inputs.ax > 0.5 || inputs.b1)
        {
            player_moveLeft();
        }
        
        if(inputs.b2)
        {
            player_fire();
               
        }
 
        // Draw all active anti-missiles (aka player missiles) at current 
        // positions.
        player_missile_draw();
        
        // Detect missile collisions with city/land and update status.
        num_remain_city = city_landscape_update();
        
        // Detect missile collisions with player anti-missiles.
        missile_contact();
        
        // Check if the player is hit by the missiles. 
        check_hit = was_player_hit();
        
        if(check_hit == 1) //whenever the player is hit, he/she loses 1 live. 
        {
            if(playerHP == 0) 
            {
                player_destroy();
                break;
            }
            playerHP = playerHP - 1;  //Player's lives will be deducted by 1
        }
        
        if(num_remain_city == 0 || playerHP == 0) //If the player loses all lives or all cities are lost, break the loop
        {
            break;
        }
        
        // Increment the level whenever 10 missiles are destroyed. 
        detect_level_up(curr_score);
  
        // Compute update time to control timing of the game loop.
        t.stop();
        dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
        
    }
    
    // Free up any dynamically allocated objects
    destroyList(missileList);
    destroyList(activePlayer.playerMissiles);
    
    endGame(); //Jump to the end game function to display the end game screen
}

/** Detect whether any missile has hit a city and if so, call 
    city_demolish.
    Also, if any missile has hit a city or the landscape,
    mark the missile's status as MISSILE_EXPLODED 
    which will cue the missile's deletion and erasure from the screen 
    on the next missile_generator call.
    @return  Number of remaining cities.
*/
int city_landscape_update(void){
    int j; 
    const int i = num_city_g; //num_city_g is changeable based on the level
    int con1, con2, con3, con4; 
    int leftSide,rightSide,subHeight;
    LLNode* currentMissile = missileList->head;
        
    while (currentMissile)
    {
        MISSILE* hostile = (MISSILE*) currentMissile->data;
        for(j = 0; j <= i; j++)
        {
            CITY activeCity = city_get_info(j);
            switch(activeCity.status)
            {
                case DEMOLISHED:
                    break;
                case EXIST:    
                    leftSide = activeCity.x;
                    rightSide = activeCity.width + leftSide; 
                    subHeight = 128 - activeCity.height; 
                    con1 = hostile->x - rightSide;
                    con2 = leftSide - hostile->x;
                    con3 = 128 - hostile->y; 
                    con4 = subHeight - hostile->y;
                                  
                    if((con1<=0) && (con2<=0) && (con3>=1) && (con4<=0)) //city collision occurs
                    {
                        activeCity.status = DEMOLISHED;
                        city_demolish(j);
                        hostile->status = MISSILE_EXPLODED;
                        num_city_g = num_city_g - 1; //number of remaining cities
                        
                        explode_animation(1,0.5*(leftSide + rightSide),subHeight,0.1);
                    }
            }
        }
        currentMissile = currentMissile->next;  
    }
    return num_city_g;
}

/** Detect whether any missile has hit the player aircraft and if so, call 
    player_destroy and mark the missile's status as MISSILE_EXPLODED 
    which will cue the missile's deletion and erasure from the screen 
    on the next missile_generator call.
    @return 1 if the player aircraft was hit and 0 otherwise.
*/
int was_player_hit(){
    LLNode* currentMissile = missileList->head;
    int hX, hY, con1, con2, con3, con4;
        
    while(currentMissile)
    {
        MISSILE* hostile = (MISSILE*) currentMissile->data;
        hX = hostile->x;
        hY = hostile->y;
    
        con1 = hX - activePlayer.x;
        con2 = activePlayer.x - hX;
        con3 = hY - activePlayer.y;
        con4 = activePlayer.y - hY;
        
        if((con1<=8) && (con2<=1) && (con3<=1) && (con4<=8))
        {
            hostile->status = MISSILE_EXPLODED; 
            return 1;   
        }
        currentMissile = currentMissile->next;
    } 
    return 0;
}

/** Detect whether any missile has hit any player missile and if so, 
    mark the status of both the missile and the player missile as 
    MISSILE_EXPLODED which will cue the missile's deletion and erasure 
    from the screen on the next missile_generator call.
*/
void missile_contact(void) {    
     int hX,hY,pX,pY;
     LLNode* currentMissile = missileList->head;
     LLNode* currentPlayerMissile = activePlayer.playerMissiles->head;
     
     while(currentMissile)
     {
        MISSILE* hostile = (MISSILE*) currentMissile->data;
        hX = hostile->x; //retrieve the coordinates of hostile
        hY = hostile->y;    
        while(currentPlayerMissile)
        {   
            PLAYER_MISSILE* protector = (PLAYER_MISSILE*) currentPlayerMissile->data;
            pX = protector->x; //retrieve the coordinates of protector
            pY = protector->y; 
            if((hX - pX)*(hX - pX)+(hY - pY)*(hY - pY) < missileExplodeRad)  //missileExplodeRad is changeable based on level 
            {
                protector->status = PMISSILE_EXPLODED; //update the status of protector
                hostile->status = MISSILE_EXPLODED;    //update the status of hostile
                curr_score = curr_score + 1; //increment the score of the player by 1
            
                explode_animation(0,0.5*(hX + pX),0.5*(hY + pY),0.1);
            }
            currentPlayerMissile = currentPlayerMissile->next;
        }
        if(hY > 128) //if missile goes over the screen
        {
            hostile->status = MISSILE_EXPLODED;    
        }
        
        currentMissile = currentMissile->next; 
     }
}
/** Display the highest score whenever the lower button is pushed during the main menu screen.  
    As long as the program is not reset, the highest top score of all attempts is saved and displayed. 
*/

void display_topScore()  
{
    uLCD.locate(3,7);
    uLCD.printf("Top Score: %d", topScore); 
    wait(5);
    uLCD.cls();
    mainMenu();  
}

/** Create explosion animations for city or missiles. 
    Depending on the type, the animation will be different.  
*/
void explode_animation(int status, int a, int b, int waitTime)
{
   if(status == 0) //missile explosion animation
   {
        uLCD.circle(a,b,10,0xFCC603);  //yellow explosion
        wait(waitTime);
        uLCD.circle(a,b,10,BACKGROUND_COLOR);
        wait(waitTime);
        uLCD.circle(a,b,8,0xFCC603);  //yellow explosion
        wait(waitTime);
        uLCD.circle(a,b,8,BACKGROUND_COLOR);  
        wait(waitTime);
        uLCD.circle(a,b,6,0xFCC603);  //yellow explosion
        wait(waitTime);
        uLCD.circle(a,b,6,BACKGROUND_COLOR); 
    }
    
    if(status == 1) //city and player destruction animation
    {
        uLCD.circle(a,b,10,0xFF00FF);  //blue explosion
        wait(waitTime);
        uLCD.circle(a,b,10,BACKGROUND_COLOR);  
        wait(waitTime);
        uLCD.circle(a,b,8,0xFF00FF);  //blue explosion
        wait(waitTime);
        uLCD.circle(a,b,8,BACKGROUND_COLOR);   
        wait(waitTime);
        uLCD.circle(a,b,6,0xFF00FF);  //blue explosion 
        wait(waitTime);        
        uLCD.circle(a,b,6,BACKGROUND_COLOR);   
    }
}

/**
   The end game screen of the program. Display the score that the player gets after the game over. 
   If the current score is higher than the Top Score, that current score becomes the new Top Score,
   and the "HIGHEST SCORE" and "CONGRATULATIONS" will also appear.  
  */
void endGame() 
{
    uLCD.cls();
    uLCD.locate(5,1);
    uLCD.printf("Game Over");
    uLCD.locate(3,3);
    uLCD.printf("Your Score: %d",curr_score);
    
    if(topScore <= curr_score && curr_score != 0)
    {
        topScore = curr_score;
        uLCD.locate(3,6);
        uLCD.printf("HIGHEST SCORE");   
        uLCD.locate(2,9);
        uLCD.printf("CONGRATULATIONS!");
    }
    wait(5);  
    uLCD.cls();
    mainMenu();   //automatically returns to main menu
}

/**
  Whenever the middle button is pushed during the main menu screen, this void function is reached. 
  This function continuously wait for the user input. Each button corresponds to a level. 
  After receiving the user input, the program will jump to the gameStart function. 
  */
void levelSelect()
{
    GameInputs inputs; 
    int standby = 1;
    while(standby)
    {
        inputs = read_inputs();
        uLCD.locate(1,1);
        uLCD.printf("Choose your level");
        uLCD.locate(5,4);
        uLCD.printf("B1-Rookie");
        uLCD.locate(4,6);
        uLCD.printf("B2-Enforcer");
        uLCD.locate(6,8);
        uLCD.printf("B3-Boss");
        
        if(inputs.b3)
        {
            standby = 0;
            level_set(1);
            uLCD.cls();
            gameStart();
        }else if(inputs.b2)
        {
            standby = 0;
            level_set(2);
            uLCD.cls();
            gameStart();   
        }else if(inputs.b1)
        {
            standby = 0;
            level_set(3); 
            uLCD.cls();
            gameStart();  
        }
    }
}

/**
  This void function set the missile speed, missile interval, and explosion radius 
  according to the desired level.  
  */
void level_set(int level)
{
    if(level == 1)
    {
        levels = 1;
        set_missile_interval(80);
        set_missile_speed(6);
        missileExplodeRad = 80;     
    }else if(level == 2)
    {
        levels = 2;
        set_missile_interval(50);
        set_missile_speed(4);
        missileExplodeRad = 60;
    }else if(level == 3)
    {
        levels = 3;
        set_missile_interval(20);
        set_missile_speed(2);
        missileExplodeRad = 30;    
    }
         
}

/**
  This function update the global variable "levels" and set the new level whenever
  the player has destroyed 10 missiles during the current session.  
  When the player is at lavel 3, no more level increment will occur 
  when 10 missiles are destroyed.  
  */
void detect_level_up(int currentScore)
{
    if(currentScore == 10)
    {
        if(levels == 1)
        {
            level_set(2);   
            level_track = 0; //level_track = 0 to prevent the program to jump right from 1 to 3 
        }else if (levels == 2 && level_track == 1)
        {
            level_set(3);    
        }
    }
    
    if(currentScore == 20 && levels == 2) 
    {
        levels = levels + 1;
        level_set(levels);   
    }
}

/* We need a random number generator (e.g., in missile_create to generate
   random positions for the missiles to start, making the game different
   every time).  C provides a pseudo random number generator (in stdlib) but
   it requires that we give it a different seed for each new game.  A common
   way to do this is to sample a clock and use the time as the seed.  However
   if we do this by starting a simple Timer t when the mbed starts running the 
   program and then sample it (t.read_ms), we will always get exactly the same
   time sample -- t.read_ms will always occur at the same time in the program's
   execution.  We introduce variability in when we sample the time by waiting
   for the user to push any button before we call t.read_ms.
*/
void set_random_seed(Timer* t) {
    GameInputs inputs;
    t->start();
    uLCD.locate(1,0);
    uLCD.printf("Ready? 3..2..1...");
    uLCD.locate(1,4);
    uLCD.printf("Push any button!");
    while(1){
      inputs = read_inputs();
      if (inputs.b1 || inputs.b2 || inputs.b3) break;
    }
    uLCD.cls();
    t->stop();
    int seed = t->read_ms();    
    srand(seed);
}
