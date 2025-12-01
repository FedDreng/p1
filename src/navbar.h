#ifndef NAVBAR_H
#define NAVBAR_H

// select element
typedef enum {
  ChooseLot,
  ChooseSize,
  ChooseType,
  ChooseBusyness,
  ChooseSpot,
  UserProfiles,
  ELEMENT_COUNT,
  Assign
} menuElement;

extern menuElement selectedElement;
extern menuElement lastElement;

void DrawNav();
menuElement navElementAssigner(int index);

#endif
