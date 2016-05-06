CPPFLAGS = -std=c++11 -O3
BOOST = -I ~/boost/boost_1_60_0 ~/boost/boost_1_60_0/stage/lib/libboost_thread.so ~/boost/boost_1_60_0/stage/lib/libboost_system.so ~/boost/boost_1_60_0/stage/lib/libboost_system.so

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

unit_test: lsm_unit_test1.cpp lsm.h lsm.cpp basicarray.cpp basicarray.h storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp
	g++ $(CPPFLAGS) lsm_unit_test1.cpp lsm.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp -o lsm_unit_test.exe

unit_test_run:
	./lsm_unit_test.exe




basic: basicarray.cpp storage.h basic_tester.cpp
	g++ $(CPPFLAGS) basic_tester.cpp basicarray.cpp storage.h -o basic.exe


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


rwlock:
	g++  readwriteTest.cpp -o rwtest.exe -pthread -std=c++11

boosteg: boost_example.cpp
	g++ -I ~/boost/boost_1_60_0 boost_example.cpp -o boosteg
	echo 1 2 3 | ./boosteg

boost_rwlock: boost_rwlock.cpp
	g++ $(CPPFLAGS) boost_rwlock.cpp -o boost_rwlock $(BOOST) 
	./boost_rwlock

boost_rwlock2: boost_rwlock2.cpp
	g++ $(CPPFLAGS) boost_rwlock2.cpp -o boost_rwlock2 $(BOOST) 

boost_rw_run:
	./boost_rwlock2

lsmmt: lsm_mt_driver.cpp lsm_mt.h lsm_mt.cpp basicarray.cpp basicarray.h storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp
	g++ $(CPPFLAGS) lsm_mt_driver.cpp lsm_mt.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp -o lsm_mt.exe $(BOOST) 

lsmmttest:
	./lsm_mt.exe

lsmmt_file: lsm_mt_driver_file.cpp lsm_mt.h lsm_mt.cpp basicarray.cpp basicarray.h storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp
	g++ $(CPPFLAGS) lsm_mt_driver_file.cpp lsm_mt.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp -o lsm_mt_file_input.exe $(BOOST) 

lsmmt_file_run:
	./lsm_mt_file_input.exe workload


lsmmt_workload: lsm_mt_driver_workload.cpp lsm_mt.h lsm_mt.cpp basicarray.cpp basicarray.h storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp
	g++ $(CPPFLAGS) lsm_mt_driver_workload.cpp lsm_mt.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp -o lsm_mt_workload_input.exe $(BOOST) 

lsmmt_workload_run:
	./lsm_mt_workload_input.exe 


lsmmt_queue: lsm_mt_admission_queue.cpp lsm.h  lsm_mt.h lsm_mt.cpp basicarray.cpp basicarray.h storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp
	g++ $(CPPFLAGS) lsm_mt_admission_queue.cpp lsm.cpp lsm_mt.cpp basicarray.cpp storage.h btree.cpp sortedarray.cpp heap_merge_k_list.cpp indexedarray.cpp -o lsm_mt_queue.exe $(BOOST) 

lsmmt_queue_run:
	./lsm_mt_queue.exe


lsm_test_loading:
	./lsm_mt_queue.exe 4 4 0 1000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 100000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 1000000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 100000000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 1000000000 0 0 0 0 0 1 10

# 10 million loading, 10 million random reads/writes. varying read-write percentage
lsm_test_vary_read_percentage:
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 100 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 90 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 80 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 70 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 60 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 40 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 30 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 20 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 10 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 0 0 0 0 1 10

# 10 million loading, 10 million random reads/writes at 50%, varying ratio between levels
lsm_test_vary_ratio:
	./lsm_mt_queue.exe 4 2 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 4 8 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 4 16 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 4 32 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 4 64 0 10000000 10000000 50 0 0 0 1 10

