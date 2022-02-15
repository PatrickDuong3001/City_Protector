//=================================================================
// The header file for general settings for the project
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

#ifndef GLOBAL_H
#define GLOBAL_H
#include "mbed.h"
#include "wave_player.h"
#include "uLCD_4DGL.h"
#include "MMA8452.h"

// === [global object] ===
extern uLCD_4DGL uLCD;
extern Serial pc;           // USB Console output
extern wave_player waver;
extern PwmOut speaker;

// === [global settings] ===
#define BACKGROUND_COLOR 0x000000 //black
#define LANDSCAPE_HEIGHT 4        // number of pixel on the screen
#define MAX_BUILDING_HEIGHT 10    // number of pixel on the screen


// === [define the macro of error handle function] ===
// when the condition (c) is not true, assert the program and show error code
#define ASSERT_P(c,e) do { \
    if(!(c)){ \
        uLCD.printf("\nERROR:%d\n",e); \
        pc.printf("\nERROR:%d\n",e); \
        while(1); \
    } \
} while (0)

// === [error code] ===
#define ERROR_NONE 0 // All good in the hood
#define ERROR_MISSILE_INDEX_GET_INFO      -1  // make sure your code gives the valid index for missile_get_info()
#define ERROR_MISSILE_INDEX_UPDATE_STATUS -2  // make sure your code gives the valid index for missile_update_status()
#define ERROR_MISSILE_SPEED               -3  // missile speed has to be between 1 and 8
#define ERROR_MISSILE_INTERVAL            -4  // missile interval has to be between 1 and 100
// other missile error code ...
#define ERROR_CITY_NUMBER                 -11 // num_city in city_landscape_init() is larger than MAX_NUM_CITY
#define ERROR_CITY_INDEX_GET_INFO         -12 // make sure your code gives the valid index for city_get_info()
#define ERROR_CITY_INDEX_DEMOLISH         -13 // make sure your code gives the valid index for city_demolish()
// DLL
#define ERROR_DLL_INSERT_HEAD             -14 // inserting into doubly linked list at head failed
#define ERROR_DLL_DELETE                  -15 // deleting node from doubly linked list failed
// other anti-missile error code ...

#endif //GLOBAL_H