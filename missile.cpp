//=================================================================
// Implementation of missile module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#include "missile_private.h"
#include "doubly_linked_list.h"

int missile_tick=0;

//Create a DLL for missiles
DLinkedList* missileDLL = NULL;

void missile_init(void)
{
    missileDLL = create_dlinkedlist();
}

// See the comments in missile_public.h
void missile_generator(void){
    missile_tick++;
    // only fire the missile at certain ticks
    if((missile_tick % MISSILE_INTERVAL)==0 || missile_tick==0){
        //printf("missile_create()");
        missile_create();
    }        
    // update the missiles and draw them
    missile_update_position();
}

void missile_create(void){
    MISSILE* M = (MISSILE*)malloc(sizeof(MISSILE));
    M->y = 0;
    //each missile has its own tick
    M->tick = 0;
    //set a random source for the missile
    M->source_x = rand() % SIZE_X;
    //set a random target for the missile
    M->target_x = rand() % SIZE_X;
    //the missile starts at its source
    M->x = M->source_x;
    
    M->status = MISSILE_ACTIVE;
    
    insertHead(missileDLL, M);
}

/** This function update the position of all missiles and draw them
*/
void missile_update_position(void){    
    //controls how fast the missile will move
    int rate = MISSILE_SPEED * 25;
    //delta_x and delta_y account for the slope of the missile
    double delta_x, delta_y;
    LLNode* current = missileDLL->head;
    MISSILE* newMissile;
    //iterate over all missiles
    while(current)
    {   newMissile = (MISSILE*) current->data;  
        if(newMissile->status == MISSILE_EXPLODED)
        {
            // clear the missile on the screen
            missile_draw(newMissile, BACKGROUND_COLOR);
                        
            // Remove it from the list
            //pc.printf("deleting missile node...\n");
            deleteNode(missileDLL, current);
            //pc.printf("missile node deleted.\n");
        }
        else 
        {
            //cover the last missile location
            missile_draw(newMissile, BACKGROUND_COLOR);

            // update missile position
            delta_y = 200/rate;
            delta_x = (newMissile->target_x - newMissile->source_x)/rate;
            newMissile->y = (int)(delta_y*(newMissile->tick%rate));
            newMissile->x = (int)(newMissile->source_x + delta_x*(newMissile->tick%rate));
            // draw missile
            missile_draw(newMissile, MISSILE_COLOR);
            //update missile's internal tick
            newMissile->tick++;            
        }       
        // Advance the loop
        current = current->next;
    }
}

// set missile speed (default speed is 4)
void set_missile_speed(int speed){
    ASSERT_P(speed>=1 && speed<=8,ERROR_MISSILE_SPEED);
    if(speed>=1 && speed<=8){  
        MISSILE_SPEED = speed;
    }
}

// set missile interval (default interval is 10)
void set_missile_interval(int interval){
    ASSERT_P(interval>=1 && interval<=100,ERROR_MISSILE_INTERVAL);
    if(interval>=1 && interval<=100){
        MISSILE_INTERVAL = interval;
    }
}

// See comments in missile_public.h
DLinkedList* get_missile_list() {
    return missileDLL;
}

/** This function draws a missile.
    @param missile The missile to be drawn
    @param color The color of the missile
*/
void missile_draw(MISSILE* missile, int color){
    int init_x,init_y,current_x,current_y;

    init_x = missile->source_x;
    init_y = 0;
    current_x = missile->x;
    current_y = missile->y;
    uLCD.line(init_x, init_y, current_x, current_y, color);
}