lsm_test_vary_initial_pages:
	./lsm_mt_queue.exe 2 4 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 8 4 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 16 4 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 32 4 0 10000000 10000000 50 0 0 0 1 10
	./lsm_mt_queue.exe 2 4 0 10000000 10000000 10 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 10 0 0 0 1 10
	./lsm_mt_queue.exe 8 4 0 10000000 10000000 10 0 0 0 1 10
	./lsm_mt_queue.exe 16 4 0 10000000 10000000 10 0 0 0 1 10
	./lsm_mt_queue.exe 32 4 0 10000000 10000000 10 0 0 0 1 10
	./lsm_mt_queue.exe 2 4 0 10000000 10000000 90 0 0 0 1 10
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 90 0 0 0 1 10
	./lsm_mt_queue.exe 8 4 0 10000000 10000000 90 0 0 0 1 10
	./lsm_mt_queue.exe 16 4 0 10000000 10000000 90 0 0 0 1 10
	./lsm_mt_queue.exe 32 4 0 10000000 10000000 90 0 0 0 1 10

# 10 million loading, 10 million random reads/writes at 50%, varying ratio between levels
lsm_test_vary_ratio_normal:
	./lsm_mt_queue.exe 4 2 0 0 10000000 50 1 0 1000000 1 10
	./lsm_mt_queue.exe 4 4 0 0 10000000 50 1 0 1000000 1 10
	./lsm_mt_queue.exe 4 8 0 0 10000000 50 1 0 1000000 1 10
	./lsm_mt_queue.exe 4 16 0 0 10000000 50 1 0 1000000 1 10
	./lsm_mt_queue.exe 4 32 0 0 10000000 50 1 0 1000000 1 10
	./lsm_mt_queue.exe 4 64 0 0 10000000 50 1 0 1000000 1 10
	./lsm_mt_queue.exe 4 2 0 0 10000000 50 1 0 1000 1 10
	./lsm_mt_queue.exe 4 4 0 0 10000000 50 1 0 1000 1 10
	./lsm_mt_queue.exe 4 8 0 0 10000000 50 1 0 1000 1 10
	./lsm_mt_queue.exe 4 16 0 0 10000000 50 1 0 1000 1 10
	./lsm_mt_queue.exe 4 32 0 0 10000000 50 1 0 1000 1 10
	./lsm_mt_queue.exe 4 64 0 0 10000000 50 1 0 1000 1 10

lsm_test_multi_threaded:
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 100 0 0 0 1 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 100 0 0 0 2 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 100 0 0 0 3 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 100 0 0 0 4 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 90 0 0 0 2 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 90 0 0 0 3 1	
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 90 0 0 0 4 1		
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 50 0 0 0 2 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 50 0 0 0 3 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 50 0 0 0 4 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 10 0 0 0 2 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 10 0 0 0 3 1
	./lsm_mt_queue.exe 4 4 0 10000000 10000000 10 0 0 0 4 1


lsm_test_loading_1mb:
	./lsm_mt_queue.exe 250 4 0 1000 0 0 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000 0 0 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 100000 0 0 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 1000000 0 0 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 0 0 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 100000000 0 0 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 1000000000 0 0 0 0 0 1 1

lsm_test_loading_10mb:
	./lsm_mt_queue.exe 2500 4 0 1000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 2500 4 0 10000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 2500 4 0 100000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 2500 4 0 1000000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 2500 4 0 10000000 0 0 0 0 0 1 10
	./lsm_mt_queue.exe 2500 4 0 100000000 0 0 0 0 0 1 10

lsm_test_loading_huge:
	./lsm_mt_queue.exe 2500 4 0 1000000000 0 0 0 0 0 1 1


lsm_test_vary_read_percentage_large:
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 100 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 80 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 70 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 60 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 40 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 30 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 20 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 0 0 0 0 1 1


lsm_test_vary_read_percentage_1mb:
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 100 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 80 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 70 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 60 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 40 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 30 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 20 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 0 0 0 0 1 1

lsm_test_vary_read_percentage_100kb:
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 100 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 80 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 70 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 60 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 40 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 30 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 20 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 0 0 0 0 1 1


