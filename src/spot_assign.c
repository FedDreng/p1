#include <stdio.h>

// simulating like 6-7 parking lots (more like 4) with 10 spots and parked
// space with numberplate

int occupy[4][10] = {0};
char parkNumberplate[4][10][20];
//
// Define the different variables
int spot_assign() {
  char carType[20];
  char numberPlate[20];
  char spot;
  int spotNumber;
  int spotIndex;
  int i;

  // get data
  while (1) {
    printf("Enter car type (HATCHBACK, SEDAN, SUV, EV): ");
    scanf("%s", carType);

    printf("Enter car number plate");
    scanf("%s", numberPlate);

    // Assign a parking lots

    if (carType[0] == 'H' || carType[0] == 'h') {
      spot = 'A';
      spotIndex = 0; // hatchback
    } else if (carType[0] == 'S' || carType[0] == 's') {
      spot = 'B';
      spotIndex = 1; // sudan
    } else if (carType[0] == 'U' || carType[0] == 'u') {
      spot = 'C';
      spotIndex = 2; // suv
    } else if (carType[0] == 'E' || carType[0] == 'e') {
      spot = 'E';
      spotIndex = 3; // EV
    } else {
      printf("Unknown car.\n");
      continue;
    }

    // check numberplate is written good
    if (numberPlate[6] == '\0') {
      printf("Invalid plate: must be at least 7 characters.\n");
      continue;
    }
    if (numberPlate[7] != '\0') {
      printf("Invalid plate: must be exactly 7 characters.\n");
      continue;
    }

    // final character must be digit
    if (numberPlate[6] < '0' || numberPlate[6] > '9') {
      printf("Invalid plate: last character must be a digit.\n");
      continue;
    }

    // use numberplate to check if spot is taken type shift
    spotNumber = numberPlate[6] - '0';

    if (occupy[spotIndex][spotNumber] == 1) {
      printf("This spot %c-%d is taken\n", spot, spotNumber);
      continue;
    }

    // check if spot is taken and store numberplates

    for (i = 0; i < 20; i++) {
      parkNumberplate[spotIndex][spotNumber][i] = numberPlate[i];
      if (numberPlate[i] == '\0')
        break;
    }

    occupy[spotIndex][spotNumber] = 1;

    // output
    printf("\nAssigned parking spot is: %c-%d\n", spot, spotNumber);
    printf("Your car %s now occupied this spot.\n", numberPlate);
  }
  return 0;
}
