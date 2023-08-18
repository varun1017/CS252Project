#include<iostream>
#include<fstream>
#include <sstream>
#include<vector>
#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include<cstring>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<dirent.h>
#include<algorithm>
//#include <experimental/filesystem>
//namespace fs = std::experimental::filesystem;

using namespace std;
//#define IP_ADDR "127.0.0.1"

string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

bool comparator(string a,string b){
    return a<b;
}

bool sortcol(const vector<int>& v1, const vector<int>& v2){
	return v1[0] < v2[0];
}

int main(int argc, char** argv)
{   
    char* file;
    file = argv[1];
    char* direc;
    direc = argv[2];

    string filename(file);
    vector<vector<string>> lines ;
    vector<string> my_files, v;
    string All_files = "";

    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    while (getline(input_file, line)){
        
        istringstream ss(line);

        vector<string> subvector;
        string word; 

        while (ss >> word) {
            
            subvector.push_back(word);
           
        }

        lines.push_back(subvector);
    }


    input_file.close();

    vector<vector<int> > vect;

    for(int i=0;i<stoi(lines[1][0]);i++){
        vect.push_back({stoi(lines[2][2*i]),stoi(lines[2][2*i+1])});
    }

    // Number of rows;
	//int m = vect.size();

	// Number of columns (Assuming all rows
	// are of same size). We can have different
	// sizes though (like Java).
	//int n = vect[0].size();

	// Displaying the 2D vector before sorting
	//cout << "The Matrix before sorting is:\n";
	// for (int i = 0; i < m; i++) {
	// 	for (int j = 0; j < n; j++)
	// 		cout << vect[i][j] << " ";
	// 	//cout << endl;
	// }

	// Use of "sort()" for sorting on basis
	// of 2nd column
	sort(vect.begin(), vect.end(), sortcol);

	// Displaying the 2D vector after sorting
	//cout << "The Matrix after sorting is:\n";
	// for (int i = 0; i < m; i++) {
	// 	for (int j = 0; j < n; j++)
	// 		cout << vect[i][j] << " ";
	// 	//cout << endl;
	// }

    for(int i=0;i<stoi(lines[1][0]);i++){
        ostringstream str1;
        ostringstream str2;
        str1<<vect[i][0];
        str2<<vect[i][1];
        lines[2][2*i] = str1.str();
        lines[2][2*i+1] = str2.str();
    }

    string search_files[stoi(lines[3][0])];
    

    for(int i=0;i<stoi(lines[3][0]);i++){
        search_files[i] = lines[4+i][0];
    }

    sort(search_files,search_files+stoi(lines[3][0]),comparator);

    for(int i=0;i<stoi(lines[3][0]);i++){
        lines[4+i][0] = search_files[i] ;
    }

    struct dirent *d;
    DIR *dr;
    dr = opendir(direc);
    bool istrue = true;
    if(dr!=NULL)
    {
        //cout<<"List of Files and Folders:-\n";
        while((d=readdir(dr))!=NULL)
            if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0 && d->d_type != DT_DIR){
                //cout<<d->d_name<<endl;
                my_files.push_back(d->d_name);
                // if(istrue){
                // All_files = All_files + d->d_name;
                // istrue = false;
                // }
                // else{
                //     All_files = All_files+"###" + d->d_name;
                // }
            }
        closedir(dr);
    }
    else
        cout<<"\nError Occurred!";
    //cout<<endl;

    sort(my_files.begin(),my_files.end(),comparator);

    for(int i=0;i<my_files.size();i++){
        cout<<my_files[i]<<endl;
    }

    All_files = lines[0][2]+All_files;

    int sfid[stoi(lines[3][0])];

	for (int i = 0; i < stoi(lines[3][0]); i++) {
		sfid[i]=0;
	}

    for(int i =0 ; i< stoi(lines[3][0]);i++){
        All_files = All_files + "#" + lines[4+i][0];
    
    }

    //cout<<"msg to neighs "<<All_files<<endl;

    int size = All_files.length();
    

    int neigh_uid[stoi(lines[1][0])];
    int my_uid = stoi(lines[0][2]);
    int no_of_search_files = stoi(lines[3][0]);
    struct sockaddr_in my_addr; // my address information
    int addrlen = sizeof(my_addr);
    

    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons(stoi(lines[0][1])); // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY;//htonl(INADDR_ANY); // auto-fill with my IP
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

    int csockfd=socket(AF_INET,SOCK_STREAM,0);

	//cout<<
    bind(csockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr));//<<" bind"<<endl;//<<" bind"<<endl;//<<endl; //binding the sockfd with the MYPORT
   
    int sockfd[stoi(lines[1][0])]; 
    socklen_t sin_size;

    for(int i =0; i< stoi(lines[1][0]); i++){
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0); // do some error checking!
        //cout<<"socket"<< sockfd[i]<<endl;
    }

    //cout<<"Out of this loop"<<endl;

    struct sockaddr_in dest_addr[stoi(lines[1][0])];

    for(int i =0; i < stoi(lines[1][0]);i++){
        dest_addr[i].sin_family = AF_INET; // host byte order
        dest_addr[i].sin_port = htons(stoi(lines[2][2*i+1])); // short, network byte order
        inet_aton("127.0.0.1", &(dest_addr[i].sin_addr));//inet_addr(IP_ADDR);
        memset(&(dest_addr[i].sin_zero), '\0', 8); // zero the rest of the struct
    }

    listen(csockfd, 20);

    for(int i=0;i<stoi(lines[1][0]);i++){
        //cout<<"Inside the loop"<<endl;
        
        if(stoi(lines[2][2*i]) < stoi(lines[0][0])){
            // don’t forget to error check the connect()!
            int x = -1;
            while(x < 0){
                int connectnum =connect(sockfd[i], (struct sockaddr *)&dest_addr[i], sizeof(dest_addr[i]));
                //cout<<"connectnum"<<" "<<connectnum<<endl;
                x = connectnum;
            }
            //cout<<"Came here";

            char msg[size+1];
            strcpy(msg,All_files.c_str());
            int len,bytes_sent;
			len=strlen(msg);
			bytes_sent=send(sockfd[i],msg,len,0);
            char rmsg[65546];
            recv(sockfd[i],rmsg,65546,0);
            //neigh_uid[i] = rmsg;
            //cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<neigh_uid[i]<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            //cout<<"1st rec msg "<<rmsg<<endl;

            string T;
            stringstream X(rmsg); // X is an object of stringstream that references the S string  
   
            bool isstrue = true;
            bool first = true;
            string tosend = "";
            // use while loop to check the getline() function condition  
            while (getline(X, T, '#')) {  
                /* X represents to read the string from stringstream, T use for store the token string and, 
                   ' ' whitespace represents to split the string where whitespace is found. */  
                if(isstrue){
                    isstrue = false;
                    neigh_uid[i] = stoi(T);
                }
                
                else{
                    //cout<<T<<endl;
                    //search if T is present;
                    if (std::find(my_files.begin(), my_files.end(), T) != my_files.end()) {
                        if(first){
                            first = false;
                            tosend = tosend + "1";
                        }
                        else{
                            tosend = tosend + "#"+ "1";
                        }
                    }
                    else{
                        if(first){
                            first = false;
                            tosend = tosend + "0";
                        }
                        else{
                            tosend = tosend + "#"+ "0";
                        }
                    }
                } 
            }  

            memset(&rmsg[0],0,sizeof(rmsg));

            //cout<<tosend<<"bye1"<<endl;     

            char msg2[tosend.length()+1];
            strcpy(msg2,tosend.c_str());

            send(sockfd[i],msg2,strlen(msg2),0);
            recv(sockfd[i],rmsg,65546,0);
            //cout<<"2nd rec msg "<<rmsg<<endl;

            string Q;
            stringstream Y(rmsg); // X is an object of stringstream that references the S string  

            int rmind=0;

            while (getline(Y, Q, '#')) {
                if(Q=="1"){
                    if(sfid[rmind] != 0 & sfid[rmind] < neigh_uid[i] ){
                        rmind++;
                        continue;
                    }
                    else{
                        sfid[rmind] = neigh_uid[i];
                    }
                
                }
                rmind++;

            }


            memset(&msg2[0],0,sizeof(msg2));
            memset(&rmsg[0],0,sizeof(rmsg));

            cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<neigh_uid[i]<<" on port "<<stoi(lines[2][2*i+1])<<endl;


        }
        else{
            
            while(true){
                socklen_t sin_size=sizeof(struct sockaddr_in);
				sockfd[i]=accept(csockfd,(struct sockaddr *)&dest_addr[i],&sin_size);
                if(sockfd[i] < 0){
                    continue;
                }
                else{
                    break;
                }
            }

            
            char msg[size+1];
            strcpy(msg,All_files.c_str());
            int len,bytes_sent;
			len=strlen(msg);
			bytes_sent=send(sockfd[i],msg,len,0);
            char rmsg[65546];
            recv(sockfd[i],rmsg,65546,0);
            //neigh_uid[i] = rmsg;
            //cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<neigh_uid[i]<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            //cout<<"1st rec msg "<<rmsg<<endl;

            string T;
            stringstream X(rmsg); // X is an object of stringstream that references the S string  
   
            bool isstrue = true;
            bool first = true;
            string tosend = "";
            // use while loop to check the getline() function condition  
            while (getline(X, T, '#')) {  
                /* X represents to read the string from stringstream, T use for store the token string and, 
                   ' ' whitespace represents to split the string where whitespace is found. */  
                if(isstrue){
                    isstrue = false;
                    neigh_uid[i] = stoi(T);
                }
                
                else{
                    //search if T is present;
                    //cout<<T<<endl;
                    if (std::find(my_files.begin(), my_files.end(), T) != my_files.end()) {
                        if(first){
                            first = false;
                            tosend = tosend + "1";
                        }
                        else{
                            tosend = tosend + "#"+ "1";
                        }
                    }
                    else{
                        if(first){
                            first = false;
                            tosend = tosend + "0";
                        }
                        else{
                            tosend = tosend + "#"+ "0";
                        }
                    }
                } 
            }  

            memset(&rmsg[0],0,sizeof(rmsg));

            //cout<<tosend<<endl;

            memset(&rmsg[0],0,sizeof(rmsg));


            char msg2[tosend.length()+1];
            strcpy(msg2,tosend.c_str());

            send(sockfd[i],msg2,strlen(msg2),0);
            recv(sockfd[i],rmsg,65546,0);
            //cout<<"2nd rec msg "<<rmsg<<endl;

            string Q;
            stringstream Y(rmsg); // X is an object of stringstream that references the S string  

            int rmind=0;

            while (getline(Y, Q, '#')) {
                if(Q=="1"){
                    if(sfid[rmind] != 0 & sfid[rmind] < neigh_uid[i] ){
                        rmind++;
                        continue;
                    }
                    else{
                        sfid[rmind] = neigh_uid[i];
                    }
                
                }
                rmind++;

            }


            memset(&msg2[0],0,sizeof(msg2));
            memset(&rmsg[0],0,sizeof(rmsg));

            cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<neigh_uid[i]<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            
        }
    }

    for (int i = 0; i < stoi(lines[3][0]); i++) {
		if (sfid[i]!=0) {
			cout<<"Found "<<lines[4+i][0]<<" at "<<sfid[i]<<" with MD5 0 at depth 1"<<endl;
		}
		else
			cout<<"Found "<<lines[4+i][0]<<" at "<<sfid[i]<<" with MD5 0 at depth 0"<<endl;
	}

    close(csockfd);
    for(int i =0; i< stoi(lines[1][0]); i++){
        
        close(sockfd[i]);
    }
}