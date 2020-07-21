cmake_minimum_required(VERSION 3.16)
project(L4AIShell)
set(CMAKE_CXX_STANDARD 17)
set(L4AIShell_SOURCES
	"src/main.cpp"
)
add_executable(L4AIShell ${L4AIShell_SOURCES})
add_dependencies(L4AIShell L4AI)
target_include_directories(L4AIShell PRIVATE "inc")
target_link_libraries(L4AIShell L4AI)

