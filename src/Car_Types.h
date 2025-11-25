#ifndef CAR_TYPES_H
#define CAR_TYPES_H

#include <stdbool.h>

// Enum representing the available car sizes.
typedef enum {
    CAR_SMALL = 1,
    CAR_MEDIUM,
    CAR_LARGE
} CarSize;
// Struct defining the core properties of a car, including size and
// additional attributes such as whether the car is electric or
// designated for handicap use.
typedef struct {
    CarSize size;      // Physical size of the car
    bool isElectric;   // Indicates if the car is electric
    bool isHandicap;   // Indicates if the car is a handicap vehicle
} CarProperties;

// Resets all properties of the given car to their default state.
// Implemented in Car_types.c (or corresponding source file).
void ResetCar(CarProperties *car);

// Outputs the car's current properties for debugging purposes.
// Useful for logging or terminal output.
void PrintCarInfo(CarProperties car);

#endif

