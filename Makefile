parallel:
	gcc-13 -lpthread job_shop_pthreads.c -o job-shop-parallel

sequential:
	gcc-13 job_shop_sequential.c -o job-shop-sequential

one_by_one:
	gcc-13 job_shop_one_by_one.c -o job-shop-one-by-one

all: parallel sequential one_by_one

clean:
	rm -rf *.o *~ job-shop-parallel
	rm -rf *.o *~ job-shop-sequential
	rm -rf *.o *~ job-shop-one-by-one