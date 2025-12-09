#include "parking_lots_matrixs_utils.h"
#include "raylib.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>

typeBoolean hasAssignedSpot;

lot parkingGrid[GRID_HEIGHT][GRID_WIDTH];

int read_x = 0, read_y = 0;

void readParkingLotFile() {

  int countedFiles = 0;

  const char *path = "../assets/parkingLots";
  struct dirent *entry;

  char *files[10];

  DIR *dir = opendir(path);
  if (dir == NULL) {
    perror("opendir");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    printf("%s\n", entry->d_name);
    files[countedFiles] = strdup(entry->d_name);
    countedFiles++;
  }
  closedir(dir);

  printf("Chose what parking lot you want to use: \n");

  for (int i = 0; i < countedFiles; i++) {
    printf("%d, %s \n", i + 1, files[i]);
  }

  scanf("%d", &countedFiles);
  countedFiles -= 1;

  char chosenFileName[256];
  sprintf(chosenFileName, "../assets/parkingLots/%s", files[countedFiles]);

  FILE *fp = fopen(chosenFileName, "r");
  if (fp == NULL) {
    perror("No files to read.");
    return;
  }

  printf("\n");

  // Calculate dimensions
  int ch;
  read_x = 0;
  read_y = 0;
  int current_x = 0;
  int max_x = 0;

  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n') {
      if (current_x > max_x) {
        max_x = current_x;
      }
      read_y++;
      current_x = 0;
    } else if (ch != ' ') {
      current_x++;
    }
  }

  // Handle last line if no newline at end
  if (current_x > 0) {
    if (current_x > max_x) {
      max_x = current_x;
    }
    read_y++;
  }

  read_x = max_x;

  printf("File dimensions: width=%d, height=%d\n", read_x, read_y);
  printf("Grid capacity: width=%d, height=%d\n", GRID_WIDTH, GRID_HEIGHT);

  if (read_x > GRID_WIDTH || read_y > GRID_HEIGHT) {
    printf("ERROR: File size (%dx%d) exceeds grid capacity (%dx%d)\n", read_x,
           read_y, GRID_WIDTH, GRID_HEIGHT);
    printf("GRID_WIDTH and GRID_HEIGHT to small\n");
    fclose(fp);
    return;
  }

  if (read_x == 0 || read_y == 0) {
    printf("ERROR: Invalid dimensions\n");
    fclose(fp);
    return;
  }

  // Initialize entire grid
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      parkingGrid[i][j].type = road;
      parkingGrid[i][j].x = j;
      parkingGrid[i][j].y = i;
      parkingGrid[i][j].occupied = FALSE;
      parkingGrid[i][j].isBlinking = FALSE;
      parkingGrid[i][j].allowed = (carSize){FALSE, FALSE, FALSE};
      parkingGrid[i][j].lot_size = (carSize){FALSE, FALSE, FALSE};
    }
  }

  // Now read and parse the file
  rewind(fp);
  int h = 0, k = 0;

  printf("\nParsing file:\n");

  while ((ch = fgetc(fp)) != EOF) {

    // Skip spaces
    if (ch == ' ') {
      continue;
    }

    // Handle newline
    if (ch == '\n') {
      printf("\n");
      k++;
      h = 0;
      continue;
    }

    // Bounds check
    if (k >= read_y || h >= read_x) {
      h++;
      continue;
    }

    // Parse character
    switch (ch) {
    case 'r':
      printf("r ");
      parkingGrid[k][h].type = road;
      parkingGrid[k][h].lot_size = (carSize){FALSE, FALSE, FALSE};
      parkingGrid[k][h].allowed = (carSize){FALSE, FALSE, FALSE};
      break;

    case 'o':
      printf("o ");
      parkingGrid[k][h].type = obstacle;
      parkingGrid[k][h].lot_size = (carSize){FALSE, FALSE, FALSE};
      parkingGrid[k][h].allowed = (carSize){FALSE, FALSE, FALSE};
      break;

    case 's':
      printf("s ");
      parkingGrid[k][h].type = parking_bay;
      parkingGrid[k][h].lot_size = (carSize){TRUE, FALSE, FALSE}; // Small space
      parkingGrid[k][h].allowed =
          (carSize){TRUE, FALSE, FALSE}; // Only small cars fit
      break;

    case 'm':
      printf("m ");
      parkingGrid[k][h].type = parking_bay;
      parkingGrid[k][h].lot_size =
          (carSize){FALSE, TRUE, FALSE}; // Medium space
      parkingGrid[k][h].allowed =
          (carSize){TRUE, TRUE, FALSE}; // Small and medium cars fit
      break;

    case 'l':
      printf("l ");
      parkingGrid[k][h].type = parking_bay;
      parkingGrid[k][h].lot_size = (carSize){FALSE, FALSE, TRUE}; // Large space
      parkingGrid[k][h].allowed = (carSize){TRUE, TRUE, TRUE}; // All cars fit
      break;

    case 'h':
      printf("h ");
      parkingGrid[k][h].type = handicaped;
      parkingGrid[k][h].lot_size =
          (carSize){FALSE, FALSE, TRUE}; // Large space (handicap accessible)
      parkingGrid[k][h].allowed = (carSize){
          FALSE, TRUE, TRUE}; // Medium and large cars (with handicap permit)
      break;

    case 'n':
      printf("n ");
      parkingGrid[k][h].type = EV;
      parkingGrid[k][h].lot_size =
          (carSize){FALSE, TRUE, FALSE}; // Medium EV space
      parkingGrid[k][h].allowed =
          (carSize){TRUE, TRUE, FALSE}; // Small and medium EVs
      break;

    case 'k':
      printf("k ");
      parkingGrid[k][h].type = EV;
      parkingGrid[k][h].lot_size =
          (carSize){FALSE, FALSE, TRUE}; // Large EV space
      parkingGrid[k][h].allowed =
          (carSize){TRUE, TRUE, TRUE}; // All EVs (small, medium, large)
      break;

    default:
      printf("?  ");
      break;
    }

    h++;
  }

  printf("\n\nGrid loaded successfully: %dx%d\n", read_x, read_y);

  fclose(fp);
}

