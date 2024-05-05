# Job Shop Scheduling

This project implements three versions of a Job Shop Scheduling algorithm:

- **Sequential**: A sequential version of the algorithm.
- **Parallel**: A parallel version of the algorithm using pthreads.

## Building the Project

To build the project, use the provided Makefile:

```bash
make all
```

This will compile two executables:

- `job_shop_sequential`: Sequential version
- `job_shop_parallel`: Parallel version

## Running the Executables

### Sequential Version

To run the sequential version, use the following command:

```bash
./job_shop_sequential <input_file> <output_file>
```

For example:

```bash
./job_shop_sequential test/examples/gg03.jss test/results/parallel.txt
```

### Parallel Version

To run the parallel version, use the following command:

```bash
./job_shop_parallel <input_file> <output_file>
```

For example:

```bash
./job_shop_parallel test/examples/gg03.jss test/results/parallel.txt 3
```
