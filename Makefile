#SO_DIRS := -Wl,-rpath,/includes/RPAudio -L./includes/RPAudio
SO_DIRS := 
DEBUG_LEVEL := -g3
CXX := g++
#OBJECTS := ./includes/RPAudio/libRPAudio.a
OBJECTS := main.o rpengine.o rppng.o ui.o io.o scene.o scenescript.o settings.o
LIB_OBJECTS := rpengine.o rppng.o ui.o io.o scene.o scenescript.o settings.o
LIB_TARGETS := rpenginelib.o rppnglib.o uilib.o iolib.o scenelib.o scenescriptlib.o settingslib.o
INCLUDES := -I./includes
LIB_FLAGS := -fPIC
CXX_FLAGS := -std=c++17 -Wall -pthread
LINK := -lrpaudio -lSDL2 -lSDL2_image -lSDL2_ttf
SRC := ./src
OS := Linux
EX := .so
PG:=
USE_LOCAL := 0
DEPBUILD := echo "Not building local target for RPAudio - No need"
END_LIB_FLAGS :=
EXE_EX := 
ifeq ($(USE_LOCAL), 1)
	DEPBUILD := ./dependency-builder.sh --use-dev --$(OS)
	SO_DIRS := -Wl,-rpath,./includes/RPAudio -L./includes/RPAudio
endif
ifeq ($(OS), Windows)
	EXE_EX := .exe
	LIB_FLAGS +=  -DBUILDING_EXAMPLE_DLL
	END_LIB_FLAGS := --shared -lstdc++ -Wl,--out-implib,librpengine.a
	CXX := x86_64-w64-mingw32-g++
	EX := .dll
	OS := Windows
	
endif

release:$(OBJECTS) tools
	$(CXX) $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) -o rpengine $(OBJECTS) $(LINK)
	make clean

lib: $(LIB_TARGETS) tools
	$(CXX) -fPIC -shared $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(LIB_OBJECTS) -o rpengine$(EX) $(LINK) $(END_LIB_FLAGS)
	make clean

optimizedLib: $(LIB_OBJECTS) tools
	$(CXX) -fPIC -shared $(PG) $(CXX_FLAGS) -O2 $(INCLUDES) $(SO_DIRS) $(LIB_OBJECTS) -o rpengine$(EX) $(LINK) $(END_LIB_FLAGS)
	make clean

main.o: rpengine.o ui.o io.o scene.o
	$(CXX) -c $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) main.cpp -o main.o
rpengine.o: rppng.o
	$(CXX) -c $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPEngine.cpp -o rpengine.o
ui.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPUI.cpp -o ui.o
io.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPIO.cpp -o io.o
rppng.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPPng.cpp -o rppng.o
scene.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPScene.cpp -o scene.o
settings.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPSettings.cpp -o settings.o

rpenginelib.o: rppnglib.o
	$(CXX) -c $(PG) $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPEngine.cpp -o rpengine.o

iolib.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPIO.cpp -o io.o
scenelib.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPScene.cpp -o scene.o
uilib.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPUI.cpp -o ui.o
settingslib.o:
	$(CXX) -c $(PG) $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPSettings.cpp -o settings.o

rppnglib.o:
	$(DEPBUILD)
	$(CXX) -c $(PG) $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPPng.cpp -o rppng.o

scenescriptlib.o: 
	$(CXX) -c $(PG) $(CXX_FLAGS) $(LIB_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPSceneScript.cpp -o scenescript.o

scenescript.o: 
	$(CXX) -c $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) $(SO_DIRS) $(SRC)/RPSceneScript.cpp -o scenescript.o
tools:
	#This is where all the tools are being build.
	$(CXX) $(PG) $(CXX_FLAGS) $(DEBUG_LEVEL) $(INCLUDES) ./Tools/RPScriptLinker.cpp -o RPScriptLinker$(EXE_EX)


clean:
	-rm *.o
