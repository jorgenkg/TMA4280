#include <mpi.h>
#include <iostream>
#include <vector>
#include <math.h>

int main(int argc, char **argv)
{   
    double tot;
    int n, myid, nproc;
    std::vector<double> vec;
    
    MPI::Init(argc, argv);
    
    nproc = MPI::COMM_WORLD.Get_size ( );
    myid = MPI::COMM_WORLD.Get_rank ( );
    
    if (myid == 0){
        printf ("* Enter exponent 2^x: ");
        scanf ("%d",&n);
        n = (int) pow(2, n);
    }
    
    vec.resize( n );
    
    if (myid == 0)
        for( double i=1; i<=n; i++ )
            vec[i-1]= 1/(i*i);
    
    MPI::COMM_WORLD.Bcast ( &n, 1, MPI::INT, 0);
    MPI::COMM_WORLD.Bcast ( &vec, vec.size(), MPI::DOUBLE, 0);
    
    if( n>0 ){
        double sum = 0.0;
        for( int j=myid+1; j<=n; j+=nproc )
            sum += vec[j-1];
        
        MPI::COMM_WORLD.Reduce (&sum, &tot, 1, MPI::DOUBLE, MPI::SUM, 0);
    }
            
    if (myid == 0) {
        double correctSum = M_PI * M_PI / (double) 6;
        std::cout << "[out] n: " << n << '\t' \
                  << "sum: " << tot << '\t' \
                  << "error: " << correctSum - tot << '\n';
    }
        
    
    MPI::Finalize(); // MPI finalize
    
    return 0;
}
