//
// Created by Sullz on 24/11/2025.
//

#ifndef P1_PREFERENCES_H
#define P1_PREFERENCES_H

#include <stdio.h>
#include <parking_lots_matrixs_utils.h>

void getPreferences(int userProfile);
void savePreferences();
void setPreferences(int userProfile);

struct userPref {
	boolean is_handicapped; // defines whether the license plate is attached to a handicap car or not
	boolean is_ev; // defines whether the license plate is attached to an EV or not
	char username[20];
	char licensePlate[8]; // the user can save their license plate so the app can remember for future use
	int prefParkingLot; // the user can choose which parking lot they prefer to park at so the app will weigh that one higher when selecting parking spot
	int prefIsolated; // the user specifies whether they want their parking spot to be isolated, IE with as few cars surrounding it as possible or not
};

// char defaultProfile[20] = "";
extern int numberOfProfiles;
extern char defaultProfile[20];

extern FILE *fUserPref;
extern FILE *fUserTemp;

#endif // P1_PREFERENCES_H
