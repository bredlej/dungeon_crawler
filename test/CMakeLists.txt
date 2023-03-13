set(INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/extern
        )
add_executable(test_level_parser test_level_parser.cpp)
target_link_libraries(test_level_parser dungeon_crawler_application gtest)
target_include_directories(test_level_parser PRIVATE ${INCLUDE_DIRS})
add_test(Test-Parsers test_level_parser)