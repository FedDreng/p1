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

void DrawNav();
menuElement navElementAssigner(int index);

#endif
