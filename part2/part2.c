#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void) {
    FILE *f;
    char ops[3][2] = {"W", "R", "D"};
    char *fileops [10][2]; //limit maximum 10 file requests
    int x;
    int y;
    for(x = 0; x < 10; x++){
	    for(y = 0; y < 2; y++) {
	        fileops[x][y] = (char *)malloc(sizeof(char *) * 10);
	    }
	}
    srand(time(NULL));
    int requestfile;
    int before;
    int rops;

    //generate random number of requests
    int requests = rand() % 11; //0 to 10
    if (requests == 0)
    {
        requests = 1;
    }
    printf("Requests generated = %d \n", requests);

    printf("Requested File:  Operation:\n");
    for(x = 0; x < requests; x++){ //for random number of requests, generate random requested file and operation
        requestfile = rand() % 6;
        if (requestfile == 0)
        {
                requestfile = 1;
        }
        while(requestfile == before){ //file requested cannot be the same file requested as previous
            requestfile = rand() % 6; //0 to 5
            if (requestfile == 0)
            {
                requestfile = 1;
            }
        }
        before = requestfile;
        rops = rand() % 3; // 0 to 2
        sprintf(fileops[x][0], "%d", requestfile);
        strcpy(fileops[x][1],ops[rops]);
        printf("%9s %12s \n", fileops[x][0], fileops[x][1]);
    }

    //write to file
    f = fopen("fileoperations.txt", "w");
    for(x = 0; x < requests; x++){
        fprintf(f, "%s %s \n", fileops[x][0], fileops[x][1]);
    }
    fclose(f);

    //=================================================================================
    int cba[400]; //400 blocks of contiguous block allocation
    int cbatable[6][2] = { {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }; //contiguous block allocation table, limit only 5 files, start at index 1
    int i;
    char * fileops2 [10][2]; //limit 10 operations
    for(x = 0; x < 10; x++){
	    for(y = 0; y < 2; y++) {
	        fileops2[x][y] = (char *)malloc(sizeof(char *) * 10);
	    }
	}

	char * filelist[20][4]; //limit 19 files
	for(x = 0; x < 20; x++){
	    for(y = 0; y < 4; y++) {
	        filelist[x][y] = (char *)malloc(sizeof(char *) * 20);
	    }
	}

    char *token;
    char delimiter[1] = " ";
    char line[48];
    int column = 0;
    int linecount = 1; //start at 1 for file list
    int linecount2 = 0; //start at 0 for file operations

    //initialize cba, 0 means it is free blocks
    for (i = 0; i < 400; i++){
        cba[i] = 0;
    }

    printf("\n");
    printf("Reading in data from filelist.txt\n");
    //read in filelist data from file
    f = fopen("filelist.txt", "r");
    if(f != NULL){
        while(fgets(line, sizeof line, f) != NULL){ //read each line
            //printf("Line: %s", line);
            token = strtok(line, delimiter);
            column = 0;
            while(token != NULL){ //read each column in line
                //printf("Token: %s \n",token);
                //save each column
                strcpy(filelist[linecount][column],token);
                column++;
                token = strtok(NULL,delimiter);
                if(column == 4){
                    break;
                }
            }
            linecount++;
        }
        printf("File Number:    Filename:    Blocks:      Size:\n");
        for(i = 1; i < linecount; i++){
            printf("%11s %12s %10s %10s \n", filelist[i][0], filelist[i][1], filelist[i][2], filelist[i][3]);
        }
    } else {
        perror("Error reading filelist.txt!");
    }
    fclose(f);

    //reset variables
    column = 0;

    printf("\n");
    printf("Reading in data from fileoperations.txt\n");
    //read in file operations from file
    f = fopen("fileoperations.txt", "r");
    if(f != NULL){
        while(fgets(line, sizeof line, f) != NULL){ //read each line
            //printf("Line: %s", line);
            token = strtok(line, delimiter);
            column = 0;
            while(token != NULL){ //read each column in line
                //printf("Token: %s \n",token);
                //save each column
                strcpy(fileops2[linecount2][column],token);
                column++;
                token = strtok(NULL,delimiter);
                if(column == 2){ //used to remove \n and \0 character from data
                    break;
                }
            }
            linecount2++;
        }
        printf("Requested File:  Operation:\n");
        for(i = 0; i < linecount2; i++){
            printf("%9s %12s \n", fileops2[i][0], fileops2[i][1]);
        }
    } else {
        perror("Error reading fileoperations.txt!");
    }
    fclose(f);

    printf("\n");
    printf("File List Size: %d\n", linecount-1);
    printf("File Operations Size: %d\n", linecount2);

    //----------------------------------------------------------
    int position = 0;
    int allocblocks;
    int freeblocks = 0;
    int z;
    int w;
    int intable = 0;
    int filenum = 0;
    int elements = linecount;
    int s;
    int r;
    int q;
    char filename[50];
    int index;
    int flag;
    int rblock;
    srand(time(NULL));
	int rsize;
	int byte = 512;
	char block[20];
	char size[20];
	FILE *fp;
	fp = fopen("resultslog.txt", "w");
	int lost = 0;

    //for each file operations
    for (i = 0; i < linecount2; i++){
        //find the requested file in filelist
        fprintf(fp, "\n============================================\n");
        printf("\n============================================\n");
        fprintf(fp, "Doing File Requested = %s  Operation = %s\n", fileops2[i][0], fileops2[i][1]);
        printf("Doing File Requested = %s  Operation = %s\n", fileops2[i][0], fileops2[i][1]);
        for(x = 1; x < elements; x++){
            if(strcmp(fileops2[i][0],filelist[x][0]) == 0){
                lost = 1;
                filenum = atoi(fileops2[i][0]);
                strcpy(filename, filelist[x][1]);
                allocblocks = atoi(filelist[x][2]);
                //find if memory already allocated for file, there is a entry in cba table
                for(w = 0; w < 5; w++){
                    if(cbatable[filenum][1] != 0){
                        intable = 1;
                    }
                }
                if (intable == 1){ //memory has file
                    fprintf(fp,"Starting position: %d\n", cbatable[filenum][1]);
                    printf("Starting position: %d\n", cbatable[filenum][1]);
                    fprintf(fp,"Block Length: %d \n", allocblocks);
                    printf("Block Length: %d \n", allocblocks);
                    //write operation
                    if (strcmp(fileops2[i][1],"W") == 0){
                        fprintf(fp,"Status: File was written! \n");
                        printf("Status: File was written! \n");
                    } else if (strcmp(fileops[i][1],"R") == 0){ //read operation
                         if( access( filename, F_OK ) != -1 ) {
                            fprintf(fp,"Status: File was read! \n");
                            printf("Status: File was read! \n");
                            fprintf(fp,"File Number: %s \nFile Start Block: %d \nFile Blocks Length: %d \n", fileops2[i][0], cbatable[filenum][0], cbatable[filenum][1]);
                            printf("File Number: %s \nFile Start Block: %d \nFile Blocks Length: %d \n", fileops2[i][0], cbatable[filenum][0], cbatable[filenum][1]);
                         } else {
                            fprintf(fp,"Status: Error! File does not exist. Cannot Read! \n");
                            printf("Status: Error! File does not exist. Cannot Read! \n");
                         }
                    } else { //delete operation
                        if( access( filename, F_OK ) != -1 ) {
                            fprintf(fp,"Status: File was deleted! \n");
                            printf("Status: File was deleted! \n");
                            //remove entry from  cba memory, cba table and filelist
                            for(s = cbatable[filenum][0]; s <= allocblocks; s++){
                                cba[s] = 0;
                            }
                            cbatable[filenum][0] = 0;
                            cbatable[filenum][1] = 0;

                            for(r = 1; r <= elements; r++){
                                if(strcmp(filelist[r][1], filename) == 0){ //if is the deleted filename
                                    index = r;
                                    flag = remove(filelist[r][1]);
                                }
                            }
                            //remove the record in filelist
                            for (r = index; r <= elements -1; r++ ){
                                strcpy(filelist[r][0],filelist[r+1][0]);
                                strcpy(filelist[r][1],filelist[r+1][1]);
                                strcpy(filelist[r][2],filelist[r+1][2]);
                                strcpy(filelist[r][3],filelist[r+1][3]);
                            }
                            elements--;
                        } else {
                            fprintf(fp,"Status: Error! File does not exist. Cannot delete! \n");
                            printf("Status: Error! File does not exist. Cannot delete! \n");
                        }
                    }
                } else { //memory does not have file
                    //find free blocks in contiguous block allocation
                    for(y = 0; y < 400; y++){
                        if(cba[y] == 0){
                            freeblocks++;
                            if(freeblocks == allocblocks){ //if enough free blocks
                                position = y - freeblocks + 1;
                                break;
                            }
                        } else {
                            freeblocks = 0;
                        }
                    }
                    if(freeblocks == allocblocks){ //have enough free blocks
                        fprintf(fp,"Starting position: %d\n", position);
                        printf("Starting position: %d\n", position);
                        fprintf(fp,"Block Length: %d \n", allocblocks);
                        printf("Block Length: %d \n", allocblocks);
                        for(z = position; z < (position+allocblocks); z++) { //allocate blocks for file
                            cba[z] = filenum;
                        }
                        //write to contiguous block allocation table
                        cbatable[filenum][0] = position;
                        cbatable[filenum][1] = allocblocks;

                        //write operation
                        if(strcmp(fileops2[i][1], "W") == 0){
                            fprintf(fp,"Status: File was written! \n");
                            printf("Status: File was written! \n");
                            //create file
                            f = fopen(filename, "w");
                            fclose(f);
                            strcpy(filelist[elements+1][1], filename);
                            //generate random number of block
                            rblock = rand() % 41;
                            if (rblock == 0){
                                rblock = 1;
                            }
                            //store block to filelist
                            sprintf(block, "%d", rblock);
                            //strcpy(filelist[elements+1][2],block);
                            //calculate size
                            rsize = rblock * byte;
                            sprintf(size, "%d", rsize);
                            //store size to filelist
                            //strcpy(filelist[elements+1][3], size);
                            elements++;

                        } else if(strcmp(fileops2[i][1], "R") == 0){  //read operation
                            if( access( filename, F_OK ) != -1 ) {
                                fprintf(fp,"Status: File was read! \n");
                                printf("Status: File was read! \n");
                                fprintf(fp,"File Number: %s \nFile Start Block: %d \nFile Blocks Length: %d \n", fileops2[i][0], cbatable[filenum][0], cbatable[filenum][1]);
                                printf("File Number: %s \nFile Start Block: %d \nFile Blocks Length: %d \n", fileops2[i][0], cbatable[filenum][0], cbatable[filenum][1]);
                            } else {
                                fprintf(fp,"Status: Error! File does not exist. Cannot Read! \n");
                                printf("Status: Error! File does not exist. Cannot Read! \n");
                                for(s = cbatable[filenum][0]; s < (cbatable[filenum][0]+allocblocks); s++){
                                    cba[s] = 0;
                                }
                                cbatable[filenum][0] = 0;
                                cbatable[filenum][1] = 0;
                            }
                        } else { //delete operation
                            if( access( filename, F_OK ) != -1 ) {
                                fprintf(fp,"Status: File was deleted! \n");
                                printf("Status: File was deleted! \n");
                                //remove entry from memory, cba table and filelist
                                for(s = cbatable[filenum][0]; s < (cbatable[filenum][0]+allocblocks); s++){
                                    cba[s] = 0;
                                }
                                cbatable[filenum][0] = 0;
                                cbatable[filenum][1] = 0;

                                for(r = 1; r <= elements; r++){
                                    if(strcmp(filelist[r][1], filename) == 0){ //if is the deleted filename
                                        index = r;
                                        flag = remove(filelist[r][1]);
                                    }
                                }
                                //remove the record in filelist
                                for (r = index; r <= elements -1; r++ ){
                                    strcpy(filelist[r][0],filelist[r+1][0]);
                                    strcpy(filelist[r][1],filelist[r+1][1]);
                                    strcpy(filelist[r][2],filelist[r+1][2]);
                                    strcpy(filelist[r][3],filelist[r+1][3]);
                                }
                                elements--;
                            } else {
                                fprintf(fp,"Status: Error! File does not exist. Cannot delete! \n");
                                printf("Status: Error! File does not exist. Cannot delete! \n");
                                for(s = cbatable[filenum][0]; s < (cbatable[filenum][0]+allocblocks); s++){
                                    cba[s] = 0;
                                }
                                cbatable[filenum][0] = 0;
                                cbatable[filenum][1] = 0;
                            }
                        }
                    } else {
                        fprintf(fp,"Status: Error! Cannot allocate memory for file %s!\n", filenum);
                        printf("Status: Error! Cannot allocate memory for file %s!\n", filenum);
                    }
                    //reset variables
                    freeblocks = 0;
                    position = 0;
                }
                //reset variables
                intable = 0;
                filenum = 0;
                flag = 0;
                break;
            }
        }
        if(lost == 0){
            printf("Status: Error! File does not exist. Cannot do operation! \n");
        }
        lost = 0;
    }
    fprintf(fp,"\n====================================================================================================\n");
    printf("\n====================================================================================================\n");
    fprintf(fp,"Contiguous Block Allocation Memory: \n");
    printf("Contiguous Block Allocation Memory: \n");
    int display = 0;
    for(x = 0; x < 4; x++){
        fprintf(fp,"Blocks %d to %d \n", display+1, display+100);
        printf("Blocks %d to %d \n", display+1, display+100);
        for(i = 0; i < 100; i++){
            if(i == 50){
                fprintf(fp,"\n");
                printf("\n");
            }
            fprintf(fp,"%d ", cba[display+i]);
            printf("%d ", cba[display+i]);
        }
        display = display + 100;
        fprintf(fp,"\n\n");
        printf("\n\n");
    }

    fclose(fp);
}



