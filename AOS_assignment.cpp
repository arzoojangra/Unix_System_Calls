//AOS Assignment 
//Submitted by: Arzoo Jangra  06
//Submitted to: Bharti Rana Ma'am
//On terminal you just have to write the filename and type of the file (i.e. "regular" for regular file
// or "pipe" for an unnamed pipe or "npipe" for a named pipe)
//All other information will be asked later 
//Syntax : ./a.out <filename> <filetype>
//Example : ./a.out myfile regular   another example ./a.out pipe1 pipe



#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>

using namespace std;


bool file_exists(char *fileName)     //To check whether the file already exists or not!
// Input: File name
// Checks if file exists or not by opening it in read only mode
// If file exists it will be opened successfully
// If file does not exists open will give error
// Output: TRUE if file exists and FALSE if file does not exists

{
    int fd = open(fileName, O_RDONLY);
    if(fd != -1)      //File was opened successfully which clearly means that the file with this name already existed!!!
    {    int status = close(fd);       //Closing the opened file!!
        if(status == -1) perror("\nError while checking file exists\n");
        return true;      //Indicates file already exists!!
    }
    return false;    //Indicates file does not exists!!!
}




void read_file(char *fileName)     //To read the contents of file

// Input: File name
// Reads the contents of the file
// Output: none


{  if(file_exists(fileName))   //If TRUE i.e. If file exists
    {  int eof, curr_offset;
        
           int fd = open(fileName, O_RDONLY);   //Opening file in Read only mode
           eof = lseek(fd, 0, SEEK_END);     //Finding end of file
           lseek(fd, 0, SEEK_SET);   //Setting the offset at 0 bytes
        
           here:
            { char buf[512];
              int status = read(fd, buf, sizeof(buf));   //Reading the file in buffer
              curr_offset = lseek(fd, 0, SEEK_CUR);    //Finding current offset position
          

              if(status == -1)    //If there was error in reading the file
                  cout << "\nUnable to read file!!!\n";
              else if(status == 0)   //If there was nothing to read
                  cout << "\nEOF reached nothing to read!!!\n";
              else
                { cout <<"\n" << status <<" - bytes read\n";    
                  cout<<"Content:   "<<buf<<endl;
                } 
             } if (curr_offset < eof) goto here;
               else   
               close(fd);
          
     } else 
        cout <<"\nNo such file\nPlease create such file first!!\n";  //File does not exists
    
}

void write_file(char *fileName)     //Writing in the file 

// Input: File name
// Checks if file has some contents or not
// If file contains some contents it asks to overwrite the file or appned in the file
// If file is empty is asks user to enter the msg to write in the file
// Output: none

{
    if(file_exists(fileName))    //If TRUE i.e. File exists
    {   char ans;
        int offset;
        int fd = open(fileName, O_WRONLY);
        offset = lseek(fd, 0, SEEK_END);   //If offset = 0 file is empty
        if (offset > 0)
        {  cout<<"\nFile has some contents do you want to overwrite the file?(y/n): ";
           cin>>ans;
           if(ans == 'y')
            {   lseek(fd, 0, SEEK_SET);  //setting offset to 0
                cout<<"\nEnter the text to write in file:\n";
                char msg[1024];
                cin.ignore();
                cin.getline(msg, 1024);
                int num = write(fd, msg, sizeof(msg));     //Writing msg to file
                if (num!= -1) 
                cout<<msg<<" was written to file successfully!!\n";
                else cout<<"\nWrite error!!!";
                close(fd);
                cout<<"\nFile closed after write operation!!\n";
            }
            else
            {   cout<<"\nDo you want to append in the file? (y/n): ";
                cin>>ans;
                if( ans == 'y')
                { lseek(fd, 0, SEEK_END);  //Setting offset to last position of the file
                  cout<<"\nEnter the text to write in file:\n";
                  char msg[1024];
                  cin.ignore();
                  cin.getline(msg, 1024);
                  int num = write(fd, msg, sizeof(msg)); // writing to file
                  if (num!= -1) 
                  cout<<msg<<" was written to file successfully!!\n";
                  else cout<<"\nWrite error!!!";
                  close(fd);
                  cout<<"\nFile closed after write operation!!\n";
                }
                else cout<<"\nWrite operation terminated!!\n";
               
            }
          }
        
        else
            {    lseek(fd, 0, SEEK_SET);   //Setting the current offset value for the file to 0
                cout<<"\nEnter the text to write in file:\n";
                char msg[1024];
                cin.ignore();
                cin.getline(msg, 1024);
                int num = write(fd, msg, sizeof(msg));
                if (num!= -1) 
                cout<<msg<<" was written to file successfully!!\n";
                else cout<<"\nWrite error!!!";
                close(fd);
                cout<<"\nFile closed after write operation!!\n";
            
            }      
    }
}
      

void create_file(char *fileName)    //To create the file

// Input: File name
// Creates the file in the mode specified by the user
// Output: none


