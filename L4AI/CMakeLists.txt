cmake_minimum_required(VERSION 3.16)
project(L4AI)
option(L4AI_BUILD_STATIC "Build the static library" OFF)
set(L4AI_SOURCES
	"src/algorithm.cpp"
	"src/convolutionperceptron.cpp"
	"src/flatperceptron.cpp"
	"src/instance.cpp"
	"src/perceptron.cpp"
	"src/perceptroninstance.cpp"
	"src/pipe.cpp"
)

if (L4AI_BUILD_STATIC)
	add_library(L4AI STATIC ${L4AI_SOURCES}) # Создание библиотеки
else()
	add_library(L4AI SHARED ${L4AI_SOURCES}) # Создание библиотеки
endif()
target_include_directories(L4AI PUBLIC "inc/")
