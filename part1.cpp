/*PART 1 for EE465 project. Simulation for M/M/1/INF, M/M/1/N and M/M/K/N queue models*/
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

//This function simulates the case III of the project 
//lambda			arrival rate
//mu				servicing rate
//tot_arrivals		Total number of arrivals to the system
//no_servers		number of servers
//queue_len			lenght of queue.
//using these parameters this function calculates the average service time, 
//average number of customers in the system,idle period, blocking ratio etc
void sim_case(double lambda, double mu, long int tot_arrivals, int no_servers, long int queue_len){
	
	long int cur_arrivals = 0, allowed_customers = 0;
	double event1 = 0.0, event2 = INFIN, event3 = INFIN; //event1: time of next arrival, event2: time of next departure
	double service_time=0.0, cur_time = 0.0, customers = 0.0;
	double avg_customers = 0.0, avg_service_time = 0.0, avg_delay = 0.0; //used for calculating statistics
	double blocking_rate = 0.0, utilization = 0.0, sec_mom_serv_time = 0.0;
	double *service_time_arr = new double[tot_arrivals];
	
	queue<double> wait_time_queue; //queue to calculate the avg delay time
	int i = 0;
	
	
	while (cur_arrivals < tot_arrivals)
	{
		if (event1 < event2 && event1 < event3)	//arrival
		{
			cur_arrivals++;
			cur_time = event1;
			event1 = cur_time + expon(lambda);
			if(customers < (queue_len+no_servers)){
				allowed_customers++;
				avg_customers+=customers;
				customers ++;
				wait_time_queue.push(cur_time);
				
				if (customers <= no_servers)	//if there is only 2 customer, he/she goes directly to service
				{
					//check which server will process this event
					if(event2 == INFIN){
						service_time = expon(mu);
						event2 = cur_time + service_time;
						avg_service_time += service_time;
						service_time_arr[i] = service_time;
						i++;
					} else {
						service_time = expon(mu);
						event3 = cur_time + service_time;
						avg_service_time += service_time;
						service_time_arr[i] = service_time;
						i++;
					}
					
				}
			} 			
		}
		else if(event2 < event3)				//departure
		{
			customers--;
			cur_time = event2;
			avg_delay += (cur_time - wait_time_queue.front());
			wait_time_queue.pop();
			if (customers > 1) // the departure left a non-empty queue behind
			{
				service_time = expon(mu);
				event2 = cur_time + service_time;
				avg_service_time += service_time;
				service_time_arr[i] = service_time;
				i++;
			}
			else 				//the departure left the queue empty
			{
				event2 = INFIN; //no departure scheduled
			}
		} else {
			customers--;
			cur_time = event3; 
			avg_delay += (cur_time - wait_time_queue.front());
			wait_time_queue.pop();
			if (customers > 1) // the departure left a non-empty queue behind
			{
				service_time = expon(mu);
				event3 = cur_time + service_time;
				avg_service_time += service_time;
				service_time_arr[i] = service_time;
				i++;
			}
			else 				//the departure left the queue empty
			{
				event3 = INFIN; //no departure scheduled
			}
			
		}
	}

	
	utilization = (avg_service_time/cur_time)/2;
	avg_customers /=allowed_customers; 
	avg_service_time /=(allowed_customers-customers);
	avg_delay /= (allowed_customers - customers);
	
	//calculating the second moment of service time
	//Step 1. calculating the variance
	int j = 0;
	double sq_delta = 0.0;
	for(; j < (allowed_customers-customers); j++){
		sq_delta += pow(service_time_arr[j],2);
	}
	//sec_mom_serv_time = sq_delta/serv2_customer;
	
	//Step 2. calulate second moment of service time
	sec_mom_serv_time = sq_delta/(allowed_customers-customers);
	
	//blocking rate
	blocking_rate = (double)(tot_arrivals - allowed_customers)/(double)tot_arrivals;
	
	cout <<"Utilization: "<<utilization<<endl;
	cout <<"Average number of packets in the system: "<<avg_customers<<endl;
	cout <<"Average delay in the system: "<<avg_delay<<endl;
	cout <<"Blocking ratio: "<<blocking_rate<<endl;
	cout <<"Idle period of server: "<<(1 - utilization)<<endl;
	/*cout <<"Average service time: "<< avg_service_time<<endl;
	cout <<"Average of second moment of the service time: "<< sec_mom_serv_time <<endl;
	cout <<"Average waiting time in queue: "<<(avg_delay - avg_service_time)<<endl;*/
	
	
	//clear dynamically allocated memory
	delete []service_time_arr;
}

//This function simulates the case I & II of the project 
//lambda			arrival rate
//mu				servicing rate
//tot_arrivals		Total number of arrivals to the system
//using these parameters this function calculates the average service time, 
//average number of customers in the system,idle period, blocking ratio etc
void sim_case(double lambda, double mu, long int tot_arrivals){
	
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
				service_time = expon(mu);
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
				service_time = expon(mu);
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
	
	cout <<"Utilization: "<<utilization<<endl;
	cout <<"Average number of packets in the system: "<<avg_customers<<endl;
	cout <<"Average delay in the system: "<<avg_delay<<endl;
	cout <<"Blocking ratio: "<<blocking_rate<<endl;
	cout <<"Idle period of server: "<<(1 - utilization)<<endl;
	/*cout <<"Average service time: "<< avg_service_time<<endl;
	cout <<"Average of second moment of the service time: "<< sec_mom_serv_time <<endl;
	cout <<"Average waiting time in queue: "<<(avg_delay - avg_service_time)<<endl;*/
	
	
	//clear dynamically allocated memory
	delete []service_time_arr;
}


int main( int argc, char* argv[]){
	//initializing random number generator 
	srand48(unsigned(time(NULL)));
	
	char* case_no = 0;
	int option;
	if(argc == 1){
		cout << "Please provide a case number from the following to simulate" << endl;
		cout << "Enter 1 for: Case I M/M/1/INFINITY with lambda = 0.3; mu = 1, FIFO, 5000 arrivals."<<endl;
		cout << "Enter 2 for: Case II M/M/1/INFINITY with lambda = 0.97; mu = 1, FIFO, 5000 arrivals."<<endl;
		cout << "Enter 3 for: Case III M/M/2/5 with lambda = 0.7; mu = 1, FIFO, 10000 arrivals."<<endl;
		cin >> option;
	} else {
		case_no = argv[1];
		option = atoi(case_no);
	}
	
	switch(option){
		case 1 : sim_case(0.3,1,5000);
			break;
		case 2 : sim_case(0.97,1,5000);
			break;
		case 3 : sim_case(0.7,1,10000,2,5);
			break;
		default	 : cout << "Invlaid input exit from application!!" << endl;
					exit(0);
	}
	
	return 0;
}
