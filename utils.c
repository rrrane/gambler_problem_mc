#include "utils.h"


int randInRange(int max){
    return (int)floor(((double)rand() / (double)RAND_MAX)*max);
}

double rand_un()
{
    return (double)rand() / (double)RAND_MAX;
}

double randn (double mu, double sigma)
{
    double U1, U2, W, mult;
    static double X1, X2;
    static int call = 0;
    
    if (call == 1)
    {
        call = !call;
        return (mu + sigma * (double) X2);
    }
    
    do
    {
        U1 = -1 + ((double) rand () / RAND_MAX) * 2;
        U2 = -1 + ((double) rand () / RAND_MAX) * 2;
        W = pow (U1, 2) + pow (U2, 2);
    }
    while (W >= 1 || W == 0);
    
    mult = sqrt ((-2 * log (W)) / W);
    X1 = U1 * mult;
    X2 = U2 * mult;
    
    call = !call;
    
    return (mu + sigma * (double) X1);
}

int get_index_based_on_probability(double *vector, int size){
	double num = rand_un();
	double probability_chunks[size];

	double val = 0.0;

	for(int i = 0; i < size; i++){
		probability_chunks[i] = val + vector[i];
		val = probability_chunks[i];
	}

	val = 0.0;

	for(int i = 0; i < size; i++){
		if(num > val && num <= probability_chunks[i])
		       return i;
		val = probability_chunks[i];
	}
	return 1;

}
