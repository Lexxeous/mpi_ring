comp:
	mpicc -o mpi_ring mpi_ring.c

run:
	mpirun -np 5 ./mpi_ring ring 5000

clean:
	rm mpi_ring