#include <stdio.h>

#include "busyness_prediction.h"

#include <string.h>
/*
 *Prototype til 2D array til at kunne kategorisere travlheden.
 *
*/
BusynessSchedule busynessSchedule();
/*
 * Prototype til en funktion som tager data
 * Bruger dataen til at returnere en struct, hvorfra travlhed kan indlæses
*/
Time userinput();
/*
 * Returnere enum typen tilsvarende den indtastet dato.
*/
days getDay(char day[]);
/*
 * Printer ud hvor travlt der er i det ønskede interval.
*/
void printBusyness(Time chosenInput, BusynessSchedule schedule);

//Udregn travlhed (Hoved funktion)
int busynessCalculator() {
    BusynessSchedule busyness = busynessSchedule();
    Time chosenTime = userinput();
    printBusyness(chosenTime, busyness);
}

//2D array til at kunne kategorisere travlheden.
BusynessSchedule busynessSchedule() {
    /*Kategorisere de syge dage i 6 intervallet af 4 timer hvor i
     *man kan indtaste travlheden (Det inddeles i intervaller af 4 timer da 7 * 24 tidrum ville være
     *besværligt at indtaste)
     *
     *De bliver givet en værdi fra 1 - 3, hvor 1 er meget stille og 3 er meget travlt.
    */
    BusynessSchedule b = {
        .data = {
            // Mandag
            {1,1,2,3,1,1},
            // Tirsdag
            {1,1,3,2,1,1},
            // Onsdag
            {1,1,3,2,1,1},
            // Torsdag
            {1,1,3,2,1,1},
            // Fredag
            {1,1,2,2,1,1},
            // Lørdag
            {1,1,1,1,1,1},
            // Søndag
            {1,1,1,1,1,1}
        }
    };

    return b;
}

//Udførelse af userinput funktion
Time userinput() {
    //Tekststeng til at opbevare valg af dag.
    char day[3];
    int dayIndex = 0;
    int hourIndex = 0;
    //Start med at spørge bruger indtil dag
    do {
        //Tjekker om dagindexet er 7, hvilket er en error
        if (dayIndex == 7) {
            printf("The chosen input is not valid, please try again.\n");
        }
        //Spørger brugeren om en dag og scanner efterfølgende.
        printf("Choose a day to check busyness: mon, tue, wed, thu, fri, sat, sun, \n>");
        scanf(" %3s", day);
        //Udregner enum dag værdien.
        dayIndex = getDay(day);
    }while (dayIndex == 7);
    //Spørger brugeren indtil et tidsrum (Intervaller af 4 timer)
    do {
        if (hourIndex < 0 || hourIndex > 5) {
            printf("The chosen input is not valid, it must be an integer between 0 and 5.\n");
        }
        printf("Choose an hour interval of the day:\n"
               "0: 00:00 - 04:00\n"
               "1: 04:00 - 08:00\n"
               "2: 08:00 - 12:00\n"
               "3: 12:00 - 16:00\n"
               "4: 16:00 - 20:00\n"
               "5: 20:00 - 24:00\n"
               ">");
        scanf(" %d", &hourIndex);
    }while (hourIndex < 0 || hourIndex > 5);
    Time chosenTime;
    chosenTime.Day = dayIndex;
    chosenTime.hour = hourIndex;
    return chosenTime;
}

void printBusyness(Time chosenInput, BusynessSchedule schedule) {
    //Finder værdien fra 1 - 3 i vores schedule
    int busynessValue = schedule.data[chosenInput.Day][chosenInput.hour];
    //Printer den tilsvarende værdi.
    switch (busynessValue) {
        case 1:
            printf("The parking lot is not very busy at the chosen time\n");
            break;
        case 2:
            printf("The parking lot is moderately busy at the chosen time\n");
            break;
        case 3:
            printf("The parking lot is very busy at the chosen time\n");
            break;
        default:
            printf("Could not find the busyness\n");
    }
}

days getDay(char day[]) {
    /*Lav en string compare check. Der skal checkes om
     *alle de først bogstaver passer i user input og navnet på dagen. Hvis det gør
     *kan der returneres den korrekte dag.
    */
    if (strcmp(("mon"),day) == 0) {
        return monday;
    }else if (strcmp(("tue"),day) == 0) {
        return tuesday;
    }else if (strcmp(("wed"),day) == 0) {
        return wednesday;
    }else if (strcmp(("thu"),day) == 0) {
        return thursday;
    }else if (strcmp(("fri"),day) == 0) {
        return friday;
    }else if (strcmp(("sat"),day) == 0) {
        return saturday;
    }else if (strcmp(("sun"),day) == 0) {
        return sunday;
    }else {
        return error;
    }

}