{
    if(file_exists(fileName))         //If TRUE i.e. If file exists
    {
        cout<<"\nFile already exists!!";

        int ch;
        cout <<"\n1. Skip";
        cout <<"\n2. Replace existing file.";
        cout <<"\nEnter Your Choice: ";
        cin >> ch;

        if(ch == 2) 
            cout << "\nReplacing existing file";
        else {
            cout << "\nFile already exits. Skipped file creation.";
            exit(0);
        }
    } 

    cout << "\nEnter Mode for file Creation: ";          //Asking the mode for file creation!!
    cout << "\n1. S_IRWXU:  Read, Write and Execute permission for the owner of the file.";
    cout << "\n2. S_IRWXG:  Read, Write and Execute permission for group.";
    cout << "\n3. S_IRWXO:  Read, Write and Execute permission for others.";
    
    int ch;
    cout << "\nEnter Your Choice(1-3): ";
    cin >> ch;
    char ans;
    int status = -1;
    switch(ch) {
        case 1:
            cout<<"\nDo you want to give read permission to the group and others (y/n):"; 
            cin>>ans;
            if (ans == 'y')
              { status = creat(fileName, S_IRWXU | S_IRGRP | S_IROTH);
                cout<<"\nAll permissions to owner and read permission to group and others!";
              }
            else 
            { status = creat(fileName, S_IRWXU); 
              cout<<"\nAll permission to owner and no permissions to group and others!";
            }
            break;
        case 2:
            status = creat(fileName, S_IRWXU | S_IRWXG | S_IROTH);
                cout<<"\nAll permissions to owner and group and read permission to others!";
                break;
        case 3:
            status = creat(fileName, S_IRWXU | S_IRWXO | S_IRGRP);
                cout<<"\nAll permissions to owner and others and read permission to group!";
                break;
        
        default: cout<<"\nCreating file with all permissions to user and read permission to group and others.";
            status = creat(fileName, S_IRWXU | S_IRGRP | S_IROTH);
            break;

    }
 
    // if new file is not created successfully open will return -1
    // which is checked to handle exception appropriately
    if(status == -1) 
    {
        perror("\ncreat() error\n");
        
    }  
    else cout<<"\nFile created Successfully!!\n";
    
 }
 
void info_file(char *fileName)    //To get the info about the file

// Input: File name
// Gives the info about the file in the mode specified by the user
// Output: none


{  if (file_exists(fileName))  //If TRUE i.e. if file exists
   {  struct stat sfile;

      if(stat(fileName, &sfile)==-1)   //If the file info can't be accessed
      cout<<"\nError Occurred\n";
  

      //Accessing data members of stat struct
      cout<<"\n\nFile Inode number: "<<sfile.st_ino;
      cout<<"\nUser ID of owner of the file: "<<sfile.st_uid;
      cout<<"\nGroup ID of owner of the file: "<<sfile.st_gid;
      cout<<"\nBest I/O block size: "<<sfile.st_blksize;
      cout<<"\nNo. of disk blocks allocated to the file: "<<sfile.st_blocks;
      cout<<"\nFile size: "<<sfile.st_size;
      cout<<"\nNumber of links: "<<(unsigned int)sfile.st_nlink;
      cout<<"\nDevice number: "<<sfile.st_dev;
      cout<<"\nFile Permissions for User: ";
      cout<<((sfile.st_mode & S_IRUSR) ? "r":"-");
      cout<<((sfile.st_mode & S_IWUSR) ? "w":"-");
      cout<<((sfile.st_mode & S_IXUSR) ? "x":"-");
      cout<<"\n";
      cout<<"\nFile Permissions for Group: ";
      cout<<((sfile.st_mode & S_IRGRP) ? "r":"-");
      cout<<((sfile.st_mode & S_IWGRP) ? "w":"-");
      cout<<((sfile.st_mode & S_IXGRP) ? "x":"-");
      cout<<"\n";
      cout<<"\nFile Permissions for Others: ";
      cout<<((sfile.st_mode & S_IROTH) ? "r":"-");
      cout<<((sfile.st_mode & S_IWOTH) ? "w":"-");
      cout<<((sfile.st_mode & S_IXOTH) ? "x":"-");
      cout<<"\n";
   }
    else cout<<"\nNo such file!!\n";    //If file doesn't exists
}

void write_pipe(char *pipeName, int pfd)  //To write in the unnamed pipe

//Input: pipeName writer for the pipe
//Takes the input from the user to enter in the pipe
//Writes the msg in the pipe
//Output: none

{  lseek(pfd, 0, SEEK_SET);
   cout<<"\nWriting in pipe: "<<pipeName;  //Writing in pipe
   int num;
   char msg[256];
   cout<<"\nEnter the msg to insert in the pipe: ";  //Asking for the msg to be written in pipe
   cin.ignore();
   cin.getline(msg, sizeof(msg));
   num = write(pfd, msg, sizeof(msg));
   if (num == -1)
   cout<<"\nWrite unsuccessfull!!!";
   else cout<<"\nWrite successfull!!!\n";
}

void read_pipe(char *pipeName, int pfd)   //To read from the unnamed pipe

//Input: pipeName reader for the pipe
//Takes the contents from the pipe 
//Writes the contents on the terminal
//Output: none

