# General
cmake_minimum_required(VERSION 3.20)
project(NavalBattle)

set(CMAKE_BUILD_TYPE Debug)

# Libraries
add_library(Ship STATIC lib/Ship.cpp)

add_library(ShipManager STATIC lib/ShipManager.cpp)
target_link_libraries(ShipManager Ship)

add_library(Cell STATIC lib/Cell.cpp)
target_link_libraries(Cell Ship)

add_library(GameField STATIC lib/GameField.cpp)
target_link_libraries(GameField Cell Ship)

# Main add_executable
add_executable(game lib/main.cpp)
target_link_libraries(game Ship ShipManager GameField)

# Testing
# enable_testing()

# add_executable(Ship_test tests/Ship_test.cpp)
# target_link_libraries(Ship_test Ship gtest_main gtest)

# add_executable(ShipManager_test tests/ShipManager_test.cpp)
# target_link_libraries(ShipManager_test ShipManager gtest_main gtest)

# add_executable(GameField_test tests/GameField_test.cpp)
# target_link_libraries(GameField_test GameField gtest_main gtest)

# include(GoogleTest)
# gtest_discover_tests(Ship_test)
# gtest_discover_tests(ShipManager_test)
# gtest_discover_tests(GameField_test)
