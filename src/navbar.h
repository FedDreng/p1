#ifndef NAVBAR_H
#define NAVBAR_H

// select element
typedef enum {
  ChooseLot,
  ChooseSize,
  ChooseType,
  ChooseBusyness,
  ELEMENT_COUNT
} menuElement;

extern menuElement selectedElement;

void DrawNav();
menuElement navElementAssigner(int index);

#endif
