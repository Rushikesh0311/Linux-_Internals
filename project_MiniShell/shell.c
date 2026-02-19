#include "header.h"

extern char prompt_string[20]; 
char *external[153]; // Stores list of all external commands -->read from file
int status;
pid_t pid = 0;

Job_cmd jobs[20];
int stopped = 0; //count of jobs

extern char input_string[100]; 

void scan_input(char *prompt_string,char* input_string)
{
    extract_external_commands(external);  // read external commands list from file

    // signal registers
    signal(SIGINT, my_handler);
    signal(SIGTSTP, my_handler);
    signal(SIGCHLD, sigchld_handler);  // for bg

    while(1)
    {
        pid = 0;
       write(1, " ", 1); // avoids the prompt strings stack
        printf("%s ",prompt_string);
        fflush(stdout);

        //scanf("%[^\n]",input_string);
        //getchar();

        if (fgets(input_string, 100, stdin) == NULL) //reads whole line correctly
            continue;

        input_string[strcspn(input_string, "\n")] = '\0'; // adjust newline

        if(!strcmp(input_string,"exit"))
        {
            exit(0);
        }

        //PS1 command
        if(!strncmp(input_string,"PS1=",4))
        {
            if(strchr(input_string,' ') != NULL)
            {
                printf("ERROR: Space not allowed in PS1 cmd\n");
            }
            else
            {
                // copy new prompt afert the PS1
                // if no changes --> prompt into new_prompt
                strcpy(prompt_string,input_string+4);
            }
        }
        else
        {
            char *get_cmd = get_command(input_string); // get the first word
            int type = check_command_type(get_cmd); // checks type

            // if(type == BUILTIN)
            //     return BUILTIN;
            // else if(type == EXTERNAL)
            //     return EXTERNAL;
            // else
            //     return NO_COMMAND;

            if (type == BUILTIN)
                execute_internal_commands(input_string);
            else if (type == EXTERNAL)
            {
                pid = fork();
                if(pid == 0)
                {
                    // restore default signals
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execute_external_commands(input_string);
                }
                else if(pid > 0){
                    // wait till the child terminate
                    waitpid(pid,&status,WUNTRACED);

                pid = 0; //reseting pid for fg
                }
            }
            else
                printf("No such command\n");


        }

    }
}

char *get_command(char *input_string)
{
    static char cmds[100];
    int i = 0;

    //Copying  chars until--> space or end of str
    while(input_string[i] != ' ' && input_string[i] != '\0')
    {
        cmds[i] = input_string[i];
        i++;
    }
    cmds[i] = '\0';
    return cmds;
}

int check_command_type(char *command)
{
    if(internal_cmd(command))
        return BUILTIN;
    else if(external_cmd(command))
        return EXTERNAL;
    else
        return NO_COMMAND;
}

int internal_cmd(char* cmd)
{ 
    //Checks -->cmd is present in builtins[] list
    extern char *builtins[];
    int i = 0;

    while(builtins[i] != NULL)
    {
        if(!strcmp(builtins[i],cmd))
            return 1;
        i++;
    }
    return 0;
    
}

int external_cmd(char* cmd)
{
    int i = 0;
    while(external[i] != NULL)
    {
        if(!strcmp(external[i],cmd))
            return 1;
        i++;
    }
    return 0;
}

void extract_external_commands(char **external)
{
    int fd = open("external_cmd.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        external[0] = NULL;
        return;
    }


    int row = 0; // idx for each external cmds
    char buff[50];
    char ch;
    int i = 0;

    while(read(fd,&ch,1))
    {
        if(ch != '\n')
        {
            if(i < 99){
                buff[i] = ch; // 99 as a max cmd len
                i++;
            }
        }
        else
        {
            buff[i] = '\0';

            //memory for the cmds
            external[row] = malloc(strlen(buff)+1);

            strcpy(external[row++],buff);

            i = 0; // reseting the buffer
        }


    }

    if(i < sizeof(buff)-1) // for last cmd --> if file does not end with \n
    {
        buff[i] = '\0';
        external[row] = malloc(strlen(buff)+1);

        strcpy(external[row++],buff);
    }
    external[row] = NULL;
    close(fd);
}

void execute_internal_commands(char *input_string)
{
    // cd command
    if(!strncmp(input_string,"cd",2))
    {
        char* dest = input_string + 3; // skipping cd move to the path

        if(chdir(dest) != 0)
            perror("cd");

    }
    else if(!strcmp(input_string,"pwd"))
    {
        char pwd[100];
        getcwd(pwd,sizeof(pwd));
        printf("%s\n",pwd);
    }
    else if(!strcmp(input_string,"exit"))
    {
        exit(0);
       // kill(getpid(),SIGKILL);
    }
    else if(!strncmp(input_string,"echo",4))
    {
        
        char *cmd = input_string + 5; // echo skip

        if(!strcmp(cmd,"$$"))
        {
            printf("%d\n",getpid());
        }
        else if(!strcmp(cmd,"$?"))
        {
          
            printf("%d\n",WEXITSTATUS(status));
        }
        else if(cmd,"$SHELL")
        {
            char *shell = getenv("SHELL"); //printf shell path 
            if(shell != NULL)
                printf("%s\n",shell);
        }
        else
        {
            printf("%s\n",cmd);
        }

        /* add new cmds-------------- > fg bg and jobs
            jobs -> print current stopped process
            fg   -> foreground
            bg   -> background

        */

    }
    else if(!strncmp(input_string,"jobs",4))
    {
        print_jobs();
    }
    else if(!strcmp(input_string,"fg"))
    {
        fg_command();
    }
    else if(!strcmp(input_string,"bg"))
    {
        bg_command();
    }

}

