

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "rlglue/RL_glue.h" /* Required for RL-Glue  */

extern double v[101];
extern int sizeof_v;

void saveResults(double* data, int dataSize, const char* filename);

int main(int argc, char *argv[]) {

	srand(time(NULL));
  	int t1 = time(NULL);	
  	int numEpisodes = 100;
  	int numRuns = 30;
	int maxStepsInEpisode = INT_MAX;
	double result[sizeof_v];
	

  	for(int i = 0; i < sizeof_v; i++){
		result[i] = 0.0;
	}
		
	for(int i = 0; i < numRuns; i++){
		RL_init();

		for(int j = 0; j < numEpisodes; j++){
			RL_episode(maxStepsInEpisode);
		}

		RL_agent_message("OVER");

		//Code to read state values
		
		for(int j = 0; j < sizeof_v; j++){
			result[j] += v[j]; //(double)gsl_vector_get(V, j);
		}

		RL_cleanup();
		printf(".");
		fflush( stdout );
	}
  	
  	printf("\nDone\n");
	int t2 = time(NULL);

	printf("Time Elapsed %d seconds\n", t2-t1);
  
  	/* average over runs */
  	for (int i =0; i < sizeof_v; i++){
    		result[i] = result[i]/(double)numRuns;
  	}
  
  	/* Save data to a file */
  	saveResults(result, sizeof_v , "RL_EXP_OUT.dat");
  
  	return 0;
}

void saveResults(double* data, int dataSize, const char* filename) {
  	FILE *dataFile;
  	int i;
  	dataFile = fopen(filename, "w");
  	for(i = 0; i < dataSize; i++){
    		fprintf(dataFile, "%lf\n", data[i]);
  	}
  	fclose(dataFile);
}
