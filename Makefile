CPPFLAGS = -std=c++0x

lsm: lsm.cpp sortedarray.cpp storage.cpp
	g++ $(CPPFLAGS) lsm.cpp sortedarray.cpp storage.cpp -o lsm.exe

test:
	./lsm.exe 100