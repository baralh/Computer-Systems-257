        
        //Heman Baral
        
        //CMSC 257
        
        //Assignment 5 

        //Server.c
        


        ///////////////////////////Include's & Define's////////////////
        #include <stdio.h>
        #include <stdlib.h>
        #include <unistd.h>
        #include <errno.h>
        #include <string.h>
        #include <fcntl.h>
        #include <signal.h>
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <arpa/inet.h>
        #include <sys/wait.h>
        #define ERROR 42
        #define SORRY 43
        #define LOG 44
        #define BUFSIZE 8096


        ///////////////////////////////Functions/////////////////////

        int catch, connection = 1, pid=1; 

        void log(int type, char *s1, char *s2, int num)
        {
           

            //Action to perform in base of the error

            switch (type) 
            {

            //Error type ERROR 
            case ERROR:

                (void)printf("ERROR: %s:%s Error no=%d exiting pid=%d\n\n", s1, s2, errno, getpid());
                break;

            //Error type SORRY 
            case SORRY:

                (void)printf( "SORRY: %s:%s\n\n", s1, s2);
                break;

            //Error type LOG 
            case LOG:

                (void)printf( "INFO: %s:%s:%d\n\n", s1, s2, num);
                break;
            }

            
            //Error or sorry stop the process 

            if (type == ERROR || type == SORRY)
                exit(3);
        }


        // Method that hands exit signal
        void handler_s(int signal){

            if(pid == 0)

                return;
            
            if(signal == SIGINT){

                if(connection!=catch)

                    log(LOG,"Exit after connections are done","",connection+catch);
               
                    wait(NULL);
                    
                    log(LOG,"Exiting gracefully","",connection+catch);
                    exit(0);
                  
                
                
            }
        }

        void proc_exit(){

           

            pid_t pid; 
            
            while (1){
                pid = waitpid(-1, NULL, WNOHANG);
                if (pid == 0)
        				return;
        			else if (pid == -1)
        				return;
        			else{

                        printf("\n");
                        log (LOG,"Return for child","proc_exit",pid);
                        connection++;
                    }
            }

        }

        void sendFile(int Descriptor, int catch){

            int fildes;
            long ret; 
            static char buffer[50+1]; 
            int buff_data;

            //read from client
            ret = read(Descriptor,buffer, 50);

            buffer[ret-1] = 0; 

            log(LOG,"Request",buffer,ret);

            buff_data = strlen(buffer);


            if((fildes = open(&buffer[4],O_RDONLY))==-1)

                log(SORRY, "failed to open file",&buffer[4],catch);
            
            log(LOG,"SEND",&buffer[4], catch);
            printf("\n");


            while (	(ret = read(fildes, buffer, 50)) > 0 ) {
               

                (void)write(Descriptor,buffer,ret);
                
            }
            (void)write(Descriptor,"cmsc257\r",7);
            
            

            

        }



        ///////////////////////Main Method//////////////////////


        int main(int arg, char **argv)
        {

            
            signal(SIGINT, handler_s); 

            int port = atoi(argv[1]);
            
            char *hello = "Hello from server";

            int i, listenDescriptor, socketDescriptor;

            socklen_t length;

            static struct sockaddr_in cli_addr;

            static struct sockaddr_in serv_addr;

            signal (SIGCHLD, proc_exit);
            
            printf("\n");
            log(LOG, "Server starting...", "Port:", port);

            //setup sockets
            if ((listenDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)

                //error creating the socket

                log(ERROR, "System call", "Socket", 0);


            serv_addr.sin_family = AF_INET;

            serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);


            serv_addr.sin_port = htons(port);

            if (bind(listenDescriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)

                //error on associates a socket with a socket address structure

                log(ERROR, "System", "Bind", 0);

            ///bound TCP socket to enter listening state

            if (listen(listenDescriptor, 64) < 0)

                log(ERROR, "System", "listen", 0);
            else 

                log(LOG,"Waiting for incoming connection...","listen",0);

            //INFINITE LOOP
            for (catch = 1;; catch++)
            {
                
                length = sizeof(cli_addr);

                if ((socketDescriptor = accept(listenDescriptor, (struct sockaddr *)&cli_addr, &length)) < 0)

                    //error on accept()

                    log(ERROR, "System", "accept", 0);

                //creates a child

                if ((pid = fork()) < 0)
                {

                //error on creating the child

                    log(ERROR, "System", "fork", 0);
                }
                else
                {
                    if (pid == 0)
                    {
                        
                        close(listenDescriptor);

                        //read input

                        log(LOG,"In child process","catch",catch);

                        sendFile(socketDescriptor, catch);

                        exit(1);
                    }
                    else
                    {   
                        log(LOG,"Closing", "Connection", socketDescriptor);

                        
                        close(socketDescriptor);
                    }
                }
            }

           

            
        }