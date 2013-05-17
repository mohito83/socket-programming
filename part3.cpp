/*PART 3 for EE465 project. Simulation for M/G/1/INF queue models*/
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<string>
#include<queue>
#include<time.h>

using namespace std;

#define INFIN		999999999

/*This function generates the expeonential random varable to given rate*/
double expon(double rate){
	double val = 0.0;
	
	do{
		val = drand48();
	}while(val == 0 || val == 1);
	
	return -log(1-val)/rate;
}

/*this function generates the service times for the given M/G/1 queue model.*/
double generator(){
	double val = 0.0, rnd = 0.0;
	//long int m = 1, M = 10000;
	double alpha = 1.3;
	do{
		rnd = drand48();
	}while(rnd == 0);
	
	val = pow((1-rnd),(-1/alpha));
		
	return val;
}

void sim_case(double lambda, long int tot_arrivals){
	
	long int cur_arrivals = 0, i = 0;
	double event1 = 0.0, event2 = INFIN; //event1: time of next arrival, event2: time of next departure
	double service_time, cur_time = 0.0, customers = 0.0;
	double avg_customers = 0.0, avg_service_time = 0.0, avg_delay = 0.0; //used for calculating statistics
	double blocking_rate = 0.0, utilization = 0.0, sec_mom_serv_time = 0.0;
	double *service_time_arr = new double[tot_arrivals];
	queue<double> wait_time_queue; //queue to calculate the avg delay time
	
	while (cur_arrivals < tot_arrivals)
	{
		if (event1 < event2)	//arrival
		{
			cur_arrivals++;
			avg_customers+=customers;
			customers ++;
			cur_time = event1;
			wait_time_queue.push(cur_time);
			event1 = cur_time + expon(lambda);
			if (customers == 1)	//if there is only 1 customer, he/she goes directly to service
			{
				service_time = generator();
				event2 = cur_time + service_time;
				avg_service_time += service_time;
				service_time_arr[i] = service_time;
				i++;
			}
		}
		else					//departure
		{
			customers--;
			cur_time = event2;
			avg_delay += (cur_time - wait_time_queue.front());
			wait_time_queue.pop();
			if (customers > 0) // the departure left a non-empty queue behind
			{
				service_time = generator();
				event2 = cur_time + service_time;
				avg_service_time += service_time;
				service_time_arr[i] = service_time;
				i++;
			}
			else 				//the departure left the queue empty
			{
				event2 = INFIN; //no departure scheduled
			}
		}
	}

	utilization = avg_service_time/cur_time;
	avg_customers /=cur_arrivals; 
	avg_service_time /=(cur_arrivals-customers);
	avg_delay /= (cur_arrivals - customers);
	
	//calculating the second moment of service time
	//Step 1. calculating the variance
	int j = 0;
	double sq_delta = 0.0;
	for(; j < (cur_arrivals-customers); j++){
		sq_delta += pow((service_time_arr[j] - avg_service_time),2);
	}
	//Step 2. calulate second moment of service time
	sec_mom_serv_time = sq_delta/(cur_arrivals - customers) + pow(avg_service_time,2);
	
	cout <<"Mean number of packets in the system: "<<avg_customers<<endl;
	cout <<"Average delay per customer: "<<avg_delay<<endl;
	cout <<"Average service time: "<< avg_service_time<<endl;
	cout <<"Average waiting time: "<<(avg_delay - avg_service_time)<<endl;
	cout <<"Idle period of server: "<<(1 - utilization)<<endl;
	cout <<"Blocking ratio: "<<blocking_rate<<endl;
	cout <<"Average of second moment of the service time: "<< sec_mom_serv_time <<endl;
	
	//clear dynamically allocated memory
	delete []service_time_arr;
}

int main(int argc, char* argv[]){

	char* cseed = 0, *ctot_arrivals = 0;
	long int seed, tot_arrivals;
	double lambda = 0.1;
	
	if(argc == 1){
		cout << "Please enter total number of arrivals and value of seed in that order" << endl;
		cin >> tot_arrivals;
		cin >> seed;
	} else {
		ctot_arrivals = argv[1];
		cseed = argv[2];
		tot_arrivals = atoi(ctot_arrivals);
		seed = atoi(cseed);
	}
	
	//initializing random number generator 
	srand48(seed);
	
	sim_case(lambda, tot_arrivals);
	
	return 0;
	
}
