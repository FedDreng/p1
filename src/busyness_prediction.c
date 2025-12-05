#include "busyness_prediction.h"
#include "raygui.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
/*
 *Prototype til 2D array til at kunne kategorisere travlheden.
 *
 */
BusynessSchedule busynessSchedule();
/*
 * Prototype til en funktion som tager data
 * Bruger dataen til at returnere en struct, hvorfra travlhed kan indlæses
 */
Time userinput(int *step);
/*
 * Returnere enum typen tilsvarende den indtastet dato.
 */
days getDay(char day[]);
/*
 * Printer ud hvor travlt der er i det ønskede interval.
 */
void printBusyness(Time chosenInput, BusynessSchedule schedule, int *step);
// Optegner en gui til at vælge en dag
int getDayGui(int *step);
// Optegner time intervallerner:
int getHourGui(int *step);

// Udregn travlhed (Hoved funktion)
int busynessCalculator() {
  // Variable til at holde styr på hvor langt man er.
  static int step = 0;
  // Starter overskrift
  DrawText("Choose a day and a time.", 280, 80, 20, BLACK);
  BusynessSchedule busyness = busynessSchedule();
  // Optegner alle elementerne til at vælge tidsrum.
  Time chosenTime = userinput(&step);
  printBusyness(chosenTime, busyness, &step);
}

// 2D array til at kunne kategorisere travlheden.
BusynessSchedule busynessSchedule() {
  /*Kategorisere de syge dage i 6 intervallet af 4 timer hvor i
   *man kan indtaste travlheden (Det inddeles i intervaller af 4 timer da 7 * 24
   * tidrum ville være besværligt at indtaste)
   *
   *De bliver givet en værdi fra 1 - 3, hvor 1 er meget stille og 3 er meget
   * travlt.
   */
  BusynessSchedule b = {.data = {// Mandag
                                 {1, 1, 2, 3, 1, 1},
                                 // Tirsdag
                                 {1, 1, 3, 2, 1, 1},
                                 // Onsdag
                                 {1, 1, 3, 2, 1, 1},
                                 // Torsdag
                                 {1, 1, 3, 2, 1, 1},
                                 // Fredag
                                 {1, 1, 2, 2, 1, 1},
                                 // Lørdag
                                 {1, 1, 1, 1, 1, 1},
                                 // Søndag
                                 {1, 1, 1, 1, 1, 1}}};

  return b;
}

// Udførelse af userinput funktion
Time userinput(int *step) {
  // Tekststeng til at opbevare valg af dag.
  char day[3];
  // Index til enum
  int dayIndex = 0;
  // Index til time interval
  int hourIndex = 0;
  // Tegn dag muligheder og få input
  dayIndex = getDayGui(step);
  // Tegn time muligheder og få input
  hourIndex = getHourGui(step);
  // Opret struct med valgt info.
  Time chosenTime;
  chosenTime.Day = dayIndex;
  chosenTime.hour = hourIndex;
  return chosenTime;
}

void printBusyness(Time chosenInput, BusynessSchedule schedule, int *step) {
  // Finder værdien fra 1 - 3 i vores schedule
  int busynessValue;
  // Printer den tilsvarende værdi.
  if (GuiButton((Rectangle){350, 350, 100, 25}, "Predict busyness") &&
      *step == 2) {
    *step = 3;
  }
  if (*step == 3) {
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
  }
}

days getDay(char day[]) {
  /*Lav en string compare check. Der skal checkes om
   *alle de først bogstaver passer i user input og navnet på dagen. Hvis det gør
   *kan der returneres den korrekte dag.
   */
  if (strcmp(("mon"), day) == 0) {
    return monday;
  } else if (strcmp(("tue"), day) == 0) {
    return tuesday;
  } else if (strcmp(("wed"), day) == 0) {
    return wednesday;
  } else if (strcmp(("thu"), day) == 0) {
    return thursday;
  } else if (strcmp(("fri"), day) == 0) {
    return friday;
  } else if (strcmp(("sat"), day) == 0) {
    return saturday;
  } else if (strcmp(("sun"), day) == 0) {
    return sunday;
  } else {
    return error;
  }
}

int getDayGui(int *step) {
  static int dayIndex = 0;
  // Optegn tekst til at beskrive det er dage
  DrawText("Which day would you like to check?", 220, 160, 20, BLACK);
  // Optegne mandag knap:
  if (GuiButton((Rectangle){12, 200, 100, 25}, "Monday")) {
    dayIndex = 0;
    *step = 1;
  }
  // Optegn tirsdag:
  if (GuiButton((Rectangle){124, 200, 100, 25}, "Tuesday")) {
    dayIndex = 1;
    *step = 1;
  }
  // Optegn onsdag:
  if (GuiButton((Rectangle){236, 200, 100, 25}, "Wednesday")) {
    dayIndex = 2;
    *step = 1;
  }
  // Optegn torsdag:
  if (GuiButton((Rectangle){348, 200, 100, 25}, "Thursday")) {
    dayIndex = 3;
    *step = 1;
  }
  // Optegn fredag:
  if (GuiButton((Rectangle){460, 200, 100, 25}, "Friday")) {
    dayIndex = 4;
    *step = 1;
  }
  // Optegn lørdag:
  if (GuiButton((Rectangle){572, 200, 100, 25}, "Saturday")) {
    dayIndex = 5;
    *step = 1;
  }
  // Optegn søndag:
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
