#define RAYGUI_IMPLEMENTATION
#include "car_assigner.h"
#include "mtest.h"
#include "parking_lots_matrixs_utils.h"
#include "raygui.h"

// ============================================
// Helper function to initialize a test car
// ============================================
static Car createTestCar(typeBoolean is_ev, typeBoolean is_handicapped,
                         typeBoolean is_small, typeBoolean is_medium,
                         typeBoolean is_large, typeBoolean want_isolated) {
  Car car = {0};
  car.size.is_small = is_small;
  car.size.is_medium = is_medium;
  car.size.is_large = is_large;
  car.is_ev = is_ev;
  car.is_handicapped = is_handicapped;
  car.want_Isolated = want_isolated;
  strcpy(car.Pref.username, "test_user");
  strcpy(car.Pref.licensePlate, "AB11232");
  return car;
}

// ============================================
// Helper function to initialize a test lot
// ============================================
static lot createTestLot(ParkingType type, typeBoolean is_small,
                         typeBoolean is_medium, typeBoolean is_large,
                         typeBoolean occupied, int x, int y) {
  lot L = {0};
  L.type = type;
  L.lot_size.is_small = is_small;
  L.lot_size.is_medium = is_medium;
  L.lot_size.is_large = is_large;
  L.occupied = occupied;
  L.x = x;
  L.y = y;
  // L.dist_to_university = 5;
  // L.dist_to_exit = 10;
  L.isBlinking = FALSE;
  return L;
}

// Helper function to create a test lot
lot create_test_lot(int x, int y, ParkingType type, carSize lot_size,
                    int dist_to_uni, int dist_to_exit) {
  lot L = {0};
  L.x = x;
  L.y = y;
  L.type = type;
  L.lot_size = lot_size;
  L.occupied = FALSE;
  L.dist_to_university = dist_to_uni;
  L.dist_to_exit = dist_to_exit;
  return L;
}

// Helper function to create a test car
Car create_test_car(typeBoolean is_small, typeBoolean is_ev,
                    typeBoolean is_handicapped, typeBoolean placement_pref,
                    typeBoolean want_isolated) {
  Car car = {0};
  car.size.is_small = is_small;
  car.size.is_medium = !is_small;
  car.size.is_large = FALSE;
  car.is_ev = is_ev;
  car.is_handicapped = is_handicapped;
  car.placementPref = placement_pref;
  car.want_Isolated = want_isolated;
  return car;
}

