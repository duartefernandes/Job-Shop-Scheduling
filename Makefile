ifeq ($(shell which gcc-13 2>/dev/null),)
    CC := gcc
else
    CC := gcc-13
endif

parallel: job_shop_pthreads.c io.c utils.c
	$(CC) -lpthread job_shop_pthreads.c io.c utils.c -o job_shop_parallel

sequential: job_shop_sequential.c io.c utils.c
	$(CC) job_shop_sequential.c io.c utils.c -o job_shop_sequential

one_by_one: job_shop_one_by_one.c io.c utils.c
	$(CC) job_shop_one_by_one.c io.c utils.c -o job_shop_one_by_one

all: parallel sequential one_by_one

clean:
	rm -rf *.o *~ job_shop_parallel
	rm -rf *.o *~ job_shop_sequential
	rm -rf *.o *~ job_shop_one_by_one