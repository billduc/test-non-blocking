/*Day la chuong trinh download mot so loai file tu URL
    URL tro den 1 file can down.
*/


#include <iostream>
#include <fstream>
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //for exit(0);
#include <sys/socket.h>
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include <arpa/inet.h>
#include <string>
#include <typeinfo>
#include <algorithm>

/*
link example for download 
http://yoursmiles.org/ismile/trollfaces/i11423.gif

___pdf:
https://student.uit.edu.vn/sites/daa/files/201704/23-ttktta_10-4-2017.pdf
student.uit.edu.vn/sites/daa/files/201704/23-ttktta_10-4-2017.pdf
https://student.uit.edu.vn/sites/daa/files/201704/189_qd-dhcntt_18-4-2017_0.pdf

___file compression:
http://crcv.ucf.edu/data/UCFCrowdCountingDataset_CVPR13.rar

___mp4: XXX
http://data.chiasenhac.com/downloads/1780/2/1779589-c8b3b5b7/128/Neu%20Duoc%20Lam%20Nguoi%20Tinh%20-%20Quynh%20Trang%20[MP4%20MV%20360p].mp4

___mp3:
http://data.chiasenhac.com/downloads/1782/3/1781714-81c76ccb/128/Attention%20-%20Charlie%20Puth%20[MP3%20128kbps].mp3

___image: 
http://thuvienanhdep.net/wp-content/uploads/2016/02/nhung-hinh-dep-ve-lang-que-viet-nam-qua-ong-kinh-cac-nha-nhiep-anh-6.jpg

___html:
http://www.fastgraph.com/help/jpeg_header_format.html
 */ 

using namespace std;

int hostname_to_ip(char * , char* );
string getDomain(const string& );
string getPath(const string& );
string getFileName(string);

int main(int argc , char *argv[])
{
    int socket_desc;

    char *message;
    char server_reply[10000];
    int total_len = 0;
    char *ip;
    char *cURL;
    int len; 
    char url[123];
    char IP[123];
    char domain[123];
    char path[345];
    char filename[234];
    printf("Enter your ULR: ");
    scanf("%s",url);
    //printf("%s\n",url); 

    //cURL = argv[1];
    string sDomain =  getDomain(url);
    for(int i = 0; i < sDomain.length(); ++i)
      domain[i] = sDomain[i];

    string sPath = getPath(url);
    for(int i = 0; i < sPath.length(); ++i)
      path[i] = sPath[i];

    string fileName = getFileName(sPath);

    for(int i = 0; i< fileName.length(); ++i)
      filename[i] = fileName[i];

    cout << "file name: " << fileName << endl;

    cout << "__" << domain << " " << path << endl;

    hostname_to_ip(domain,IP);
    printf("%s ",IP);

    FILE *file = NULL;
    struct sockaddr_in server;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    //server.sin_addr.s_addr = inet_addr("198.11.181.184");
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }

    puts("Connected\n");

    //Send request
    //message = "GET /download/pdfurl-guide.pdf HTTP/1.1\r\nHost: www.axmag.com\r\n\r\n Connection: keep-alive\r\n\r\n Keep-Alive: 300\r\n";
    string mes = "GET " + sPath + " HTTP/1.1\r\nHost: " + sDomain + "\r\n\r\n Connection: keep-alive\r\n\r\n Keep-Alive: 300\r\n\r\n";
