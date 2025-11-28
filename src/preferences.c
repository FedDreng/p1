#include <preferences.h>
#include <stdio.h>
#include <stdlib.h>

char defaultProfile[20] = ""; // the current default profile, will be read from file when program launches
int numberOfProfiles = 2; // the number of profiles currently found in file
char prefFileLocation[] = "../assets/userPreferences.txt"; // preference file absolute path
FILE *fUserPref;
FILE *fUserTemp;
struct userPref currentUser;
struct userPref tempUser;

void getPreferences(int userProfile) {   // function to read preference profiles from file
  printf("We attempt to read our preference txt file and print the contents\n");
  fUserTemp = fopen(prefFileLocation, "r");

  char tempName[20];
  char tempPlate[20];
  int tempNum;
  numberOfProfiles = 0;
  while (fscanf(fUserTemp, "%s %s %d", tempName, tempPlate, &tempNum) == 3) {
    numberOfProfiles++;
    printf("Profile: %d, Username: %s, License Plate: %s, Parking Preference: %d\n", numberOfProfiles, tempName, tempPlate, tempNum);
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

void changeProfileName(int userProfile, char newName[20]) {

}