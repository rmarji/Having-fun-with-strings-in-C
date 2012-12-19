
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>

int main (int argc, char *argv[])
{
    DIR * d;
    char * dir_name = argv[1];
	int pid;
	
    /* Open the current directory. */

    d = opendir (dir_name);

    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {
        struct dirent * entry;
        int count=0;
        
        entry = readdir (d);
        if (! entry) {
            break;
        }
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)//Ignore root & parent directories
            continue;
        printf ("%s\n", entry->d_name);
        char cmd[1000] = "grep -c main ";
        char *cmd2 = (char*)entry->d_name;
        strcat(cmd, cmd2);

//       if ( execl("/bin/sh" , "sh " , "-c" ,  cmd , (char *)0) == -1 )
	   
	   pid = fork(); 
	  
	   if(pid == -1) printf("fork() Failed !!\n"); 
	   
	   else if(pid == 0) 
	   {     // Child
		  //printf("Im in child\n");	
		  //system(cmd);
		  count  = getCount(entry->d_name,argv[2]);
		  printf("count is :: %d \n " , count);
		  break;
	   }
	   
	   else                  // Parent
	   {
		   	//printf("Im in parent before wait\n"); 
			wait(NULL);
			//printf("\nIm in parent after wait\n"); 
			
		  
		  
	   }
	}
	if (pid != 0)
	{
		
        	int who = RUSAGE_SELF;
			struct rusage usage;
			int ret;
			ret = getrusage(who, &usage);
			printf("\nthis is the time in scnds %ld.%06ld\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);  
	}
    
    	
       
    
    /* Close the directory. */
    if (closedir (d)) {
        fprintf (stderr, "Could not close '%s': %s\n", dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }

    return 0;
}

int getCount(char* d_nam,char* subStr)
{
	FILE *fp;
    char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char dname [1234]="./";
	char *result; 
	int count=0;
	
	strcat (dname,d_nam);
	
    fp = fopen(dname, "r");
    if (fp == NULL)
    	exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
    	//printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
        
        result = volnitsky(line, read, subStr, strlen(subStr));
	
		if(result)
		{
			*(result + 60) = '\0';
			count++;
			//printf("count is : and the result is :%s"  , result );
		} else {
			//printf("Substring not found.\n");
		}
		
		
		
		/*result = strstr( line, subStr );
	
		if (result == NULL)
		puts( "String not found" );
		else
		{
			count++;
			printf("count is :%d and the result is :%s" , count , result );
		}
		*/
		
		/*
		result = BoyerMoore( line, read, subStr, strlen(subStr) );
	
		if (result == NULL)
		puts( "String not found" );
		else
		{
			count++;
			printf("count is :%d and the result is :%s" , count , result );
		}
		*/
    }

    free(line);  
    return count;
}




