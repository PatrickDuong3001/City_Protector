//=================================================================
// Implementation for the player module
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
#include "player_private.h"

PLAYER player; // structure of player

PLAYER player_get_info(void){ // getter for user to acquire info without accessing structure
    return player;
}

// initialize the player's position, missile status, draw player, 
void player_init(void) {    
    player.x = PLAYER_INIT_X; player.y = PLAYER_INIT_Y; player.status = ALIVE;    
    player.playerMissiles = create_dlinkedlist();    
    player.delta = PLAYER_DELTA;
    player.width = PLAYER_WIDTH; 
    player.height = PLAYER_HEIGHT;
    player_draw(PLAYER_COLOR);
}

// This function moves player PLAYER_DELTA pixels to the left, except if it would go off screen.
void player_moveLeft(void) { 
    if(player.x - player.delta < 0)  //if move left, player'll be offscreen, do nothing. 
    {
        return;
    }
    player_draw(BACKGROUND_COLOR);
    player.x = player.x - player.delta;
    player_draw(PLAYER_COLOR);
}

// This function moves player PLAYER_DELTA pixels to the right, except if it would go off screen.
void player_moveRight(void) { 
    if(player.x + player.delta > 117)  //if move right, player'll be offscreen, do nothing. 
    {
        return;
    }
    player_draw(BACKGROUND_COLOR);
    player.x = player.x + player.delta;
    player_draw(PLAYER_COLOR);
}

/**
  The function allocats a PLAYER_MISSILE, initialize
  it and insert it into the player's playerMissiles list. 
  */
void player_fire() { 
    PLAYER_MISSILE* pm = (PLAYER_MISSILE*) malloc(sizeof(PLAYER_MISSILE));  //allocate a PLAYER_MISSILE
    pm->status = PMISSILE_ACTIVE; 
    pm->x = player.x + player.width/2; //define the missile coordinates
    pm->y = player.y - player.delta;   
    insertHead(player.playerMissiles,pm); //add missile to list
}

// draw/updates the line of any active missiles, "erase" deactive missiles
void player_missile_draw(void) {
  PLAYER_MISSILE* playerMissile;
  LLNode* current = player.playerMissiles->head;
  while(current){
    playerMissile = (PLAYER_MISSILE*)current->data;                    
    if(playerMissile->status == PMISSILE_EXPLODED) {
      //pc.printf("pmd:exploded\n");
      uLCD.line(playerMissile->x, player.y-player.delta, playerMissile->x, playerMissile->y, BACKGROUND_COLOR);
      //pc.printf("deleting node...\n");
      deleteNode(player.playerMissiles, current);
      //pc.printf("node deleted.\n");
    }
    else { // update missile position
      playerMissile->y -= PLAYER_MISSILE_SPEED;
      if (playerMissile->y < 0) {
        //pc.printf("pmd:at top of screen\n");
        uLCD.line(playerMissile->x, player.y-player.delta, playerMissile->x, 0, BACKGROUND_COLOR);
        // Remove from list   
        //pc.printf("deleting node...\n");
        deleteNode(player.playerMissiles, current);
        //pc.printf("node deleted.\n");             
      }
      else {
        //pc->printf("pmd:normal\n");
        // draw missile
        uLCD.line(playerMissile->x, playerMissile->y+PLAYER_MISSILE_SPEED, playerMissile->x, playerMissile->y, PLAYER_MISSILE_COLOR);
      }
    }  
    current = current->next;              
  }
}
    

// ==== player_private.h implementation ====
void player_draw(int color) { //new fancier aircraft appearance
    int a = player.x;
    int b = player.y;
    int c = player.height;
    int d = player.width;
    int f = a+0.5*d;
    int h = b+0.5*c;
    uLCD.circle(f,h,2,color);
}

// destroy and "erase" the player off the screen. change status to DESTROYED
void player_destroy() {
    player_draw(BACKGROUND_COLOR);
    player.status = DESTROYED;
}

