SO_DIRS := -Wl,-rpath,./includes/RPAudio -L./includes/RPAudio
OBJECTS := ./includes/RPAudio/libRPAudio.a
INCLUDES := -I./includes
CXX_FLAGS :=
LINK := -lRPAudio

default:
	g++ $(CXX_FLAGS) $(INCLUDES) $(SO_DIRS) main.cpp -o rpengine $(OBJECTS) $(LINK)
