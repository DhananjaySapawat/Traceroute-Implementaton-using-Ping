#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include<fstream>
using namespace std;
int arr[30] = {0};
int n ;
string exec(string command) {
   char buffer[128];
   string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}
string GetIpAddress(string PingOutput){
	    int i = 0;
       while(PingOutput.substr(i,4)!="from"){
        	i++;
        }
	    int j = i+4;
	    string a = PingOutput.substr(j,1);
      while(a!="T"){
        	j++;
        	a = PingOutput.substr(j,1);
        	if(a=="t"){
        		break;
        	}
       }
       while(PingOutput[j]!=':'){
       	j--;
       }
       int k = j - i-5; 
       return PingOutput.substr(i+5,k);
}
bool TraceOver(string HopsPingOutput){
	 int i = 0 ;
	 while(i<(HopsPingOutput.length()- 3) ){
	 	  if(HopsPingOutput.substr(i,3)=="exp"){
	 	  	return true;
	 	  }
	 	  i++;
	 }
    return false;
}
bool CheckRequestTimeOut(string HopsPingOutput){
	int i = 0 ;
	 while(i<(HopsPingOutput.length()- 3) ){
	 	  if(HopsPingOutput.substr(i,3)=="Req"){
	 	  	return true;
	 	  }
	 	  i++;
	 }
    return false;
}
string PrintTime(string PingOutput){
	int i = 0 ;
	string s ;
	while(PingOutput[i]!='m'){
		i++;
	}
	s = PingOutput.substr(0,i);
	while(i<=5){
		cout<<" ";
		i++;
	}
	cout<<s<<" ms";
}
string RTT(string PingOutput){
	int a = PingOutput.length();
	int b = a-1 ;
	int d = 0 ;
	while(PingOutput.substr(b-1,1)!="="){
		b--;
		d++;
	}
	return PingOutput.substr(b+1,d-2);
}
void TimeOFPingToIPAddress(string IPAddress,int no){
    string PingCommand = "ping "+IPAddress ;
	 string PingOutput = exec(PingCommand);
	 if(CheckRequestTimeOut(PingOutput)){
	 	cout<<"     *   "<<"     *   "<<"     *   ";
	 	return;
	 }
	 arr[no] = stoi(RTT(PingOutput));
	 int i =  0;
	 int j = 0;
    while(i!=3){
        	j++;
	 	if(PingOutput.substr(j,2)=="ti"){
	 		PrintTime(PingOutput.substr(j+5,10));
	 		i++;
	 	}
	 }
}
void PrintSNo(int i){
	if(i/10>=1){
		cout<<" "<<i;
		return;
	}
	cout<<"  "<<i;
}
void tracert(string site){
	int i = 1;
	cout<<endl;
	string SitePing = "ping  "+site+" -4";
	string SitePingOutput = exec(SitePing);
	cout<<"Tracing route to "+site+" ["+GetIpAddress(SitePingOutput)+"]"<<endl;
	cout<<"over a maximum of 30 hops:"<<endl<<endl;
	while(i<=30){
	 string HopsPing = "ping -i "+ to_string(i)+" "+site+" -4";
	 string HopsPingOutput = exec(HopsPing);
	 PrintSNo(i);
	 if(CheckRequestTimeOut(HopsPingOutput)){
	 	   cout<<"     *   "<<"     *   "<<"     *   ";
	   	cout<<"  Request timed out."<<endl;
	   	i++;
	   	continue;
	   }
	  string IPAddresss = GetIpAddress(HopsPingOutput);
	  TimeOFPingToIPAddress(IPAddresss,i);
	  cout<<"  "<<IPAddresss<<endl;
	
	  if(!TraceOver(HopsPingOutput)){
	  	n = i ; 
	  	break;
	  }
	 i++;
	}
	cout<<endl;
	cout<<"Trace complete."<<endl;
	return;
}
int main() {
    
   string domain;
   getline (cin, domain);
   tracert(domain);
    ofstream answer;
    string out = "RTTOutput.txt";
    answer.open(out);
    for(int i = 1;i<=n;i++){
    	answer<<i<<","<<arr[i]<<endl;
    }
     answer.close();
	return 0 ;
}