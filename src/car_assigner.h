#ifndef CAR_ASSIGNER_H
#define CAR_ASSIGNER_H

#include "preferences.h"
// car builder
typedef struct {
  carSize size;  // small / medium / large
  boolean is_ev; // needs EV charger
  boolean is_handicapped;
  // boolean wants_uni_close;
  // boolean wants_exit_close;
  boolean placementPref;
  boolean want_Isolated;
  struct userPref Pref;
} Car;

void mouseAssigner();
void assigner(int x, int y);
void assignCar(Car *car);
Car createCarFromInput(Car current);
#endif
