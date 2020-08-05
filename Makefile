SO_DIRS := -Wl,-rpath,./includes/RPAudio -L./includes/RPAudio
DEBUG_LEVEL := -g3
#OBJECTS := ./includes/RPAudio/libRPAudio.a
OBJECTS := main.o rpengine.o rppng.o
INCLUDES := -I./includes
CXX_FLAGS := -std=c++17 -Wall -pthread
LINK := -lrpaudio
SRC := ./src

release: main.o
	g++ $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) -o rpengine $(OBJECTS) $(LINK)
	make clean

main.o: rpengine.o
	g++ -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) main.cpp -o main.o
rpengine.o: rppng.o
	g++ -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPEngine.cpp -o rpengine.o
rppng.o:
	g++ -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPPng.cpp -o rppng.o
clean:
	-rm *.o
