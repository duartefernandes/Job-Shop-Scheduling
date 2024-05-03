# Compiler choice
ifeq ($(shell which gcc-13 2>/dev/null),)
    CC := gcc
else
    CC := gcc-13
endif

# Directories
SRC_DIR := src

all: parallel sequential one_by_one

# Parallel
parallel: $(SRC_DIR)/job_shop_pthreads.c $(SRC_DIR)/io/io.c $(SRC_DIR)/utils/utils.c
	$(CC) -lpthread $^ -o job_shop_parallel

# Sequential
sequential: $(SRC_DIR)/job_shop_sequential.c $(SRC_DIR)/io/io.c $(SRC_DIR)/utils/utils.c
	$(CC) $^ -o job_shop_sequential

# One by one
one_by_one: $(SRC_DIR)/job_shop_one_by_one.c $(SRC_DIR)/io/io.c $(SRC_DIR)/utils/utils.c
	$(CC) $^ -o job_shop_one_by_one

clean:
	rm -rf *.o *~ job_shop_parallel
	rm -rf *.o *~ job_shop_sequential
	rm -rf *.o *~ job_shop_one_by_one
	rm -rf *.o *~ job_shop_parallel.exe
	rm -rf *.o *~ job_shop_sequential.exe
	rm -rf *.o *~ job_shop_one_by_one.exe