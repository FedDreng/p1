#ifndef CAR_TYPES_H
#define CAR_TYPES_H

#include <stdbool.h>

// --- Enum for bilstørrelse ---
typedef enum {
    CAR_SMALL = 1,
    CAR_MEDIUM,
    CAR_LARGE
} CarSize;

// --- Struktur for bilens egenskaber ---
typedef struct {
    CarSize size;
    bool isElectric;
    bool isHandicap;
} CarProperties;

// --- Funktioner til håndtering af bilinput ---

// Reset bilens state
void ResetCar(CarProperties *car);

// Debug-print (fx til terminal)
void PrintCarInfo(CarProperties car);

#endif

