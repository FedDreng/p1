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
int currentUserNR;
struct userPref tempUser;

int prefOption = -1;
char tempName[21] = "\0";
char tempPlate[8] = "\0";
typeBoolean tempHandi = false;
typeBoolean tempEV = false;
char tempClose[2] = "\0";
char tempIso[2] = "\0";
int keyPressedPref;
char* intEnd;
//int letterCount[6] = {0, 0, 0, 0, 0, 0};
typeBoolean textBoxEditable[6] = {0, 0, 0, 0, 0, 0};

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


char *strupr(char *s) { // simple function to turn lowercase into uppercase chars
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
  if (userProfile == 0) { // when called with argument 0 should read file and count amount of profiles
    printf("We attempt to read our preference txt file and print the contents\n");
    fUserTemp = fopen(prefFileLocation, "r");
    numberOfProfiles = 0;
    while (fscanf(fUserTemp, "%s , %s , %d , %d , %d , %d", tempUser.username, tempUser.licensePlate, &tempUser.is_handicapped, &tempUser.is_ev, &tempUser.prefClose, &tempUser.prefIsolated) == 6) {
      numberOfProfiles++;
      printf("Profile: %d, Username: %s, License Plate: %s, Handicapped and/or EV: %d, %d, Parking Preference: %d, %d\n", numberOfProfiles, tempUser.username, tempUser.licensePlate, tempUser.is_handicapped, tempUser.is_ev, tempUser.prefClose, tempUser.prefIsolated);
    }
    printf("A total of %d profiles were loaded successfully\n", numberOfProfiles);
    fclose(fUserTemp);

    // fUserPref = fopen(prefFileLocation, "r");
    // for (int i = 0; i < numberOfProfiles; i++) { // will be changed to only get the data from the profile line that is called as the argument for the function
    //   printf("Attempting to read profile %d: ", i + 1);
    //   if (fscanf(fUserPref, "%s , %s , %d , %d , %d , %d", tempUser.username, tempUser.licensePlate, &tempUser.is_handicapped, &tempUser.is_ev, &tempUser.prefClose, &tempUser.prefIsolated) == 6) {
    //     printf("Profile %d read correctly.\n", i + 1);
    //   } else {
    //     printf("Profile %d could not be read.\n", i + 1);
    //   }
    // }
    //fclose(fUserPref);

    if (strlen(currentUser.username) < 1) { // if no profile is set as current, reads first (default) profile to current
      getPreferences(1);
      currentUserNR = 1;
    }

  } else if (userProfile > 0) { // when called with an argument other than 0 should read the specified profile if it exists
    fclose(fUserPref);
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
          currentUserNR = i;
        } else {
          perror("Could not read specified profile.\n");
          getPreferences(currentUserNR);
        }
      }
    }
    fclose(fUserPref);
  }
}

void savePreferences(typeBoolean makeDefault) { // function to save preference profiles to file
  if (makeDefault == 0) { // when called with argument 0 it adds the profile at the end of the file
    fclose(fUserPref);
    fUserPref = fopen(prefFileLocation, "a");
    fprintf(fUserPref, "\n%s , %s , %d , %d , %s , %s", tempName, strupr(tempPlate), tempHandi, tempEV, tempClose, tempIso);
    fclose(fUserPref);

  } else if (makeDefault == 1) { // when called with argument 1 it adds the profile at the top of the file
    fclose(fUserPref);

    struct userPref *allUsers = calloc(numberOfProfiles+1, sizeof(struct userPref));
    for (int h = 0; h <= strlen(tempName); h++) {
      if (h == strlen(tempName)) {
        allUsers[0].username[h] = '\0';
      } else {
        allUsers[0].username[h] = tempName[h];
      }
    }
    for (int h = 0; h <= strlen(tempPlate); h++) {
      if (h == strlen(tempPlate)) {
        allUsers[0].licensePlate[h] = '\0';
      } else {
        allUsers[0].licensePlate[h] = tempPlate[h];
      }
    }
    allUsers[0].is_handicapped = tempHandi;
    allUsers[0].is_ev = tempEV;
    allUsers[0].prefClose = strtod(tempClose, &intEnd);
    allUsers[0].prefIsolated = strtod(tempIso, &intEnd);
    fUserPref = fopen(prefFileLocation, "r");
    for (int i = 1; i <= numberOfProfiles; i++) {
      if (fscanf(fUserPref, "%s , %s , %d , %d , %d , %d", allUsers[i].username, strupr(allUsers[i].licensePlate), &allUsers[i].is_handicapped, &allUsers[i].is_ev, &allUsers[i].prefClose, &allUsers[i].prefIsolated) == 6) {
        printf("Profile %d read to struct\n", i);
      }
    }
    fclose(fUserPref);
    fUserPref = fopen(prefFileLocation, "w");
    for (int j = 0; j < numberOfProfiles+1; j++) {
      printf("Profile %d: %s, %s, %d, %d, %d, %d", j+1, allUsers[j].username, strupr(allUsers[j].licensePlate), allUsers[j].is_handicapped, allUsers[j].is_ev, allUsers[j].prefClose, allUsers[j].prefIsolated);
      if (j == 0) {
        fprintf(fUserPref, "%s , %s , %d , %d , %d , %d", allUsers[j].username, strupr(allUsers[j].licensePlate), allUsers[j].is_handicapped, allUsers[j].is_ev, allUsers[j].prefClose, allUsers[j].prefIsolated);
      } else {
        fprintf(fUserPref, "\n%s , %s , %d , %d , %d , %d", allUsers[j].username, strupr(allUsers[j].licensePlate), allUsers[j].is_handicapped, allUsers[j].is_ev, allUsers[j].prefClose, allUsers[j].prefIsolated);
      }
    }
    fclose(fUserPref);
    free(allUsers);
  }
}

