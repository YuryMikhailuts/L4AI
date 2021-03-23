cmake_minimum_required(VERSION 3.16)
project(Tests)  
set(  
  TESTS_DATA_DIRECTORY "../TestsData"
  CACHE STRING
  "Путь к каталогу с данными (ресурсами) для запуска тестов.")
message(STATUS "---------------------------------")  
message(STATUS "Сборка проекта тестов")  
message(STATUS "---------------------------------")  
message(STATUS "Путь к каталогу с данными для тестов \"${TESTS_DATA_DIRECTORY}\"") 
add_definitions(-DTESTS_DATA_DIRECTORY="${TESTS_DATA_DIRECTORY}")
set(CMAKE_CXX_STANDARD 17)
set(Tests_SOURCES
        "src/perceptrontests.cpp"
        "src/pipetests.cpp"
        "src/mytests.cpp"
        "src/testsutils.cpp"
        src/smarttests.cpp)
add_executable(Tests ${Tests_SOURCES})
add_dependencies(Tests L4AI)
target_include_directories(Tests PRIVATE "inc")
target_link_libraries(Tests L4AI)
add_custom_target(all_tests
COMMAND Tests)
add_custom_target(perceptron_tests
COMMAND Tests "'Перцептрон'" "'.*'")
add_custom_target(pipe_tests
COMMAND Tests "'Конвейер'" "'.*'")
add_custom_target(pipeline_tests
COMMAND Tests "'Конвейер/Линейный конвейер'" "'.*'")