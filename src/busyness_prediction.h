#ifndef P1_BUSINESS_PREDICTION_H
#define P1_BUSINESS_PREDICTION_H
typedef enum {
  monday,
  tuesday,
  wednesday,
  thursday,
  friday,
  saturday,
  sunday,
  error,
} days;

typedef struct {
  int data[7][6];
} BusynessSchedule;

typedef struct {
  days Day;
  int hour;
} Time;

int busynessCalculator();

#endif
