#include <iostream>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

int main(){
    string str;
    ifstream file("text.html");
    string line;
    if (file.is_open()){
        while ( getline (file,line) )
            {
              //std::cout << line << '\n';
                str.append(line);
            }
        file.close();
    }
    //cout<< str;
    // int be = str.find("<title>");
    // int en = str.find("</title>");
    // cout<< be <<" " << en << endl; 
    // string re;
    // re =  str.substr(be+7,en - be - 7);
    // cout<< endl << endl << "result: "<< re << endl;


     regex e(".*<title>(.*?)</title>.*");
     regex ee("<title>(.*?)</title>");
     //bool ok = regex_match(str,e);
     std::smatch match;
     string result;
    if (std::regex_search(str, match, ee) && match.size() > 1) {
        result = match.str(1);
    } else {
        result = std::string("");
    } 
    cout << result;
     bool ok = regex_search(str,e);
     if (ok)
        cout << "yes" << endl;
        else
        cout << "no" << endl;
 //    while(1){
 //        cin >> str;
 //        //regex e("abc.", regex_constants::icase);
	// //regex e("abc?");
	// regex e("abc*");
 //        //regex e("abc+");
	// bool match = regex_match(str,e);

 //        cout << (match ? "Matched" : "Not matched") << endl;
 //    }
}
