comp:
	mpicc -o mpi_ring mpi_ring.c

run:
	mpirun -np 7 ./mpi_ring hello 70

clean:
	rm mpi_ring