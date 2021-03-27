cmake_minimum_required(VERSION 3.16)
message(STATUS "---------------------------------")  
message(STATUS "Сборка проекта библиотеки искуственного интеллекта L4AI.")  
message(STATUS "---------------------------------")  
project(L4AI)
set(CMAKE_CXX_STANDARD 17)
option(L4AI_BUILD_STATIC "Build the static library" OFF)
set(L4AI_SOURCES
	"src/algorithm.cpp"
        "src/convolutionperceptron.cpp"
        "src/flatperceptron.cpp"
        "src/instance.cpp"
        "src/perceptron.cpp"
        "src/perceptroninstance.cpp"
        "src/pipe.cpp"
        "src/calculator.cpp"
        "src/trainer.cpp"
        "src/pipeline.cpp"
        "src/pipelineinstance.cpp"
        "src/smart_objects.cpp"
        "src/configurable.cpp"
        "src/smart_to_json.cpp"
        "src/smart_from_json.cpp"
        )

if (L4AI_BUILD_STATIC)
	add_library(L4AI STATIC ${L4AI_SOURCES}) # Создание библиотеки
else()
	add_library(L4AI SHARED ${L4AI_SOURCES}) # Создание библиотеки
endif()
target_include_directories(L4AI PUBLIC "inc/")
