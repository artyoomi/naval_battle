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
target_link_libraries(GameField Cell Ship ShipManager)

file(GLOB SKILL_LIB_SRC "lib/skills/*.cpp")
add_library(SkillManager STATIC lib/SkillManager.cpp ${SKILL_LIB_SRC})

add_library(File STATIC lib/save_load/File.cpp)

add_library(GameState STATIC lib/save_load/GameState.cpp)
target_link_libraries(GameField SkillManager File)

add_library(Game STATIC lib/Game.cpp)
target_link_libraries(GameField ShipManager SkillManager)

# Main add_executable
add_executable(game lib/main.cpp)
target_link_libraries(game Game GameField ShipManager SkillManager GameState)
