/*
 * Draft by Sam at 06-16-2019
 * show the metadata of a file 
 * 
 * */
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
 
/* display the file permision 
 * input: the mode_t(file type and mode) of a statBuffer 
 * */
 
void disFilePerm(mode_t filePerm){
	char userR = 0;
	char userW = 0;
	char userX = 0;

	char groupR = 0;
	char groupW = 0;
	char groupX = 0;

	char otherR = 0;
	char otherW = 0;
	char otherX = 0;

	userR = (filePerm & S_IRUSR) ? 'r' : '-';
	userW = (filePerm & S_IWUSR) ? 'w' : '-';
	userX = (filePerm & S_IXUSR) ? 'x' : '-'; 

	groupR = (filePerm & S_IRGRP) ? 'r' : '-';
	groupW = (filePerm & S_IWGRP) ? 'w' : '-';
	groupX = (filePerm & S_IXGRP) ? 'x' : '-'; 

	otherR = (filePerm & S_IROTH) ? 'r' : '-';
	otherW = (filePerm & S_IWOTH) ? 'w' : '-';
	otherX = (filePerm & S_IXOTH) ? 'x' : '-'; 

	printf("Mode:\t\t\t\t\t%c%c%c%c%c%c%c%c%c\n",userR,userW,userX,groupR,groupW,groupX,otherR,otherW,otherX);

}
 
/* 
 * display the stat buffer 
 * input the pointer of statBuffer
 * 
 * */
void displayStatInfo(struct stat *statBuffer){
	printf("\n===================== File Metadata ==============================\n\n");
	/* print the file type */
    printf("File type:\t\t\t\t");
    switch (statBuffer->st_mode & S_IFMT) {
    case S_IFREG:  printf("Regular file\n");            break;
    case S_IFDIR:  printf("Directory\n");               break;
    case S_IFCHR:  printf("Character device\n");        break;
    case S_IFBLK:  printf("Block device\n");            break;
    case S_IFLNK:  printf("Symbolic (soft) link\n");    break;
    case S_IFIFO:  printf("FIFO or pipe\n");            break;
    case S_IFSOCK: printf("Socket\n");                  break;
    default:       printf("Unknown file type\n");       break;
    }
 
    printf("Device containing:\t\t\tmajor=%ld   minor=%ld\n",(long) major(statBuffer->st_dev), (long) minor(statBuffer->st_dev));
    printf("I-node number:\t\t\t\t%ld\n", (long) statBuffer->st_ino);
    disFilePerm(statBuffer->st_mode);
    if (S_ISCHR(statBuffer->st_mode) || S_ISBLK(statBuffer->st_mode)){
		printf("Device number (st_rdev):\t\tmajor=%ld; minor=%ld\n",(long) major(statBuffer->st_rdev), (long) minor(statBuffer->st_rdev));
	}
    printf("Number of (hard) links:\t\t\t%ld\n", (long) statBuffer->st_nlink);
    printf("Ownership:\t\t\t\tUID=%ld   GID=%ld\n", (long) statBuffer->st_uid, (long) statBuffer->st_gid);
    printf("File size:\t\t\t\t%lld bytes\n", (long long) statBuffer->st_size);
    printf("Optimal I/O block size:\t\t\t%ld bytes\n", (long) statBuffer->st_blksize);
    printf("Number of (512B) blocks allocated:\t%lld\n", (long long) statBuffer->st_blocks);
    printf("Last file access:\t\t\t%s", ctime(&statBuffer->st_atime));
    printf("Last file modification:\t\t\t%s", ctime(&statBuffer->st_mtime));
    printf("Last status change:\t\t\t%s", ctime(&statBuffer->st_ctime));
    printf("\n==================================================================\n\n");
 }
 
int main(int argc, char *argv[]){
    struct stat statBuffer;
	if (argc != 2 || argv[1] == NULL){
			printf("Invalid patameters ! \n");
			printf("Usage: ");
			printf("filemetadata <FILENAME> \n");
			return 1;
	}
 
	/* just use lstat to show the info
	 * lstat() is identical to stat(), except that if pathname is a symbolic link, then it returns information about the  link
     * itself, not the file that it refers to.
	 */
    if (lstat(argv[1], &statBuffer) == -1){
		perror("lstat");
        return 2;
	}
	displayStatInfo(&statBuffer);
    return 0;
 }
