#ifndef CAR_INPUT_H
#define CAR_INPUT_H

#include "Car_Types.h"
// This header defines the CarInputState struct, which bundles together the
// current car properties and the GUI step counter.
// The struct is used by the implementation in Car_input.c.
typedef struct {
    CarProperties car;      // All properties of the car
    int guiStep;            // State/step used by the GUI to control input

} CarInputState;
// Returns an updated CarInputState based on the current input conditions.
// The function is implemented in Car_input.c.

CarInputState GetCarInput(CarInputState state);
//CarProperties GetCarInput(void);


#endif