void createParkingLotGrid() {

  ParkingType design[GRID_HEIGHT][GRID_WIDTH] = {
      {road, road, road, road, road, road, road},
      {road, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
      {road, handicaped, parking_bay, parking_bay, parking_bay, parking_bay,
       parking_bay},
      {road, road, road, road, road, road, road},
      {road, parking_bay, parking_bay, parking_bay, parking_bay, parking_bay,
       parking_bay},
      {road, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
      {road, road, road, road, road, road, road}};

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      parkingGrid[y][x].x = x;
      parkingGrid[y][x].y = y;
      parkingGrid[y][x].type = design[y][x];

      if (design[y][x] == parking_bay) {
        parkingGrid[y][x].allowed = (carSize){true, true, true};
      } else if (design[y][x] == handicaped) {
        parkingGrid[y][x].allowed = (carSize){false, true, true};
      } else if (design[y][x] == EV) {
        parkingGrid[y][x].allowed = (carSize){true, true, false};
      } else {
        parkingGrid[y][x].allowed = (carSize){false, false, false};
      }

      parkingGrid[y][x].occupied = FALSE;
    }
  }
}

void loadOccipied() {
  FILE *PloadOccipied = fopen("../assets/occipied. txt", "r");
  if (!PloadOccipied) {
    return; // File doesn't exist yet, that's okay
  }

  char line[256];
  Occipied record;

  printf("\nLoading occupied spots:\n");

  while (fgets(line, sizeof(line), PloadOccipied)) {

    if (sscanf(line, "%49[^,],%19[^,],%d,%d", record.username, record.license,
               &record.posX, &record.posY) == 4) {

      if (record.posX >= 0 && record.posX < read_x && record.posY >= 0 &&
          record.posY < read_y) {

        parkingGrid[record.posY][record.posX].occupied = TRUE;
        strcpy(parkingGrid[record.posY][record.posX].username, record.username);
        strcpy(parkingGrid[record.posY][record.posX].licensePlate,
               record.license);

        printf("  %s at [%d][%d]\n", record.username, record.posY, record.posX);
      } else {
        printf("  OUT OF boundss: (%d,%d)\n", record.posX, record.posY);
      }
    }
  }

  fclose(PloadOccipied);
}

