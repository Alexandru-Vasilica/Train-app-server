cmake_minimum_required(VERSION 3.26)
project(tema2)

set(CMAKE_CXX_STANDARD 17)

add_executable(tema2 server-dev/main.cpp
        server-dev/Train.cpp
        server-dev/Train.h
        server-dev/TrainRoute.cpp
        server-dev/TrainRoute.h
        server-dev/TrainData.cpp
        server-dev/TrainData.h
        server-dev/Commands.cpp
        server-dev/Commands.h
        server-dev/CommandQueue.cpp
        server-dev/CommandQueue.h
        server-dev/ThreadPool.cpp
        server-dev/ThreadPool.h
        server-dev/requests.h)

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(tema2 PRIVATE pugixml::static pugixml::pugixml)
