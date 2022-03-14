#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "sys/types.h"
#include "regex.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>


#define BUFF_SIZE 1024

        struct Time{
            char name1[20];
            int mon1;
            int day1;
            int hour1;
            int min1;
            int update1;
        };

int main(int argc, char *argv[]) {

        char buff[BUFF_SIZE];

char *input1 = argv[1];
if(argc == 1)
{
        printf("\n");
        printf(" **command description** \n");
        printf(" 'usrlog -a' : Displays all user's summary information \n");
        printf(" 'usrlog -install' : initialize this program \n");
        printf(" 'usrlog -u USER NAME : Displays user's history information \n");
        printf(" 'usrlog -u USER NAME -n NUMBER' : shows the command history of the user \n");
        printf("\n");

        return 0;
}

/*
        char *command = argv[1];
        char *username = argv[2];

        printf("first : %s", command);
        printf("two : %s", username);
*/

if( 0 == strcmp(input1, "-install")) {
        //last command's result copy
        //user process kill
        char output[100];
        FILE *p = popen("last", "r");
        if(p != NULL) {
                char strTemp[255];
                while ( !feof(p) ) {
                        fgets(strTemp, sizeof(strTemp), p);
                        //printf("%s \n",strTemp);

                        if( NULL != strstr(strTemp, "still logged in")) {
                                char *ptr = NULL;
                                char *tmp = strTemp;
                                ptr = strsep(&tmp, " ");

                                if( 0 == strcmp(ptr, "root"))
                                        continue;

                                char *s2 = malloc(sizeof(char) * 20);


                                strcpy(s2, "pkill -u ");
                                strcat(s2, ptr);

                                //printf("%s", s2);
                                system(s2);

                                strcpy(s2, "");
                        }
                }

                while(fgets(output, sizeof(output),p) != NULL);
                //printf("%s \n", output);
        }

        //line read as fgest()
        FILE *pFile = NULL;
        pFile = fopen( "/etc/passwd", "r");

        if( pFile != NULL) {
                char strTemp[255];

                mkdir("/project/result", 0754);

                while( !feof(pFile) ) {

                        fgets( strTemp, sizeof(strTemp), pFile);

                        //find for adduser commend user
                        if( NULL != strstr(strTemp, "/bin/bash") ) {

                                char *UserName;
                                char *UID;
                                char *GID;
                                char *UserInfo;
                                char *userDT;


                                //str extract
                                char *ptr = NULL;
                                int i = 1;

                                char *tmp = strTemp;

                                ptr = strsep(&tmp, ":");
                                while ( ptr != NULL ) {
                                        //printf( "%s \n", ptr); //for test
                                        switch (i) {
                                                case 1:
                                                        UserName = ptr;
                                                break;

                                                case 2:
                                                        //password
                                                break;

                                                case 3:
                                                        UID = ptr;
                                                break;

                                                case 4:
                                                        GID = ptr;
                                                break;

                                                case 5:
                                                        UserInfo = ptr;
                                                break;

                                                case 6:
                                                        userDT = ptr;
                                                break;
                                        }
                                        i++;

                                        ptr = strsep(&tmp, ":");
                                }
                                //for test
                                //printf("name: %s, UID: %s, GID: %s, userinfo: %s, directory: %s \n", UserName, UID, GID, UserInfo, userDT);

                                //if root, continue //read commend
                                if( strcmp(UserName,"root") ) {

                                        //make write folder
                                        char sDir[] = "/project/result/";
                                        strcat(sDir, UserName);
                                        //printf("%s", sDir); //for test
                                        mkdir( sDir, 0644);

                                        //open/write
                                        int fd;
                                        strcat(sDir,"/info");
                                        if ( 0 < (fd = open(sDir, O_WRONLY | O_CREAT, 0644)))
                                        {

                                                lseek(fd, 0, SEEK_SET);
                                                lseek(fd, 10-strlen(UserName), SEEK_CUR);
                                                write(fd, UserName, strlen(UserName) );

                                                lseek(fd, 10-strlen(UID), SEEK_CUR);
                                                write(fd, UID, strlen(UID));

                                                lseek(fd, 10-strlen(GID), SEEK_CUR);
                                                write(fd, GID, strlen(GID));

                                                lseek(fd, 20-strlen(userDT), SEEK_CUR);
                                                write(fd, userDT, strlen(userDT));

                                                close(fd);
                                        } else {
                                                printf( "file open failed\n");
                                        }

                                        char *s3 = malloc(sizeof(char) * 50);
                                        strcpy(s3, "cat /dev/null > ");
                                        strcat(s3, userDT);
                                        strcat(s3, "/.bash_history");
                                        system(s3);
                                        free(s3);

/*
                                        FILE *pFile2 = NULL;
                                        pFile2 = fopen( strcat(userDT,"/.bash_history"), "r");
                                        if( pFile != NULL) {
                                                char strTemp2[255];

                                                while( !feof(pFile2) ) {
                                                        fgets( strTemp2, sizeof(strTemp2), pFile2);
                                                        //printf(" %s ", strTemp2);

                                                }



                                        } else { //need to write error
                                                printf("error detected");
                                        }

*/

                                }
                        }
                }




                fclose(pFile);

                printf("install complete\n");


        } else {
                printf("passwd file open error \n");
        }


                //standard : date
                //write system function & read & delete
                FILE *fp;
                char path[1035];

                fp = popen("/usr/bin/last", "r");
                if( fp != NULL) {
                        while (fgets(path,sizeof(path)-1,fp) != NULL) {
                                char *thisTime;
                                time_t timer;
                                struct tm *t;
                                timer = time(NULL);
                                t = localtime(&timer);
                                int y = t->tm_year + 1900;
                                //printf("year = %d \n", y);

                                char year[4];
                                sprintf(year, "%d", y);

                                char *week;
                                char *mon;
                                char *day;
                                char *time;

                                int i = 0;
                                char *ptr = strtok(path, " ");
                                char *UserName = ptr;
                                while (ptr != NULL)
                                {
                                        i++;
                                        //printf("%s\n", ptr);
                                        ptr = strtok(NULL, " ");

                                        if(i == 3) {
                                                week = ptr;
                                                //printf("week: %s \n", week);

                                        } else if ( i == 4) {
                                                mon = ptr;
                                                //printf("month: %s \n", mon);

                                                if( 0 == strcmp(mon, "Dec")) {
                                                        mon = "12";
                                                        //printf("month %s \n", mon);
                                                } else {
                                                        continue;
                                                }

                                        } else if ( i == 5) {
                                                day = ptr;
                                                //printf("day: %s \n", day);
                                        } else if ( i == 6) {
                                                time = ptr;
                                                //printf("time: %s \n", time);
                                        }

                                }



                                int fdd;

                                if ( 0 < (fdd = open("/project/result/lastest", O_WRONLY | O_CREAT, 0644)))
                                {
                                        //write(fdd, UserName, strlen(UserName) );
                                        //write(fdd, "    ", 4);
                                        write(fdd,year,strlen(year));
                                        write(fdd, "    ", 4);
                                        write(fdd,mon, strlen(mon));
                                        write(fdd, "    ", 4);
                                        write(fdd,day, strlen(day));
                                        write(fdd, "    ", 4);
                                        write(fdd,time,strlen(time));
                                }
                                break;
                        }
                }
                pclose(fp);
        return 0;
}


if( 0 == strcmp(input1, "-update")) {

    char *o_year = "2018";
    char *o_mon;
    char *o_day;
    char *o_hour;
    char *o_min;

    char *UserName;
    char *year = "2018";
    char *week;
    char *mon;
    char *day;
    char *hour;
    char *min;
    char *time;

    FILE *ffp;
    int d;
    ffp = fopen("/project/result/lastest", "r" );

    if( ffp != NULL )
	{
	    char strTemp3[255];
		char *pStr2;
        int ii = 0;
	    while( !feof( ffp ) )
		{
			pStr2 = fgets( strTemp3, sizeof(strTemp3), ffp );
			//printf( "%s", pStr );
            if(ii == 0) {
                int j = 0;
                char *ptr44 = strtok(pStr2, " ");
                while (ptr44 != NULL)
                {
	                if(j == 1) {
                        o_mon = ptr44;
                    } else if (j == 2) {
                        o_day = ptr44;
                    } else if (j == 3) {
                        char *ptr5 = strtok(ptr44, ":");
                        o_hour = ptr5;

                        ptr5 = strtok(NULL, ":");
                        o_min = ptr5;
                    }
                    //printf("%s\n", ptr44);
                    ptr44 = strtok(NULL, " ");
                    j++;
                }
            }
            break;
		}
	}
	fclose(ffp);

    int i_mon;
    int i_day;
    int i_hour;
    int i_min;

    i_mon = atoi(o_mon);
    i_day = atoi(o_day);
    i_hour = atoi(o_hour);
    i_min = atoi(o_min);







    FILE *fp;
    char path[300];
    fp = popen("/usr/bin/last", "r");
    if( fp != NULL) {

        //compare
        while (fgets(path,sizeof(path)-1,fp) != NULL) {

                /* for product
                if( NULL == strstr(path, "still logged in"))
                    continue;
                */

                int i = 0;
                char *ptr = strtok(path, " ");
                UserName = ptr;
                while (ptr != NULL)
                {
                        i++;
                        //printf("%s\n", ptr);
                        ptr = strtok(NULL, " ");

                        if(i == 3) {
                                week = ptr;
                                //printf("week: %s \n", week);

                        } else if ( i == 4) {
                                mon = "12";
                                //printf("month: %s \n", mon);

                        } else if ( i == 5) {
                                day = ptr;
                                //printf("day: %s \n", day);
                        } else if ( i == 6) {
                                time = ptr;
                                char *ptr5 = strtok(time, ":");
                                hour = ptr5;

                                ptr5 = strtok(NULL, ":");
                                min = ptr5;


                        }
                }

                break;
        }

/*
        printf("o_mon is %d \n", i_mon);
        printf("o_year is %s \n", o_year);
        printf("o_min is %d \n", i_day);
        printf("o_min is %d \n", i_min);
        printf("o_min is %d \n", i_hour);

        printf("o_mon is %d \n", atoi(mon));
        printf("o_min is %d \n", atoi(day));
        printf("o_min is %d \n", atoi(min));
        printf("o_min is %d \n", atoi(hour));
*/



         //for test (algoligm)
         int good = 1;
                if( i_mon == atoi(mon) ) {

                        if( i_day  > atoi(day)) {
                            good = 0;
                        } else if ( i_day == atoi(day)) {
                            if( i_hour > atoi(hour)) {
                                good = 0;
                            } else if ( i_hour == atoi(hour)) {
                                if( i_min >= atoi(min)) {
                                    good = 0;

                                }
                            }
                        }

                }
        //printf("test is good1 \n");

        //go save and copy and update
        if(good == 1) {

            //update
            int fdd;
            if ( 0 < (fdd = open("/project/result/lastest", O_RDWR | O_CREAT | S_IROTH, 0644)))
            {
                //write(fdd, UserName, strlen(UserName) );
                //write(fdd, "    ", 4);
                write(fdd,year,strlen(year));
                write(fdd, "    ", 4);
                write(fdd,mon, strlen(mon));
                write(fdd, "    ", 4);
                write(fdd,day, strlen(day));
                write(fdd, "    ", 4);
                write(fdd,hour,strlen(hour));
                write(fdd,":",1);
                write(fdd,min,strlen(min));
            }




            //save
            char *s3 = malloc(sizeof(char) * 100);
            strcpy(s3,"/project/result/");
            strcat(s3,UserName);
            char *info = "/info";
            strcat(s3,info);

            char *s4 = malloc(sizeof(char) * 100);
            strcpy(s4, s3 );
            //strcat(s3,UserName);
            //printf("%s", s3);
            //printf("month: %s \n", mon);

            int fdd1;
            int iiiii;
            char buf[20];

            //printf("month: %s \n", mon);
            fdd1 = open(s3, O_RDWR);
            lseek(fdd1, 0, SEEK_END);
            write(fdd1,"\n",2);
            char *year = "18";
            //printf("month: %s \n", mon);
            write(fdd1,year,strlen(year));
            write(fdd1, "    ", 4);
            write(fdd1,mon, strlen(mon));
            write(fdd1, "    ", 4);
            write(fdd1,day, strlen(day));
            write(fdd1, "    ", 4);
            write(fdd1,hour,strlen(hour));
            write(fdd1,":",1);
            write(fdd1,min,strlen(min));

            free(s3);
            free(s4);



            //find num
            int nResult;
            int i;
            char *s11 = malloc(sizeof(char) * 100);
            for (i = 1; ; i++) {

                char s2[10];
                char *s3 = malloc(sizeof(char) * 100);
                char *s4 = malloc(sizeof(char) * 100);

                strcpy(s3,"/project/result/");
                strcat(s3,UserName);
                sprintf(s2, "%d", i);
                char *info = s2;
                char *slash = "/";

                strcat(s3,slash);
                strcat(s3,info);
                strcpy(s4, s3 );

                nResult = access(s4, 0);

                if( nResult != 0) {
                    nResult = i; //nResult is new number
                    printf("success : %d \n",i);
                    strcpy(s11, s4 );
                    free(s3);
                    free(s4);
                    break;
                } else {
                    free(s3);
                    free(s4);
                }
            }
            printf("%s \n", s11);

            //copy
            char s22[10];
            char *s33 = malloc(sizeof(char) * 100);
            char *s44 = malloc(sizeof(char) * 100);
            strcpy(s33,"/home/");
            strcat(s33,UserName);
            sprintf(s22, "%d", nResult);
            char *info2 = "/.bash_history";
            strcat(s33,info2);
            strcpy(s44, s33 );

            char *s55 = malloc(sizeof(char) * 100);
            char *s66 = malloc(sizeof(char) * 100);

            printf("%s \n", s44);
            printf("%s \n", s11);

            strcpy(s55,"cp ");
            strcat(s55,s44);
            strcat(s55," ");
            strcat(s55,s11);
            strcpy(s66, s55 );
            printf("copy bash_history success \n");

            system(s66);


            //clear
            char *s333 = malloc(sizeof(char) * 100);
            char *s444 = malloc(sizeof(char) * 100);
            strcpy(s333,"cat /dev/null > ");
            strcat(s333,s55);
            strcpy(s444, s333 );
            system(s444);
            printf("clear bash_history success \n\n");
        }
    }
}



if( 0 == strcmp(input1, "-u")) {


        if( argv[3] != NULL && 0 == strcmp(argv[3], "-n") ) {

            printf("%s's number %s command history \n", argv[2], argv[4]);

            char *s8 = malloc(sizeof(char) * 100);
            char *s9 = malloc(sizeof(char) * 100);
            strcpy(s8, "/project/result/");
            char *input8 = argv[2];
            strcat(s8,input8);
            char *ttt = argv[4];
            strcat(s8,"/");
            strcat(s8,ttt);
            strcpy(s9, s8);

            //printf("%s", s9);


            char buff2[1];
            int fd2;
            if( 0< (fd2 = open( s9, O_RDONLY ))) {
                while( 0 < read(fd2, buff, 1)) {

                    printf("%s", buff);
                }
                //printf("%s", buff);
            }

             printf("\n");


            return 0;
        }



        char *s33 = malloc(sizeof(char) * 100);
        char *s44 = malloc(sizeof(char) * 100);
        strcpy(s33, "/project/result/");
        char *input2 = argv[2];
        strcat(s33,input2);
        char *tt = "/info";
        strcat(s33,tt);
        //printf("%s",s33);


        char *result = malloc(sizeof(char) * 500);
        strcpy(result, "");

        char buff[1];
        int fd;
        if( 0< (fd = open( s33, O_RDONLY ))) {


            int f =0;
            while( 0 < read(fd, buff, 1)) {
                strcat(result, buff);
                //printf("%s", buff);
            }

            int j = 0;
            char *result2 = malloc(sizeof(char) * 500);
            //printf("%s\n", result);


            char *ptr0 = strtok(result, "\n");
            strcpy(result2, ptr0 );

            while (result2 != NULL)
            {
                if( j == 0) {
                    printf("%s's connection history \n", argv[2] );

                } else if( j != 0) {
                    printf( "%d. %s \n", j,result2 );
                }

/*
                if( j == 0) {
                    printf("%s's connection history \n", argv[2] );
                    j++;
                } else {

                    char *ptr44 = strtok(result2, " ");
                    //printf( "%s", ptr44 );
                    int jj = 0;


                    while(1) {
                        if( jj == 0) {

                        } else if(jj == 1) {
                            //printf("%s", ptr44);
                            printf("%d. 2018-%s",j,ptr44);
                        } else if (jj == 2) {
                            printf("-%s ",ptr44);
                        } else if (jj == 3) {

                            char *ptr5 = strtok(ptr44, ":");
                            printf("%s",ptr5);
                            ptr5 = strtok(NULL, ":");
                            printf(":%s\n",ptr5);
                            break;
                        }
                        //printf("%s\n", ptr44);
                        ptr44 = strtok(NULL, " ");
                        jj++;
                    }

                }
*/

                result2 = strtok(NULL, "\n");
                j++;
            }


        }


        return 0;

		    /*
			pStr2 = fgets( strTemp3, sizeof(strTemp3), fp );
			printf("%s", strTemp3);
			//printf( "%s", pStr );
            if(i != 0) {
                int j = 0;
                char *ptr44 = strtok(pStr2, " ");
                printf( "%s", ptr44 );
                while (ptr44 != NULL)
                {
	                if(j == 1) {
                        printf("%s", ptr44);
                        mon = ptr44;
                    } else if (j == 2) {
                        day = ptr44;
                    } else if (j == 3) {
                        char *ptr5 = strtok(ptr44, ":");
                        hour = ptr5;

                        ptr5 = strtok(NULL, ":");
                        min = ptr5;
                    }
                    //printf("%s\n", ptr44);
                    ptr44 = strtok(NULL, " ");
                    j++;
                }

                printf("%d. login 2018-%s-%s %s:%s \n", i, mon, day, hour, min);
            } else {
                printf("%s's connection history \n", argv[2] );

            }
            i++;
            */

}










































//for product
if( 0 == strcmp(input1, "--update")) {
        //need cron!
        //last compare && copy file
        // 1,2,3,4,5 line insert
        //soooooooooooooooooooooooooooooooooooooo complicated


        struct Time t1;
        struct Time t2;
        struct Time t3;
        t1.update1 = 0;
        t2.update1 = 0;
        t3.update1 = 0;

        FILE *ffp;
        char path2[1035];
        int d;
        ffp = fopen("/project/result/lastest", "r" );


	if( ffp != NULL )
	{
		char strTemp3[255];
		char *pStr2;
        int ii = 0;

		while( !feof( ffp ) )
		{
			pStr2 = fgets( strTemp3, sizeof(strTemp3), ffp );
			//printf( "%s", pStr );


            if(ii == 0) {
                int j = 0;
                char *ptr4 = strtok(pStr2, " ");
                while (ptr4 != NULL)
                {

                    if(j == 1) {
                        t1.mon1 = atoi(ptr4);
                    } else if (j == 2) {
                        t1.day1 = atoi(ptr4);
                    } else if (j == 3) {
                        char *ptr5 = strtok(ptr4, ":");
                        t1.hour1 = atoi(ptr5);

                        ptr5 = strtok(NULL, ":");
                        t1.min1 = atoi(ptr5);
                    }

                    //printf("%s\n", ptr4);
                    ptr4 = strtok(NULL, " ");
                    j++;
                }
            }


            /* for product
            else {
                //input user
                if(ii == 1) {
                    int j = 0;
                    char *ptr4 = strtok(pStr2, " ");
                    strcpy(t2.name1, ptr4);
                    while (ptr4 != NULL)
                    {

                        if(j == 1) {
                            t2.mon1 = atoi(ptr4);
                        } else if (j == 2) {
                            t2.day1 = atoi(ptr4);
                        } else if (j == 3) {
                            char *ptr5 = strtok(ptr4, ":");
                            t2.hour1 = atoi(ptr5);

                            ptr5 = strtok(NULL, ":");
                            t2.min1 = atoi(ptr5);
                        }

                        printf("%s\n", ptr4);
                        ptr4 = strtok(NULL, " ");
                        j++;
                    }
                } else if ( ii == 2 ) {
                    int j = 0;
                    char *ptr4 = strtok(pStr2, " ");
                    strcpy(t3.name1, ptr4);
                    while (ptr4 != NULL)
                    {

                        if(j == 1) {
                            t2.mon1 = atoi(ptr4);
                        } else if (j == 2) {
                            t2.day1 = atoi(ptr4);
                        } else if (j == 3) {
                            char *ptr5 = strtok(ptr4, ":");
                            t2.hour1 = atoi(ptr5);

                            ptr5 = strtok(NULL, ":");
                            t2.min1 = atoi(ptr5);
                        }

                        printf("%s\n", ptr4);
                        ptr4 = strtok(NULL, " ");
                        j++;
                    }
                }
            }
            ii++;
            */

            break; //for test

		}
		fclose( ffp );
	}



    //standard : date
    //write system function & read & delete
    FILE *fp;
    char path[1035];



    fp = popen("/usr/bin/last", "r");
    if( fp != NULL) {

        //compare
        while (fgets(path,sizeof(path)-1,fp) != NULL) {

                /* for product
                if( NULL == strstr(path, "still logged in"))
                    continue;
                */

                char *thisTime;

                char *week;
                char *mon;
                char *day;
                char *hour;
                char *min;
                char *time;

                int i = 0;
                char *ptr = strtok(path, " ");
                char *UserName = ptr;
                while (ptr != NULL)
                {
                        i++;
                        //printf("%s\n", ptr);
                        ptr = strtok(NULL, " ");

                        if(i == 3) {
                                week = ptr;
                                //printf("week: %s \n", week);

                        } else if ( i == 4) {
                                mon = ptr;
                                //printf("month: %s \n", mon);

                                if( 0 == strcmp(mon, "Dec")) {
                                        mon = "12";
                                        //printf("month %s \n", mon);
                                } else {
                                        break;
                                }

                        } else if ( i == 5) {
                                day = ptr;
                                //printf("day: %s \n", day);
                        } else if ( i == 6) {
                                time = ptr;
                                char *ptr5 = strtok(time, ":");
                                hour = ptr5;

                                ptr5 = strtok(NULL, ":");
                                min = ptr5;
                        }
                }


                //for test (algoligm)
                if( t1.mon1 == atoi(mon) ) {

                        if( t1.day1 > atoi(day)) {
                          break;
                        } else if (t1.day1 == atoi(day)) {
                            if(t1.hour1 > atoi(hour)) {
                                break;
                            } else if (t1.hour1 == atoi(hour)) {
                                if(t1.min1 >= atoi(min)) {
                                    break;
                                }
                            }
                        }
                }
                printf("month: %s \n", mon);
                printf("day: %s \n", day);
                //bring user
                char name[20];
                strcpy(name, UserName);
                //printf("%s", UserName);



                if(strstr(UserName, "user1")) {
                    printf("month: %s \n", mon);
                    printf("day: %s \n", day);
                    //save && copy
/*
                    strcpy(t2.name, UserName);
                    t2.mon = atoi(mon);
                    t2.day = atoi(day);
                    t2.hour = atoi(hour);
                    t2.min = atoi(min);
*/

                    if( strlen(t2.name1) != 0 && t2.mon1 == atoi(mon) ) {
                            if( t2.day1 > atoi(day)) {
                              t2.update1 = 1;
                              continue;
                            } else if (t2.day1 == atoi(day)) {
                                if(t2.hour1 > atoi(hour)) {
                                    t2.update1 = 1;
                                    continue;
                                } else if (t2.hour1 == atoi(hour)) {
                                    if(t2.min1 >= atoi(min)) {
                                        t2.update1 = 1;
                                        continue;
                                    }
                                }
                            }
                    } ////////////////////////이거 앞에 할때는 비교후 이상없으면 업데이트
                    //////////////////////뒤에하면 새롭게 추가시켜줘야함

                    char *s3 = malloc(sizeof(char) * 100);
                    strcpy(s3,"/project/result/user1/info");
                    //strcat(s3,UserName);
                    printf("%s", s3);
                    printf("month: %s \n", mon);


                   int fdd1;
                   int iiiii;
                   char buf[20];

                    printf("month: %s \n", mon);
                   fdd1 = open("/project/result/user1/info", O_RDWR);
                   lseek(fdd1, 0, SEEK_END);
                   write(fdd1,"\n",2);
                   char *year = "18";
                    printf("month: %s \n", mon);
                   write(fdd1,year,strlen(year));
                   write(fdd1, "    ", 4);
                   write(fdd1,mon, strlen(mon));
                   write(fdd1, "    ", 4);
                   write(fdd1,day, strlen(day));
                   write(fdd1, "    ", 4);
                    printf("month: %s \n", time);
                   write(fdd1,time,strlen(time));




                } else if (NULL == strstr(UserName, "user1")) {

                    //date compare && save

                }

        }
    }

}

}
