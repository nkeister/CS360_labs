#include "lab3header.h"
void initialize(char **env)
{
    int i;
    int envLen = 0;

    PATH = HOME = NULL;
    pathList = NULL;

    for (i = 0; NULL != env[i]; i++)
    {
        envLen += strlen(env[i]) + 1;
    }

    fprintf(stderr, "Showing PATH:\n");
    PATH = envz_get(*env, envLen, "PATH"); //finds PATH name from env[46]
    if (PATH)
    {
        fprintf(stderr, "PATH=%s\n\n", PATH); //sends formatted output to PATH
    }
    else
    {
        exit(-1);
    }

    char *pathTmp = NULL;
    char *token = NULL;
    Path_t *tptr = NULL;
    Path_t *nptr = NULL;
    char *name = NULL;

    fprintf(stderr, "PATH into dir strings:\n");
    if (PATH)
    {
        pathTmp = (char *)malloc(strlen(PATH) + 1); //allocate memory for path
        strcpy(pathTmp, PATH);

        token = strtok(pathTmp, ":"); //token PATH
        fprintf(stderr, "%s  ", token);

        nptr = (Path_t *)malloc(sizeof(Path_t));
        nptr->name = (char *)malloc(strlen(token));
        strcpy(nptr->name, token);
        nptr->next = NULL;

        pathList = tptr = nptr;

        while (1)
        {
            token = strtok(NULL, ":");
            if (!token)
            {
                break;
            }
            fprintf(stderr, "%s  ", token); //fprintf full PATH

            nptr = (Path_t *)malloc(sizeof(Path_t));
            nptr->name = (char *)malloc(strlen(token));
            strcpy(nptr->name, token);
            nptr->next = NULL;
            tptr->next = nptr;
            tptr = nptr;
        }
        free(pathTmp);
    }
    else
    {
        exit(-1);
    }

    HOME = envz_get(*env, envLen, "HOME");
    if (HOME)
    {

        fprintf(stderr, "\n\nHOME=%s\n\n", HOME); //fprintf HOME dir
    }
    else
    {
        exit(-1);
    }
}

char *strReverse(char *string)
{
    char temp;
    int start = 0;
    int end = strlen(string) - 1;

    while (start < end)
    {
        temp = string[start];
        string[start] = string[end];
        string[end] = temp;
        ++start;
        --end;
    }
}

char *removePre(char *string)
{
    while ('\0' != *string)
    {
        if (isspace(*string))
        {
            string++;
        }
        else
        {
            return string;
        }
    }
}

char *removePost(char *string)
{
    char end = strlen(string) - 1;
    while (isspace(string[end--]))
        ;
    string[end + 2] = '\0';
    return string;
}

