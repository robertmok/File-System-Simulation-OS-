#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>

int main(void) {
	FILE *fp[10];
	FILE *f;
	int i = 0;
	char fname[50];
	int byte = 512;
	char block[20];
	char size[20];
	char * filelist[20][4]; //limit 19 files
    int x;
	int y;
	for(x = 0; x < 20; x++){
	    for(y = 0; y < 4; y++) {
	        filelist[x][y] = (char *)malloc(sizeof(char *) * 20);
	    }
	}
	char * filelisttemp[20][4]; //limit 19 files
	for(x = 0; x < 20; x++){
	    for(y = 0; y < 4; y++) {
	        filelisttemp[x][y] = (char *)malloc(sizeof(char *) * 20);
	    }
	}
	int rblock;
    char num[50];
    int index;

	srand(time(NULL));
	int rsize;
    int filenum = 1;

	//generate 10 files
	for (i = 1; i < 11; i++){

	    //store filenum
	    sprintf(filelist[i][0], "%d", filenum);
	    filenum++;

		sprintf(fname, "%d.txt", i);
		fp[i] = fopen(fname, "w");
		fclose(fp[i]);
		strcpy(filelist[i][1], fname);
        //printf("%s \n", filelist[i][0]);

		//generate random number of block
		rblock = rand() % 41;
		if (rblock == 0){
		    rblock = 1;
		}
		sprintf(block, "%d", rblock);
		//store into filelist
		strcpy(filelist[i][2],block);
        //printf("%s %s\n", filelist[i][0], filelist[i][1]);

		//calculate size
		rsize = rblock * byte;
		sprintf(size, "%d", rsize);
		//store into filelist
		strcpy(filelist[i][3],size);

		//printf("%s %s %s \n", filelist[i][0], filelist[i][1], filelist[i][2]);
	}

	int choice = 0;
	char filename[50];
	int flag;
	char newname[50];
	int elements = 10; //10 files default
    int quit = 0;
    int here = 0;

	while(quit == 0){
		printf("Press 1 for CREATE, 2 for DELETE, 3 for RENAME, 4 for LIST Files, 0 for QUIT: ");
		scanf("%d",&choice);
		if (choice == 1){ //create file
			printf("Enter filename for the file: ");
			scanf("%s", &filename);

			//check if file exist
			if( access( filename, F_OK ) != -1 ) {
				// file exists
				printf("Error: File already exist! Cannot Create! \n\n");
			} else {
				// file doesn't exist
				sprintf(filelist[elements+1][0], "%d", filenum);
				filenum++;

				f = fopen(filename, "w");
				fclose(f);
				//store filename to filelist
				strcpy(filelist[elements+1][1], filename);

				//generate random number of block
				rblock = rand() % 41;
				if (rblock == 0){
                    rblock = 1;
                }
				//store block to filelist
				sprintf(block, "%d", rblock);
				strcpy(filelist[elements+1][2],block);

				//calculate size
                rsize = rblock * byte;
				sprintf(size, "%d", rsize);
				//store size to filelist
				strcpy(filelist[elements+1][3], size);

				elements++;
				printf("File Created! \n\n");
				//for(i = 1; i <= elements; i++){
				  //  printf("%s %s %s \n", filelist[i][0], filelist[i][1], filelist[i][2]);
				//}
			}

		} else if (choice == 2){ //delete file
			printf("Enter file number to delete: ");
			scanf("%s", &filename);

			//check if file exist
			for(i = 1; i <=elements; i++){
			    if(strcmp(filelist[i][0], filename) == 0){ //if is the filenumber
                    flag = remove(filelist[i][1]);
                    here = 1;
                    break;
                }
			}
			if(flag == 0 && here == 1) {
				printf("File deleted! \n\n");

                //find location of removed file
                for(i = 1; i <= elements; i++){
                    if(strcmp(filelist[i][0], filename) == 0){ //if is the deleted filename
                        index = i;
                    }
                }

                //remove the record in filelist
                for (i = index; i <= elements -1; i++ ){
                    strcpy(filelist[i][0],filelist[i+1][0]);
                    strcpy(filelist[i][1],filelist[i+1][1]);
                    strcpy(filelist[i][2],filelist[i+1][2]);
                    strcpy(filelist[i][3],filelist[i+1][3]);
                }
                elements--;

			} else if(here == 0) {
					printf("Error: File number does not exist! \n\n");
			} else {
				printf("Error: Cannot delete file!\n\n");
			}
		} else if (choice == 3){ //rename file
			printf("Enter file number to rename: ");
			scanf("%s", &filename);
			//check if file exist
			for(i = 1; i <=elements; i++){
			    if(strcmp(filelist[i][0], filename) == 0){ //if is the filenumber
                    here = 1;
                    break;
                }
			}
			if(here == 1){
				// file exists
				printf("Enter new filename: ");
                scanf("%s", &newname);
                for(i = 1; i <=elements; i++){
			        if(strcmp(filelist[i][0], filename) == 0){ //if is the filenumber
                        flag = rename(filelist[i][1], newname);
                        break;
                    }
			    }
				if(flag == 0) {
					printf("File renamed! \n\n");
					//locate the file in filelist and rename it
                    for(i = 1; i <= elements; i++){
                        if(strcmp(filelist[i][0], filename) == 0){
                            strcpy(filelist[i][1], newname);
                        }
                    }
				} else {
					printf("Error: Cannot rename file! \n\n");
				}
			} else {
				// file doesn't exist
				printf("Error: File number does not exist! \n\n");
			}

		} else if (choice == 4){ //list files
		    printf("--------------------------------------------------\n");
		    printf("File Number:    Filename:    Blocks:      Size:\n");
            for(i = 1; i <= elements; i++){
                printf("%11s %12s %10s %10s \n", filelist[i][0], filelist[i][1], filelist[i][2], filelist[i][3]);
            }
            printf("--------------------------------------------------\n");
		} else if (choice == 0){
			quit = 1;
		} else {
			printf("\n Invalid Choice Input! \n\n");
		}
		//reset vars
		flag = 0;
		here = 0;
	}
	//write filelist to file
    f = fopen("filelist.txt", "w");
    for(i = 1; i <= elements; i++){
        fprintf(f, "%s %s %s %s \n", filelist[i][0], filelist[i][1], filelist[i][2], filelist[i][3]);
    }
    fclose(f);
}
