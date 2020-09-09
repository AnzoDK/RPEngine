SO_DIRS := -Wl,-rpath,./includes/RPAudio -L./includes/RPAudio
DEBUG_LEVEL := -g3
CXX := g++
#OBJECTS := ./includes/RPAudio/libRPAudio.a
OBJECTS := main.o rpengine.o rppng.o ui.o io.o
LIB_OBJECTS := rpengine.o rppng.o ui.o io.o
INCLUDES := -I./includes
LIB_FLAGS := -fPIC
CXX_FLAGS := -std=c++17 -Wall -pthread
LINK := -lrpaudio -lSDL2 -lSDL2_image -lSDL2_ttf
SRC := ./src
OS := Linux
EX := .so
END_LIB_FLAGS :=
ifeq ($(OS), Windows)
	LIB_FLAGS +=  -DBUILDING_EXAMPLE_DLL
	LIB_OBJECTS += OpenAL32.dll
	OBJECTS += OpenAL32.dll
	END_LIB_FLAGS := --shared -lstdc++ -Wl,--out-implib,librpengine.a
	CXX := x86_64-w64-mingw32-g++
	EX := .dll
	OS := Windows
	
endif

release: main.o
	$(CXX) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) -o rpengine $(OBJECTS) $(LINK)
	make clean

lib: rpenginelib.o uilib.o iolib.o
	$(CXX) -fPIC -shared $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(LIB_OBJECTS) -o rpengine$(EX) $(LINK) $(END_LIB_FLAGS)
	make clean

main.o: rpengine.o ui.o
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) main.cpp -o main.o
rpengine.o: rppng.o
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPEngine.cpp -o rpengine.o
ui.o:
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPUI.cpp -o ui.o
io.o:
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPIO.cpp -o io.o
rppng.o:
	$(CXX) -c $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPPng.cpp -o rppng.o

rpenginelib.o: rppnglib.o
	$(CXX) -c $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPEngine.cpp -o rpengine.o

iolib.o:
	$(CXX) -c $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPIO.cpp -o io.o
uilib.o:
	$(CXX) -c $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPUI.cpp -o ui.o

rppnglib.o:
	./dependency-builder.sh --use-dev --$(OS)
	$(CXX) -c $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPPng.cpp -o rppng.o

clean:
	-rm *.o
