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
#include <algorithm> 
//#include <experimental/filesystem>
//namespace fs = std::experimental::filesystem;

using namespace std;
//#define IP_ADDR "127.0.0.1"

bool sortcol(const vector<int>& v1, const vector<int>& v2){
	return v1[0] < v2[0];
}

bool strsortcol(const vector<string>& v1, const vector<string>& v2){
    return v1[0] < v2[0];
}

bool comparator(string a,string b){
    return a<b;
}




int main(int argc, char** argv)
{   
    char* file;
    file = argv[1];
    char* direc;
    direc = argv[2];

    string filename(file);
    vector<vector<string>> lines;
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

    sort(vect.begin(), vect.end(), sortcol);

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

    vector<string> my_files;
    struct dirent *d;
    DIR *dr;
    dr = opendir(direc);
    if(dr!=NULL)
    {
        //cout<<"List of Files and Folders:-\n";
        while((d=readdir(dr))!=NULL)
            if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0 && d->d_type != DT_DIR){
                //cout<<d->d_name<<endl;
                my_files.push_back(d->d_name);
            }
        closedir(dr);
    }
    else
        cout<<"\nError Occurred!";
    

    // string path = direc;
    // for (const auto & entry : fs::directory_iterator(path)){
    //      cout << entry.path() << endl;
    // }
    //cout << lines.size() <<" "<<lines[2][1]<<endl;
    
    //cout<<msg<<endl;
    //void *buf = rmsg;

    // for(int i=0; i<size; i++)
    // {
    //     msg[i] = lines[0][2][i];
    // }

    // listen on sock_fd, new connection on new_fd

    

    // int csockfd;

    //cout<<stoi(lines[1][0])<<" no.of neighbors"<<endl;

    sort(my_files.begin(),my_files.end(),comparator);

    for(int i=0;i<my_files.size();i++){
        cout<<my_files[i]<<endl;
    }
    

    string All_files = lines[0][2] +"#1";
    for(int i =0;i < my_files.size();i++){
        All_files = All_files + "#" + my_files[i];
    }
    // cout<<All_files<<endl;
    // cout<<"--------------------------------"<<endl;

    int size = All_files.length();



    string msgfneigh1[stoi(lines[1][0])];
    string msgfneigh2[stoi(lines[1][0])];


    int numofsoc = 0;
    for(int i =0; i< stoi(lines[1][0]); i++) {
        if (stoi(lines[2][2*i]) < stoi(lines[0][0])) {
            numofsoc++;
        }
    }

    int neigh_uid[stoi(lines[1][0])];
    int my_uid = stoi(lines[0][2]);
    
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
            string T;
            stringstream X(rmsg); // X is an object of stringstream that references the S string 
            bool first = true;
            while (getline(X, T, '#')) {  
                /* X represents to read the string from stringstream, T use for store the token string and, 
                   ' ' whitespace represents to split the string where whitespace is found. */  
                if(first){
                    first = false;
                    neigh_uid[i] = stoi(T);
                }
            }
            cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<neigh_uid[i]<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            //neigh_uid[i] = atoi(rmsg);
            //cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<rmsg/*neigh_uid[i]*/<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            msgfneigh1[i] = rmsg;
            memset(&rmsg[0],0,sizeof(rmsg));
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
            string T;
            stringstream X(rmsg); // X is an object of stringstream that references the S string 
            bool first = true;
            while (getline(X, T, '#')) {  
                /* X represents to read the string from stringstream, T use for store the token string and, 
                   ' ' whitespace represents to split the string where whitespace is found. */  
                if(first){
                    first = false;
                    neigh_uid[i] = stoi(T);
                }
            }
            cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<neigh_uid[i]<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            //neigh_uid[i] = atoi(rmsg);
            //cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<rmsg/*neigh_uid[i]*/<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            msgfneigh1[i] = rmsg;
            memset(&rmsg[0],0,sizeof(rmsg));

        }
    }

    string final2hopmsg;

    

    for(int i=0;i<stoi(lines[1][0]);i++){
        //cout<<msgfneigh1[i]<<endl;
        
        if(i==0){
            final2hopmsg = msgfneigh1[i];
        }

        else{
            final2hopmsg = final2hopmsg + "/" + msgfneigh1[i];
        }
        
    }


    for(int i=0;i<stoi(lines[1][0]);i++){
        //cout<<"Inside the loop"<<endl;
        
        if(stoi(lines[2][2*i]) < stoi(lines[0][0])){
           
            int size1 = final2hopmsg.length();
            char msg1[size1+1];
            strcpy(msg1,final2hopmsg.c_str());
            int len,bytes_sent;
            len=strlen(msg1);
            bytes_sent=send(sockfd[i],msg1,len,0);
            char rmsg1[65546];
            recv(sockfd[i],rmsg1,65546,0);
            //cout<<rmsg1<<endl;
            //neigh_uid[i] = atoi(rmsg);
            //cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<rmsg/*neigh_uid[i]*/<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            msgfneigh2[i] = rmsg1;
            memset(&rmsg1[0],0,sizeof(rmsg1));
            memset(&msg1[0],0,sizeof(msg1));
            
        }
        else{
                        
            int size1 = final2hopmsg.length();
            char msg1[size1+1];
            strcpy(msg1,final2hopmsg.c_str());
            int len,bytes_sent;
            len=strlen(msg1);
            bytes_sent=send(sockfd[i],msg1,len,0);
            char rmsg1[65546];
            recv(sockfd[i],rmsg1,65546,0);
            //neigh_uid[i] = atoi(rmsg);
            //cout<<"connected to "<<stoi(lines[2][2*i])<<" with unique-ID "<<rmsg/*neigh_uid[i]*/<<" on port "<<stoi(lines[2][2*i+1])<<endl;
            msgfneigh2[i] = rmsg1;
            memset(&rmsg1[0],0,sizeof(rmsg1));
            memset(&msg1[0],0,sizeof(msg1));
            

        }
    }

    // for(int i=0;i<stoi(lines[1][0]);i++){
    //     cout<<msgfneigh2[i]<<endl;
    // }

    vector<vector<string>> neigh1;
    for(int i=0;i<stoi(lines[1][0]);i++){
        
        vector<string> v1;

        string T;
        stringstream X(msgfneigh1[i]); // X is an object of stringstream that references the S string


        while (getline(X, T, '#')) {

            v1.push_back(T);

        }
        
        neigh1.push_back(v1);

    }




    vector<vector<string>> neigh2;
    int h2c=0;
    for(int i=0;i<stoi(lines[1][0]);i++){

        string T;
        stringstream X(msgfneigh2[i]); // X is an object of stringstream that references the S string

        

        while (getline(X, T, '/')) {

            vector<string> v1;

            string U;
            stringstream Y(T);
            while(getline(Y,U,'#')){
                v1.push_back(U);
                
            }
            h2c++;
            neigh2.push_back(v1);
        }

    }


    sort(neigh1.begin(),neigh1.end(),strsortcol);
    sort(neigh1.begin(),neigh1.end(),strsortcol);



    for(int i=0;i<stoi(lines[3][0]);i++){
        bool val=true;

        if(val){

            for(int j=0;j<stoi(lines[1][0]);j++){
                if(std::find(neigh1[j].begin(), neigh1[j].end(), lines[4+i][0]) != neigh1[j].end()){
                    cout<<"Found "<<lines[4+i][0]<<" at "<<neigh1[j][0]<<" with MD5 0 at depth 1"<<endl;
                    val = false;
                    break;
                }
            }
        }

        if(val){
        
            for(int j=0;j<h2c;j++){
                if(std::find(neigh2[j].begin(), neigh2[j].end(), lines[4+i][0]) != neigh2[j].end()){
                    cout<<"Found "<<lines[4+i][0]<<" at "<<neigh2[j][0]<<" with MD5 0 at depth 2"<<endl;
                    val = false;
                    break;
                }

            }
        }

        if(val){
            cout<<"Found "<<lines[4+i][0]<<" at 0 with MD5 0 at depth 0"<<endl;
        }

    }

    close(csockfd);
    for(int i =0; i< stoi(lines[1][0]); i++){
        
        close(sockfd[i]);
    }
}