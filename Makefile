CPPFLAGS = -std=c++0x

lsm: lsm.cpp basicarray.cpp storage.cpp btree.cpp
	g++ $(CPPFLAGS) lsm.cpp basicarray.cpp storage.cpp btree.cpp -o lsm.exe

test:
	./lsm.exe 100