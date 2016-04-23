import numpy as np

target = open("workload", 'w')

c0_pages = 10
ratio = 3

one_page = 512
multiplier = 4
READ_PROB = 1

initial_load  = one_page*multiplier
read_update = initial_load * 500

total_workload = initial_load + read_update

print initial_load
target.write(str(total_workload) +"\n")

#load data
for x in range(initial_load):
	target.write(str(0) + " " + str(x) + " "+str(x)+"\n")


for x in range(read_update):

	prob = np.random.uniform()

	is_read = 0

	if prob < READ_PROB:
		is_read = 1

	key = np.random.randint(0, initial_load)
	value = np.random.randint(0, initial_load)

	#print is_read, key, value
	target.write(str(is_read) + " " + str(x%initial_load) + " "+str(x%initial_load)+"\n")

target.close()