//=================================================================
// The header file defining the missile module
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
/** @file missile_public.h */
#ifndef MISSILE_PUBLIC_H
#define MISSILE_PUBLIC_H

#include "doubly_linked_list.h"

typedef enum {
    MISSILE_EXPLODED=0,
    MISSILE_ACTIVE=1,
} MISSILE_STATUS;

/// The structure to store the information of a missile
typedef struct {
    int x;                   ///< The x-coordinate of missile current position
    int y;                   ///< The y-coordinate of missile current position
    double source_x;           ///< The x-coordinate of the missile's origin
    double target_x;           ///< The x-coordinate of the missile's target
    int tick;                  ///< The missile's internal tick
    MISSILE_STATUS status;   ///< The missile status, see MISSILE_STATUS
} MISSILE;

/** Call missile_init() only once at the begining of your code */
void missile_init(void);

/** Call missile_init() only once at the begining of your code */
void missile_init(void);

/** This function draw the missiles onto the screen
    Call missile_generator() repeatedly in your game-loop. ex: main()
*/
void missile_generator(void);

/** This function will return a linked-list of all active MISSILE structures.
    This can be used to modify the active missiles. Marking missiles with status
    MISSILE_EXPLODED will cue their erasure from the screen and removal from the
    list at the next missile_generator() call.
*/
DLinkedList* get_missile_list();

/** Set the speed of missiles, Speed has range of 1-8 with 1 being fastest and 8 being slowest
*/
void set_missile_speed(int speed);

/** Set the interval that the missiles fire, interval has range of 1-100 with 1 being fired in
    very quick succession and 100 being fired very slowly after one another
*/
void set_missile_interval(int interval);

#endif //MISSILE_PUBLIC_H