int tokenize(char *string, char *cmdArgs, char *file)
{
    int state = -1;
    string = removePre(string);
    while ('\0' != *string)
    {
        if (('>' != *string) && ('<' != *string))
        {
            *cmdArgs = *string;
            cmdArgs++;
        }
        else
        {
            break;
        }
        string++;
    }

    *cmdArgs = '\0';
    cmdArgs = removePost(cmdArgs);

    if (*string == '\0')
    {
        return 0;
    }

    if (*string == '>')
    {
        string++;
        if ('>' == *string)
        {
            state = 3;
        }
        else if (isspace(*string))
        {
            state = 2;
        }
        else
        {
            return -1;
        }
    }
    else if (*string == '<')
    {
        string++; //take inout  < from file
        if (isspace(*string))
        {
            state = 1;
        }
        // fail ret -1
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    string++;

    string = removePre(string);

    while ('\0' != *string)
    {
        if (!isspace(*string))
        {
            *file = *string;
            file++;
        }
        else
        {
            break;
        }
        string++;
    }

    *file = '\0';

    return state;
}

void exe(char **env)
{
    char *token;
    char tmpString[LINE_MAX];
    char cmdPath[512];
    char cmdArgs[512];
    char doCommand[512];
    char redirectOutput[64];
    char **myargv;
    char myargc = 0;
    int redirect = 0;
    int pid = 1;
    int pd[2];
    int success = 0;
    int hasPipe = 0;
    Path_t *nptr = NULL;

    // do pipe
    strReverse(head);
    token = strtok(head, "|");

    if (strcmp(tail, "(null)") == 0)
        return;
    strReverse(token);
    token = removePre(token);
    token = removePre(token);
    strcpy(tail, token);
    token = strtok(NULL, "");
    if (token == NULL)
    {
        hasPipe = 0;
        strcpy(head, tail);
        strcpy(tail, "(null)");
    }
    else
    {
        hasPipe = 1;
        strReverse(token);
        token = removePre(token);
        token = removePre(token);
        strcpy(head, token);
    }

    // check if pipe
    if (hasPipe)
    {
        pipe(pd);
        pid = fork();
        // check fork
        if (pid < 0)
        {
            fprintf(stderr, "Fork failed!");
            exit(-1);
        }
    }

    // Parent Process:
    if (pid) //parent as pipe WRITER
    {
        if (hasPipe)
        {
            close(pd[1]); //close pd[1]
            close(0);     //close 0
            dup(pd[0]);   //replace 0 with pd[0]
        }

        fprintf(stderr, "PROC %d: head=%s   tail=%s\n", getpid(), head, tail);

        if (0 != strcmp(tail, "(null)"))
        {
            strcpy(doCommand, tail);
        }
        else
        {
            strcpy(doCommand, head);
        }

        // print do command
        fprintf(stderr, "PROC %d do_command: line=%s\n", getpid(), doCommand);

        redirect = tokenize(doCommand, cmdArgs, redirectOutput);
        if (redirect)
        {
            if (redirect == -1)
            {
                strcpy(cmdArgs, doCommand);
            }
            // < takes input from file
            else if (redirect == 1)
            {
                close(0);
                open(redirectOutput, O_RDONLY);
                fprintf(stderr, "PROC %d redirect input from %s\n", getpid(), redirectOutput);
            }
            // > send out to file
            else if (redirect == 2)
            {
                close(1);
                open(redirectOutput, O_WRONLY | O_CREAT, 0644);
                fprintf(stderr, "PROC %d redirect output to %s\n", getpid(), redirectOutput);
            }
            // >> append output to outfile
            else if (redirect == 3)
            {
                close(1);
                open(redirectOutput, O_RDWR | O_APPEND | O_CREAT, 0644);
                fprintf(stderr, "PROC %d redirect output to %s\n", getpid(), redirectOutput);
            }
        }

        // Extract command
        strcpy(tmpString, cmdArgs);
        token = strtok(tmpString, " ");
        strcpy(command, token);

        // Extract arguments:
        myargc = 2;
        myargv = calloc(myargc, sizeof(char *));
        myargv[0] = token;

        token = strtok(NULL, " ");
        myargv[1] = token;

        while (NULL != token)
        {
            token = strtok(NULL, " ");
            myargc++;
            myargv = realloc(myargv, sizeof(char *) * myargc);
            myargv[myargc - 1] = token;
        }

        // Try every path list if command executes successfully
        fprintf(stderr, "PROC %d tries %s in each PATH dir\n", getpid(), command);
        nptr = pathList;
        success = 0;
        while (NULL != nptr)
        {
            strcpy(cmdPath, "");
            strcat(cmdPath, nptr->name);
            strcat(cmdPath, "/"); //concatinate path with'/'
            strcat(cmdPath, command);
            fprintf(stderr, "cmd=%s\n", cmdPath); //fprintf cmd= path dir[46]
            if (-1 != execve(cmdPath, myargv, env))
            {
                success = 1;
                break;
            }
            nptr = nptr->next; //point to next env[46]
        }
        if (!success)
        {
            fprintf(stderr, "invalid command %s\n", command);
        }
    }

    else //child as pipe READER
    {
        close(pd[0]);
        close(1);
        dup(pd[1]);
        /*
            The pipe writer redirects its fd1⁄41 to pd[1], and the pipe 
            reader redirects its fd1⁄40 to pd[0]. Thus, the two
            processes are connected through the pipe.
            - pd[0] is for reading file
            - pd[1] is for writing to the pipe

        */
        exe(env);
    }
}