#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int main(int argc, char ** argv ){
	cout << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << endl;

	clock_t t;
	//t = clock();
	int start_s=clock();
	char cmd[200];
	//for(int i = 0; i < 10000; ++i)
	//	{
			//sprintf(cmd,"curl -I http://222.255.27.28 -H \"Host: dantri.com.vn\" -H \"User-Agent: xxx\" > dir/test%d.txt",i);
	  		//cout << cmd << endl;
			//system(cmd);	
	//	}
	//sleep(3);
	//int i=system ("dir");
  	//printf ("The value returned was: %d.\n",i);
	//cout << "hello" << endl;

	// = clock() - t;
	//printf("It took me %d clock (%f second).\n",t,((float)t) / CLOCKS_PER_SEC);
	//intf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);	
	int stop_s=clock();
	cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
	return 0; 
}
