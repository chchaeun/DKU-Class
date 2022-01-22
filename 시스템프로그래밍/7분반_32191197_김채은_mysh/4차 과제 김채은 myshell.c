/* my simple shell by. kimce. kce3615@naver.com */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/wait.h>

bool cd(int argc, char* argv[]);
bool quit(int argc, char* argv[]);
bool help(int argc, char* argv[]);

struct cmdSet {
        char *name;
        char *info;
        bool (*cmdFunc) (int argc, char *argv[]);            
};

struct cmdSet cmds[5] = {
	{"cd", "Change directory", cd},
        {"help", "Show help info.", help},
	{"quit", "Quit my shell", quit},
        {">", "Redirection"},
        {"&", "Background processing"}
};

bool cd(int argc, char* argv[]) {

        if (argc == 2) {
                if (chdir(argv[1]))
                        printf("%s is no directory\n", argv[1]);
        }
        else {
                printf("[!] Enter correct argument: cd [directory name]\n");
        }
        return true;
}

bool quit(int argc, char* argv[]) {
        return false;
}

bool help(int argc, char* argv[]) {
	int i;
        printf("---------------------------------------------------\n");
        printf("Simple shell by. Kimce\n");
        printf("Here's some commands.\n");
        for (i = 0; i < 5; i++) {
                printf("%s: %s\n", cmds[i].name, cmds[i].info);
        }
        printf("---------------------------------------------------\n");
        return true;
}

void redir(int argc, char* argv[]) {
        int fd;
        if (argc == 4) {
                fd = open(argv[3], O_WRONLY | O_CREAT, 0664);
                if (fd < 0) {
                        printf("[!] file open error: no. %d\n", argv[3], errno);
                        exit(-1);
                }
                dup2(fd, STDOUT_FILENO);
                argv[2] = NULL;
        }
        close(fd);
        return;
}

int tokenize(char* buf, char* delims, char* tokens[], int maxTokens) {
        int count = 0;
        char* t;
        t = strtok(buf, delims);

        while (t != NULL && count < maxTokens) {
                tokens[count++] = t;
                t = strtok(NULL, delims);
        }
        tokens[count] = NULL;
	return count;	
}
bool run(char* line)
{
        char delims[] = " \n";
        char* tokens[80];
 	int i;       
	int t_count;
        int status;
        pid_t pid;

        bool doBack = false;
        bool doRedir = false;

        char* isRedir = strchr(line, '>');
        char* isBack = strchr(line, '&');
        if (isRedir != NULL) doRedir = true;
        if (isBack != NULL) doBack = true;

        t_count = tokenize(line, delims, tokens, sizeof(tokens) / sizeof(char*));
        if (t_count == 0)
                return true;
	
	if(doBack == true)
		tokens[t_count-1] = NULL;	
        
	for (i = 0; i < 3; i++) {
                if (strcmp(cmds[i].name, tokens[0]) == 0)
                        return cmds[i].cmdFunc(t_count, tokens);
        }

        if ((pid = fork()) < 0) {
                perror("fork error");
                exit(-1);
        }
        else if (pid == 0) {
                if (doRedir == true) {
                        redir(t_count, tokens);
                }
                execvp(tokens[0], tokens);
        }

        if (doBack == false)
                wait(&status);
        

        return true;

}

int main(void)
{
        char line[80];
        while (1) {
                printf(" %s : mysh $ ", get_current_dir_name());
                fgets(line, sizeof(line) - 1, stdin);
                if (run(line) == false)
                        break;
        }

        return 0;
}

