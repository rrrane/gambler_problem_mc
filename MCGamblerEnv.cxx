
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rlglue/Environment_common.h" /* Required for RL-Glue interface*/
#include "rlglue/RL_glue.h"
#include "utils.h"



static gsl_vector* local_observation;
static reward_observation_terminal_t this_reward_observation;
static const int nStatesSimpleEnv = 101;


void env_init() 
{
	local_observation = gsl_vector_calloc(1);
	
	this_reward_observation.observation=local_observation;
	this_reward_observation.reward=0;
	this_reward_observation.terminal=0;

	return;
}

const observation_t* env_start()
{
	//Select state from 1 to 99 at random
	int obs = randInRange(99);
	obs++;
  	gsl_vector_set(local_observation,0,obs);

  	return this_reward_observation.observation;
}

const reward_observation_terminal_t *env_step(const action_t *this_action)
{
  	int episode_over = 0; 
  	int act = (int)gsl_vector_get(this_action, 0);
  	double the_reward =  0.0;

	//Select pr(heads) = 0.25 and pr(tails) = 0.75
  	int rand_no = randInRange(100);
	int mul = 1;

	if(rand_no > 25){
		mul = -1;
	}

		

  	int obs = (int)gsl_vector_get(local_observation, 0); /* Get current state */
	

	obs += mul*act; /* State transition */
	
	if(obs<=0 || obs>=100){
		episode_over = 1;
		/*
		const char *message = "OVER";
		RL_agent_message(message);
		printf("\nMessage Sent\n");	
		*/
	}
	
	if(obs == 100){
		the_reward = 1.0;
	}

  	gsl_vector_set(local_observation, 0, obs);
  	this_reward_observation.reward = the_reward;
  	this_reward_observation.terminal = episode_over;
  
  return &this_reward_observation;
}


void env_cleanup()
{
  gsl_vector_free(local_observation);
}

const char* env_message(const char* inMessage) 
{
  if(strcmp(inMessage,"what is your name?")==0)
  return "my name is skeleton_environment!";
  
  /* else */
  return "I don't know how to respond to your message";
}
