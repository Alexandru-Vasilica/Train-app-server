cmake_minimum_required(VERSION 3.26)
project(tema2)

set(CMAKE_CXX_STANDARD 17)

add_executable(tema2 main.cpp
        Train.cpp
        Train.h
        TrainRoute.cpp
        TrainRoute.h
        TrainData.cpp
        TrainData.h
        Commands.cpp
        Commands.h
        CommandQueue.cpp
        CommandQueue.h
        ThreadPool.cpp
        ThreadPool.h
        requests.h)

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(tema2 PRIVATE pugixml::static pugixml::pugixml)
