cmake_minimum_required(VERSION 3.16)
project(Tests)
set(CMAKE_CXX_STANDARD 17)
set(Tests_SOURCES
	"src/perceptrontests.cpp"
	"src/mytests.cpp"
)
add_executable(Tests ${Tests_SOURCES})
add_dependencies(Tests L4AI)
target_include_directories(Tests PRIVATE "inc")
target_link_libraries(Tests L4AI)
add_custom_command(TARGET Tests POST_BUILD Tests)

