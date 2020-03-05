#include "common.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* make sure to use syserror() when a system call fails. see common.h */

void
usage()
{
    fprintf(stderr, "Usage: cpr srcdir dstdir\n");
    exit(1);
}

// Copying a file
void copy_file(char* srcfile, char* dstfile)
{
    int src,dst,bytesRead;
    char buf[4096];
    struct stat entryStat;
    
    // Read the stats on the source file
    if(stat(srcfile,&entryStat)==-1) {
        syserror(stat,srcfile);
    }
    
    // Open the source file in read-only mode
    src = open(srcfile,O_RDONLY);
    if(src == -1) {
        syserror(open,srcfile);
    }
    
    // Create the file at the destination with the same permissions as source
    dst = creat(dstfile,entryStat.st_mode);
    if(dst == -1) {
        syserror(creat,dstfile);
    }
    
    // Continuously read and write until read returns 0 bytes
    while((bytesRead = read(src,buf,4096))) {
        if(write(dst,buf,bytesRead) == -1) {
            syserror(write,dstfile);
        }
    }
    
    // Close files
    close(src);
    close(dst);
}

// Copying a directory
void copy_directory(char* srcdir, char* dstdir)
{
    DIR* src_p = opendir(srcdir);
    struct dirent* entry;
    struct stat entryStat;
    
    // Create the directory at destination as read/write/execute for user for now
    // This is done because if the directory is read only it will be unable to be
    // modified if permissions are set now
    if(mkdir(dstdir,S_IRWXU)==-1) {
        syserror(mkdir,dstdir);
    }
    
    // Skip the first 2 "folders" in every directory
    readdir(src_p); // ./.
    readdir(src_p); // ./..
    
    while((entry = readdir(src_p))) {
        // Set up file path for current directory entry
        char filename[256];
        strcpy(filename,srcdir);
        strcat(filename,"/");
        strcat(filename,entry->d_name);
        
        // Set up file path for destination entry
        char destname[256];
        strcpy(destname,dstdir);
        strcat(destname,"/");
        strcat(destname,entry->d_name);
        
        // Read the stats on the file
        if(stat(filename,&entryStat)==-1) {
            syserror(stat,filename);
        }
        
        // If the current entry is a directory, make a folder in
        // the destination and everything within over
        if((entryStat.st_mode&S_IFMT)== S_IFDIR) {
            copy_directory(filename,destname);
        } else if((entryStat.st_mode&S_IFMT)== S_IFREG) {
            copy_file(filename,destname);
        }
    }
    
    // Read the stats on the source directory
    if(stat(srcdir,&entryStat)==-1) {
        syserror(stat,srcdir);
    }
    // Set proper permissons
    if(chmod(dstdir,entryStat.st_mode)==-1) {
        syserror(chmod,dstdir);
    }
    
    // Close directory
    closedir(src_p);
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        usage();
    }
    
    // Begin recursive copy
    copy_directory(argv[1],argv[2]);
    
    return 0;
}