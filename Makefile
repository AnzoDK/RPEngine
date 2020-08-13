SO_DIRS := -Wl,-rpath,./includes/RPAudio -L./includes/RPAudio
#OBJECTS := ./includes/RPAudio/libRPAudio.a
OBJECTS := main.o rpengine.o rppng.o
INCLUDES := -I./includes
CXX_FLAGS := -std=c++17 -Wall -pthread
LINK := -lrpaudio -lSDL2 -lSDL2_image
SRC := ./src

release: main.o
	g++ $(CXX_FLAGS) $(INCLUDES) $(SO_DIRS) -o rpengine $(OBJECTS) $(LINK)
	make clean

main.o: rpengine.o
	g++ -c $(CXX_FLAGS) $(INCLUDES) $(SO_DIRS) main.cpp -o main.o
rpengine.o: rppng.o
	g++ -c $(CXX_FLAGS) $(INCLUDES) $(SO_DIRS) $(SRC)/RPEngine.cpp -o rpengine.o
rppng.o:
	g++ -c $(CXX_FLAGS) $(INCLUDES) $(SO_DIRS) $(SRC)/RPPng.cpp -o rppng.o
clean:
	-rm *.o
