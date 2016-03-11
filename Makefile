CPPFLAGS = -std=c++11 -g

lsm: lsm_driver.cpp lsm.h lsm.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp
	g++ $(CPPFLAGS) lsm_driver.cpp lsm.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp -o lsm.exe

lsmtest:
	./lsm.exe

basic: basicarray.cpp storage.h btree.cpp basic_tester.cpp
	g++ $(CPPFLAGS) basic_tester.cpp basicarray.cpp storage.h btree.cpp -o basic.exe


basictest:
	./basic.exe 3

sorted: basicarray.cpp storage.h btree.cpp sorted_tester.cpp sortedarray.cpp
	g++ $(CPPFLAGS) sorted_tester.cpp basicarray.cpp sortedarray.cpp storage.h btree.cpp -o sorted.exe


sortedtest:
	./sorted.exe 15

btree: btree_test.cpp basicarray.cpp storage.h btree.cpp
	g++ $(CPPFLAGS) btree_test.cpp basicarray.cpp storage.h btree.cpp -o btree.exe


btreetest:
	./btree.exe 100