//=================================================================
// The header file is for module "player"
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#ifndef PLAYER_PRIVATE_H
#define PLAYER_PRIVATE_H

#include "mbed.h"
#include "hardware.h"
#include "globals.h"
#include "player_public.h"

//==== [private settings] ====
#define PLAYER_INIT_X 60
#define PLAYER_INIT_Y 100
#define PLAYER_DELTA 3 // used in design of player, pixels to move, euclidean distance
#define PLAYER_WIDTH 10 
#define PLAYER_HEIGHT 3
#define PLAYER_COLOR 0xFF69B4 //hot pink
#define PLAYER_MISSILE_SPEED 3
#define PLAYER_MISSILE_COLOR 0x0000FF //blue


//==== [private type] ====

void player_draw(int color);
void player_set_color(int color);
void player_missile_draw(PLAYER_MISSILE* missile, int color);

//==== [private function] ====


#endif //PLAYER_PRIVATE_H

