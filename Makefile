CPPFLAGS = -std=c++11 -g

clean:
	rm -f C:/tmp/LSM*.bin 

lsm: lsm_driver.cpp lsm.h lsm.cpp basicarray.cpp basicarray.h storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp
	g++ $(CPPFLAGS) lsm_driver.cpp lsm.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp -o lsm.exe

lsmtest:
	./lsm.exe

lsm2: lsm_driver2.cpp lsm.h lsm.cpp basicarray.cpp basicarray.h storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp
	g++ $(CPPFLAGS) lsm_driver2.cpp lsm.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp -o lsm2.exe

lsmtest2:
	./lsm2.exe


lsm3: lsm_driver3.cpp lsm.h lsm.cpp basicarray.cpp basicarray.h storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp
	g++ $(CPPFLAGS) lsm_driver3.cpp lsm.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp -o lsm3.exe

lsmtest3:
	./lsm3.exe


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

indexedarray: indexedarray_tester.cpp storage.h indexedarray.cpp heap_merge_k_list.cpp
	g++ $(CPPFLAGS) indexedarray_tester.cpp storage.h indexedarray.cpp heap_merge_k_list.cpp -o indexedarray.exe


indexedarraytest:
	./indexedarray.exe 100