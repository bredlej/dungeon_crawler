set(INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/extern
        )
add_executable(test_level_parser test_level_parser.cpp)
target_link_libraries(test_level_parser dungeon_crawler_application gtest)
target_include_directories(test_level_parser PRIVATE ${INCLUDE_DIRS})

add_executable(test_skill_parser test_skill_parser.cpp)
target_link_libraries(test_skill_parser dungeon_crawler_application gtest)
target_include_directories(test_skill_parser PRIVATE ${INCLUDE_DIRS})

add_executable(test_battle_director test_battle_director.cpp)
target_link_libraries(test_battle_director dungeon_crawler_application gtest)
target_include_directories(test_battle_director PRIVATE ${INCLUDE_DIRS})

add_executable(test_combat test_combat.cpp)
target_link_libraries(test_combat dungeon_crawler_application gtest)
target_include_directories(test_combat PRIVATE ${INCLUDE_DIRS})

add_executable(test_battle_system test_battle_system.cpp)
target_link_libraries(test_battle_system dungeon_crawler_application gtest)
target_include_directories(test_battle_system PRIVATE ${INCLUDE_DIRS})

add_test(Test-Parsers test_level_parser)
add_test(Test-Parsers test_skill_parser)
add_test(Test-Logic test_battle_director)
add_test(Test-Logic test_combat)
add_test(Test-Logic test_battle_system)