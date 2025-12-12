#define RAYGUI_IMPLEMENTATION
#include "car_assigner.h"
#include "mtest.h"
#include "parking_lots_matrixs_utils.h"
#include "preferences.h"
#include "raygui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern lot parkingGrid[GRID_HEIGHT][GRID_WIDTH];

// ============================================
// Grid Initialization Helper
// ============================================
static void initializeTestGrid(void) {
  ParkingType design[GRID_HEIGHT][GRID_WIDTH] = {
      {road, road, road, road, road, road, road, road, road, road},
      {road, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle,
       obstacle, obstacle, road},
      {road, handicaped, parking_bay, parking_bay, parking_bay, parking_bay,
       parking_bay, parking_bay, parking_bay, road},
      {road, road, road, road, road, road, road, road, road, road},
      {road, parking_bay, parking_bay, parking_bay, parking_bay, parking_bay,
       parking_bay, parking_bay, parking_bay, road},
      {road, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle,
       obstacle, obstacle, road},
      {road, road, road, road, road, road, road, road, road, road},
      {road, EV, EV, EV, EV, EV, EV, EV, EV, road},
      {road, road, road, road, road, road, road, road, road, road},
      {road, road, road, road, road, road, road, road, road, road}};

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      parkingGrid[y][x].x = x;
      parkingGrid[y][x].y = y;
      parkingGrid[y][x].type = design[y][x];
      parkingGrid[y][x].occupied = FALSE;
      parkingGrid[y][x].isBlinking = FALSE;
      parkingGrid[y][x].dist_to_university = 0;
      parkingGrid[y][x].dist_to_exit = 0;

      if (design[y][x] == parking_bay) {
        parkingGrid[y][x].allowed = (carSize){TRUE, TRUE, TRUE};
        parkingGrid[y][x].lot_size = (carSize){FALSE, TRUE, FALSE};
      } else if (design[y][x] == handicaped) {
        parkingGrid[y][x].allowed = (carSize){FALSE, TRUE, TRUE};
        parkingGrid[y][x].lot_size = (carSize){FALSE, FALSE, TRUE};
      } else if (design[y][x] == EV) {
        parkingGrid[y][x].allowed = (carSize){TRUE, TRUE, TRUE};
        parkingGrid[y][x].lot_size = (carSize){FALSE, TRUE, FALSE};
      } else {
        parkingGrid[y][x].allowed = (carSize){FALSE, FALSE, FALSE};
        parkingGrid[y][x].lot_size = (carSize){FALSE, FALSE, FALSE};
      }

      memset(parkingGrid[y][x].username, 0, sizeof(parkingGrid[y][x].username));
      memset(parkingGrid[y][x].licensePlate, 0,
             sizeof(parkingGrid[y][x].licensePlate));
    }
  }
}

// ============================================
// Helper function to create a test car
// ============================================
static Car createTestCar(typeBoolean is_small, typeBoolean is_medium,
                         typeBoolean is_large, typeBoolean is_ev,
                         typeBoolean is_handicapped,
                         typeBoolean want_isolated) {
  Car car = {0};
  car.size.is_small = is_small;
  car.size.is_medium = is_medium;
  car.size.is_large = is_large;
  car.is_ev = is_ev;
  car.is_handicapped = is_handicapped;
  car.want_Isolated = want_isolated;
  car.placementPref = FALSE;
  strcpy(car.Pref.username, "testuser");
  strcpy(car.Pref.licensePlate, "TEST123");
  return car;
}

// ============================================
// Helper function to create a test lot
// ============================================
static lot createTestLot(int x, int y, ParkingType type, carSize lot_size,
                         typeBoolean occupied, int dist_to_uni,
                         int dist_to_exit) {
  lot L = {0};
  L.x = x;
  L.y = y;
  L.type = type;
  L.lot_size = lot_size;
  L.occupied = occupied;
  L.dist_to_university = dist_to_uni;
  L.dist_to_exit = dist_to_exit;
  L.isBlinking = FALSE;

  if (type == parking_bay) {
    L.allowed = (carSize){TRUE, TRUE, TRUE};
  } else if (type == handicaped) {
    L.allowed = (carSize){FALSE, TRUE, TRUE};
  } else if (type == EV) {
    L.allowed = (carSize){TRUE, TRUE, TRUE};
  } else {
    L.allowed = (carSize){FALSE, FALSE, FALSE};
  }

  strcpy(L.username, "");
  strcpy(L.licensePlate, "");
  return L;
}

