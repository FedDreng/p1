//
// Created by Sullz on 24/11/2025.
//

#ifndef P1_PREFERENCES_H
#define P1_PREFERENCES_H

#include <stdio.h>

void getPreferences();
void savePreferences();
void setPreferences(int userProfile);

struct userPref {
  char username[20];
  char licensePlate[7]; // the user can save their license plate so the app can
                        // remember for future use
  int prefParkingLot; // the user can choose which parking lot they prefer to
                      // park at so the app will weigh that one higher when
                      // selecting parking spot
};

// char defaultProfile[20] = "";
extern int numberOfProfiles;
extern char defaultProfile[20];

extern FILE *fUserPref;
extern FILE *fUserTemp;

#endif // P1_PREFERENCES_H
