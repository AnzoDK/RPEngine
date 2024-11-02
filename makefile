DEBUG := -g3
OPTIMIZE := -O2
CXX := g++
CXX_OPTIONS := -std=c++17
LINK := -lX11

release:
	make debug
	make clean

optimized:
	$(CXX) $(OPTIMIZE) $(CXX_OPTIONS) testprogram.cpp -o test.out $(LINK)

debug:
	$(CXX) $(DEBUG) $(CXX_OPTIONS) testprogram.cpp -o test.out $(LINK)
	make clean

clean:
	-rm *.o
