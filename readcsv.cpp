#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;


uint32_t ip_to_int (const char * ip)
{
    /* The return value. */
    unsigned v = 0;
    /* The count of the number of bytes processed. */
    int i;
    /* A pointer to the next digit to process. */
    const char * start;

    start = ip;
    for (i = 0; i < 4; i++) {
        /* The digit being processed. */
        char c;
        /* The value of this byte. */
        int n = 0;
        while (1) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }
            /* We insist on stopping at "." if we are still parsing
               the first, second, or third numbers. If we have reached
               the end of the numbers, we will allow any character. */
            else if ((i < 3 && c == '.') || i == 3) {
                break;
            }
            else {
                return -1;
            }
        }
        if (n >= 256) {
            return -1;
        }
        v *= 256;
        v += n;
    }
    return v;
}

void print_ip(int ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

void int_to_ip(uint32_t ip, char * addr){
	unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    sprintf(addr, "%d.%d.%d.%d", bytes[3], bytes[2],bytes[1], bytes[0]);
}

int main(){

	string txt;

	uint32_t ip = ip_to_int("172.16.63.8"); 
	cout << ip << endl;
	struct in_addr addr = {ip};

	cout << "addr: " << inet_ntoa(addr) << endl;
	print_ip(ip);
	char address[20];
	int_to_ip(ip, address);
	cout << "ad: " << address << endl;
	//ifstream file ("GeoIPCountryWhois.csv");

	//while (file.good())
	//{
	//	getline(file, txt, ',');
	//	cout << txt << endl; 
	//}

	return 0;
}