void setPreferences() { // function to alter data in a specific profile
  struct userPref *allUsers = calloc(numberOfProfiles+1, sizeof(struct userPref));
  fclose(fUserPref);
  fUserPref = fopen(prefFileLocation, "r");
  for (int i = 0; i < numberOfProfiles; i++) {
    if (fscanf(fUserPref, "%s , %s , %d , %d , %d , %d", allUsers[i].username, strupr(allUsers[i].licensePlate), &allUsers[i].is_handicapped, &allUsers[i].is_ev, &allUsers[i].prefClose, &allUsers[i].prefIsolated) == 6) {
      printf("Profile %d read to struct\n", i);
    }
    if (i+1 == currentUserNR) {
      printf("\ncurrent: %s, allUsers: %s i: %d", currentUser.username, allUsers[i].username, i);
      printf("\ncurrent: %s, allUsers: %s\n", currentUser.licensePlate, allUsers[i].licensePlate);
      for (int h = 0; h <= strlen(tempName); h++) {
        if (h == strlen(tempName)) {
          allUsers[i].username[h] = '\0';
        } else {
          allUsers[i].username[h] = currentUser.username[h];
        }
      }
      for (int h = 0; h <= strlen(currentUser.licensePlate); h++) {
        if (h == strlen(currentUser.licensePlate)) {
          allUsers[i].licensePlate[h] = '\0';
        } else {
          allUsers[i].licensePlate[h] = currentUser.licensePlate[h];
        }
      }
      allUsers[i].is_handicapped = currentUser.is_handicapped;
      allUsers[i].is_ev = currentUser.is_ev;
      allUsers[i].prefClose = currentUser.prefClose;
      allUsers[i].prefIsolated = currentUser.prefIsolated;
    }
  }
  fclose(fUserPref);
  fUserPref = fopen(prefFileLocation, "w");
  for (int k = 0; k < numberOfProfiles; k++) {
    printf("%s , %s , %d , %d , %d , %d\n", allUsers[k].username, strupr(allUsers[k].licensePlate), allUsers[k].is_handicapped, allUsers[k].is_ev, allUsers[k].prefClose, allUsers[k].prefIsolated);
    if (k == 0) {
      fprintf(fUserPref, "%s , %s , %d , %d , %d , %d", allUsers[k].username, strupr(allUsers[k].licensePlate), allUsers[k].is_handicapped, allUsers[k].is_ev, allUsers[k].prefClose, allUsers[k].prefIsolated);
    } else {
      fprintf(fUserPref, "\n%s , %s , %d , %d , %d , %d", allUsers[k].username, strupr(allUsers[k].licensePlate), allUsers[k].is_handicapped, allUsers[k].is_ev, allUsers[k].prefClose, allUsers[k].prefIsolated);
    }
  }
  fclose(fUserPref);
  free(allUsers);
}

void changePreferences() { // function which draws the constant menu elements for preference GUI
  DrawText(TextFormat("User Preferences"/*" Current Option %d"*/, prefOption), 280, 80, 24, BLACK);
  changePrefGui();
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
      return;
    }
    prefOption = -1;
  }
}

