#include <preferences.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <stddef.h>

#include "navbar.h"
#include "raygui.h"
#include "raylib.h"

char defaultProfile[20] = ""; // the current default profile, will be read from file when program launches
int numberOfProfiles = 2; // the number of profiles currently found in file
char prefFileLocation[] = "../assets/userPreferences.txt"; // preference file absolute path
FILE *fUserPref;
FILE *fUserTemp;
struct userPref currentUser;
struct userPref tempUser;

int prefOption = -1;
char tempName[21] = "\0";
char tempPlate[8] = "\0";
boolean tempHandi = false;
boolean tempEV = false;
char tempClose[2] = "\0";
char tempIso[2] = "\0";
int keyPressedPref;
char* intEnd;
//int letterCount[6] = {0, 0, 0, 0, 0, 0};
boolean textBoxEditable[6] = {0, 0, 0, 0, 0, 0};

int boxX1;
int boxX2;

const char *prefElementNames[TOTAL_ELEMENTS] = {
  "Create New Profile",
  "Change Username",
  "Change License Plate",
  "Change Handicap Status",
  "Change EV Status",
  "Change Location Preference",
  "Change Isolation Preference"
};

const char *handicappedElementNames[HANDIELEMENTS] = {
  "Not Handicapped",
  "Handicapped"
};

const char *EVElementNames[EVELEMENTS] = {
  "Not EV",
  "EV"
};

// our own strupr() because that funciton is not working on every PC because it
// is an outdated function
char *strupr(char *s) {
  if (!s)
    return NULL;
  char *p = s;
  while (*p) {
    *p = (char)toupper((unsigned char)*p);
    p++;
  }
  return s;
}

void getPreferences(int userProfile) {   // function to read preference profiles from file
  printf("We attempt to read our preference txt file and print the contents\n");
  fUserTemp = fopen(prefFileLocation, "r");


  numberOfProfiles = 0;
  while (fscanf(fUserTemp, "%s , %s , %d , %d , %d , %d", tempUser.username, tempUser.licensePlate, &tempUser.is_handicapped, &tempUser.is_ev, &tempUser.prefClose, &tempUser.prefIsolated) == 6) {
    numberOfProfiles++;
    printf("Profile: %d, Username: %s, License Plate: %s, Handicapped and/or EV: %d, %d, Parking Preference: %d, %d\n", numberOfProfiles, tempUser.username, tempUser.licensePlate, tempUser.is_handicapped, tempUser.is_ev, tempUser.prefClose, tempUser.prefIsolated);
  }
  printf("A total of %d profiles were loaded successfully\n", numberOfProfiles);
  fclose(fUserTemp);

  if (userProfile == 0) { // when called with argument 0 should read all profiles and create struct to house them
    struct userPref *allUsers = calloc(numberOfProfiles, sizeof(struct userPref));
    fUserPref = fopen(prefFileLocation, "r");
    for (int i = 0; i < numberOfProfiles; i++) { // will be changed to only get the data from the profile line that is called as the argument for the function
      printf("Attempting to read profile %d: ", i + 1);
      if (fscanf(fUserPref, "%s , %s , %d , %d , %d , %d", tempUser.username, tempUser.licensePlate, &tempUser.is_handicapped, &tempUser.is_ev, &tempUser.prefClose, &tempUser.prefIsolated) == 6) {
        printf("Profile %d read correctly.\n", i + 1);
      } else {
        printf("Profile %d could not be read.\n", i + 1);
      }
    }
    fclose(fUserPref);
    if (strlen(currentUser.username) < 1) {
      getPreferences(1);
    }

    // savePreferences(0);
    free(allUsers);
  } else if (userProfile > 0) { // when called with an argument other than 0 should read the specified profile if it exists
    struct userPref *allUsers = calloc(numberOfProfiles, sizeof(struct userPref));
    fUserPref = fopen(prefFileLocation, "r");
    for (int i = 1; i <= userProfile; i++) {
      if (i < userProfile) {
        if (fscanf(fUserPref, "%s , %s , %d , %d , %d , %d", tempUser.username, tempUser.licensePlate, &tempUser.is_handicapped, &tempUser.is_ev, &tempUser.prefClose, &tempUser.prefIsolated) == 6) {
          printf("Profile %d read correctly.\n", i);
        } else {
          printf("Could not read profile %d.\n", i);
        }
      } else if (i == userProfile) {
        if (fscanf(fUserPref, "%s , %s , %d , %d , %d , %d", currentUser.username, currentUser.licensePlate, &currentUser.is_handicapped, &currentUser.is_ev, &currentUser.prefClose, &currentUser.prefIsolated) == 6) {
          printf("Selected profile read correctly.\n");
        } else {
          perror("Could not read specified profile.\n");
        }
      }
    }
    fclose(fUserPref);
    free(allUsers);
  }
}

