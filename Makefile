ifeq ($(shell which gcc-13 2>/dev/null),)
    CC := gcc
else
    CC := gcc-13
endif

parallel: job_shop_pthreads.c io.c makespan.c
	$(CC) -lpthread job_shop_pthreads.c io.c makespan.c -o job-shop-parallel

sequential: job_shop_sequential.c io.c makespan.c
	$(CC) job_shop_sequential.c io.c makespan.c -o job-shop-sequential

one_by_one: job_shop_one_by_one.c io.c makespan.c
	$(CC) job_shop_one_by_one.c io.c makespan.c -o job-shop-one-by-one

build all: parallel sequential one_by_one

clean:
	rm -rf *.o *~ job-shop-parallel
	rm -rf *.o *~ job-shop-sequential
	rm -rf *.o *~ job-shop-one-by-one