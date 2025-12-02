#ifndef P1_PREFERENCES_H
#define P1_PREFERENCES_H

#include <parking_lots_matrixs_utils.h>
#include <stdio.h>

void getPreferences(int userProfile);
void savePreferences(boolean makeDefault);
void setPreferences();
void changePreferences();
int changePrefGui();

extern int prefOption;
extern struct userPref currentUser;

typedef enum {
  newProfile,
  username,
  licensePlate,
  handicapped,
  isEV,
  prefClose,
  prefIsolated,
  TOTAL_ELEMENTS
} changeType;

typedef enum {
  notHandicapped,
  isHandicapped,
  HANDIELEMENTS
} handicappedElement;

typedef enum { yesEV, noEV, EVELEMENTS } EVElements;

extern changeType selectedType;

struct userPref {
  char username[20];
  char licensePlate[8]; // the user can save their license plate so the app can
                        // remember for future use
  boolean is_handicapped; // defines whether the license plate is attached to a
                          // handicap car or not
  boolean
      is_ev; // defines whether the license plate is attached to an EV or not
  int prefClose; // the user can choose which parking lot they prefer to park at
                 // so the app will weigh that one higher when selecting parking
                 // spot
  int prefIsolated; // the user specifies whether they want their parking spot
                    // to be isolated, IE with as few cars surrounding it as
                    // possible or not
};

// char defaultProfile[20] = "";
extern int numberOfProfiles;
extern int currentUserNR;
extern char defaultProfile[20];

extern FILE *fUserPref;
extern FILE *fUserTemp;

#endif // P1_PREFERENCES_H
