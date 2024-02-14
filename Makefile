
all:
	g++ -o clientApp client.cpp
	g++ -o serverApp server/main.cpp server/TrainData.h server/TrainData.cpp server/pugixml.hpp server/pugixml.cpp server/pugiconfig.hpp server/Train.h server/Train.cpp server/Commands.h server/Commands.cpp server/CommandQueue.h server/CommandQueue.cpp server/ThreadPool.h server/ThreadPool.cpp server/TrainRoute.h server/TrainRoute.cpp server/requests.h
clean:
	rm -f serverApp clientApp