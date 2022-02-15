//=================================================================
// The header file is for module "city landscape"
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#ifndef CITY_LANDSCAPE_PRIVATE_H
#define CITY_LANDSCAPE_PRIVATE_H

#include "mbed.h"
#include "globals.h"
#include "city_landscape_public.h"

//==== [private type] ====
// N/A


//==== [private function] ====
// N/A


//==== [private macros] ====
// The bottom of the screen => y=127
// Gut the landscape grow up from the bottom of the screen. It is awkward.
// Thus, we use a macro to reverse the coordinate for convenience.
#define REVERSE_Y(x) (SIZE_Y-(x))

//==== [private settings] ====
// You could modify these settings, but try to keep them be used only inside city_landscape.cpp
// Here are the settings to define the looking of your city landscape
#define CITY_TO_SCREEN_MARGIN 25 // pixel on the screen
#define CITY_WIDTH 10 // pixel on the screen
#define BUILDING_WIDTH 2 // pixel on the screen
#define NUM_BUILDING (CITY_WIDTH/BUILDING_WIDTH)
#define BUILDING_COLOR 0x00FF00
#define LANDSCAPE_COLOR 0xCCAA00





#endif //CITY_LANDSCAPE_PRIVATE_H