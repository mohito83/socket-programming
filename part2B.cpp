/*PART 2B for EE465 project. Simulation for M/M/1/INF multiple queue models*/
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

/*this function generates a random number and based on the  provided it
* decides which server does the arrival goes.
*/
int server_switch(){
	double val = 0.0;
	val = drand48();
	if(val>=0 && val<0.2){
		return 1;
	}
	if(val>=0.2 && val<0.4) {
		return 2;
	}
	if(val>=0.4 && val<0.6) {
		return 3;
	}
	if(val>=0.6 && val<0.8) {
		return 4;
	}
	if(val>=0.8 && val<1) {
		return 5;
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

//This function find the minimum of the given counter values and returns an 
//integer value identifying the minimum of counters.
int min(double event1, double event2a, double event2b, double event2c, double event2d, double event2e){
	double temp = INFIN;
	int val = -1;
	
	if(temp > event1){
		temp = event1;
		val = 0;
	}
	if(temp > event2a){
		temp = event2a;
		val = 1;
	}
	if(temp > event2b){
		temp = event2b;
		val = 2;
	}
	if(temp > event2c){
		temp = event2c;
		val = 3;
	}
	if(temp > event2d){
		temp = event2d;
		val = 4;
	}
	if(temp > event2e){
		temp = event2e;
		val = 5;
	}
	
	return val;
}

//This function simulates the case I & II of the project 
//lambda			arrival rate
//mu				servicing rate
//tot_arrivals		Total number of arrivals to the system
//using these parameters this function calculates the average service time, 
//average number of customers in the system,idle period, blocking ratio etc
void sim_case(double lambda, double mu, long int tot_arrivals){
	
	long int cur_arrivals = 0, i = 0;
	double event1 = 0.0, event2a = INFIN, event2b = INFIN, event2c = INFIN, event2d = INFIN, event2e = INFIN; //event1: time of next arrival, event2a,b,c,d,e: time of next departure from servers 1,2,3,4,5 respectively
	double service_time, cur_time = 0.0;
	double customers1 = 0.0, customers2 = 0.0, customers3 = 0.0,customers4 = 0.0,customers5 = 0.0;
	double avg_customers = 0.0, avg_delay = 0.0; //used for calculating statistics
	queue<double> wait_time_queue1,wait_time_queue2,wait_time_queue3,wait_time_queue4,wait_time_queue5; //queue to calculate the avg delay time
	
	double* pack_distribution = new double[21];
	int tot_customers_in_system = 0;
		
	while (cur_arrivals < tot_arrivals)
	{
	
	
		//arrival of packets
		if(min(event1,event2a,event2b,event2c,event2d,event2e) == 0){
			
			//flip the coin to decide which server this arrival will be sent for processing
			switch(server_switch()){
				case 1:
				//upon packet arrival calculate the # of customer in the system and update the ditribution matrix
				tot_customers_in_system = customers1+customers2+customers3+customers4+customers5;
				update_distribution_counter(pack_distribution,tot_customers_in_system);
				cur_arrivals++;
				avg_customers+=customers1;
				customers1 ++;
				cur_time = event1;
				wait_time_queue1.push(cur_time);
				event1 = cur_time + expon(lambda);
				if (customers1 == 1)	//if there is only 1 customer, he/she goes directly to service
				{
					service_time = expon(mu);
					event2a = cur_time + service_time;
				}
				break;
				
				case 2: 
				//upon packet arrival calculate the # of customer in the system and update the ditribution matrix
				tot_customers_in_system = customers1+customers2+customers3+customers4+customers5;
				update_distribution_counter(pack_distribution,tot_customers_in_system);
			
				cur_arrivals++;
				avg_customers+=customers2;
				customers2 ++;
				cur_time = event1;
				wait_time_queue2.push(cur_time);
				event1 = cur_time + expon(lambda);
				if (customers2 == 1)	//if there is only 1 customer, he/she goes directly to service
				{
					service_time = expon(mu);
					event2b = cur_time + service_time;
				}
				break;
				
				case 3:
				//upon packet arrival calculate the # of customer in the system and update the ditribution matrix
				tot_customers_in_system = customers1+customers2+customers3+customers4+customers5;
				update_distribution_counter(pack_distribution,tot_customers_in_system);
			
				cur_arrivals++;
				avg_customers+=customers3;
				customers3 ++;
				cur_time = event1;
				wait_time_queue3.push(cur_time);
				event1 = cur_time + expon(lambda);
				if (customers3 == 1)	//if there is only 1 customer, he/she goes directly to service
				{
					service_time = expon(mu);
					event2c = cur_time + service_time;
				}
				break;
				
				case 4: 
				//upon packet arrival calculate the # of customer in the system and update the ditribution matrix
				tot_customers_in_system = customers1+customers2+customers3+customers4+customers5;
				update_distribution_counter(pack_distribution,tot_customers_in_system);
			
				cur_arrivals++;
				avg_customers+=customers4;
				customers4 ++;
				cur_time = event1;
				wait_time_queue4.push(cur_time);
				event1 = cur_time + expon(lambda);
				if (customers4 == 1)	//if there is only 1 customer, he/she goes directly to service
				{
					service_time = expon(mu);
					event2d = cur_time + service_time;
				}
				break;
				
				case 5:
				//upon packet arrival calculate the # of customer in the system and update the ditribution matrix
				tot_customers_in_system = customers1+customers2+customers3+customers4+customers5;
				update_distribution_counter(pack_distribution,tot_customers_in_system);
			
				cur_arrivals++;
				avg_customers+=customers5;
				customers5 ++;
				cur_time = event1;
				wait_time_queue5.push(cur_time);
				event1 = cur_time + expon(lambda);
				if (customers5 == 1)	//if there is only 1 customer, he/she goes directly to service
				{
					service_time = expon(mu);
					event2e = cur_time + service_time;
				}
				break;
			}//end of switch case
		}else { 	//departure of packets 
			// compare the updated next service time for each server and the one which is minimum should be executed first.
			switch(min(event1,event2a,event2b,event2c,event2d,event2e)){
				case 1:
				customers1--;
				cur_time = event2a;
				avg_delay += (cur_time - wait_time_queue1.front());
				wait_time_queue1.pop();
				if (customers1 > 0) // the departure left a non-empty queue behind
				{
					service_time = expon(mu);
					event2a = cur_time + service_time;
				}
				else 				//the departure left the queue empty
				{
					event2a = INFIN; //no departure scheduled
				}
				break;
			
				case 2:
				customers2--;
				cur_time = event2b;
				avg_delay += (cur_time - wait_time_queue2.front());
				wait_time_queue2.pop();
				if (customers2 > 0) // the departure left a non-empty queue behind
				{
					service_time = expon(mu);
					event2b = cur_time + service_time;
				}
				else 				//the departure left the queue empty
				{
					event2b = INFIN; //no departure scheduled
				}	
				break;
				
				case 3:
				customers3--;
				cur_time = event2c;
				avg_delay += (cur_time - wait_time_queue3.front());
				wait_time_queue3.pop();
				if (customers3 > 0) // the departure left a non-empty queue behind
				{
					service_time = expon(mu);
					event2c = cur_time + service_time;
				}
				else 				//the departure left the queue empty
				{
					event2c = INFIN; //no departure scheduled
				}
				break;
				
				case 4:
				customers4--;
				cur_time = event2d;
				avg_delay += (cur_time - wait_time_queue4.front());
				wait_time_queue4.pop();
				if (customers4 > 0) // the departure left a non-empty queue behind
				{
					service_time = expon(mu);
					event2d = cur_time + service_time;
								
				}
				else 				//the departure left the queue empty
				{
					event2d = INFIN; //no departure scheduled
				}
				break;
				
				case 5:
				customers5--;
				cur_time = event2e;
				avg_delay += (cur_time - wait_time_queue5.front());
				wait_time_queue5.pop();
				if (customers5 > 0) // the departure left a non-empty queue behind
				{
					service_time = expon(mu);
					event2e = cur_time + service_time;
								
				}
				else 				//the departure left the queue empty
				{
					event2e = INFIN; //no departure scheduled
				}
				break;
			} //end of switch case
		} //end of else
	
	} // end of while

	avg_customers /=cur_arrivals; 
	avg_delay /= (cur_arrivals - (customers1+customers2+customers3+customers4+customers5));
	
	cout <<"Average number of packets in the system: "<<avg_customers<<endl;
	cout <<"Average delay in the system: "<<avg_delay<<endl;
	cout <<"The distribution of number of packets present in the system: "<<endl;

	int j = 0;
	for(;j<21;j++){
		cout << j <<"-"<<(pack_distribution[j]/cur_arrivals)*100.0<<"% ";
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
		cout << "Enter 3 for: Case III lambda = 0.5; mu = 1, p=0"<<endl;
		cin >> option;
	} else {
		case_no = argv[1];
		option = atoi(case_no);
	}
	
	switch(option){
		case 1 : sim_case(0.2*5,1,tot_arrivals);
			break;
		case 2 : sim_case(0.8*5,1,tot_arrivals);
			break;
		case 3 : sim_case(0.5*5,1,tot_arrivals);
			break;
		
		default	 : cout << "Invalid input exit from application!!" << endl;
					exit(0);
	}
	
	return 0;
}
