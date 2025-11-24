#ifndef CAR_INPUT_H
#define CAR_INPUT_H

#include "Car_Types.h"

typedef struct {
    CarProperties car;
    int guiStep;
} CarInputState;

CarInputState GetCarInput(CarInputState state);
//CarProperties GetCarInput(void);


#endif

