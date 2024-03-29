#include <iostream>
#include <vector>
#include <math.h>

int main(int argc, char **argv)
{   
    double tot, error;
    int k;
    
    double correctSum = M_PI * M_PI / (double) 6;
    std::vector<double> vec ( (int) pow(2, 14) );
    std::vector<double> vec_results ( 11 );
    
    for( double i=1; i<=vec.size(); i++ )
        vec[i-1]= 1/(i*i);
    
    for( int i=3; i<=14; i++){
        tot = 0;
        k = (int) pow(2, i);
        
        for( int j=0; j<k; j++ )
            tot += vec[j];
        
        std::cout << "k: " << k << '\t' \
                  << "sum: " << tot << '\t' \
                  << "error: " << correctSum - tot << '\n';
    }
    
    return 0;
}
