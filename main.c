/******************************************************************************
 *
 *  File Name........: main.c
 *
 *  Description......: shell 
 *
 *  Author...........: Manojkumar Muralidharan
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/resource.h>
#include "parse.h"

extern char **environ;
int change_priority=0;
    int fd1[2];
  int fd2[2];
  	char **paths;
	char *path;
	int gpid;
 static char *host = "my$hell";
	int priority=4;
	int child_stat=1;
int exit_from_loop=0;
 void sigquit_hnd(int sig){ 
	 //(void)sig; 
	 signal(sig, SIG_IGN);
	 printf("\r\n");
	 fflush(STDIN_FILENO);
	 //fflush(STDOUT_FILENO);
	 //fopen(STDOUT_FILENO);
	 //setlinebuf(STDOUT_FILENO);
	 printf("%s%% ",host);
	 fflush(STDIN_FILENO);
	 signal(sig, sigquit_hnd);
}
 void sigint_hnd(int sig){ 
	 //(void)sig; 
	 //signal(sig, SIG_IGN);
	 
	 if(child_stat==1){
	printf("\r\n");	
	printf("%s%% ",host);
	fflush(STDIN_FILENO);
	}else{
	printf("\r\n");	
	//printf("%s%%",host);
	fflush(STDIN_FILENO);
	}

	 //fflush(STDOUT_FILENO);
	 //fopen(STDOUT_FILENO);
	 //setlinebuf(STDOUT_FILENO);
	 //printf("%s%%-",host);
	 //fflush(STDIN_FILENO);
	 //signal(sig, sigint_hnd);
}

void sigterm_hnd(int sig)
{
	signal(sig, SIG_IGN);
	
    signal(SIGTERM,SIG_IGN);
    // printf("goign the call to itself");
     //   }
    killpg(getpgrp(),SIGTERM);
    signal(sig, sigterm_hnd);
    exit(0);
}

void setpath(char *path){
char **env;

for (env = environ; *env!=NULL; ++env){
					
				  	if(strncmp(*env,"PATH",4)==0 && (*env)[4]=='='){
				  		strcpy(path,*env+5);
						break;
				  	}
				  			  
				}

}
void getpaths (char **paths1,char *path1){
int i,j,lastfound;
// remoevd pathnos_p = 0

for(i=0,j=0,lastfound=0;1;i++){
					if(path[i]==':'||path[i]=='\0'){
							
						j++;

														
						if(path[i]=='\0')
						break;

					}

				}
		
paths=malloc(sizeof(char*)*j);

for(i=0,j=0,lastfound=0;1;i++){
					if(path[i]==':'||path[i]=='\0'){
						paths[j]=malloc(sizeof(char)*i);
					
						if(lastfound==0)
						paths[j++]=strndup(path+lastfound,i);
						else
						paths[j++]=strndup(path+lastfound+1,i-lastfound-1);

						lastfound=i;
										
						if(path[i]=='\0')
						break;

					}

				}
//return paths;
}

void commands(Cmd c,int input,int output){
	int i,pid,pathnos,j,cmd_exist;
	char cwdir_cld[1000];
	char *command;
	//char *path;
	//char **paths;
	char **env,**temp_env;
	char *searchenv;
	char *newenv;
	int len;
	int env_found;
	int backup;
	int infile,outfile;
	/*for ( i = 1; c->args[i] != '\0'; i++ );
      c->args[i]=(char*)0;*/
      backup=dup(STDOUT_FILENO);

      if ( c->out != Tnil )
	        if(c->out == Tout) {
			//printf(">(%s) ", c->outfile);
			outfile=open(c->outfile, O_WRONLY|O_CREAT|O_TRUNC, 0660);
			if(outfile<0){
				perror("File cannot be created");
				exit(1);
			}
			if(dup2(outfile,output)<0){
				perror("dup2 Failed");
				exit(-1);
			}

		}else if(c->out == Tapp){
			outfile=open(c->outfile, O_CREAT|O_APPEND|O_WRONLY, 0660); // need to create a file or not 
			if(outfile<0){
				perror("File cannot be created");
				exit(1);
			}
			if(dup2(outfile,output)<0){
				perror("dup2 Failed");
				exit(-1);
			}

		}
		else if(c->out == ToutErr){
			outfile=open(c->outfile, O_CREAT|O_APPEND|O_WRONLY, 0660); // need to create a file or not 
			if(outfile<0){
				perror("File cannot be created");
				exit(1);
			}
			if(dup2(outfile,output)<0){
				perror("dup2 Failed");
				exit(-1);
			}
			if(dup2(outfile,STDERR_FILENO)<0){
				perror("dup2 Failed");
				exit(-1);
			}

		}


    // Where implementation
    if(output!=STDOUT_FILENO){
			//if(output==fd1[1])
				//close(output-1);
			//if(output==fd2[1])
			//	close(output-1);
		//	printf("comes in output change");
		if(dup2(output,STDOUT_FILENO)<0){
				perror("dup2++ Failed");
				exit(-1);
			}
	}

	if(strcmp(c->args[0],"where")==0){
	
		if(c->args[1]!=NULL){
			path=(char*)malloc(200);
			setpath(path);
			char *temp = path;
			while(strchr(path,':')!=NULL){
			pathnos++;
			path = strchr(path,':')+1;
			}
			pathnos++;//for last path
			path = temp;
			paths = malloc(sizeof(char*) * pathnos);
			
			getpaths(paths,path);
				/*
		for(j=0;paths[j]!=NULL;j++){
			printf("\n%s",paths[j]);
		}*/
		j=0;
		int l;
			for(l=1;c->args[l]!=NULL;l++){
			printf("%s:",c->args[l]);
			while(paths[j]!=NULL){
					
					command=malloc(sizeof(char)*(strlen(paths[j])+sizeof(c->args[0]))+1);
					strcpy(command,paths[j++]);
					strcat(command,"/");
					strcat(command,c->args[l]);
					//printf("\ncommand = %s\n",command);
					if((cmd_exist=open(command, O_RDONLY,0660))>0){
					printf(" %s",command);
					}
				
				}

			printf("\n");
			j=0;
			}
		}
		else{
			printf("\nwhere [command]\n");
		}
		return;
		
	}
	
	

    //PWD inplementation
	 if(strcmp(c->args[0],"pwd")==0){
		if(getcwd(cwdir_cld, 1000)!=NULL)
		printf("%s\n",cwdir_cld);
	}

    //Change Directory
    if(strcmp(c->args[0],"cd")==0){
    	 if(!c->args[1]){
    	if(chdir("/home")<0){
    		printf("cd: home: No such file or directory\n");
    	}
    	//if(getcwd(cwdir_cld, 1000)!=NULL)
    	 //printf("Current working directory of the child is : %s\n",cwdir_cld);
		}else{
		 if(chdir(c->args[1])<0){
		 	printf("cd: %s: No such file or directory\n",c->args[1]);	
		 }
		 //if(getcwd(cwdir_cld, 1000)!=NULL)
    	 //printf("Current working directory of the child is : %s\n",cwdir_cld);

		}
    }
    
    //Echo Implementation
    if(strcmp(c->args[0],"echo")==0){
    	for(i=1;c->args[i]!=NULL;i++){
	    	//Check for $ENV_Variables
	    	/*if(c->args[i][0]=='$')	{
	    			
	    			searchenv=c->args[i];
	    			searchenv++;
	    			len=strlen(c->args[i])-1;
	    			
	    			for(env=environ;*env!=NULL;++env){
	    		    	if (strncmp(*env,searchenv, len) == 0 && (*env)[len] == '=') {
	    				printf("%s ",*env+len+1);
	    				}
	    			}
	    	
	    	}else*/
	    	printf("%s ",c->args[i]);

    	}
    	printf("\n");
	}

	 //Logout Implemenatation
	 if(strcmp(c->args[0],"logout")==0){
	 	exit(0);
	 }

	 //List environment vriable
	 if(strcmp(c->args[0],"setenv")==0){
	 	if(c->args[1]==NULL){
		  for (env = environ; *env!=NULL; ++env){
			  	printf("%s\n", *env);	
			  
			}
		}
		else if(c->args[2]==NULL){
			env_found=0;
			for (env = environ; *env!=NULL; ++env){
				len=strlen(c->args[1]);
			  	if(strncmp(*env,c->args[1],len)==0 && (*env)[len]=='='){
			  		env_found=1;
			  		break;
			  	}
			  			  
			}
			newenv=malloc(strlen(c->args[1])+2);
			strcpy(newenv,c->args[1]);
			strcat(newenv,"=");
				if(env_found==0){
					putenv(newenv);	
				}else{
					for (env = environ; *env!=NULL; ++env){
						len=strlen(c->args[1]);
					  	if(strncmp(*env,c->args[1],len)==0 && (*env)[len]=='='){
					  		*env=newenv;
					  		break;
					  	}
					  			  
					}
			}
				

			
		}else{
			newenv=malloc(strlen(c->args[1])+2+strlen(c->args[2]));
			strcpy(newenv,c->args[1]);
			strcat(newenv,"=");
			strcat(newenv,c->args[2]);
			env_found=0;
			len=strlen(c->args[1]);
			for (env = environ; *env!=NULL; ++env){
				if(strncmp(*env,c->args[1],len)==0 && (*env)[len]=='='){
			  		env_found=1;
			  		break;
			  	}
			  			  
			}
			if(env_found==0){
				putenv(newenv);	
			}else{
				len=strlen(c->args[1]);
				for (env = environ; *env!=NULL; ++env){
					
				  	if(strncmp(*env,c->args[1],len)==0 && (*env)[len]=='='){
				  		*env=newenv;
				  		break;
				  	}
				  			  
				}
			}
		}

    }

    //unsetenv Implmentation
     if(strcmp(c->args[0],"unsetenv")==0){
     	if(c->args[1]==NULL)
     	printf("\n No Arguments specified \n");
     	else{
     		len=strlen(c->args[1]);
     		for(env=environ;*env!=NULL;++env){
     			if(strncmp(*env,c->args[1],len)==0 && (*env)[len]=='='){
     				temp_env=*env;
     				for(;*env!=NULL;env++){
     				*env=*(env+1);	
     				}
     				
     				env=temp_env;

     			}
     		}

     	}

     }

     //nice implementation
     if(strcmp(c->args[0],"nice")==0){
     	int which = PRIO_PROCESS;
		id_t pid;
		int ret_value;
		int nice_pid;

		char *custom_priority;
		char *temp_priority;

		pid=getpid();
     	//printf("\n nargs=%d\n",c->nargs);
     	if(c->nargs==1){
     		ret_value=setpriority(which,pid,4);
     	}
     	if(c->nargs==2){
     		custom_priority=malloc(sizeof(char)*strlen(c->args[1]));
     		temp_priority=malloc(sizeof(char)*strlen(c->args[1]));

       		/*strcpy(temp_priority,c->args[1]);
     		if(c->args[1][0]!='+'){
     			printf("\nsyntax: nice [+ number] [command]\n");
     			return;
     		}*/
     		//strcpy(custom_priority,c->args[1]+1);
     		/*if(!isnumber(c->args[1])){
     			printf("yes");
     		}*/
     		priority=atoi(c->args[1]);
     		if(priority==0){
     			//printf("\n is digit=%d",isdigit(c->args[1][0]));
     			if(isdigit(c->args[1][0])){
     				//printf("yes it is a Zero");
     			}
     			else{
     				//printf("not a zero");
	     			nice_pid=fork();
		     		if(nice_pid<0){
		     			perror("\nCannot Fork\n");
		     			exit(0);
		     		}else if(nice_pid==0){
		     			ret_value=setpriority(which,getpid(),4);
		     			if(ret_value<0){
		     			perror("nice Failed-");
		     			exit(0);
		     			}
		     			execvp(c->args[1],&c->args[1]);

		     		}else{
		     			priority=4;
		     			nice_pid=0;
		     			wait(NULL);
		     			//printf("\n");
		     			//return;		
		     		}
     			}
     		}
     		//printf("\npri=%d,%s\n",priority,custom_priority);
     		if(priority<0){
     			printf("\nsyntax: nice [+ number] [command]\n");
     			return;
     		}
     		//printf("\npri=%d,%s\n",priority,custom_priority);
     		ret_value=setpriority(which,pid,priority);
     			priority=4;
     	}
     	if(c->nargs==3){
     		
     		priority=atoi(c->args[1]);
     		//printf("\npri=%d\n",priority);
     		if(priority<0){
     			printf("\nsyntax: nice [+ number] [command]\n");
     			return;
     		}
     		
     		nice_pid=fork();
	     		if(nice_pid<0){
	     			perror("\nCannot Fork\n");
	     			exit(0);
	     		}else if(nice_pid==0){
	     			ret_value=setpriority(which,getpid(),priority);
	     			if(ret_value<0){
	     			perror("nice Failed-");
	     			exit(0);
	     			}
	     			execvp(c->args[2],&c->args[2]);

	     		}else{
	     			priority=4;
	     			nice_pid=0;
	     			wait(NULL);
	     			//printf("\n");
	     			//return;		
	     		}
     		//cmd
     	    //redirection(c,input,output);
     		
     	}
     	if(ret_value<0){
     		perror("nice Failed");
     		exit(0);
     	}

     }