/*    string headrequest = "HEAD "  + sPath + " HTTP/1.1\r\nHost: " + sDomain + "\r\n\r\n";
    
    char mess_head[234];

    for(int i = 0; i < headrequest.length(); ++i)
        mess_head[i] = headrequest[i];
    
    if ( send(socket_desc, mess_head, strlen(mess_head), 0) < 0)
    {
        puts("Send head request failed");
        return 1;
    }

    char reply_buf_headrequest[12345];
    int len_head =  recv(socket_desc, reply_buf_headrequest, sizeof reply_buf_headrequest , 0); 
    
    puts(reply_buf_headrequest); */
    //string mes = "GET " + sPath + "\r\n\r\n Connection: keep-alive\r\n\r\n Keep-Alive: 300\r\n\r\n";
    //string mes = "GET https://student.uit.edu.vn/sites/daa/files/201704/23-ttktta_10-4-2017.pdf HTTP/1.1";
    //std::cout << mes << std::endl;
    char mess[345];
    for(int i = 0; i < mes.length(); ++i)
      mess[i] = mes[i];

    if( send(socket_desc , mess , strlen(mess) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

    puts("Data Send\n"); 

    //remove(filename);
    //file = fopen(filename, "w+b");

    ofstream out;
    out.open(filename, ios::binary);

    if(file == NULL){
        printf("File could not opened");
    }   
    //int received_len = recv(socket_desc, server_reply , sizeof(server_reply) - 1 , 0);
    bool header_skipped=false;
    while(1)
    {
        
        int received_len = recv(socket_desc, server_reply , sizeof server_reply , 0);
        //server_reply  = strstr(server_reply, "\r\n\r\n");
        if (received_len > 0)     // if bytes received
        {
            size_t data_offset = 0;      // normally take data from begin of butter 
            if (!header_skipped) {    // if header was not skipped, look for its end
                char *eoh = "\r\n\r\n";
                auto it = search (server_reply, server_reply + received_len, eoh, eoh + 4); 
                if (it != server_reply + received_len) {   // if header end found: 
                    data_offset = it - server_reply + 4;      // skip it
                    header_skipped = true;              // and then do not care any longer
                }                             // because data can also containt \r\n\r\n
            }
            out.write(server_reply + data_offset, received_len - data_offset); // write, ignoring before the offset
        }
       

        if( received_len < 0 ){
            puts("recv failed");
            break;
        }

         if( received_len == 0 ){
            puts(" finished");
            break;
        }
        //server_reply[received_len] = '\0';
        // char* tt = (char*)server_reply;
        // std::cout << tt << std::endl;

        total_len += received_len;

        puts(server_reply);   

        //fwrite(server_reply , sizeof(server_reply) , 1, file);

        //fwrite(server_reply , received_len , 1, file);

        //fwrite(content, sizeof(content) - 1, 1, file);
        printf("\nReceived byte size = %d\nTotal lenght = %d\n", received_len, total_len);
        //memset( server_reply, '\0', sizeof(server_reply) );
    }

    puts("\nReply received\n");

    //fclose(file);
    out.close();
    return 0;
}

int hostname_to_ip(char *hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    printf("__%s\n",hostname);


    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        printf("___error here\n");
        herror("gethostbyname");
        return 1;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
     
    return 1;
}


string _trim(const string& str)
{
    size_t start = str.find_first_not_of(" \n\r\t");
    size_t until = str.find_last_not_of(" \n\r\t");
    string::const_iterator i = start==string::npos ? str.begin() : str.begin() + start;
    string::const_iterator x = until==string::npos ? str.end()   : str.begin() + until+1;
    return string(i,x);
}
 
void parse_url(const string& raw_url) 
{
    string path,domain,x,protocol,port,query;
    int offset = 0;
    size_t pos1,pos2,pos3,pos4;
    x = _trim(raw_url);
    offset = offset==0 && x.compare(0, 8, "https://")==0 ? 8 : offset;
    offset = offset==0 && x.compare(0, 7, "http://" )==0 ? 7 : offset;
    pos1 = x.find_first_of('/', offset+1 );
    path = pos1==string::npos ? "" : x.substr(pos1);
    domain = string( x.begin()+offset, pos1 != string::npos ? x.begin()+pos1 : x.end() );
    path = (pos2 = path.find("#"))!=string::npos ? path.substr(0,pos2) : path;
    port = (pos3 = domain.find(":"))!=string::npos ? domain.substr(pos3+1) : "";
    domain = domain.substr(0, pos3!=string::npos ? pos3 : domain.length());
    protocol = offset > 0 ? x.substr(0,offset-3) : "";
    query = (pos4 = path.find("?"))!=string::npos ? path.substr(pos4+1) : "";
    path = pos4!=string::npos ? path.substr(0,pos4) : path;
    cout << "[" << raw_url << "]" << endl;
    cout << "protocol: " << protocol << endl;
    cout << "domain: " << domain << endl;
    cout << "port: " << port << endl;
    cout << "path: " << path << endl;
    cout << "query: " << query << endl;
}

string getDomain(const string& raw_url){
    string path,domain,x;
    int offset = 0;
    size_t pos1,pos2,pos3,pos4;
    x = _trim(raw_url);
    offset = offset==0 && x.compare(0, 8, "https://")==0 ? 8 : offset;
    offset = offset==0 && x.compare(0, 7, "http://" )==0 ? 7 : offset;
    pos1 = x.find_first_of('/', offset+1 );
    path = pos1==string::npos ? "" : x.substr(pos1);
    domain = string( x.begin()+offset, pos1 != string::npos ? x.begin()+pos1 : x.end() );
    
    return domain;
}

string getPath(const string& raw_url) 
{
    string path,domain,x;
    int offset = 0;
    size_t pos1,pos2,pos3,pos4;
    x = _trim(raw_url);
    offset = offset==0 && x.compare(0, 8, "https://")==0 ? 8 : offset;
    offset = offset==0 && x.compare(0, 7, "http://" )==0 ? 7 : offset;
    pos1 = x.find_first_of('/', offset+1 );
    path = pos1==string::npos ? "" : x.substr(pos1);
    path = (pos2 = path.find("#"))!=string::npos ? path.substr(0,pos2) : path;
    return path;
}

string getFileName(string path){
  string name;
  for(int i = path.length(); i >= 0 ; --i)
    if (path[i] =='/')
      break;
    else
      name = path[i] + name; 
  
  return name;
}