// Test:  EV car in EV spot + large size match
TEST_CASE(test_ev_spot_perfect_match, {
  Car car = create_test_car(FALSE, TRUE, FALSE, FALSE, FALSE);
  lot L = create_test_lot(0, 0, EV, (carSize){FALSE, TRUE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected:  700 (VERY_IMPORTANT for EV) + 250 (SECOND_SPOT for medium car in
  // medium EV space)
  CHECK_EQ_INT(1000, score);
})

// Test: EV car in regular spot + size match
TEST_CASE(test_ev_car_regular_spot, {
  Car car = create_test_car(FALSE, TRUE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 0 (NO_SCORE for EV in non-EV spot) + 300 (FIRST_SPOT for medium
  // car in medium spot)
  CHECK_EQ_INT(300, score);
})

// Test:  Handicapped car in handicapped spot
TEST_CASE(test_handicapped_car_in_handicapped_spot, {
  Car car = create_test_car(FALSE, FALSE, TRUE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, handicaped, (carSize){FALSE, FALSE, TRUE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 600 (IMPORTANT for handicapped) + 250 (SECOND_SPOT for medium car
  // in large spot)
  CHECK_EQ_INT(850, score);
})

// Test: Small car in small spot
TEST_CASE(test_small_car_small_spot, {
  Car car = create_test_car(TRUE, FALSE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){TRUE, FALSE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 300 (FIRST_SPOT for small car in small spot)
  CHECK_EQ_INT(300, score);
})

// Test: Small car in medium spot (downsize penalty)
TEST_CASE(test_small_car_medium_spot, {
  Car car = create_test_car(TRUE, FALSE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 250 (SECOND_SPOT for small car in medium spot)
  CHECK_EQ_INT(250, score);
})

// Test: Medium car in large spot
TEST_CASE(test_medium_car_large_spot, {
  Car car = create_test_car(FALSE, FALSE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){FALSE, FALSE, TRUE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 250 (SECOND_SPOT for medium car in large spot)
  CHECK_EQ_INT(250, score);
})

// Test: Distance to university penalty
TEST_CASE(test_distance_to_university_penalty, {
  Car car = create_test_car(FALSE, FALSE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, 5, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 300 (FIRST_SPOT) - 5 (dist_to_university penalty when
  // placementPref is false)
  CHECK_EQ_INT(295, score);
})

// Test: Distance to exit penalty
TEST_CASE(test_distance_to_exit_penalty, {
  Car car = create_test_car(FALSE, FALSE, FALSE, TRUE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, 0, 5);

  int score = calculateLotScore(&car, &L);
  // Expected: 300 (FIRST_SPOT) - 5 (dist_to_exit penalty when placementPref is
  // true)
  CHECK_EQ_INT(295, score);
})

// Test: Combined EV and size scoring
TEST_CASE(test_ev_and_size_combined, {
  Car car = create_test_car(FALSE, TRUE, FALSE, FALSE, FALSE);
  lot L = create_test_lot(0, 0, EV, (carSize){FALSE, TRUE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 700 (VERY_IMPORTANT) + 300 (FIRST_SPOT for medium in medium EV)
  CHECK_EQ_INT(1000, score);
})

// Test: EV + Handicapped car
TEST_CASE(test_ev_and_handicapped, {
  Car car = create_test_car(FALSE, TRUE, TRUE, FALSE, FALSE);
  lot L = create_test_lot(0, 0, EV, (carSize){FALSE, FALSE, TRUE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 700 (VERY_IMPORTANT for EV) + 0 (NO_SCORE for handicapped in
  // non-handicapped EV spot) + 250 (SECOND_SPOT for medium in large)
  CHECK_EQ_INT(950, score);
})

// Test:  Occupied spot returns IMPOSSIBLE
TEST_CASE(test_occupied_spot_impossible, {
  Car car = create_test_car(FALSE, FALSE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, 0, 0);
  L.occupied = TRUE;

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(-1000000, score);
})

// Test: Road type returns IMPOSSIBLE
TEST_CASE(test_road_impossible, {
  Car car = create_test_car(FALSE, FALSE, FALSE, FALSE, FALSE);
  lot L = create_test_lot(0, 0, road, (carSize){FALSE, TRUE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(-1000000, score);
})

// Test: Non-handicapped car in handicapped spot returns IMPOSSIBLE
TEST_CASE(test_non_handicapped_in_handicapped_spot, {
  Car car = create_test_car(FALSE, FALSE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, handicaped, (carSize){FALSE, FALSE, TRUE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(-1000000, score);
})

// Test: Car size mismatch (large car in small spot) returns IMPOSSIBLE
TEST_CASE(test_size_mismatch_large_in_small, {
  Car car = create_test_car(FALSE, FALSE, FALSE, FALSE, FALSE);
  car.size.is_small = FALSE;
  car.size.is_medium = FALSE;
  car.size.is_large = TRUE;
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){TRUE, FALSE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  CHECK_EQ_INT(-1000000, score);
})

// Test: EV car in non-EV parking bay
TEST_CASE(test_ev_in_parking_bay, {
  Car car = create_test_car(FALSE, TRUE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 0 (NO_SCORE for EV in non-EV spot) + 300 (FIRST_SPOT for medium
  // in medium spot)
  CHECK_EQ_INT(300, score);
})

// Test: Multiple distance penalties
TEST_CASE(test_combined_penalties, {
  Car car = create_test_car(FALSE, FALSE, FALSE, FALSE, FALSE);
  lot L =
      create_test_lot(0, 0, parking_bay, (carSize){FALSE, TRUE, FALSE}, 5, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 300 (FIRST_SPOT) - 5 (dist_to_university)
  CHECK_EQ_INT(295, score);
})

// Test: Perfect match - EV car in EV spot with no distance penalties
TEST_CASE(test_perfect_ev_spot, {
  Car car = create_test_car(FALSE, TRUE, FALSE, FALSE, FALSE);
  lot L = create_test_lot(0, 0, EV, (carSize){FALSE, TRUE, FALSE}, 0, 0);

  int score = calculateLotScore(&car, &L);
  // Expected: 700 (VERY_IMPORTANT) + 300 (FIRST_SPOT for medium in medium)
  CHECK_EQ_INT(1000, score);
})

MAIN_RUN_TESTS(test_ev_spot_perfect_match, test_ev_car_regular_spot,
               test_handicapped_car_in_handicapped_spot,
               test_small_car_small_spot, test_small_car_medium_spot,
               test_medium_car_large_spot, test_distance_to_university_penalty,
               test_distance_to_exit_penalty, test_ev_and_size_combined,
               test_ev_and_handicapped, test_occupied_spot_impossible,
               test_road_impossible, test_non_handicapped_in_handicapped_spot,
               test_size_mismatch_large_in_small, test_ev_in_parking_bay,
               test_combined_penalties, test_perfect_ev_spot)