{  lseek(pfd, 0, SEEK_SET);
   cout<<"\nReading from pipe: "<<pipeName;  //Reading from the pipe
   int fd, num;
   char buf[256];
   num = read(pfd, buf, sizeof(buf));
   if (num == -1)
   cout<<"\nRead unsuccessfull!!!";
   else cout<<"\nRead successfull!!!";
   cout<<"\nPipe contents: "<<buf<<"\n";  //Displaying pipe contents
}

void write_npipe(char *pipeName)   //To write in the named pipe

//Input: pipeName 
//Takes the input from the user to enter in the pipe
//Writes the msg in the pipe
//Output: none

{  cout<<"\nWriting in pipe: "<<pipeName; //Writing in pipe
   int fd, num;
   char msg[256];
   fd = open(pipeName, O_RDWR);   //opens the pipe
   if (fd == -1)
   cout<<"\nError!!";
   else 
   {  cout<<"\nEnter the msg to insert in the pipe: ";  //Asking for the msg to write in pipe
      cin.ignore();
      cin.getline(msg, sizeof(msg));
      num = write(fd, msg, sizeof(msg));
      if (num == -1)
      cout<<"\nWrite unsuccessfull!!!";
      else cout<<"\nWrite successfull!!!";
   } 
}

void read_npipe(char *pipeName)   //To read from the pipe

//Input: pipeName 
//Takes the contents from the pipe 
//Writes the contents on the terminal
//Output: none

{  cout<<"Reading from pipe: "<<pipeName; //Reading from the pipe
   int fd, num;
   char buf[256];
   fd = open(pipeName, O_RDWR);   //opening pipe 
   if (fd == -1)
   cout<<"\nError!!";
   else 
     {  num = read(fd, buf, sizeof(buf));  //Reading from the pipe
        if (num == -1)
        cout<<"\nRead unsuccessfull!!!";
        else 
         {  cout<<"\nRead successfull!!!";
            cout<<"\nPipe contents: "<<buf;  //Displaying pipe contents
         }
     }
}

int main(int argc, char *argv[])    //Main function
{  umask(0000);
   int fd, ch, pfd[2], fd2;
   char *fileContent;
   
      cout<<"\n\nProvided file name is: "<<argv[1];
      cout<<"\nProvided file type is: "<<argv[2];
      char type[10], mode[10];
      strcpy(type, argv[2]);
            
   if (strcmp(type, "regular") == 0) //if file type is regular file
    { cout<<"\nEnter what to do??";
      cout<<"\n1.Create";
      cout<<"\n2.Read";
      cout<<"\n3.Write";
      cout<<"\n4.Info";
      cout<<"\nEnter choice(1-4): ";
      cin>>ch;    
      switch(ch)
      {   case 1:  cout<<"\nCreating file: "<<argv[1];  //Creating regular file
                   create_file(argv[1]);
                   break;
          case 2:  cout<<"\nReading file: "<<argv[1];  //Opening regular file
                   read_file(argv[1]);
                   break;
          case 3:  cout<<"\nWriting file: "<<argv[1]; //Reading from a regular file
                   write_file(argv[1]);
                   cout<<endl<<endl;
                   break;
          case 4:  cout<<"\nInfo about the file: "<<argv[1];  //Writing in file
                   info_file(argv[1]);
                   break;
          
      }
    }
    
    else if(strcmp(type, "npipe") == 0)  //if filetype is named pipe
       {     cout<<"\nCreating pipe: "<<argv[1];
             mknod(argv[1], 010777, 0);  //Creating pipe
             if (fd == -1)
              {  cout<<"\nFailed to create a named pipe!!\n";
                 exit(1);
              }
             else cout<<"\nPipe created successfully!!\n";
             
            
              cout<<"\nWant to write in pipe?(y/n):";   //Writing to pipe
              char ans;
              cin>>ans;    
              if(ans =='y')
               {   write_npipe(argv[1]);
                   cout<<"\nWant to read from pipe?(y/n):";   //Reading from pipe
                   cin>>ans;
                   if (ans == 'y')
                    read_npipe(argv[1]);
                   else return 0;
                   cout<<"\nWant info about the file?(y/n):";   //Info about the pipe
                   cin>>ans;
                   if (ans == 'y')
                    info_file(argv[1]);
                   else return 0;
                   
               }
              
               else 
               close(fd2);
               cout<<"\nPipe closed after all operations!\n";
             
             return 0;
         
         }
        
        
       else if(strcmp(type, "pipe") == 0) //If filetype is unnamed pipe
         {       cout<<"\nYou are creating a unnamed pipe therefore all the operations will be performed on it after creation as an unnamed pipe gets deleted once it is used!!";
                 int pfd[2];
                 cout<<"\nCreating pipe: "<<argv[1];  //Creating pipe 
                 pipe(pfd);
                 if (fd2 == -1)
                  {  cout<<"\nFailed to create a pipe!!\n";
                     exit(1);
                  }
                 else cout<<"\nPipe created successfully!!";
                 
                 write_pipe(argv[1], pfd[1]);
                 read_pipe(argv[1], pfd[0]);
                 return 0;
         }            
            
      return 0;
}     
