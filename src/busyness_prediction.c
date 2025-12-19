#include "busyness_prediction.h"
#include "raygui.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
/*
 *Prototype of function that returns 2D array with categorized data
 *
 */
BusynessSchedule busynessSchedule();
/*
 * Prototype of a function, wich holds the functions to draw days and hours gui
 * Creates a time struct with information about the chosen time from the user.
 * Takes a step describing where in the process of the function it is
 */
Time userinput(int *step);
/*
 * Draws a text about how busy it is from the chosen user input
 * Takes a time struct describing the day and hour index.
 * Takes a BusynessSchedule struct, having a 2D array of data about how busy it is at times.
 * Takes a step describing where in the process of the function it is
 */
int printBusyness(Time chosenInput, BusynessSchedule schedule, int *step);
//Check boundaries of 2D array, takes a time struct to check from.
int boundariesCheck(Time date);
// Draws a GUI of all the days, taking in a pointer to a step variable, updating it accordingly
int getDayGui(int *step);
// Draws the hour intervals, also takes the step pointer and updating it accordingly.
int getHourGui(int *step);

// Calculates the busyness of the parking lot. The main part of the function
int busynessCalculator() {
  // The step variable to see where in the function the user is.
  static int step = 0;
  // Draws the title of the page
  DrawText("Choose a day and a time.", 280, 80, 20, BLACK);
  BusynessSchedule busyness = busynessSchedule();
  // Draws all the elements of the program, getting the user input
  Time chosenTime = userinput(&step);
  //Draws the actual busyness of the chosen time
  printBusyness(chosenTime, busyness, &step);
}

// 2D array to catogorise the data
BusynessSchedule busynessSchedule() {
  /*Catogorises the 7 days into 6 intervals of 4 hours.
   *
   *It will be given a value from 1 - 3, where the 1 is not very busyness and the 3 is very busy
   */
  BusynessSchedule b = {.data = {// Monday
                                 {1, 1, 2, 3, 1, 1},
                                 // Tuesday
                                 {1, 1, 3, 2, 1, 1},
                                 // Wednesday
                                 {1, 1, 3, 2, 1, 1},
                                 // Thursday
                                 {1, 1, 3, 2, 1, 1},
                                 // Friday
                                 {1, 1, 2, 2, 1, 1},
                                 // Saturday
                                 {1, 1, 1, 1, 1, 1},
                                 // Sunday.
                                 {1, 1, 1, 1, 1, 1}}};

  return b;
}

// Ge the user input
Time userinput(int *step) {
  // Index to day
  int dayIndex = 0;
  // Index of hour interval
  int hourIndex = 0;
  // Draw all the days, returning what was pressed
  dayIndex = getDayGui(step);
  // Draw all the hours, returning the hour interval
  hourIndex = getHourGui(step);
  // Creates struct with the returend data and returns it.
  Time chosenTime;
  chosenTime.Day = dayIndex;
  chosenTime.hour = hourIndex;
  return chosenTime;
}

int printBusyness(Time chosenInput, BusynessSchedule schedule, int *step) {
  int busynessValue;
  // When the button is clicked, the step updates to 3.
  if (GuiButton((Rectangle){350, 350, 100, 25}, "Predict busyness") &&
      *step == 2) {
    *step = 3;
  }
  // When the step is 3, the busyness is drawn.
  if (*step == 3) {
    if (boundariesCheck(chosenInput) == -1) {
      perror("Invalid input");
      return 1;
    }
    busynessValue = schedule.data[chosenInput.Day][chosenInput.hour];
    switch (busynessValue) {
    case 1:
      DrawText("The parking lot is not very busy at the chosen time.", 140, 400,
               20, BLACK);
      break;
    case 2:
      DrawText("The parking lot is moderately busy at the chosen time.", 120,
               400, 20, BLACK);
      break;
    case 3:
      DrawText("The parking lot is very busy at the chosen time", 150, 400, 20,
               BLACK);
      break;
    default:
      printf("Could not find the busyness\n");
    }
    return 0;
  }
  return 1;
}

//Checks the boundaries
int boundariesCheck(Time date) {
  if (date.Day > 6 || date.hour > 5) {
    return -1;
  }else {
    return 0;
  }
}

int getDayGui(int *step) {
  static int dayIndex = 0;
  // Text to describe the user what to do
  DrawText("Which day would you like to check?", 220, 160, 20, BLACK);
  // Draw monday button
  if (GuiButton((Rectangle){12, 200, 100, 25}, "Monday")) {
    dayIndex = 0;
    *step = 1;
  }
  // Draw tuesday button
  if (GuiButton((Rectangle){124, 200, 100, 25}, "Tuesday")) {
    dayIndex = 1;
    *step = 1;
  }
  // Draw wednesday button
  if (GuiButton((Rectangle){236, 200, 100, 25}, "Wednesday")) {
    dayIndex = 2;
    *step = 1;
  }
  // Draw Thursday button
  if (GuiButton((Rectangle){348, 200, 100, 25}, "Thursday")) {
    dayIndex = 3;
    *step = 1;
  }
  // Draw friday button
  if (GuiButton((Rectangle){460, 200, 100, 25}, "Friday")) {
    dayIndex = 4;
    *step = 1;
  }
  // Draw saturday button
  if (GuiButton((Rectangle){572, 200, 100, 25}, "Saturday")) {
    dayIndex = 5;
    *step = 1;
  }
  // Draw sunday button
  if (GuiButton((Rectangle){684, 200, 100, 25}, "Sunday")) {
    dayIndex = 6;
    *step = 1;
  }
  return dayIndex;
}

int getHourGui(int *step) {
  static int hour = 0;
  DrawText("Which time interval would you like to check?", 180, 250, 20, BLACK);
  if (GuiButton((Rectangle){28, 300, 100, 25}, "00:00 - 04:00")) {
    hour = 0;
    *step = 2;
  }
  if (GuiButton((Rectangle){156, 300, 100, 25}, "04:00 - 08:00")) {
    hour = 1;
    *step = 2;
  }
  if (GuiButton((Rectangle){284, 300, 100, 25}, "08:00 - 12:00")) {
    hour = 2;
    *step = 2;
  }
  if (GuiButton((Rectangle){412, 300, 100, 25}, "12:00 - 16:00")) {
    hour = 3;
    *step = 2;
  }
  if (GuiButton((Rectangle){540, 300, 100, 25}, "16:00 - 20:00")) {
    hour = 4;
    *step = 2;
  }
  if (GuiButton((Rectangle){668, 300, 100, 25}, "20:00 - 24:00")) {
    hour = 5;
    *step = 2;
  }

  return hour;
}
