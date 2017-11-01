#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h> 
#include <iostream>
#include <fstream>
#include <string.h>
#include <cassert>

#include <arpa/inet.h>
#include <string>
#include <vector>
#include<unistd.h>
#include <sys/select.h>

/* According to earlier standards */
#include <sys/types.h>


using namespace std;

vector<string> host;
vector<string> domain;
#define CHUNK_SIZE 512

void adddata(){
	host.push_back("125.212.233.133");
	domain.push_back("24h.com.vn");

	host.push_back("125.212.247.3");
	domain.push_back("24h.com.vn");

	host.push_back("123.30.134.2");
	domain.push_back("nhaccuatui.com");

	host.push_back("123.30.215.16");
	domain.push_back("tuoitre.vn");

	host.push_back("212.193.33.27");
	domain.push_back("codeforces.com");

	host.push_back("222.255.27.168");
	domain.push_back("genk.vn");

	host.push_back("222.255.27.28");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.9");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.22");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.169");
	domain.push_back("dantri.com.vn");
}

void set_nonblock(int socket) {
    int flags;
    flags = fcntl(socket,F_GETFL,0);
    assert(flags != -1);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

int recv_timeout(int s , int timeout)
{
    int size_recv , total_size= 0;
    struct timeval begin , now;
    char chunk[CHUNK_SIZE];
    double timediff;
     
    //make socket non blocking
    fcntl(s, F_SETFL, O_NONBLOCK);
     
    //beginning time
    gettimeofday(&begin , NULL);
     
    while(1)
    {
        gettimeofday(&now , NULL);
         
        //time elapsed in seconds
        timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
         
        //if you got some data, then break after timeout
        if( total_size > 0 && timediff > timeout )
        {
            break;
        }
         
        //if you got no data at all, wait a little longer, twice the timeout
        else if( timediff > timeout*2)
        {
            break;
        }
         
        memset(chunk ,0 , CHUNK_SIZE);  //clear the variable
        if((size_recv =  recv(s , chunk , CHUNK_SIZE , 0) ) < 0)
        {
            //if nothing was received then we want to wait a little before trying again, 0.1 seconds
            usleep(100000);
        }
        else
        {
            total_size += size_recv;
            printf("%s" , chunk);
            //reset beginning time
            gettimeofday(&begin , NULL);
        }
    }
     
    return total_size;
}

int recv_to(int fd, char *buffer, int len, int flags, int to) {

   fd_set readset;
   int result, iof = -1;
   struct timeval tv;

   // Initialize the set
   FD_ZERO(&readset);
   FD_SET(fd, &readset);
   
   // Initialize time out struct
   tv.tv_sec = 0;
   tv.tv_usec = to * 1000;
   // select()
   result = select(fd+1, &readset, NULL, NULL, &tv);

   // Check status
   if (result < 0)
      return -1;
   else if (result > 0 && FD_ISSET(fd, &readset)) {
      // Set non-blocking mode
      if ((iof = fcntl(fd, F_GETFL, 0)) != -1)
         fcntl(fd, F_SETFL, iof | O_NONBLOCK);
      // receive
     string res;
     int i = 0;
      while (1){
      	int result_len = recv(fd, buffer, len, flags);
      	cout << "res len: " << result_len << endl;
      	if (result_len <= 0)
      		break;
      	else
      		{
      			res+= buffer;
      			cout <<"_____" <<i << endl;
      			++i;
      		}
      }
      //puts(buffer);
      cout << res << endl;
      cout << "ok " << fd << endl;
      memset(buffer,sizeof(buffer),0);
      // set as before
      if (iof != -1)
         fcntl(fd, F_SETFL, iof);
      return result;
   }
   return -2;
}

int main(int argc, char ** argv){
	freopen("out.txt","w",stdout);
	clock_t t;
	//t = clock();
	int start_s=clock();
	
	int listen_sd, max_sd, new_sd, rc, on = 1;

	char buffer[80000];

	struct sockaddr_in 	addr;
	struct timeval 		timeout;
	fd_set       master_set, working_set;

	adddata();

	//listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	
	for(int i = 0; i < host.size(); ++i)
	//for(int i = 0; i < 4; ++i)
	{
		listen_sd = socket(AF_INET, SOCK_STREAM, 0);
		
		cout << "____" << host[i] << "+++" << domain[i] << " sock: " << listen_sd << endl;
		addr.sin_addr.s_addr = inet_addr(host[i].c_str());
		addr.sin_family = AF_INET;
		addr.sin_port = htons(80);

		if (listen_sd < 0){
			perror("socket() failer");
			exit(-1);
		}

		if (connect(listen_sd , (struct sockaddr *)&addr , sizeof(addr)) < 0)
	    {
	        puts("connect error");
	        return 1;
	    }

	    puts("Connected\n");

		//string sDomain = "dantri.com.vn";
	    string sDomain = domain[i];

		string mess = "GET / HTTP/1.1\r\nHost: "+ sDomain +"\r\nConnection: keep-alive \r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6)\r\nAccept: */*\r\n\r\n";
		//string mess = "GET / HTTP/1.1\r\nHost: " + sDomain + "\r\nConnection: close\r\n\r\n";

		//cout<< "query:\n"  << mess << endl;

	    if( send(listen_sd , mess.c_str() , mess.length() , 0) < 0)
	    {
	        puts("Send failed");
	        return 1;
	    }

	    //puts("Data Send\n"); 

	    int count  = 0;

	    recv_to(listen_sd,buffer,8888,O_NONBLOCK,5);
	    //recv_timeout(listen_sd, 2);
	    // while(1)
	    // {
	        
	    //     int received_len = recv(listen_sd, buffer , 2048 , 0);

	    //     puts(buffer);   
	    //     cout ++;

	    //     if (cout == 1)
	    //     	break;
	    //     //fwrite(server_reply , sizeof(server_reply) , 1, file);

	    //     //fwrite(server_reply , received_len , 1, file);

	    //     //fwrite(content, sizeof(content) - 1, 1, file);
	    //     //printf("\nReceived byte size = %d\nTotal lenght = %d\n", received_len, total_len);
	    //     //memset( server_reply, '\0', sizeof(server_reply) );
    	// }

    	puts("\nReply received\n");
    	cout << "Case: " << i <<  endl;
    	//close(listen_sd);
	}	
	int stop_s=clock();
	cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
	return 0;
}