lsm_test_multi_threaded_100kb:
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 100 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 100 0 0 0 2 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 100 0 0 0 3 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 100 0 0 0 4 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 90 0 0 0 2 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 90 0 0 0 3 1	
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 90 0 0 0 4 1		
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 50 0 0 0 2 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 50 0 0 0 3 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 50 0 0 0 4 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 10 0 0 0 2 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 10 0 0 0 3 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 10 0 0 0 4 1

lsm_test_vary_read_percentage_100kb_normal:
	./lsm_mt_queue.exe 25 4 0 0 20000000 100 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 80 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 70 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 60 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 40 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 30 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 20 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 0 1 0 10000000 1 1


lsm_test_multi_threaded_100kb_normal:
	./lsm_mt_queue.exe 25 4 0 0 20000000 100 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 100 1 0 10000000 2 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 100 1 0 10000000 0 3 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 100 1 0 10000000 4 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 90 0 0 0 2 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 90 0 0 0 3 1	
	./lsm_mt_queue.exe 25 4 0 0 20000000 90 0 0 0 4 1		
	./lsm_mt_queue.exe 25 4 0 0 20000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 50 0 0 0 2 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 50 0 0 0 3 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 50 0 0 0 4 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 10 0 0 0 2 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 10 0 0 0 3 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 10 0 0 0 4 1


lsm_test_vary_ratio_normal_100kb:
	./lsm_mt_queue.exe 25 2 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 8 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 16 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 32 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 64 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 2 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 8 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 16 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 32 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 64 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 2 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 4 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 8 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 16 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 32 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 25 64 0 0 20000000 90 1 0 10000000 1 1

lsm_test_vary_ratio_normal_1mb:
	./lsm_mt_queue.exe 250 2 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 4 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 8 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 16 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 32 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 64 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 2 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 4 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 8 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 16 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 32 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 64 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 2 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 4 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 8 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 16 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 32 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 250 64 0 0 20000000 90 1 0 10000000 1 1


lsm_test_vary_ratio_normal_10mb:
	./lsm_mt_queue.exe 2500 2 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 8 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 16 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 32 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 64 0 0 20000000 50 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 2 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 8 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 16 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 32 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 64 0 0 20000000 10 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 2 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 8 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 16 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 32 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 64 0 0 20000000 90 1 0 10000000 1 1



lsm_test_vary_ratio_normal_10mb_part2:
	./lsm_mt_queue.exe 2500 2 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 8 0 0 20000000 90 1 0 10000000 1 1
	./lsm_mt_queue.exe 2500 16 0 0 20000000 90 1 0 10000000 1 1

lsm_test_loading_10mb_normal:
	./lsm_mt_queue.exe 2500 4 0 0 1000 0 1 0 100000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 10000 0 1 0 100000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 100000 0 1 0 100000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 1000000 0 1 0 100000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 10000000 0 1 0 100000000 1 1
	./lsm_mt_queue.exe 2500 4 0 0 100000000 0 1 0 100000000 1 1


lsm_test_vary_ratio_all:
	./lsm_mt_queue.exe 25 2 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 25 8 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 25 16 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 25 2 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 25 8 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 25 16 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 25 2 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 25 8 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 25 16 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 250 2 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 250 8 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 250 16 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 250 2 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 250 8 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 250 16 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 250 2 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 250 4 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 250 8 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 250 16 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 2500 2 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 2500 8 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 2500 16 0 10000000 10000000 50 0 0 0 1 1
	./lsm_mt_queue.exe 2500 2 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 2500 8 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 2500 16 0 10000000 10000000 10 0 0 0 1 1
	./lsm_mt_queue.exe 2500 2 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 2500 4 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 2500 8 0 10000000 10000000 90 0 0 0 1 1
	./lsm_mt_queue.exe 2500 16 0 10000000 10000000 90 0 0 0 1 1

lsm_test_loading_reading_100kb:
	./lsm_mt_queue.exe 25 4 0 1000 1000 100 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000 10000 100 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 100000 100000 100 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 1000000 1000000 100 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 10000000 10000000 100 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 100000000 100000000 100 0 0 0 1 1
	./lsm_mt_queue.exe 25 4 0 1000000000 1000000000 100 0 0 0 1 1