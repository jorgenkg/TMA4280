#include <mpi.h>
#include <iostream>
#include <vector>
#include <math.h>

int main(int argc, char **argv)
{
	double tot;
	int myid, nproc;

	MPI::Init(argc, argv);

	nproc = MPI::COMM_WORLD.Get_size();   // number of processes
	myid = MPI::COMM_WORLD.Get_rank();    // this process' ID


	//  This could be calculated on one process and seeded to the other processes,
	//  but the broadcast of this value would be more costly than just calculating
	//  it. 
	int n = 1 << 20;
	int p = n / size;
	int r = n % size;
	int m = p + (rank < r);
	//  

	// This structure will contain portion of the sequence we are responsible
	// for summing up.
	std::vector<double> digitVector(m);


	if (myid == 0)
	{ // This is the master process

		double a = 1.0;
		for (int i = 1; i<nproc; i++){
			// Loop over the slave processes and pass them a vector with digits to sum up.
			// It would be faster to calculate the sequence directly on each individual process.
			int q = p + (i < r);
			for (int j = 0; j < q; j++, a++)
				digitVector[j] = 1.0 / (a * a);

			// Use MPI.Send to pass the vector
			MPI::COMM_WORLD.Send(&digitVector[0], digitVector.size(), MPI_DOUBLE, i, 0);
		}

		// Create our own digit sequence
		for (int j = 0; j < m; j++, a++)
			v[j] = 1.0 / (a * a);
	}
	else
	{ // This is a "slave" process.

		// Receive the initialized vector to sum up
		MPI::COMM_WORLD.Recv(&digitVector[0], digitVector.size(), MPI_DOUBLE, 0, 0);
	}


	// Do the calculation
	double sum = 0.0;
	for (int j = 0; j<digitVector.size(); j++)
		sum += digitVector[j];


	// Collect the partial sums by reduction
	MPI::COMM_WORLD.Reduce(&sum, &tot, 1, MPI::DOUBLE, MPI::SUM, 0);


	if (myid == 0)
	{ // This is the master process
		// Print the result
		double correctSum = M_PI * M_PI / (double)6;
		std::cout << "[out] n: " << n << '\t' \
			<< "sum: " << tot << '\t' \
			<< "error: " << correctSum - tot << '\n';
	}


	MPI::Finalize(); // MPI finalize

	return 0;
}
