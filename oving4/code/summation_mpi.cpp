#include <mpi.h>
#include <iostream>
#include <vector>
#include <math.h>

int main(int argc, char **argv)
{   
    double tot;
    int n, myid, nproc, partSize;
    
    MPI::Init(argc, argv);
    
    nproc = MPI::COMM_WORLD.Get_size ( );   // number of processes
    myid = MPI::COMM_WORLD.Get_rank ( );    // this process' ID
    
    if (myid == 0){
        n = pow(2, 20);
        partSize = (int)(n / nproc);
    }
    
    MPI::COMM_WORLD.Bcast( &partSize, 1, MPI_INT, 0 );
    
    std::vector<double> myline( partSize );
    
    if (myid == 0){
        for( int i=1; i<nproc; i++ ){
            for( double y=i * partSize + 1; y<=(i+1) * partSize; y++){
                myline[((int)y) % partSize - 1] = 1/(y*y);
            }
            
            MPI::COMM_WORLD.Send( &myline[0], myline.size(), MPI_DOUBLE, i, 0 );
        }
        
        for( double y=1; y<partSize; y++){
            myline[y-1] = 1/(y*y);
        }
    }
    else {
        MPI::COMM_WORLD.Recv( &myline[0], myline.size(), MPI_DOUBLE, 0, 0 );
    }
    
    
    // Do the calculation
    double sum = 0.0;
    for( int j=0; j<myline.size(); j++ )
        sum = sum + myline[j];
    
    
    // Collect the partial sums by reduction
    MPI::COMM_WORLD.Reduce (&sum, &tot, 1, MPI::DOUBLE, MPI::SUM, 0);
    
    // Print the result from the main process
    if (myid == 0) {
        double correctSum = M_PI * M_PI / (double) 6;
        std::cout << "[out] n: " << n << '\t' \
                  << "sum: " << tot << '\t' \
                  << "error: " << correctSum - tot << '\n';
    }
        
    
    MPI::Finalize(); // MPI finalize
    
    return 0;
}