Color getParkingColor(ParkingType t) {
  switch (t) {
  case road:
    return GRAY;
  case obstacle:
    return RED;
  case handicaped:
    return BLUE;
  case parking_bay:
    return GREEN;
  case EV:
    return YELLOW;
  default:
    return WHITE;
  }
}

void showParkingGridRayLib() {
  int gridWidth = read_x * CELL_SIZE;
  int gridHeight = read_y * CELL_SIZE;

  int offsetX = (GetScreenWidth() - gridWidth) / 2;
  int offsetY = ((GetScreenHeight() - gridHeight) / 2) + 60 / 2;

  const char *msg;
  if (hasAssignedSpot) {
    msg = "You have been assigned this spot";
    DrawText(msg, (GetScreenWidth() - MeasureText(msg, 24)) / 2, offsetY - 40,
             24, GREEN);
  } else {
    msg = "Could not find a suitable spot, but you can manually select one if "
          "possible";
    DrawText(msg, (GetScreenWidth() - MeasureText(msg, 20)) / 2, offsetY - 40,
             20, RED);
  }

  for (int y = 0; y < read_y; y++) {
    for (int x = 0; x < read_x; x++) {
      lot *currentLot = &parkingGrid[y][x];
      Rectangle rect = {offsetX + x * CELL_SIZE, offsetY + y * CELL_SIZE,
                        CELL_SIZE, CELL_SIZE};

      if (currentLot->isBlinking) {
        // BLINKING SPOT - alternates every 0.33 seconds
        double time = GetTime();
        int blinkPhase = (int)(time * 3.0) % 2; // Blinks 3 times per second

        if (blinkPhase == 0) {
          // Blink phase - show bright ORANGE
          DrawRectangleRec(rect, ORANGE);
          // Draw thick yellow border for extra visibility
          DrawRectangleLinesEx(rect, 4, YELLOW);
        } else {
          // Non-blink phase - show base color with green border
          DrawRectangleRec(rect, getParkingColor(currentLot->type));
          DrawRectangleLinesEx(rect, 4, GREEN);
        }

      } else if (currentLot->occupied) {
        // OCCUPIED SPOT
        DrawRectangleRec(rect, DARKGRAY);
        DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, BLACK);

      } else {
        // NORMAL SPOT
        DrawRectangleRec(rect, getParkingColor(currentLot->type));
        DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, BLACK);
      }
    }
  }
}

// Function to check if a car can fit in a given parking lot
typeBoolean canFit(carSize car, lot *currentLot) {

  // If the lot is already occupied, the car cannot fit
  if (currentLot->occupied)
    return FALSE;

  // Check if the lot type is one of the allowed types
  // Only parking_bay, handicaped, or EV types are valid for parking
  if (currentLot->type != parking_bay && currentLot->type != handicaped &&
      currentLot->type != EV)
    return FALSE;

  // Check if car fits in the space size
  // Small cars can fit in any size space (small, medium, or large)
  if (car.is_small) {
    return TRUE; // Small cars fit anywhere
  }

  // Medium cars need medium or large spaces
  if (car.is_medium) {
    if (currentLot->lot_size.is_medium || currentLot->lot_size.is_large) {
      return TRUE;
    }
    return FALSE; // Medium car can't fit in small space
  }

  // Large cars need large spaces only
  if (car.is_large) {
    if (currentLot->lot_size.is_large) {
      return TRUE;
    }
    return FALSE; // Large car can't fit in small or medium space
  }

  return FALSE;
}

// what i changed.
// made some updates to carFit function,
// made some changes to the file reader, now the x and y (h and k) is at the
// right spot. As well as the file get closes propperly. The grid gets
// Initialized before putting in the values. made the blinker work and got chat
// to make it look cool because why not. carSize got inplemented the right way,
// occipied gets set to false by default, updated loadOccipied to work with the
// new part and now it got a way to tell if the thing is out of bounds
// Added some debuging to see what is going on and what was wrong..
