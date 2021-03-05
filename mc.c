#include <stdlib.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>  

int main(int argc, char const *argv[]) 
{ 	
	int PORT=atoi(argv[2]); 
	int sock = 0, valread,valread1; 
	struct sockaddr_in serv_addr; 

	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	}
	char *addu="Adduser ";
	char *lstu="Listusers";
	char *setu="SetUser ";
	char *quit="Quit";
	char ADDU[1024]="ADDU ";
	char LSTU[1024]="LSTU";
	char SETU[1024]="USER ";
	char done[1024]="Done";
	char wrong[1024]="Wrong";
	char read1[1024]="Read";
	char readm[1024]="READM";
	char doneu[1024]="DONEU";
	char send[1024]="Send ";
	char ssend[1024]="SEND ";
	char delm[1024]="DELM";
	char dele[1024]="Delete";
	while(1)
	{
		char n[1024];
	    	printf("Main-Prompt> ");
	    	scanf("%[^\n]%*c", n);
		if(strncmp(n,addu,8)==0)
		{	
				char str3[1024];
				char temp[1024];
				strcpy(temp,ADDU);
				strcpy(str3,n+8);
				strcat(temp,str3);
				write(sock ,temp , strlen(temp)); 
				valread = read( sock , buffer, 1024); 
				printf("%s\n",buffer);
				memset(str3, 0, strlen(str3));
				memset(temp, 0, strlen(temp));

		}
		else if(strncmp(n,lstu,9)==0)
		{	
				write(sock , LSTU , strlen(LSTU)); 
				valread = read( sock , buffer, 1024); 
				for(int i=0;i<strlen(buffer);i++)
				{
					if(buffer[i]=='\n')
					{
						buffer[i]=' ';
					}
				}
				printf("%s\n",buffer);
		}
		else if(strncmp(n,setu,8)==0)
		{	
				char str3[1024];
				char temp[1024];
				strcpy(temp,SETU);
				strcpy(str3,n+8);
				strcat(temp,str3);
				write(sock , temp , strlen(temp)); 
				valread = read( sock , buffer, 1024); 
				if(strcmp(buffer,wrong)==0)
				{
					printf("The specified user: %s does not exist\n",str3);
				}
				else
				{
					printf("%s\n",buffer);
					while(1)
					{
						printf("Sub-Prompt- %s> ",str3);
						char s[1024];
						scanf("%[^\n]%*c",s);
						if(strcmp(s,done)==0)
						{
							char res[1024]={0};
							write(sock , doneu , strlen(doneu));
							valread1 = read( sock , res, 1024);
							printf("%s\n",res);
							memset(res, 0, strlen(res));
							break;
						}
						else if(strcmp(s,read1)==0)
						{
							char res[1024]={0};
							write(sock , readm , strlen(readm));
							valread1 = read( sock , res, 1024);
							printf("%s\n",res); 
							memset(res, 0, strlen(res));
						}
						else if(strcmp(s,dele)==0)
						{
							char res[1024]={0};
							write(sock , delm , strlen(delm));
							valread1 = read( sock , res, 1024);
							printf("%s\n",res); 
							memset(res, 0, strlen(res));
						}
						else if(strncmp(s,send,5)==0)
						{
							char res[1024]={0};
							char res1[1024]={0};
							strcat(res,ssend);
							strcat(res,s+5);
							strcat(res," ");
							//printf("%s\n",res);
							printf("Type message: ");	
							while(1)
							{
								char buff[1024]={0};
								fgets(buff,1024,stdin);
								strcat(res,buff);
								if(strstr(buff,"###")!= NULL)
								{
									//printf("in if\n");
									break;
								}
								memset(buff, 0, strlen(buff));
							}
							//printf("%s\n",res);
							write(sock , res , strlen(res));
							valread1 = read( sock , res1, 1024);
							if(strcmp(res1,"invalid")==0)
							{
								printf("Entered UserId does not Exist\n");
							}
							else
							{
								printf("Mail has been sent\n");
							}
							memset(res1, 0, strlen(res1));
							memset(res, 0, strlen(res));
						}
						else
						{
							printf("Enter a valid Sub-Prompt Command\n");
						}
						memset(s, 0, strlen(s));					
					}
					
				}
				memset(str3, 0, strlen(str3));
				memset(temp, 0, strlen(temp));

		}
		else if(strcmp(n,quit)==0)
		{
			write(sock , "QUIT" , strlen("QUIT")); 
			valread = read( sock , buffer, 1024); 
			printf("%s\n",buffer );
			break;
		}
		else
		{
			printf("Enter a valid Command\n");
		}
		
		memset(n, 0, strlen(n));
		memset(buffer, 0, strlen(buffer));
	}
	return 0; 
} 