int changePrefGui() { // function which draws the specific menu elements for preference GUI
  switch (prefOption) { // switch checking which submenu GUI to draw
    default: // main preference menu
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
    case 0: // submenu for creating a new profile
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
      if (GuiButton((Rectangle){GetScreenWidth()/2-120, 400, 90, 20}, "Make Default")) {
        savePreferences(1);
        tempName[0] = '\0';
        tempPlate[0] = '\0';
        tempHandi = 0;
        tempEV = 0;
        tempClose[0] = '\0';
        tempIso[0] = '\0';
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2-25, 400, 50, 20}, "Confirm")) {
        savePreferences(0);
        tempName[0] = '\0';
        tempPlate[0] = '\0';
        tempHandi = 0;
        tempEV = 0;
        tempClose[0] = '\0';
        tempIso[0] = '\0';
      }
      break;
    case 1: // submenu for changing username
      DrawText(TextFormat("Current Profile: %s", currentUser.username), 290, 100, 20, BLACK);
      DrawText("Input New Name", 300, 200, 20, BLACK);
      if (GuiTextBox((Rectangle){GetScreenWidth()/2-200, 220, 400, 30}, tempName, 21, textBoxEditable[0])) {
        textBoxEditable[0] = !textBoxEditable[0];
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2+205, 220, 50, 30}, "Change")) {
        if (strcmp(tempName,currentUser.username) != 0) {
          for (int i = 0; i <= strlen(tempName); i++) {
            if (i == strlen(tempName)) {
              currentUser.username[i] = '\0';
            } else {
              currentUser.username[i] = tempName[i];
            }
          }
          setPreferences();
          tempName[0] = '\0';
        }
      }
      break;
    case 2: // submenu for changing license plate
      DrawText(TextFormat("Current Profile and License Plate: %s, %s", currentUser.username, currentUser.licensePlate), 200, 100, 20, BLACK);
      DrawText("Input New License Plate", 300, 200, 20, BLACK);
      if (GuiTextBox((Rectangle){GetScreenWidth()/2-200, 220, 400, 30}, tempPlate, 8, textBoxEditable[0])) {
        textBoxEditable[0] = !textBoxEditable[0];
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2+205, 220, 50, 30}, "Change")) {
        if (strcmp(tempPlate,currentUser.licensePlate) != 0) {
          for (int i = 0; i <= strlen(tempPlate); i++) {
            if (i == strlen(tempPlate)) {
              currentUser.licensePlate[i] = '\0';
            } else {
              currentUser.licensePlate[i] = tempPlate[i];
            }
          }
          setPreferences();
          tempPlate[0] = '\0';
        }
      }
      break;
    case 3: // submenu for changing handicapped status
      DrawText(TextFormat("Current Profile: %s", currentUser.username), 290, 100, 20, BLACK);
      if (GuiButton((Rectangle){GetScreenWidth()/2-200, 220, 400, 30}, handicappedElementNames[currentUser.is_handicapped])) {
        currentUser.is_handicapped = !currentUser.is_handicapped;
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2+205, 220, 50, 30}, "Change")) {
        setPreferences();
      }
      break;
    case 4: // submenu for changing EV status
      DrawText(TextFormat("Current Profile: %s", currentUser.username), 290, 100, 20, BLACK);
      if (GuiButton((Rectangle){GetScreenWidth()/2-200, 220, 400, 30}, EVElementNames[currentUser.is_ev])) {
        currentUser.is_ev = !currentUser.is_ev;
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2+205, 220, 50, 30}, "Change")) {
        setPreferences();
      }
      break;
    case 5: // submenu for changing proximity preference
      DrawText(TextFormat("Current Profile and Proximity Preference: %s, %d", currentUser.username, currentUser.prefClose), 200, 100, 20, BLACK);
      DrawText("Input New Proximity Preference", 300, 200, 20, BLACK);
      if (GuiTextBox((Rectangle){GetScreenWidth()/2-200, 220, 400, 30}, tempClose, 2, textBoxEditable[0])) {
        textBoxEditable[0] = !textBoxEditable[0];
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2+205, 220, 50, 30}, "Change")) {
        if (strtod(tempClose, &intEnd) != currentUser.prefClose) {
          currentUser.prefClose = strtod(tempClose, &intEnd);
          setPreferences();
        }
        tempClose[0] = '\0';
      }
      break;
    case 6: // submenu for changing isolation preference
      DrawText(TextFormat("Current Profile and Isolation Preference: %s, %d", currentUser.username, currentUser.prefIsolated), 200, 100, 20, BLACK);
      DrawText("Input New Isolation Preference", 300, 200, 20, BLACK);
      if (GuiTextBox((Rectangle){GetScreenWidth()/2-200, 220, 400, 30}, tempIso, 2, textBoxEditable[0])) {
        textBoxEditable[0] = !textBoxEditable[0];
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2+205, 220, 50, 30}, "Change")) {
        if (strtod(tempIso, &intEnd) != currentUser.prefIsolated) {
          currentUser.prefIsolated = strtod(tempIso, &intEnd);
          setPreferences();
          tempIso[0] = '\0';
        }
      }
      break;
    case 7: // submenu for changing active profile
      DrawText(TextFormat("Current Profile: %s", currentUser.username), 290, 100, 20, BLACK);
      DrawText("Input Profile Number", 300, 200, 20, BLACK);
      if (GuiTextBox((Rectangle){GetScreenWidth()/2-200, 220, 400, 30}, tempName, 21, textBoxEditable[0])) {
        textBoxEditable[0] = !textBoxEditable[0];
      }
      if (GuiButton((Rectangle){GetScreenWidth()/2+205, 220, 50, 30}, "Change")) {
        if (strtod(tempName, &intEnd) != currentUserNR) {
          getPreferences(0);
          if (strtod(tempName, &intEnd) <= numberOfProfiles && tempName[0] != '\0' && strtod(tempName, &intEnd) != 0) {
            getPreferences(strtod(tempName, &intEnd));
          }
          tempName[0] = '\0';
        }
      }
      break;
  }
  return prefOption;
}