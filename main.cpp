#include <zip.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include "stdio.h"
/*//////////////////////////////////
   BUILD MACHINE 
          NAME="Ubuntu"
          VERSION=20.04.1 LTS 
   Text editor used - Visual Studio Code
   
  PreRequesties
   Install libzip library 
     - sudo apt-get install -y libzip-dev
     - https://libzip.org
  Compile command 
     - g++ main.cpp -o main.exe  -lzip
  Input method
     - as comand line argument
       - ./main.exe <Source File Path>
       or
     - as input
       Enter the Source File path
      -<Ente the file path>

////////////////////////////////*/

int main(int argc, char *argv[])
{
    
    char FilePath[255];  //storing the file name
    memset(FilePath,0,255);  //memet the memory with value 0
    struct zip_stat sb;  // declare the file information structure

    if(argc>1)
    {
      //copyin the source file from argument
      int pathlen =strlen(argv[1]);
      memcpy(FilePath,argv[1],pathlen);
    } 
    else
    {
      printf("Enter the Source File path");
      fgets(FilePath,255,stdin);
      //geting the source file from user input
    }

    printf("File path : %s\n",FilePath);


    int err = 0;

    //Open the ZIP archive 
    zip *z = zip_open(FilePath, 0, &err);
    
  
     if(z==NULL)
     {
        printf("file open failed");
        return 0;
     }
     

      printf("Unzipping started .....\n");
      for (int i = 0; i < zip_get_num_entries(z, 0); i++) 
      {
        if (zip_stat_index(z, i, 0, &sb) == 0) 
        {
            ///////////////////////// printing the file information
            int len = strlen(sb.name);
            printf("\n Name: [%s], ", sb.name);
            printf("Size: [%llu], ",(long long unsigned int) sb.size);
            printf("mtime: [%u] \n", (unsigned int)sb.mtime);
            ////////////////////////
            if(sb.name[len-1]=='/')
            {
              // creating the folder when the file type is folder
              mkdir(sb.name,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

            }
            else
            {
            
                 char contents[1025];
                 zip_file *f = zip_fopen(z, sb.name, 0);         //open file        
                 int readByte=0;
                 FILE * DestFile=fopen(sb.name,"w");  //  open destination file
                 while(readByte<sb.size)
                 {
                   
                   int dataread= zip_fread(f, contents, 1024);  // read data from source
                  
                   fwrite(contents,sizeof(char),dataread, DestFile); //write the data to destination file
                    
                    readByte+=1024;
                 }
                 zip_fclose(f);  //close the source handle
                 fclose(DestFile); //close destination file handle
            }
            
        }
      }
    printf("Unzipping Completed .....\n");
  
    zip_close(z); //closing the file handle
   
}
