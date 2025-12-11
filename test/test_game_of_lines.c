#include "game_of_lines.h"
#include "mtest.h"

#define DELTA 0.000001

extern cell board[100][100]; // Import global board from game_of_lines.c

/* Helper: reset board to empty */
void reset_board(int size) {
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      board[i][j] = empty;
}

// TEST: update_arena initializes board on "empty"
TEST_CASE(update_arena_initializes_board,
          /* Arrange */
          int size = 3;
          reset_board(size);

          /* Act: calling update_arena(empty, ...) should clear board fully */
          update_arena(empty, size, 0, 0);

          /* Assert */
          for (int i = 0; i < size; i++) for (int j = 0; j < size; j++)
              CHECK_EQ_INT(board[i][j], empty);)

// TEST: update_arena puts value at correct position
TEST_CASE(update_arena_places_value,
          /* Arrange */
          int size = 3;
          reset_board(size);

          /* Act */
          update_arena(p1, size, 1, 2);

          /* Assert */
          CHECK_EQ_INT(board[2][1], p1);)

// TEST: winner detects horizontal win
TEST_CASE(winner_horizontal,
          /* Arrange */
          int size = 3;
          int win = 3; reset_board(size);

          board[0][0] = p1; board[0][1] = p1; board[0][2] = p1;

          /* Act & Assert */
          CHECK_EQ_INT(winner(size, win), p1);)

// TEST: winner detects vertical win
TEST_CASE(winner_vertical,
          /* Arrange */
          int size = 3;
          int win = 3; reset_board(size);

          board[0][1] = p2; board[1][1] = p2; board[2][1] = p2;

          /* Act & Assert */
          CHECK_EQ_INT(winner(size, win), p2);)

// TEST: winner detects diagonal down-right win
TEST_CASE(winner_diag_down_right,
          /* Arrange */
          int size = 3;
          int win = 3; reset_board(size);

          board[0][0] = p1; board[1][1] = p1; board[2][2] = p1;

          /* Act & Assert */
          CHECK_EQ_INT(winner(size, win), p1);)

// TEST: winner detects diagonal down-left win
TEST_CASE(winner_diag_down_left,
          /* Arrange */
          int size = 3;
          int win = 3; reset_board(size);

          board[0][2] = p1; board[1][1] = p1; board[2][0] = p1;

          /* Act & Assert */
          CHECK_EQ_INT(winner(size, win), p1);)

//  TEST: no winner when board has random pieces
TEST_CASE(winner_none,
          /* Arrange */
          int size = 3;
          int win = 3; reset_board(size);

          board[0][0] = p1; board[0][1] = p2; board[1][0] = p2;

          /* Act & Assert */
          CHECK_EQ_INT(winner(size, win), -1);)

// Register all test cases

MAIN_RUN_TESTS(update_arena_initializes_board, update_arena_places_value,
               winner_horizontal, winner_vertical, winner_diag_down_right,
               winner_diag_down_left, winner_none)
