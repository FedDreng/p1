#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <preferences.h>


char defaultProfile[20] = "";
int numberOfProfiles = 2;
char prefFileLocation[] = "..\\assets\\parkingLots\\userPreferences.txt";

FILE *fUserPref;
FILE *fUserTemp;

void getPreferences() {
    printf("We attempt to read our preference txt file and print the contents\n");
    fUserPref = fopen(prefFileLocation, "r");

    char tempName[20];
    char tempPlate[20];
    int tempNum;
    numberOfProfiles = 0;
    while (fscanf(fUserPref, "%s %s %d", tempName, tempPlate, &tempNum) == 3) {
        numberOfProfiles++;
        printf("Profile: %d, Username: %s, License Plate: %s, Parking Preference: %d\n", numberOfProfiles, tempName, tempPlate, tempNum);
    }
    //struct userPref *allUsers = calloc(numberOfProfiles, sizeof(struct userPref));

    //free(allUsers);
    fclose(fUserPref);
}

void savePreferences() {
    fUserPref = fopen(prefFileLocation, "r+");
}

void setPreferences(int userProfile) {
    struct userPref *allUsers = calloc(numberOfProfiles, sizeof(struct userPref));

    //for (int i = 0; i < (sizeof(allUsers[userProfile-1].username)/sizeof(char)); i++) {//this is to be changed to take a name input and store it as a username for the user profile
    //    allUsers[userProfile].username[i-1] = 'a'; //test input, username should become "aaaaaaaaaaaaaaaaaaaa"
    //}
    allUsers[userProfile].prefParkingLot = 1; //this is to be changed to take input in the form of preferred parking area, with the given number representing each area of the parking lot or determining which specific parking lot is preferred

    fUserPref = fopen(prefFileLocation, "r+");
    fprintf(fUserPref, "I wrote to the file, and deleted the old stuff");
    free(allUsers);
}

void changeProfileName(int userProfile, char newName[20]) {

}