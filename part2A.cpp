/*PART 2 for EE465 project. Simulation for M/M/1/INF queue models with feedback*/
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<string>
#include<queue>
#include<time.h>
#include <fstream>


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


void print_stats(ostream &stats_file, double interarrival, double feedback)
{
	stats_file << interarrival << "," << feedback << endl;
}


/*this function generates a random number and based on the threshold provided it
* decides whether to allow packets to be feeded back into the queue or not.
*/
bool isfeedback(double threshold){
	double val = 0.0;
	val = drand48();///RAND_MAX;
	if(threshold==0 || val>threshold){
		return false;
	}else {
		return true;
	}
}

//This function update the packet distribution of the system.
void update_distribution_counter(double *pack_distribution, int total_customers){
	switch(total_customers){
		case 0: pack_distribution[0]++;
		break;
		case 1: pack_distribution[1]++;
		break;
		case 2: pack_distribution[2]++;
		break;
		case 3: pack_distribution[3]++;
		break;
		case 4: pack_distribution[4]++;
		break;
		case 5: pack_distribution[5]++;
		break;
		case 6: pack_distribution[6]++;
		break;
		case 7: pack_distribution[7]++;
		break;
		case 8: pack_distribution[8]++;
		break;
		case 9: pack_distribution[9]++;
		break;
		case 10: pack_distribution[10]++;
		break;
		case 11: pack_distribution[11]++;
		break;
		case 12: pack_distribution[12]++;
		break;
		case 13: pack_distribution[13]++;
		break;
		case 14: pack_distribution[14]++;
		break;
		case 15: pack_distribution[15]++;
		break;
		case 16: pack_distribution[16]++;
		break;
		case 17: pack_distribution[17]++;
		break;
		case 18: pack_distribution[18]++;
		break;
		case 19: pack_distribution[19]++;
		break;
		case 20: pack_distribution[20]++;
		break;
	}

}

//This function simulates the case I & II of the project 
//lambda			arrival rate
//mu				servicing rate
//tot_arrivals		Total number of arrivals to the system
//using these parameters this function calculates the average service time, 
//average number of customers in the system,idle period, blocking ratio etc
void sim_case(double lambda, double mu, double prob, long int tot_arrivals){
	
	long int cur_arrivals = 0, i = 0;
	double event1 = 0.0, event2 = INFIN; //event1: time of next arrival, event2: time of next departure
	double service_time, cur_time = 0.0, customers = 0.0;
	double avg_customers = 0.0, avg_delay = 0.0; //used for calculating statistics
	queue<double> wait_time_queue; //queue to calculate the avg delay time
	
	double* pack_distribution = new double[21];
	
	long int feedbackPackets = 0;
	
	while (cur_arrivals < tot_arrivals)
	{
		
		if (event1 < event2)	//arrival
		{
		
			//upon packet arrival calculate the # of customer in the system and update the ditribution matrix
			update_distribution_counter(pack_distribution,(int)customers);
				
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
				if(isfeedback(prob)){
					feedbackPackets++;
									
					//updating the packet distribution in the system at this point.
					//update_distribution_counter(pack_distribution,(int)customers);	
					
					wait_time_queue.push(cur_time);
					//avg_customers+=customers;
					customers++;
				}
			}
			else 				//the departure left the queue empty
			{
				event2 = INFIN; //no departure scheduled
			}
		}
	}

	//cout <<"feddback packets="<<feedbackPackets<<endl;
	//cout << "current arrivals="<<cur_arrivals<<" && "<<(cur_arrivals - customers)<<endl;
	
	avg_customers /=cur_arrivals; 
	avg_delay /= (cur_arrivals - customers);
	
	cout <<"Average number of packets in the system: "<<avg_customers<<endl;
	cout <<"Average delay in the system: "<<avg_delay<<endl;
	cout <<"The distribution of number of packets present in the system: "<<endl;
	
	int j = 0;
	for(;j<21;j++){
		cout << j <<"-"<<(pack_distribution[j]/(cur_arrivals+feedbackPackets))*100.0<<"% ";
	}
	cout << endl;
	
	delete []pack_distribution;
}


int main( int argc, char* argv[]){
	//initializing random number generator 
	srand48(unsigned(time(NULL)));
	long int tot_arrivals = 50000;
	
	char* case_no = 0;
	int option;
	if(argc == 1){
		cout << "Please provide a case number from the following to simulate" << endl;
		cout << "Enter 1 for: Case I lambda = 0.2; mu = 1, p=0"<<endl;
		cout << "Enter 2 for: Case II lambda = 0.8; mu = 1, p=0"<<endl;
		cout << "Enter 3 for: Case III lambda = 0.5; mu = 1, p=."<<endl;
		cout << "Enter 4 for: Case IV lambda = 0.5; mu = 1, p=0.08"<<endl;
		cout << "Enter 5 for: Case V lambda = 0.5; mu = 1, p=0.4"<<endl;
		cin >> option;
	} else {
		case_no = argv[1];
		option = atoi(case_no);
	}
	
	switch(option){
		case 1 : sim_case(0.2,1,0,tot_arrivals);
			break;
		case 2 : sim_case(0.8,1,0,tot_arrivals);
			break;
		case 3 : sim_case(0.5,1,0,tot_arrivals);
			break;
		case 4 : sim_case(0.5,1,0.08,tot_arrivals);
			break;
		case 5 : sim_case(0.5,1,0.4,tot_arrivals);
			break;
		default	 : cout << "Invlaid input exit from application!!" << endl;
					exit(0);
	}
	
	return 0;
}
