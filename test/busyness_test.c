#define RAYGUI_IMPLEMENTATION
#include "busyness_prediction.h"
#include "mtest.h"
#include "raygui.h"

BusynessSchedule schedule;
//Logic Checks on reading the input
TEST_CASE(busyness_matrix_reader_1, {
    //Arrange
    schedule = busynessSchedule();

    //Act
    int busyness = schedule.data[1][1];

    //Assert
    CHECK_EQ_INT(1, busyness);
})

TEST_CASE(busyness_matrix_reader_2, {
    schedule = busynessSchedule();

    int busyness = schedule.data[4][2];

    CHECK_EQ_INT(2, busyness);
})

TEST_CASE(busyness_matrix_reader_3, {
    schedule = busynessSchedule();

    int busyness = schedule.data[0][3];

    CHECK_EQ_INT(3, busyness);
})
//Boundary checks
TEST_CASE(busyness_boundary_test_1, {
    //Arrange
    schedule = busynessSchedule();
    Time time;
    time.Day = 8;
    time.hour = 4;
    int step = 3;

    //Act
    int retunrVal = boundariesCheck(time);

    //Assert
    printf("%d", retunrVal);
    CHECK_EQ_INT(-1, retunrVal);
})

TEST_CASE(busyness_boundary_test_2, {
    //Arrange
    schedule = busynessSchedule();
    Time time;
    time.Day = 4;
    time.hour = 9;
    int step = 3;

    //Act
    int retunrVal = boundariesCheck(time);

    //Assert
    printf("%d", retunrVal);
    CHECK_EQ_INT(-1, retunrVal);
})

TEST_CASE(busyness_boundary_test_3, {
    //Arrange
    schedule = busynessSchedule();
    Time time;
    time.Day = 8;
    time.hour = 8;
    int step = 3;

    //Act
    int retunrVal = boundariesCheck(time);

    //Assert
    printf("%d", retunrVal);
    CHECK_EQ_INT(-1, retunrVal);
})

TEST_CASE(busyness_boundary_test_4, {
    //Arrange
    schedule = busynessSchedule();
    Time time;
    time.Day = 4;
    time.hour = 4;
    int step = 3;

    //Act
    int retunrVal = boundariesCheck(time);

    //Assert
    printf("%d", retunrVal);
    CHECK_EQ_INT(0, retunrVal);
})



MAIN_RUN_TESTS(busyness_matrix_reader_1, busyness_matrix_reader_2, busyness_matrix_reader_3,
    busyness_boundary_test_1, busyness_boundary_test_2, busyness_boundary_test_3, busyness_boundary_test_4)
