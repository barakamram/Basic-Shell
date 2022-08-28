#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define TRUE 1
#define MAX 1024
int flag = 0;

void copyFile(char *src, char *dest) {
    char c;
    FILE *src_file = fopen(src, "rb");
    if (src_file == NULL) {
        printf("Cannot open file %s\n", src);
        return;
    }
    FILE *dest_file = fopen(dest, "wb");
    if (dest_file == NULL) {
        printf("Cannot open file %s\n", dest);
        return;
    }
    while (fread(&c, 1, 1, src_file) == 1) {
        fwrite(&c, 1, 1, dest_file);
    }
    fclose(src_file);
    fclose(dest_file);
}

void connectToServer() {
    flag = 1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port= htons(5678);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int status = connect(sock, (struct sockaddr*)&server_addr,sizeof(server_addr));

    if (status == -1) {
        printf("Cannot connect to server...\n");
        return;
    }
    dup2(1,666);
    dup2(sock,1);
}

int main() {
    char cmd[MAX] = "";
    while(TRUE){
        fgets(cmd,MAX,stdin);
        cmd[strlen(cmd) - 1] = '\0';

        if (!strncmp(cmd, "EXIT",4)){
            printf("Shell is closed by 'EXIT' command. \n");
            return 0;
        }

        else if (!strncmp(cmd, "getcwd",6)){
            char curr_dir[256];
            getcwd(curr_dir,sizeof (curr_dir));
            printf("Current working directory is: %s\n",curr_dir);
        }

        else if(!strncmp(cmd, "ECHO",4)){
            char* str = cmd + 5;
            printf("%s\n",str);
        }

        else if(!strncmp(cmd, "TCP PORT",8)){
            printf("Connecting to server...!\n");
            connectToServer();
        }

        else if (!strncmp(cmd, "LOCAL",5)){
            dup2(666,1);
            printf("Redirected to Shell\n");
            continue;
        }

        else if(!strncmp(cmd, "DIR",3)){
            DIR *dir;
            struct dirent *dirent;
            dir = opendir(".");
            if (dir) {
                while ((dirent = readdir(dir)) != NULL) {
                    if (strcmp(dirent->d_name,".") == 0 || strcmp(dirent->d_name,"..") == 0){
                        continue;
                    }
                    printf("%s\n", dirent->d_name);
                }
                closedir(dir);
            }
        }

        /*(system call) to change the current working directory */
        else if(!strncmp(cmd, "CD",2)){
            char* new_dir = cmd + 3;
            if (chdir(new_dir) == -1) {
                printf("%s: no such directory\n", new_dir);
            }
            else{
              printf("successfully enter to directory: %s\n", new_dir);
            }
        }

        else if(!strncmp(cmd, "COPY",4)){
            char src[MAX];
            char dest[MAX];
            int count = 5;
            int dest_count = 0;
            while (cmd[count-5] != ' ') { //Extracting the SRC & DEST names
                src[count-5] = cmd[count];
                count++;
            }
            src[count] = '\0';
            count++;
            for (int i = count; i < strlen(cmd); ++i) {
                dest[dest_count] = cmd[i];
                dest_count++;
            }
            dest[dest_count] = '\0';
            copyFile(src, dest);
        }

        else if(!strncmp(cmd, "DELETE",6)){
            char *delete_file = cmd + 7;
            if(unlink(delete_file) == -1){ //Return -1 if couldnt delete the file.
                printf("File name doesn't exist\n");
            }
        }

        /*The next "else" condition will be doing the same but without calling system() */
        else{
            char *token;
            token = strtok(cmd, " "); //breaks the string into token in array
            char* param[MAX];
            int i=0;
            while( token != NULL ) {
                param[i++] = token;
                token = strtok(NULL, " ");
            }
            param[i] = NULL;
            pid_t pid = fork(); //Forking
            if(pid == -1){
                printf("Error in forking\n");
            }
            else if (pid == 0){
                if(execvp(param[0],param) == -1){ // The Bash won't complete the task
                    printf("Bad command\n");
                    break;
                }
            }
            else{
                waitpid(pid,NULL,0);
            }
        }
    }
    return 0;
}