void execute_external_commands(char *input_string)
{
     

    // pipe sepration  --> cmd1[left] pipe [right]cmd2 
    // char *left_side = strtok(input_string,"|");
    // char *right_side = strtok(NULL,"|");

    char *commands[50]; // to store cmds which are splited with the pipe
    int cmd_count = 0;
    int pipe_fd[2];
    int prev_pipefd[2];

    commands[cmd_count] = strtok(input_string,"|");
    while(commands[cmd_count] != NULL)
    {
        cmd_count++;
        commands[cmd_count] = strtok(NULL,"|");
    }

    if(cmd_count == 1)
    {
        char *args[50];
        int i = 0;

        //args[i] = strtok(input_string," ");
        args[i] = strtok(commands[0]," ");

        // loop throught the input string --> cmd end
        while(args[i] != NULL)
        {
            args[++i] = strtok(NULL," ");

        }

        pid_t pid = fork();

        if(pid == 0)
        {
            execvp(args[0],args);
        }
        else if(pid > 0)
        {
            int status;
            waitpid(pid,&status,0);
        }
    }
    else
    {
        //pipe proccess

       for(int i=0;i<cmd_count;i++)
       {
            if(i != cmd_count - 1)
                pipe(pipe_fd);

            pid_t pid = fork();

            if(pid == 0)
            {
                // child pro
                if(i != 0)
                {
                    // not 1st then reading from prev pipe end
                    dup2(prev_pipefd[0],0);
                    close(prev_pipefd[0]);
                }

                if(i != cmd_count - 1)
                {
                    dup2(pipe_fd[1],1);
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                }

                char *arg[50];
                int j = 0;

                //for  extra  spaces
                while (*commands[i] == ' ')
                    commands[i]++;

                // token by space
                arg[j] = strtok(commands[i], " ");
                while(arg[j] != NULL)
                {
                    j++;
                    arg[j] = strtok(NULL, " ");
                }

                execvp(arg[0], arg);

            }
            else if(pid > 0)
            {
                if(i != 0)
                {
                    close(prev_pipefd[0]);
                }

                if(i != cmd_count - 1)
                {
                    prev_pipefd[0] = pipe_fd[0];
                    close(pipe_fd[1]);
                }
            }
       }
       // waiting for child proccess
        for(int i=0;i<cmd_count;i++)
            wait(NULL);
        

    }
}

void my_handler(int signum)
{
    if(signum == SIGINT)
    {
        if(pid == 0)
        {
            printf("\n%s ",prompt_string);
            fflush(stdout);
            //write(1, "\n", 1);

        }
        // if(pid != 0)
        // {
        //     kill(pid,SIGINT);
        // }
        // write(1,"\n",1);
       
    }

    if(signum == SIGTSTP)
    {
        if(pid == 0)
        {
            printf("\n%s ",prompt_string);
            fflush(stdout);
           
        }
        // if(pid != 0)
        // {
        //     kill(pid,SIGTSTP);
        //     insert_first();
        // }
        // write(1,"\n",1);
        else
        {
            kill(pid,SIGTSTP);

            insert_first();

            printf("\nStopped : %s\n", input_string);
            fflush(stdout);
        }
    }

    if(signum == SIGCHLD)
    {
        waitpid(-1,&status,WNOHANG);
    }
}

void  insert_first()
{
    // storing pid of stopped proccess
    jobs[stopped].pid = pid;

    // storing cmd string
    strcpy(jobs[stopped].cmd,input_string);

    stopped++; // + stop count
}

void delete_at_first()
{
    if(stopped == 0)
        return;

    for(int i=0;i<stopped-1; i++)
    {
        jobs[i] = jobs[i+1];
    }

    stopped--;
}

void sigchld_handler(int signum)
{
    // for bg --> cleans zombie
    int child_pid;

    while(child_pid = waitpid(-1,&status,WNOHANG) > 0);

}

void bg_command()
{
    if(stopped == 0)
    {
        printf("NO jobs\n");
        return;
    }
    int fg_cmd = jobs[0].pid;
    kill(fg_cmd,SIGCONT);
    delete_at_first();

}

void fg_command(){

    if(stopped == 0)
    {
        printf("NO Jobs\n");
        return;
    }

    int fg_cmd = jobs[0].pid;
    printf("%s\n",jobs[0].cmd);

    kill(fg_cmd,SIGCONT);

    waitpid(fg_cmd,&status,WUNTRACED);

    delete_at_first();
}

void print_jobs()
{
    if(stopped == 0)
    {
        printf("No stopped jobs\n");
        return;
    }

    for(int i = 0; i < stopped; i++)
    {
        printf("[%d] %d %s\n",i+1,jobs[i].pid,jobs[i].cmd);
    }
}

