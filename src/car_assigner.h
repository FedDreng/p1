#ifndef CAR_ASSIGNER_H
#define CAR_ASSIGNER_H

#include "preferences.h"
// car builder
typedef struct {
  carSize size; // small / medium / large
  boolean is_ev;
  boolean is_handicapped;
  boolean placementPref;
  boolean want_Isolated;
  struct userPref Pref;
} Car;

void mouseAssigner();
void loadOccipied();
void assigner(int x, int y);
void assignCar(Car *car);
Car createCarFromInput(Car current);
#endif
