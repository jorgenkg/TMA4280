#include <iostream>
#include <vector>
#include <math.h>


int main(int argc, char **argv)
{   
    int n;
    double tot, error, correctSum;
    
    correctSum = M_PI * M_PI / (double) 6;
    std::vector<double> vec_results ( 11 );
    std::vector<double> vec ( (int) pow(2, 14) );
    
    // Create the vector
    for(double i=1; i<=vec.size(); i++ )
        vec[i-1]= 1/(i*i);
    
    // Loop over the specified n values
    for( int i=3; i<=14; i++)
    {
        n = pow(2, i);
        tot = 0.0;
        
        // Perform the calculation
        #pragma omp parallel for schedule(static) reduction(+:tot)
        for( int j=0; j<n; j++ )
            tot += vec[j];
        
        // Collect the results for printing (or file writing).
        vec_results[i-3] = tot;
    }
    
    for( int i=0; i<=vec_results.size(); i++)
        std::cout << "k: " << pow(2, i+3) << '\t' \
                  << "sum: " << vec_results[i] << '\t' \
                  << "error: " << correctSum - vec_results[i] << '\n';
        
    
    
    return 0;
}
