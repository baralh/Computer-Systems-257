                //Heman Baral

                //CMSC 257

                //Assignment 5

                //Client.c


                
                ////////////////////////////Indlude's & Define's/////////////////////


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
                #include <pthread.h>
                #define ERROR 42
                #define SORRY 43
                #define LOG 44
                #define BUFSIZE 8096
                #define SLEEP 1



                



                ///////////////////////////Functions's//////////////////////////////////




                    double ftime (void)
                    {

                   struct timeval t;

                    gettimeofday(&t, NULL);
                    
                    return (double) t.tv_sec + (double) t.tv_usec / 1000000.0;


                }


             

                int String_position(char *yString, char *noString)

                {
                    char *p = strstr(yString, noString);
                    if (p)
                        return p - yString;
                    return -1;
                }


                        void log(int type, char *s1, char *s2, int num)
                {

                    switch (type)
                    {

                    case ERROR:

                        (void)printf("ERROR: %s:%s Errno=%d Exiting pid=%d\n", s1, s2, errno, getpid());

                        break;

                    case SORRY:

                        (void)printf("SORRY: %s:%s\n", s1, s2);

                        break;

                    case LOG:

                        (void)printf("INFO: %s:%s:%d\n", s1, s2, num);

                        break;
                    }

                    if (type == ERROR || type == SORRY)

                        exit(3);
                }

                struct arg_struct
                {
                    int vargp;
                    int socketfd;
                    char *addr;
                    int port;
                };




                void *getData(void *argss)
                {
                    int socketfd;

                    FILE *recFile;

                    char filename[50],command[50], serv_reply[50];
                    

                    struct arg_struct *args = (struct arg_struct *)argss;

                    struct sockaddr_in serv_addr;

                    serv_addr.sin_addr.s_addr = inet_addr(args->addr);

                    serv_addr.sin_family = AF_INET;

                    serv_addr.sin_port = htons(args->port);

                    socketfd = socket(AF_INET, SOCK_STREAM, 0);







                    if (connect(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                    {
                        log(ERROR, "Error Connecting to the Server", "connect", 0);

                        return NULL;
                    }

                    printf("\n");

                    log(LOG, "Connection ON", args->addr, args->vargp);

                    printf("\n");

                    sprintf(filename,"file%03d.txt",args->vargp);
                    
                    sprintf(command,"GET %s\r",filename);

                    log(LOG, "Sent", command, 0);

                    if (send(socketfd, command, strlen(command), 0) < 0)
                    {
                        printf("\n");
                        log(ERROR, "Sent Request Error", command, 0);
                    }
                   
                    int loc;
                    int ret;
                    recFile = fopen(filename, "w");

                    while ((ret = read(socketfd, serv_reply, 50)) > 0)
                    {

                        loc = String_position(serv_reply, "cmsc257");

                        if (loc != -1)
                        {

                            serv_reply[loc] = 0;

                            int i;

                            fwrite(serv_reply, sizeof(char), loc, recFile);
                            

                            close(socketfd);

                            break;
                        }

                        fwrite(serv_reply, sizeof(char), ret, recFile);
                    }

                    free(args);

                    pthread_exit(NULL);
                }





                ///////////////////////////Main Method////////////////////////////////////

                int main(int argc, char *argv[])
                {
                    double start, stop, used;

                    int socketfd, port;

                    int numFiles = 10;

                    int sleepTime = 1; 

                    struct sockaddr_in serv_addr;

                    pthread_t tid[11];

                    if(argc == 5){

                        numFiles = atoi(argv[3]);

                        sleepTime = atoi(argv[4]);
                    }
                    port = atoi(argv[2]);

                    char *addr = argv[1];

                    socketfd = socket(AF_INET, SOCK_STREAM, 0);

                    if (socketfd < 0)
                    {
                        printf("\n");

                        log(ERROR, "Error Creating Socket", "Socket", 0);
                    }

                    serv_addr.sin_addr.s_addr = inet_addr(addr);

                    serv_addr.sin_family = AF_INET;

                    serv_addr.sin_port = htons(port);

                    int i = 1;

                    
                    
                    
                    start = ftime();

                    for (i; i < numFiles+1; i++)
                    {
                        struct arg_struct *args = malloc(sizeof(struct arg_struct));

                        args->addr = addr;

                        args->port = port;

                        args->vargp = (rand() % 25)+1;

                        printf("\n");

                        log(LOG,"Making a Download Handler","thread", i);

                        printf("\n");

                        pthread_create(&tid[i], NULL, getData, args);

                        sleep(sleepTime);
                        
                    }

                    for (i = 1; i < numFiles+1; i++)

                        pthread_join(tid[i], NULL);

                    stop = ftime();

                    used = stop-start;

                    printf("\n");
                    printf("Time Elapsed: %0.4f\n",used);

                    printf("\n\n\n");
                    log(LOG,"Successful!!! exiting gracefully","exit", 0);
                    exit(0);
                    return 0;
                }