// ============================================
// Helper function to verify spot was written to occipied. txt
// ============================================
static typeBoolean isSpotInOccupiedFile(const char *username,
                                        const char *licensePlate) {
  const char *paths[] = {"../assets/occipied.txt", "assets/occipied.txt",
                         "occipied.txt", "./assets/occipied.txt", NULL};

  for (int i = 0; paths[i] != NULL; i++) {
    FILE *file = fopen(paths[i], "r");
    if (file == NULL) {
      continue;
    }

    char file_username[256];
    char file_licensePlate[256];
    int file_x, file_y;
    char line[512];
    typeBoolean found = FALSE;

    while (fgets(line, sizeof(line), file)) {
      if (sscanf(line, "%255[^,],%255[^,],%d,%d", file_username,
                 file_licensePlate, &file_x, &file_y) == 4) {
        if (strcmp(file_username, username) == 0 &&
            strcmp(file_licensePlate, licensePlate) == 0) {
          found = TRUE;
          break;
        }
      }
    }

    fclose(file);
    if (found) {
      return TRUE;
    }
  }

  return FALSE;
}

// ============================================
// SCORING SYSTEM TESTS
// ============================================

TEST_CASE(test_ev_spot_perfect_match, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, TRUE, FALSE, FALSE);
  lot L = createTestLot(0, 0, EV, (carSize){FALSE, TRUE, FALSE}, FALSE, 0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(1000, score);
})

TEST_CASE(test_ev_car_regular_spot, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, TRUE, FALSE, FALSE);
  lot L = createTestLot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, FALSE,
                        0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(300, score);
})

TEST_CASE(test_handicapped_car_in_handicapped_spot, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, FALSE, TRUE, FALSE);
  lot L = createTestLot(0, 0, handicaped, (carSize){FALSE, FALSE, TRUE}, FALSE,
                        0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(850, score);
})

TEST_CASE(test_small_car_small_spot, {
  initializeTestGrid();
  Car car = createTestCar(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
  lot L = createTestLot(0, 0, parking_bay, (carSize){TRUE, FALSE, FALSE}, FALSE,
                        0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(300, score);
})

TEST_CASE(test_occupied_spot_impossible, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  lot L = createTestLot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, TRUE,
                        0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(-1000000, score);
})

TEST_CASE(test_road_impossible, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  lot L = createTestLot(0, 0, road, (carSize){FALSE, TRUE, FALSE}, FALSE, 0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(-1000000, score);
})

TEST_CASE(test_non_handicapped_in_handicapped_spot, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  lot L = createTestLot(0, 0, handicaped, (carSize){FALSE, FALSE, TRUE}, FALSE,
                        0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(-1000000, score);
})

TEST_CASE(test_grid_initialization, {
  initializeTestGrid();

  CHECK_EQ_INT(GRID_WIDTH, 10);
  CHECK_EQ_INT(GRID_HEIGHT, 10);

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      CHECK_EQ_INT(FALSE, parkingGrid[y][x].occupied);
      CHECK_EQ_INT(x, parkingGrid[y][x].x);
      CHECK_EQ_INT(y, parkingGrid[y][x].y);
    }
  }
})

// ============================================
// FILE WRITING TESTS
// ============================================

TEST_CASE(test_single_car_written_to_file, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  strcpy(car.Pref.username, "testuser");
  strcpy(car.Pref.licensePlate, "TEST123");

  // Set currentUser so assignCar can access the preferences
  strcpy(currentUser.username, "testuser");
  strcpy(currentUser.licensePlate, "TEST123");

  // Call assignCar - it will find the best spot and write to file
  assignCar(&car);

  // Check if the entry was written (don't check x,y since assignCar picks it)
  CHECK_TRUE(isSpotInOccupiedFile("testuser", "TEST123"));
})

