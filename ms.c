
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <time.h>  
#include <string.h> 


int main(int argc, char const *argv[]) 
{
	int PORT=atoi(argv[1]); 
	int server_fd, new_socket, valread,valread1; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	}
	char *addu="ADDU ";
	char *lstu="LSTU";
	char *setu="USER ";
	char *quit="QUIT";
	char wrong[1024]="Wrong";
	char readm[1024]="READM";
	char doneu[1024]="DONEU";
	char ssend[1024]="SEND ";
	char delm[1024]="DELM";
	while(1)
	{
		valread = read( new_socket , buffer, 1024);
		//printf("%s\n",buffer); 
		if(strncmp(buffer,addu,5)==0)
		{
			if(strlen(buffer)>5)
			{	
				char str3[1024];
				char buff[1024];
				char temp[1024];
				strcpy(str3,buffer+5);
				FILE *ptr;
				if ((ptr = fopen(str3,"r")) == NULL)
				{

       					ptr = fopen(str3,"w");	
					fclose(ptr);
					FILE * list;
					list=fopen("list.txt","a");
					strcpy(temp,str3);
					fputs(strcat(temp,"\n"),list);
					strcat(str3," has been added as User\n");
					write(new_socket ,str3 , strlen(str3) );
					fclose(list); 
					
   				}
				else
				{
					strcat(str3," already exists\n");
					write(new_socket ,str3, strlen(str3)); 
				}
				memset(temp, 0, strlen(temp));
				memset(str3, 0, strlen(str3));
				memset(buff, 0, strlen(buff));
			}
			else
			{
				write(new_socket , "enter a UserID after Adduser\n", strlen("enter a UserID after Adduser\n") ); 
			}
		}
		else if(strncmp(buffer,lstu,4)==0)
		{
			char str[1024]={0};
			char buff[1024]={0};
			FILE *f1 = fopen("list.txt", "r"); 
			if(f1==NULL)
			{
				//printf("after if\n");	
				write(new_socket , "\n\0", strlen("\n\0") );

			}
			else
			{
				fclose(f1);
				FILE *f = fopen("list.txt", "r");
		    		while (fgets(buff, 1024, f) != NULL)
				{
					strcat(str,buff);
				}
				write(new_socket , str, strlen(str) );
				fclose(f);
				memset(buff, 0, strlen(buff));
			}
			
		}
     		else if(strncmp(buffer,setu,5)==0)
		{
			char str[1024]={0};
			char buff[1024];
			char ch;
			strcpy(str,buffer+5);
			FILE *ptr;
			if ((ptr = fopen(str,"r")) == NULL)
			{
				strcpy(buff,wrong);
				write(new_socket ,buff, sizeof(buff));
				memset(buff, 0, strlen(buff));	
				memset(str, 0, strlen(str));
				memset(buffer, 0, strlen(buffer));
				continue;
			}
			else
			{
				int count=0;
				//printf("serverside in else before while");
				while (fgets(buff, sizeof(buff), ptr)) 
				{
					for(int i=0;i<strlen(buff);i++)
					{
						if(buff[i]=='#' && i<=(strlen(buff)-3))
						{
							if(buff[i+1]=='#' && buff[i+2]=='#')
							{
								count=count+1;
								break;		
							}	
						}			
					}
			    	}
				//printf("serverside in else after while");
				char temp[1024]={0};
				char temp1[1024]={0};
				sprintf(temp, "%d", count);
				strcat(temp1,"The user ");
				strcat(temp1,str);
				strcat(temp1," exists and has ");
				strcat(temp1,temp);
				strcat(temp1," messages in their spool file.");
				write(new_socket ,temp1, strlen(temp1));
				memset(temp, 0, strlen(temp));	
				memset(temp1, 0, strlen(temp1));
				count=0; 		
				fclose(ptr);
				FILE *rea;
				int reaco=0;
				rea = fopen(str,"r");
				while(1)
				{
					char res[1024]={0};
					valread1 = read( new_socket , res, 1024);
					if(strcmp(res,doneu)==0)
					{
						char ret[1024]={0};
						strcat(ret,"Closing the private session of ");
						strcat(ret,str);
						write(new_socket ,ret, strlen(ret));
						memset(ret, 0, strlen(ret));	
						memset(res, 0, strlen(res));
						fclose(rea);
						break;
						
					}
					else if(strcmp(res,readm)==0)
					{
						char ret[1024]={0};	
						char buff1[1024]={0};
						int count2=0;
						//printf("serverside in else before while");
						FILE *fox;
						fox=fopen(str,"r");
						if(fgets(buff1, sizeof(buff1), fox)==NULL)
						{
							strcat(ret,"No more Mail");
							write(new_socket ,ret, strlen(ret));
							//fclose(fox);

						}
						else
						{
							
							memset(buff1, 0, strlen(buff1));
							if(fgets(buff1, sizeof(buff1), rea)==NULL)
							{
								fseek(rea, 0, SEEK_SET);
								reaco=0;
								fgets(buff1, sizeof(buff1), rea);
							}
							reaco++;
							do
							{
								strcat(ret,buff1);
								for(int i=0;i<strlen(buff1);i++)
								{
									
									if(buff1[i]=='#' && i<=(strlen(buff1)-3))
									{
										if(buff1[i+1]=='#' && buff1[i+2]=='#')
										{
											count2=count2+1;
											break;		
										}	
									}
												
								}
								if(count2!=0)
								{
									break;
								}
								reaco=reaco+1;
						    	}while (fgets(buff1, sizeof(buff1), rea)!=NULL);
							count2=0;
							
							//printf("%s\n",ret);
							//fflush(stdout);
							write(new_socket ,ret, strlen(ret));
							//printf("%d \n %s 1",xxx,ret);
							//printf("below write\n");
							//fflush(stdout);
						}
						fclose(fox);	
						memset(buff1, 0, strlen(buff1));	
						memset(ret, 0, strlen(ret));	
						memset(res, 0, strlen(res));			
					}
					else if(strcmp(res,delm)==0)
					{
						char ret[1024]={0};	
						char buff1[1024]={0};
						FILE * see;
						int seco=0;
						see=fopen(str,"r");
						FILE *fox;
						FILE *writer;
						writer=fopen("temp.txt","w");
						int sid=0;
						fox=fopen(str,"r");
						if(fgets(buff1, sizeof(buff1), fox)==NULL)
						{
							strcat(ret,"No more Mail");
							remove("temp.txt");
							write(new_socket ,ret, strlen(ret));
							fclose(fox);
							fclose(writer);
							fclose(see);
							memset(buff1, 0, strlen(buff1));	
							memset(ret, 0, strlen(ret));	
							memset(res, 0, strlen(res));
							continue;	
						}
						if(fgets(buff1, sizeof(buff1), rea)==NULL)
						{
							fseek(rea, 0, SEEK_SET);
							reaco=0;
						}
						else //to bring back pointer 1 step
						{
							fclose(rea);
							rea=fopen(str,"r");
							while(sid!=reaco)
							{
								sid++;
								fgets(buff1,sizeof(buff1),rea);
							}
						}
						
						memset(buff1, 0, strlen(buff1));	
						while(seco!=reaco)
						{
							//printf("%d %d\n",seco,reaco);
							fgets(buff1, sizeof(buff1), see);
							seco=seco+1;
							fprintf(writer, "%s",buff1);	
						}
						while(1)
						{
							//printf("%d %d\n",seco,reaco);
							fgets(buff1, sizeof(buff1), see);
							seco++;
							if(strstr(buff1,"###")!=NULL)
							{
								//printf("%d %d\n",seco,reaco);;
								break;	
							}
						}
						memset(buff1, 0, strlen(buff1));
						while(fgets(buff1, sizeof(buff1), see)!=NULL)
						{
							//printf("%d %d\n",seco,reaco);
							seco++;
							fprintf(writer, "%s",buff1);
						}
						
						//printf("out if everything\n");
						strcat(ret,"Message Deleted");
						write(new_socket ,ret, strlen(ret));
						fclose(writer);
						fclose(see);
						remove(str);
						rename("temp.txt",str);
						fclose(rea);
						rea=fopen(str,"r");
						sid=0;
						memset(buff1, 0, strlen(buff1));
						while(sid!=reaco)
						{
							sid++;
							fgets(buff1,sizeof(buff1),rea);
						}
						sid=0;
						
						fclose(fox);
						memset(buff1, 0, strlen(buff1));	
						memset(ret, 0, strlen(ret));	
						memset(res, 0, strlen(res));			
					}
					else if(strncmp(res,ssend,5)==0)
					{
						char ret[1024]={0};
						char buff1[1024]={0};
						strcpy(buff1,res+5);
						FILE * poi;
						char to[1024]={0};
						int i;
						for(i=0;i<strlen(buff1);i++)
						{
							//printf("in for\n");
							if(buff1[i]==' ' || buff1[i]=='\0')
							{
								to[i]='\0';
								break;							
														
							}	
							//printf("after if\n");
							to[i]=buff1[i];
						}
						FILE *check;
						if ((check = fopen(to,"r")) == NULL)
						{
							write(new_socket ,"invalid", strlen("invalid"));
							memset(ret, 0, strlen(ret));
							memset(buff1, 0, strlen(buff1));		
							memset(res, 0, strlen(res));
							memset(to, 0, strlen(to));
						}
						else
						{
							fclose(check);
							struct tm* local; 
						    	time_t t = time(NULL); 
						    	// Get the localtime 
						    	local = localtime(&t);
							char ttime[1024]={0}; 
							strcpy(ttime,asctime(local));
							if(ttime[8]==' ')
							{
								ttime[8]='0';
							}
							ttime[24]=ttime[20];
							ttime[25]=ttime[21];
							ttime[26]=ttime[22];
							ttime[27]=ttime[23];
							ttime[20]='I';
							ttime[21]='S';
							ttime[22]='T';
							ttime[23]=' ';
							
							ttime[28]='\n';
							ttime[29]='\0';
							//printf("%s\n",res);
							//printf("%s\n",buff1);
							//printf("{%s}\n",to);
							i=0;
							poi=fopen(to,"a");
							char mess[1024];
							strcpy(mess,res+5+strlen(to));
							//printf("%s\n",mess);
							char e[1024]={0};
							strcat(e,"From: ");
							strcat(e,str);
							strcat(e,"\n");
							strcat(e,"To: ");
							strcat(e,to);
							strcat(e,"\n");
							strcat(e,"Date: ");
							strcat(e,ttime);
							strcat(e,"Subject:");
							strcat(e,mess);
							//printf("%s\n",e);
							fputs(e,poi);
							fclose(poi);
							write(new_socket ,"valid", strlen("valid"));
							memset(to, 0, strlen(to));
							memset(e, 0, strlen(e));
							memset(ret, 0, strlen(ret));
							memset(buff1, 0, strlen(buff1));		
							memset(res, 0, strlen(res));
						}
					}
					
				}				
			}
			
			memset(buff, 0, strlen(buff));	
			memset(str, 0, strlen(str));
		}
		else if(strcmp(buffer,quit)==0)
		{
			write(new_socket , "Session ended\n", strlen("Session ended\n") );
			close(new_socket);
			
			break;
		}
		else
		{
			write(new_socket , "Enter a valid Command\n", strlen("Enter a valid Command\n") );
		}
		memset(buffer, 0, strlen(buffer));
	} 
	return 0; 
} 

