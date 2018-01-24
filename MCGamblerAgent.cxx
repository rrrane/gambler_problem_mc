
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


#include "rlglue/Agent_common.h" /* Required for RL-Glue */
#include "rlglue/RL_glue.h"

static gsl_vector* local_action;
static action_t* this_action;
static gsl_vector* last_observation;


static double Q[101][101];
static int N[101][101];
static int limit[101];
static int maxindex[101];

double v[101];
int sizeof_v = 101;

static int deterministic[101];
static int first_episode;
static int episode_over;

void agent_init()
{
	
  	//Allocate Memory
  	local_action = gsl_vector_calloc(1);
  	this_action = local_action;
  	last_observation = gsl_vector_calloc(1);
	
	
	
	for(int i = 0; i < 101; i++)
  	{
		limit[i] = i<100-i?i:100-i;
	  	for(int j = 0; j < 101; j++){
		  	Q[i][j] = 0.0;
			N[i][j] = 0;	
		}

		deterministic[i] = 0;
		v[i] = 0.0;
  	}
	
	first_episode = 1;
	episode_over = 0;

}

const action_t *agent_start(const observation_t *this_observation) {
 	
		
	for(int i = 0; i < 101; i++){
		int limit = i<100-i?i:100-i;
		for(int j = 1; j<= limit; j++){
			N[i][j] = 0;
			Q[i][j] = 0.0;
		}
		v[i] = 0.0;
		maxindex[i] = 1;
	}

	int obs = (int)gsl_vector_get(this_observation,0);
	
	int action = deterministic[obs];

	
	if(first_episode)
		action = randInRange(limit[obs])+1;//get_index_based_on_probability(policy[obs], 101);
	
	


	N[obs][action]++;

	gsl_vector_set(local_action, 0, action);
  	
  	gsl_vector_memcpy(last_observation,this_observation);/*save observation, might be useful on the next step */
  
  	return this_action;
}

const action_t *agent_step(double reward, const observation_t *this_observation) {
  
	
	int last_obs = (int)gsl_vector_get(last_observation, 0);
	int current_obs = (int)gsl_vector_get(this_observation, 0);
	int last_action = (int)gsl_vector_get(local_action, 0);
		
	int action = deterministic[current_obs];

	
	if(first_episode)
		action = randInRange(limit[current_obs])+1;//get_index_based_on_probability(policy[obs], 101);


 	N[current_obs][action]++; 
  	gsl_vector_set(local_action,0,action);
  	gsl_vector_memcpy(last_observation,this_observation);
  	

  	return this_action;
}


void agent_end(double reward) {
  /* final learning update at end of episode */
	double final_return = RL_return();
	
	if(final_return > 0){	
		for(int i = 0; i < 101; i++){
			for(int j = 1; j <= limit[i]; j++){
				if(N[i][j] > 0){
					Q[i][j] += final_return;
					if(Q[i][maxindex[i]] < Q[i][j])
						maxindex[i] = j;
				}
			}
		}
	

		
		//Average action values

		for(int i = 0; i < 101; i++){
			for(int j = 1; j <= limit[i]; j++)
			{
				if(N[i][j]>0)
					Q[i][j] = Q[i][j] / (double)N[i][j];
			}
		}
	

	}		
		

	
	//------------------- Policy Improvement ---------------------------------------

	for(int i = 1; i < 100; i++){
		double max = 0.0;
		
		deterministic[i] = randInRange(limit[i])+1;

		if(final_return > 0){
			int ties[limit[i]];

			int count = 0;
			
			for(int j = maxindex[i]; j <= limit[i]; j++){
				
				if(Q[i][maxindex[i]] == Q[i][j])
				{
					ties[count] = j;
					count++;
				}
			}
			
			
			int num = randInRange(count);

			deterministic[i] = ties[num];
		}
	}
		
		
	//----------------------END Policy Improvement-----------------------------------
	

	//If this was a first episode, update first_episode flag
	if(first_episode)
		first_episode = 0;


}

void agent_cleanup() {
  /* clean up mememory */
  gsl_vector_free(local_action);
  gsl_vector_free(last_observation);
}

const char* agent_message(const char* inMessage) {
  	/* might be useful to get information from the agent */
  	if(strcmp(inMessage,"OVER")==0){
		
		//Calculate state values from action values

		for(int i = 0; i < 101; i++){
			int maxindex = 0;
			for(int j = 1; j < limit[i]; j++){
				if(Q[i][maxindex] < Q[i][j])
				maxindex = j;
			}
			v[i] = Q[i][maxindex];
		}

				
		
		return "GOT-IT";
  	}
  	
  	/* else */
  	return "I don't know how to respond to your message";
}