/*     if(output!=STDOUT_FILENO){
			//if(output==fd1[1])
				//close(output-1);
				close(output);
	
	
	}*/
	//close(output);
	
	if(dup2(backup,STDOUT_FILENO)<0){
				perror("dup2++ Failed");
				exit(-1);
			}



    
}
static void prPipe(Pipe p)
{
  int i = 0;
  int pid;

  int pipeflag;
  int input;
  int output;
  char reread;
  int backup_input,backup_output;
  input=STDIN_FILENO;
  output=STDOUT_FILENO;
	char line[100];
  Cmd c;

  int builtin;

  if ( p == NULL )
    return;

backup_input=dup(STDIN_FILENO);
backup_output=dup(STDOUT_FILENO);
//printf("hi");
pipeflag=0;

        if(p->head->next!=NULL)
	pipeflag=1;
	else
	pipeflag=0;

int count=0;
pipe(fd1);
pipe(fd2);
 
 // printf("Begin pipe%s\n", p->type == Pout ? "" : " Error");
  for ( c = p->head; c != NULL; c = c->next ) {
  	if(exit_from_loop==1){
  	exit_from_loop=0;
  	break;
  	}
   // printf("  Cmd #%d: ", ++i);
   if(c->next!=NULL)
	pipeflag=1;
	else
	pipeflag=0;

if(pipeflag==1){
	if(count==0){
	pipe(fd1);
	output=fd1[1];}
	else{
		pipe(fd2);
	output=fd2[1];}
	
}else{
	output=STDOUT_FILENO;
	//input=STDIN_FILENO;

}

	//printf("\nOUT=%d",STDIN_FILENO);
	//printf("\nIN=%d",STDOUT_FILENO);


if((strcmp(c->args[0],"cd")==0||strcmp(c->args[0],"echo")==0||strcmp(c->args[0],"setenv")==0||strcmp(c->args[0],"unsetenv")==0||strcmp(c->args[0],"logout")==0)||strcmp(c->args[0],"pwd")==0||strcmp(c->args[0],"where")==0||strcmp(c->args[0],"nice")==0){

if(pipeflag==1){
	//builtin=fork();
	//if(builtin<0){
	//	perror("cannot fork");
//		exit(0);//
//	}else if(builtin==0){
	commands(c,input,output);
//	exit(0);	
//	}
//	else if(builtin>0){
		//returned to parent
	//}
	
}else{
	commands(c,STDIN_FILENO,STDOUT_FILENO);
   }

}
else
    redirection(c,input,output);

	if(c->next!=NULL)
	pipeflag=1;
	else
	pipeflag=0;

if(pipeflag==1){
	if(count==0){
	input=fd1[0];
	count++;
	output=STDOUT_FILENO;	
	}
	else{
	input=fd2[0];
	count--;
	output=STDOUT_FILENO;
	}
}else
{
	output=STDOUT_FILENO;
	//input=STDIN_FILENO;
}



  }

 
 // printf("End pipe\n");
  prPipe(p->next);

}