void savePreferences(boolean makeDefault) { // function to save preference profiles to file
  if (makeDefault == 0) {
    fclose(fUserPref);
    fUserPref = fopen(prefFileLocation, "a");
    fprintf(fUserPref, "\n%s , %s , %d , %d , %s , %s", tempName, strupr(tempPlate), tempHandi, tempEV, tempClose, tempIso);
    fclose(fUserPref);
  }
}

void setPreferences(int userProfile) { // function to alter a specific profile

}

void changePreferences() {
  DrawText(TextFormat("User Preferences"/*" Current Option %d"*/, prefOption), 280, 80, 24, BLACK);
  changePrefGui();
}

int changePrefGui() {
  if (GuiButton((Rectangle){(GetScreenWidth()-80), 70, 60, 20}, "Return")) {
    tempName[0] = '\0';
    tempPlate[0] = '\0';
    tempHandi = 0;
    tempEV = 0;
    tempClose[0] = '\0';
    tempIso[0] = '\0';
    if (prefOption < 0) {
      selectedElement = ChooseLot;
      prefOption = -1;
      return 0;
    }
    prefOption = -1;
  }

  switch (prefOption) {
    default:
      DrawText("Choose what to do:", 300, 100, 20, BLACK);
      int elementGap = 15;
      int elementWidth = 200;

      int posX1 = GetScreenWidth() / 2 - (elementGap + elementWidth) * (TOTAL_ELEMENTS-1) / 4;
      int posX2 = GetScreenWidth() / 2 - (elementGap + elementWidth) * (TOTAL_ELEMENTS-1) / 4;
      if (GuiButton((Rectangle){20, 70, 150, 20},prefElementNames[0])) {
        prefOption = (changeType)0;
      }
      if (GuiButton((Rectangle){(GetScreenWidth()-250), 70, 150, 20}, "Change Current Profile")) {
        tempName[0] = '\0';
        prefOption = 7;
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
      break;
    case 0:
      boxX1 = GetScreenWidth()*1/3-200;
      boxX2 = GetScreenWidth()*2/3-200;

      for (int j = 0; j < 6; j++) {
        DrawText(prefElementNames[j+1], boxX1, 120+(40*j), 15, BLACK);
      }

      if (GuiTextBox((Rectangle){boxX2, 120, 400, 30}, tempName, 21, textBoxEditable[0])) {
        textBoxEditable[0] = !textBoxEditable[0];
      }
      if (GuiTextBox((Rectangle){boxX2, 160, 400, 30}, tempPlate, 8, textBoxEditable[1])) {
        textBoxEditable[1] = !textBoxEditable[1];
      }
      if (GuiButton((Rectangle){boxX2, 200, 400, 30}, handicappedElementNames[tempHandi])) {
        tempHandi = !tempHandi;
      }
      if (GuiButton((Rectangle){boxX2, 240, 400, 30}, EVElementNames[tempEV])) {
        tempEV = !tempEV;
      }
      if (GuiTextBox((Rectangle){boxX2, 280, 400, 30}, tempClose, 2, textBoxEditable[2])) {
        textBoxEditable[2] = !textBoxEditable[2];
      }
      if (GuiTextBox((Rectangle){boxX2, 320, 400, 30}, tempIso, 2, textBoxEditable[3])) {
        textBoxEditable[3] = !textBoxEditable[3];
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2-25, 390, 50, 20}, "Confirm")) {
        savePreferences(0);
        tempName[0] = '\0';
        tempPlate[0] = '\0';
        tempHandi = 0;
        tempEV = 0;
        tempClose[0] = '\0';
        tempIso[0] = '\0';
      }
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      DrawText(TextFormat("Current Profile: %s", currentUser.username), 290, 100, 20, BLACK);
      DrawText("Input Profile Number", 300, 200, 20, BLACK);
      if (GuiTextBox((Rectangle){GetScreenWidth()/2-200, 220, 400, 30}, tempName, 21, textBoxEditable[0])) {
        textBoxEditable[0] = !textBoxEditable[0];
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2+205, 220, 50, 30}, "Change")) {
        getPreferences(0);
        if (strtod(tempName, &intEnd) <= numberOfProfiles && tempName[0] != '\0' && strtod(tempName, &intEnd) != 0) {
          getPreferences(strtod(tempName,&intEnd));
        }
        tempName[0] = '\0';
      }
      break;
  }
  return prefOption;
}