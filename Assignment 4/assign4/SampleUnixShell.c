//Heman Baral
//CMSC 257 
//Assignment 4
//November 27, 2017


#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>


#define BUFFERSIZE 64

#define DELIMITER " \t\r\n\a"

void sigHandler (int a) {
    signal(SIGINT, sigHandler);
}

char *READ(void) {
    char *line = NULL;
    ssize_t BUFFER_SIZE = 0;
    getline(&line, &BUFFER_SIZE, stdin);
    return line;
}

char  **splitLine(char *line) {
    int BUFFER_SIZE = BUFFERSIZE;
    int current = 0;
    char **tokens = malloc(BUFFER_SIZE*sizeof(char*));
    char *token;

    if(!tokens) {
        fprintf(stderr, "Allocation Error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIMITER);
    while (token != NULL) {
        tokens[current] = token;
        current++;

        if (current >= BUFFER_SIZE) {
            BUFFER_SIZE += BUFFERSIZE;
            tokens = realloc(tokens, BUFFER_SIZE*sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Allocation Error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, DELIMITER);
    }
    tokens[current] = NULL;
    return tokens;
}

int launch(char **args) {
    pid_t pid, wpid;
    int status, exitStatus;

    pid = fork();
    if (pid == 0) {
        if(execvp(args[0], args) == -1) {

            printf("\n");
            perror("Error!!!");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {

        printf("\n");
        perror("Error!!!");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
        if (WIFEXITED(status)) {
            exitStatus = WEXITSTATUS(status);
            if (exitStatus == 0) {

                printf("\n");
                printf("The run was SUCESSFUl \n The exit status is %d\n", exitStatus);
            }
            else {

                printf("\n");
                printf("Error!!!\n The exit status is %d\n", exitStatus);
            }
        }
    }
    return 1;
}



int cd(char **args);
int ExitShell(char **args);
int pid(char **args);
int ppid(char **args);

char* builtin_str[] = {
    "cd",
    "exit",
    "pid",
    "ppid"
};

int (*builtin_func[])(char **) = {
    &cd,
    &ExitShell,
    &pid,
    &ppid
};

int numBuiltins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int cd(char **args) {
    if (args[1] == NULL) {
        char Dir[256];
        getcwd(Dir, sizeof(Dir));
        fprintf(stdout, "%s\n", Dir);
    }
    else {
        if (chdir(args[1]) != 0) {
            char Dir[256];
            getcwd(Dir, sizeof(Dir));
            fprintf(stdout, "%s\n", Dir);
        }
    }
    return 1;
}

int ExitShell(char **args) {
    int r;
    if (signal(SIGTERM, sigHandler) == SIG_ERR) {
        printf("\n");
        printf("Error!!!\n Unable to set signal handler.\n");
        exit(0);
    }
    r = raise(SIGTERM);
    if (exit != 0) {
        exit(0);
    }
    return 0;
}

int pid(char **args) {
    printf("%d\n", getpid());
}

int ppid(char **args) {
    printf("%d\n", getppid());
}

int execute(char **args) {
    int i;
    if (args[0] == NULL) {
        return 1;
    }
    
    for (i = 0; i < numBuiltins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    return launch(args);
}

int main(int argc, char **argv) {
    signal(SIGINT, sigHandler);
    char *prompt;
    int stringSize = 0;
    if (argc > 1) {
        int i;
        if (strcmp(argv[1], "-p") == 0) {
            stringSize = strlen(argv[2]);
            prompt = malloc(stringSize);
            prompt[0] = '\0';
            strcat(prompt, argv[2]);
            char *line;
            char **args;
            int status;

            do {
                printf("%s> ",prompt);
                line = READ();
                args = splitLine(line);
                status = execute(args);

                free(line);
                free(args);
            }
            while (status);
        }
        else {
            printf("\n");
            printf("Invlaid flag\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (argc <= 1) {
        char *line;
        char **args;
        int status;

        do {
            printf("my257sh> ");
            line = READ();
            args = splitLine(line);
            status = execute(args);

            free(line);
            free(args);
        }
        while(status);
    }
    return EXIT_SUCCESS;
}