void redirection(Cmd c,int input,int output){
	int outfile,infile,cmd_exist,pathnos,lastfound;
	int pid;
	//char *args1[]={"grep", "Villanova", NULL};
	char **argslist;

	//char paths[50][100];
	int listsize;
	int pathflags,i,j=0;
	char *command;

	if ( c ) {

	for(listsize=0;c->args[listsize]!=NULL;listsize++);
	
	argslist=malloc(listsize*sizeof(char*));

	//For USHRC Fix-----//
	for(listsize=0;c->args[listsize]!=NULL;listsize++){
	argslist[listsize]=NULL;	
	}
	argslist[listsize]=NULL;
	//For USHRC Fix-----//


	for(listsize=0;c->args[listsize]!=NULL;listsize++){
	argslist[listsize]=malloc(strlen(c->args[listsize])+1);	
	strcpy(argslist[listsize],c->args[listsize]);
	}

	/*
	printf("\n c->args\n");
	for(listsize=0;c->args[listsize]!=NULL;listsize++)
	printf("%s\t",c->args[listsize]);

	printf("\n argslist\n");
	for(listsize=0;argslist[listsize]!=NULL;listsize++)
	printf("%s",argslist[listsize]);
	*/
	//printf("\nin=%d out=%d\n",STDIN_FILENO,STDOUT_FILENO);
	//fflush(1);
	//printf("\nin=%d out=%d\n",input,output);

	pid=fork();
    
    if(pid<0){
		
		printf("\n Cannot Fork \n");
		
	}else if(pid==0){


		setpgid(0,getppid());
		gpid=getpgrp();
			
			if(output!=STDOUT_FILENO){
			//if(output==fd1[1])
				close(output-1);
			//if(output==fd2[1])
			//	close(output-1);
		//	printf("comes in output change");
		if(dup2(output,STDOUT_FILENO)<0){
				perror("dup2++ Failed");
				exit(-1);
			}
		}
		//close(output);


		if(input!=STDIN_FILENO){
			//if(input==fd1[0])
				close(input+1);
			//if(input==fd2[0])
			//	close(input+1);
		//	printf("comes in input change");
		if(dup2(input,STDIN_FILENO)<0){
				//perror("%d",input);
				perror("dup2-- Failed1++");
				exit(-1);
			}
		}
              /*  int fd[2];
		pipe(fd);*/

		//close(input);
       
		if(c->in == Tin){
			infile=open(c->infile, O_RDONLY, 0660);
			if(infile<0){
				printf("\nFile open error\n %d",infile);
			}
			else{
				if(dup2(infile,input)<0){
				perror("dup2 Failed");
				exit(-1);
				}
				
			}
		}
		if ( c->out != Tnil )
	        if(c->out == Tout) {
			//printf(">(%s) ", c->outfile);
			outfile=open(c->outfile, O_WRONLY|O_CREAT|O_TRUNC, 0660);
			if(outfile<0){
				perror("File cannot be created");
				exit(1);
			}
			if(dup2(outfile,output)<0){
				perror("dup2 Failed");
				exit(-1);
			}

		}else if(c->out == Tapp){
			outfile=open(c->outfile, O_CREAT|O_APPEND|O_WRONLY, 0660); // need to create a file or not 
			if(outfile<0){
				perror("File cannot be created");
				exit(1);
			}
			if(dup2(outfile,output)<0){
				perror("dup2 Failed");
				exit(-1);
			}

		}
		else if(c->out == ToutErr){
			outfile=open(c->outfile, O_CREAT|O_APPEND|O_WRONLY, 0660); // need to create a file or not 
			if(outfile<0){
				perror("File cannot be created");
				exit(1);
			}
			if(dup2(outfile,output)<0){
				perror("dup2 Failed");
				exit(-1);
			}
			if(dup2(outfile,STDERR_FILENO)<0){
				perror("dup2 Failed");
				exit(-1);
			}

		}
		//printf("Inside child process");
		command=malloc(sizeof(char)*(sizeof(5)+sizeof(c->args[0]))+1);

		//set path 
		for(i=0;c->args[0][i]!='\0';i++){
			if(c->args[0][i]=='/'){
				pathflags=1;
				break;
			}
		}
		if(pathflags==1){
			if(c->args[0][0]=='/'){
				strcpy(command,c->args[0]);
			}
			else if(c->args[0][0]=='.'){
				strcpy(command,c->args[0]);
			}else if(c->args[0][0]!='/'&&c->args[0][0]!='.'){
				strcpy(command,c->args[0]);

			}
		}else{
			path=(char*)malloc(200);
			setpath(path);
			char *temp = path;
			while(strchr(path,':')!=NULL){
			pathnos++;
			path = strchr(path,':')+1;
			}
			pathnos++;//for last path
			path = temp;
			paths = (char*)malloc(sizeof(char*) * pathnos);
			//search the path variable
			/*for(i=0,pathnos=0;i<=strlen(c->args[0])-1;i++){
					if(c->args[0][i]==':'){
						//pathnos
						pathnos++;
						break;
					}
				}*/
			getpaths(paths,path);
			
			while(paths[j]!=NULL){
				
				command=malloc(sizeof(char)*(strlen(paths[j])+sizeof(c->args[0]))+1);
				strcpy(command,paths[j++]);
				strcat(command,"/");
				strcat(command,c->args[0]);
				//printf("\ncommand = %s, size = %d\n",command,strlen(c->args[0]));
				if((cmd_exist=open(command, O_RDONLY,0660))>=0){
				//printf("\ncommand inside = %s\ncmd=%d/n",command,cmd_exist);
				close(cmd_exist);
				break;
				}
				
			}
		
		}





		if((cmd_exist=open(command, O_RDONLY,0660))<0){
			//printf("\n%s\n",command);
			printf("%s: command cannot be found...\n",c->args[0]);
			exit(-1);
			return ;

		}

		if(execv(command,argslist)<0){
			printf("-1");
			perror("command cannot be found...\n");
			exit(-1);
		}
	}else{

		if(output!=STDOUT_FILENO){
			//close(output);
		if(dup2(STDOUT_FILENO,output)<0){
				perror("dup2++ Failed");
				exit(-1);
			}
		}

		if(input!=STDIN_FILENO){
			if(input==fd1[0]){
				//pipe(fd2);
				close(fd1[1]);}
				//close(input+1);
			if(input==fd2[0]){
				//pipe(fd1);
				//close(fd2[0]);
				close(fd2[1]);

			}
			//printf("works2");
				//close(input+1);
		//	printf("comes in input change");
		//close(input);
		if(dup2(STDIN_FILENO,input)<0){

				perror("dup2-- Failed2");
				exit(-1);
			}
		}
		//close(input);
		//close(input+1);
		//close(output);
		//wait(NULL);
		int exit_status;
		waitpid(pid,&exit_status,NULL);
		if(WIFEXITED(exit_status)==1){
			//printf("++%d",WEXITSTATUS(exit_status));
			if(WEXITSTATUS(exit_status)<0){
				printf("Unknown command in Pipe\n");
				exit_from_loop=1;
			}

		}
		//printf("Child ended");
		return;
	}

}
}


