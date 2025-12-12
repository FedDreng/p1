#ifndef CAR_ASSIGNER_H
#define CAR_ASSIGNER_H

#include "preferences.h"
// car builder
typedef struct {
  carSize size; // small / medium / large
  typeBoolean is_ev;
  typeBoolean is_handicapped;
  typeBoolean placementPref;
  typeBoolean want_Isolated;
  struct userPref Pref;
} Car;

void mouseAssigner(Car *car);
void loadOccipied();
void assigner(int x, int y);
void assignCar(Car *car);
Car createCarFromInput(Car current);

// for testing.
int calculateLotScore(const Car *car, const lot *L);
lot *chooseBestLot(const Car *car);
int isIsolated(int x, int y);
#endif
