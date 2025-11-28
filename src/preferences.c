#include <preferences.h>
#include <stdio.h>
#include <stdlib.h>

#include "raygui.h"
#include "raylib.h"

char defaultProfile[20] = ""; // the current default profile, will be read from file when program launches
int numberOfProfiles = 2; // the number of profiles currently found in file
char prefFileLocation[] = "../assets/userPreferences.txt"; // preference file absolute path
FILE *fUserPref;
FILE *fUserTemp;
struct userPref currentUser;
struct userPref tempUser;

const char *prefElementNames[TOTAL_ELEMENTS] = {
  "Create New Profile", "Change Username", "Change License Plate", "Change Handicap Status", "Change EV Status", "Change Location Preference", "Change Isolation Preference"};

void getPreferences(int userProfile) {   // function to read preference profiles from file
  printf("We attempt to read our preference txt file and print the contents\n");
  fUserTemp = fopen(prefFileLocation, "r");


  numberOfProfiles = 0;
  while (fscanf(fUserTemp, "%s %s %d", tempUser.username, tempUser.licensePlate, &tempUser.prefParkingLot) == 3) {
    numberOfProfiles++;
    printf("Profile: %d, Username: %s, License Plate: %s, Parking Preference: %d\n", numberOfProfiles, tempUser.username, tempUser.licensePlate, tempUser.prefParkingLot);
  }
  printf("A total of %d profiles were loaded successfully\n", numberOfProfiles);
  fclose(fUserTemp);

  if (userProfile == 0) { // when called with argument 0 should read all profiles and create struct to house them
    struct userPref *allUsers = calloc(numberOfProfiles, sizeof(struct userPref));
    fUserPref = fopen(prefFileLocation, "r");
    for (int i = 0; i < numberOfProfiles; i++) { // will be changed to only get the data from the profile line that is called as the argument for the function
      printf("Attempting to read profile %d: ", i + 1);
      if (fscanf(fUserPref, "%s %s %d", allUsers[i].username, allUsers[i].licensePlate, &allUsers[i].prefParkingLot) == 3) {
        printf("Profile %d read correctly.\n", i + 1);
      } else {
        printf("Profile %d could not be read.\n", i + 1);
      }
    }
    fclose(fUserPref);

    savePreferences();
    free(allUsers);
  } else if (userProfile > 0) { // when called with an argument other than 0 should read the specified profile if it exists
    struct userPref *allUsers = calloc(numberOfProfiles, sizeof(struct userPref));

    for (int i = 0; i < userProfile; i++) {
      if (i < userProfile) {
        if (fscanf(fUserPref, "%s %s %d", tempUser.username, tempUser.licensePlate, &tempUser.prefParkingLot) == 3) {
          printf("Profile %d read correctly.\n", i + 1);
        } else {
          printf("Could not read profile %d.\n", i + 1);
        }
      } else if (i == userProfile) {
        if (fscanf(fUserPref, "%s %s %d", currentUser.username, currentUser.licensePlate, &currentUser.prefParkingLot) == 3) {
          printf("Selected profile read correctly.\n");
        } else {
          perror("Could not read specified profile.\n");
        }
      }
    }
    free(allUsers);
  }
}

void savePreferences() { // function to save preference profiles to file

}

void setPreferences(int userProfile) { // function to alter a specific profile

}

void changePreferences() {
  DrawText("User Preferences", 300, 80, 20, BLACK);
  DrawText("Choose what to do:", 300, 120, 18, BLACK);

}

int changePrefGui() {
  static int prefOption = 0;

  int elementGap = 15;
  int elementWidth = 200;

  int posX1 = GetScreenWidth() / 2 - (elementGap + elementWidth) * (TOTAL_ELEMENTS-1) / 4;
  int posX2 = GetScreenWidth() / 2 - (elementGap + elementWidth) * (TOTAL_ELEMENTS-1) / 4;
  if (GuiButton((Rectangle){40,80,elementWidth,20},prefElementNames[0])) {
    prefOption = (changeType)0;
  }
  for (int n = 1; n < TOTAL_ELEMENTS; n++) {
    if (n <= 3) {
      if (GuiButton((Rectangle){posX1,130,elementWidth,20},prefElementNames[n])) {
        prefOption = (changeType)n;
      }
      posX1 += elementWidth + elementGap;
    } else if (n > 3) {
      if (GuiButton((Rectangle){posX2,155,elementWidth,20},prefElementNames[n])) {
        prefOption = (changeType)n;
      }
      posX2 += elementWidth + elementGap;
    }
  }
  return prefOption;
}