TEST_CASE(test_multiple_cars_written_to_file, {
  initializeTestGrid();

  Car car1 = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  strcpy(car1.Pref.username, "user1");
  strcpy(car1.Pref.licensePlate, "ABC123");
  strcpy(currentUser.username, "user1");
  strcpy(currentUser.licensePlate, "ABC123");

  assignCar(&car1);

  Car car2 = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  strcpy(car2.Pref.username, "user2");
  strcpy(car2.Pref.licensePlate, "XYZ789");
  strcpy(currentUser.username, "user2");
  strcpy(currentUser.licensePlate, "XYZ789");

  assignCar(&car2);

  CHECK_TRUE(isSpotInOccupiedFile("user1", "ABC123"));
  CHECK_TRUE(isSpotInOccupiedFile("user2", "XYZ789"));
})

TEST_CASE(test_chooseBestLot_returns_valid_spot, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);

  lot *best = chooseBestLot(&car);

  CHECK_TRUE(best != NULL);
  CHECK_TRUE(best->type == parking_bay || best->type == EV);
  CHECK_EQ_INT(FALSE, best->occupied);
})

TEST_CASE(test_assignCar_marks_spot_occupied, {
  initializeTestGrid();
  Car car = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  strcpy(car.Pref.username, "testuser");
  strcpy(car.Pref.licensePlate, "TEST123");
  strcpy(currentUser.username, "testuser");
  strcpy(currentUser.licensePlate, "TEST123");

  // Find spot before assignment
  lot *best = chooseBestLot(&car);
  int best_x = best->x;
  int best_y = best->y;

  // Assign the car
  assignCar(&car);

  // Check if that spot is now marked as occupied
  CHECK_EQ_INT(TRUE, parkingGrid[best_y][best_x].occupied);
})

TEST_CASE(test_assignCar_avoids_occupied_spots, {
  initializeTestGrid();

  Car car1 = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  strcpy(car1.Pref.username, "user1");
  strcpy(car1.Pref.licensePlate, "ABC123");
  strcpy(currentUser.username, "user1");
  strcpy(currentUser.licensePlate, "ABC123");

  assignCar(&car1);

  Car car2 = createTestCar(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
  strcpy(car2.Pref.username, "user2");
  strcpy(car2.Pref.licensePlate, "XYZ789");
  strcpy(currentUser.username, "user2");
  strcpy(currentUser.licensePlate, "XYZ789");

  lot *best1 = chooseBestLot(&car1);
  lot *best2 = chooseBestLot(&car2);

  // The two cars should get different spots
  if (best1 != NULL && best2 != NULL) {
    if (best1->x == best2->x && best1->y == best2->y) {
      CHECK_FAIL("Both cars assigned to same spot");
    }
  }
})

TEST_CASE(test_ev_car_prefers_ev_spot, {
  initializeTestGrid();
  Car ev_car = createTestCar(FALSE, TRUE, FALSE, TRUE, FALSE, FALSE);
  lot ev_lot =
      createTestLot(7, 7, EV, (carSize){FALSE, TRUE, FALSE}, FALSE, 0, 0);
  lot regular_lot = createTestLot(4, 4, parking_bay,
                                  (carSize){FALSE, TRUE, FALSE}, FALSE, 0, 0);

  int ev_score = calculateLotScore(&ev_car, &ev_lot);
  int regular_score = calculateLotScore(&ev_car, &regular_lot);

  CHECK_TRUE(ev_score > regular_score);
})

MAIN_RUN_TESTS(test_ev_spot_perfect_match, test_ev_car_regular_spot,
               test_handicapped_car_in_handicapped_spot,
               test_small_car_small_spot, test_occupied_spot_impossible,
               test_road_impossible, test_non_handicapped_in_handicapped_spot,
               test_grid_initialization, test_single_car_written_to_file,
               test_multiple_cars_written_to_file,
               test_chooseBestLot_returns_valid_spot,
               test_assignCar_marks_spot_occupied,
               test_assignCar_avoids_occupied_spots,
               test_ev_car_prefers_ev_spot)
