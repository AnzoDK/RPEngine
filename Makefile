SO_DIRS := -Wl,-rpath,./includes/RPAudio -L./includes/RPAudio
DEBUG_LEVEL := -g3
CXX := g++
#OBJECTS := ./includes/RPAudio/libRPAudio.a
OBJECTS := main.o rpengine.o rppng.o ui.o
LIB_OBJECTS := rpengine.o rppng.o ui.o
INCLUDES := -I./includes
LIB_FLAGS := -fPIC
CXX_FLAGS := -std=c++17 -Wall -pthread
LINK := -lrpaudio -lSDL2 -lSDL2_image -lSDL2_ttf
SRC := ./src

release: main.o
	$(CXX) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) -o rpengine $(OBJECTS) $(LINK)
	make clean

lib: rpenginelib.o uilib.o
	$(CXX) -fPIC -shared $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(LIB_OBJECTS) -o rpengine.so $(LINK)
	make clean

main.o: rpengine.o ui.o
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) main.cpp -o main.o
rpengine.o: rppng.o
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPEngine.cpp -o rpengine.o
ui.o:
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPUI.cpp -o ui.o
rppng.o:
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPPng.cpp -o rppng.o

rpenginelib.o: rppnglib.o
	$(CXX) -c $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPEngine.cpp -o rpengine.o
uilib.o:
	$(CXX) -c $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPUI.cpp -o ui.o
rppnglib.o:
	$(CXX) -c $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPPng.cpp -o rppng.o

clean:
	-rm *.o
