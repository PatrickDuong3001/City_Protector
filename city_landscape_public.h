// =================================================================
// The header file is for module "city landscape"
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
/** @file city_landscape_public.h */
#ifndef CITY_LANDSCAPE_PUBLIC_H
#define CITY_LANDSCAPE_PUBLIC_H

/// The enum define the status of a city
typedef enum {
    EXIST=1,      ///< The city will be shown on screen
    DEMOLISHED=0   ///< The city won't be shown on screen
} CITY_STATUS;

/// The structure to store the information of a city
typedef struct {
    int x;        ///< Bottom-left corner of the city. x coordinate on the screen.
    int y;        ///< Bottom-left corner of the city. y coordinate on the screen.
    int width;    ///< The width of the city. The shape of the city is a rectangle.
    int height;   ///< The height of the city
    CITY_STATUS status;  ///< See enum CITY_STATUS
} CITY;

#define MAX_NUM_CITY 6

/** Call city_landscape_init() only once at the begining of your code
    @param num_city number of cities to be drawn. It must be less/equal to MAX_NUM_CITY.
*/
void city_landscape_init(int num_city);

/** Get the information of city
    @param index The index in city_record. It must be smaller than MAX_NUM_CITY.
    @return The structure of city information
*/
CITY city_get_info(int index);

/** Remove the city from record and screen
    @param index The index in city_record. It must be smaller than MAX_NUM_CITY.
*/
void city_demolish(int index);

/** Draw all exist cities onto the screen
    @brief You might not need to use this function, but you could still use it if you want.
*/
void draw_cities(void);

/** Draw the landscape
    @brief You might not need to use this function, but you could still use it if you want.
*/
void draw_landscape(void);


#endif //CITY_LANDSCAPE_H