void init_signals(void){
	
//	char *ush_env=getenv("ush_env");/
//	if(ush_env==NULL){
	setpgid(0,0);
//    gpid=getpgrp();
 //   setenv("ush_env","1",1);
//	}
}


int main(int argc, char *argv[])
{
  Pipe p;
signal( SIGQUIT, sigquit_hnd);
//signal( SIGQUIT, SIG_IGN );
//signal( SIGINT,  SIG_IGN );
signal( SIGINT,  sigint_hnd );
signal( SIGTERM, SIG_IGN);
//signal( SIGTERM, sigterm_hnd);
int f_intemp,f_outtemp,rcfile;

int backup_input=0,backup_output=0;
backup_input=dup(STDIN_FILENO);
backup_output=dup(STDOUT_FILENO);

//printf("\nin=%d out=%d\n",STDIN_FILENO,STDOUT_FILENO);
//f_intemp=dup(STDIN_FILENO);
//printf("\nin=%d out=%d\n",STDIN_FILENO,STDOUT_FILENO);
const char *name = "HOME";
char *value=malloc(sizeof(char*)*strlen(getenv("HOME")));


strcpy(value,getenv(name));

strcat(value,"/");
strcat(value,".ushrc");

rcfile=open(value, O_RDONLY, 0600);

if(rcfile<0){
printf("Could not open ushrc\n");
}
else{


	//f_outtemp=dup(STDIN_FILENO);
	if(dup2(rcfile,STDIN_FILENO)<0){
	perror("Parsing ushrc Failed\n");
	//exit(0);
	}else{

		p=parse();
		char *end="end";

		if(p!=NULL){
			while(strcmp(p->head->args[0],end)!=0){
			
				prPipe(p);
				freePipe(p);
				p=parse();
				if(p==NULL)
				break;
				//printf("");
			}	
		}
		//close(STDIN_FILENO);
		close(rcfile);
		//int rcfile1;
		//rcfile1=open("/home/rockydj/os2/ushrc1", O_RDONLY, 0600);
		//printf("%d",rcfile1);
		if(dup2(backup_input,STDIN_FILENO)<0){
			perror("dup2 Failed");
			exit(0);
		}
	}
	//printf("\n%dEnter any number %% ",STDIN_FILENO);
	//printf("\n%s%%\n",host);
}
int temp;
//scanf("%d",&temp);
//fflush(STDIN_FILENO);
//printf("\n");
//fflush(STDOUT_FILENO);
//signal( SIGKILL, sigterm_handler);

init_signals();

//printf("%d my process is ",getpid());
  while ( 1 ) {
  	child_stat=1;
    printf("%s%% ", host);
    fflush(stdout);
    p = parse();
    child_stat=0;
    prPipe(p);
    freePipe(p);
  }
}

/*........................ end of main.c ....................................*/



