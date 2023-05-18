 /*
 * Created on November 29, 2021, 5:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <ctype.h> 
#include <sys/stat.h>
#include <time.h>

// THESE VARIABLES ARE USED TO STORE THE AVAILABILITY OF '-m' '-t' and '-c' ARGUMENTS
bool mflag = false;
bool tflag = false;
bool cflag = false;

time_t start,end;   // used to calculate duration and time

int wcount=0;     // word count  
int ecount=0;     // characters with errors count
int ewcount=0;    // words with error count
int chrcount=0;   // character count
int ccount=0;     // converted characters count
float duration=0; //    execution time

// THESE TWO ARRAYS STORE CHARACTERS AND THEIR CORRESPONDING MORSE CODE
const char chr[] = {'\0','\n',' ','a','b','c','d','e','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','1','2','3','4','5','6','7','8','9','0','.',',',':','?','\'','-','/','(',')','\"','=','+','@'};
const char *morse[] = {"\0","\n","/",".-","-...","-.-.-","-..",".","..-..","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..",".----","..---","...--","....-",".....","-....","--...","---..","----.","-----",".-.-.-","--..--","---...","..--..",".----.","-....-","-..-.","-.--.","-.--.-",".-..-.","-...-",".-.-.",".--.-.","...-.","........","-.-",".-...","...-.-","-.-.-"};
// LENGTH OF ABOVE ARRAYS
const int chrlen = sizeof(chr)/sizeof(chr[0]);
const int morselen = sizeof(morse)/sizeof(morse[0]);

// array for month
const char *mon[] = {"Jan", "Feb", "Mar", "Apr" , "May" , "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


// THIS FUNCTION IS USED TO REMOVE EXTENSION FROM A FILE NAME

char *removeExt(char *filename){
    size_t len = strlen(filename);          // get filename length
    char *newfilename = malloc(len);        // allocate same memory for new filename
    for(int i = len-1; i>=0 ; i--){         // check filename backward 
        if(filename[i]=='.'){               // if a '.' exists 
            memcpy(newfilename,filename,i);     // remove extension and assign to new filename
            newfilename[i]=0;
            break;
        }
        else
            newfilename=filename;           // if extension does not exists, keep it as it is 
    }
    return newfilename;
}


//THIS FUNCTION IS USED TO PRINT STATISTICS OF THE SUCCESSFULL CONVERSION

int printstat(char *input, char *output){
    end = time(NULL);
    struct tm tm = *localtime(&end);
    duration=end-start;
   
    // print statistics if '-c' option is supplied 
    if (cflag==true){
        printf("\nInput file: %s\n", input);
        printf("Output file: %s\n", output);
        printf("Time completed : %d-%s-%02d %d:%d:%d\n", tm.tm_year+1900, mon[tm.tm_mon], tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
        printf("Duration [seconds]: %f\n", duration);
        printf("Word count in input file: %d\n", wcount);
        printf("Word converted: %d\n", wcount-ewcount);
        printf("Word with errors: %d\n", ewcount);
        printf("Total number of characters: %d\n", chrcount);
        printf("Number of characters have been converted: %d\n", ccount);
        printf("Number of characters NOT converted: %d\n\n",ecount);
    }
   
    // generate name for logfile 

    char *newip=removeExt(input);
    char *newop=removeExt(output);
    char logfile[100];
    sprintf(logfile,"%s_%s_%d%d%02d_%d%d%d.log",newip,newop,tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    FILE *log = fopen(logfile, "w");
    // Write into a log file
    fprintf(log,"\nInput file: %s\n", input);
    fprintf(log,"Output file: %s\n", output);
    fprintf(log,"Time completed : %d-%s-%d %d:%d:%d\n", tm.tm_year+1900, mon[tm.tm_mon], tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    fprintf(log,"Duration [seconds]: %f\n", duration);
    fprintf(log,"Word count in input file: %d\n", wcount);
    fprintf(log,"Word converted: %d\n", wcount-ewcount);
    fprintf(log,"Word with errors: %d\n", ewcount);
    fprintf(log,"Total number of characters: %d\n", chrcount);
    fprintf(log,"Number of characters have been converted: %d\n", ccount);
    fprintf(log,"Number of characters NOT converted: %d\n\n",ecount);
    
    FILE *templog = fopen("temp.log","r");                // open temporary log file
    struct stat sb;                     // used to
    stat("temp.log", &sb);              // read templog file
    char *line = malloc(sb.st_size);    // line-by-line
    while(fscanf(templog, "%[^\n] ", line) != EOF){
        fprintf(log, "%s\n", line);
        printf("%s\n", line);
    }
    fclose(templog);
    fclose(log);
    remove("temp.log");		// remove temperory log file
    return (EXIT_SUCCESS);
}


// THIS FUNCTION IS USED TO PRINT THE HELP MESSAGE

int printhelp(){
    char *helpmsg = "Usage: morse [INPUT FILE] [OUTPUT FILE] [OPTIONAL ARGUMENTS]...\nEncode and Decode Morse code from and to readable text.\n\nOptional Commands:\n\t-t\tForce to take input file as a text file and the output file as the Morse code one.(Use this option only if the input file is a text file.Otherwise the output file will be faulty)\n\t-m\tForce to take the input file as Morse code and the output file as the interpreted text file.(Use this option only if the input file is a Morse code file.Otherwise the output file will be faulty)\n\t-c\tPrint the statistics of the successful conversion to the screen.\n\n\t\t-h\tdisplay this help and write it to \'readme.txt\' file\n\nThe order of the command line arguments must be considered. Output filename must be parsed after the input filename and optional arguments should be parsed at the end.Both \'-m\' and \'-t\' options cannot be used together and \'-h\' option cannot be used with other arguments. The maximum number of optional argument can be parsed together is 2 (either \'-m\' or \'-t\' combined with \'-c\')\n\nStatistic of a successful conversion is writed into a file named as <inputfile>_<outputfile>_<date>_<time>.log\n\teg:\tinput_output_20211115_10100.log";
    printf("%s",helpmsg); 	// print above help message
    // write it into readme file
    FILE *readme = fopen("readme.txt", "w");
    fprintf(readme, "%s", helpmsg);
    fclose(readme);
    return (EXIT_SUCCESS);
}



//THIS FUNCTION IS USED TO ENCODE TEXT INTO MORSE CODE
//AND CODE FOR COUNTING SUCCESSFULL CONVERSIONS AND ERRORS ARE ALSO INCLUDED

int encoder(char* input, char* output){
    FILE *fp = fopen(input,"r");        // open input file
    FILE *templog = fopen("temp.log","a");                // open temporary log file
    char ch;
    int sr;
    bool ew=false; 	// error word
    int i,linecount=1;
    int temp=0;   // used to help count words with errors
    FILE *opfp = fopen(output,"w"); // open output file for overwriting
    sr = fscanf(fp , "%c", &ch);    //read input file character-by-character
    while(sr!=EOF){                 // iterate until End Of File
        for(i=0;i<chrlen;i++){                  // iterate for all pre-defined characters 
            if(tolower(ch)==chr[i]){                            // if character is known
                fprintf(opfp, "%s ", morse[i]);                 // write corresponding morse code
                ccount++;                                     // increment converted character count
                break;
            }
        }
        temp++;
        if(ccount!=temp) {
            ecount++;      // if there was a character with error, these counts should differ. So increment error count
            fprintf(templog,"Error 05: Invalid character %c on line %d\n" , ch,linecount);	// write error message to templog
	    ew=true;
        }
        temp=ccount;                    // set count for next word
        chrcount++;                     // increment character count
        if(ch==' ' || ch=='\n' || ch=='\t' || ch=='\0'){    // if character is a whitespace
            wcount++;                                       // increment word count
            ccount--;                                       // and prevent it from being counted as a character
            temp--;
            chrcount--;
	    if(ew) ewcount++;
	    ew=false;
            if(ch=='\n') linecount ++;
        }
        sr = fscanf(fp , "%c", &ch);    // read next character
    }
    fclose(fp);                 
    fclose(opfp);                   // close files
    fclose(templog);
    return (EXIT_SUCCESS);
}



//THIS FUNCTION IS USED TO DECODE MORSE CODE INTO TEXT
//AND CODE FOR COUNTING SUCCESSFULL CONVERSIONS AND ERRORS ARE ALSO INCLUDED

int decoder(char* input, char* output){
    FILE *fp = fopen(input,"r");                    // open input file
    FILE *templog = fopen("temp.log","a");                // open temporary log file
    struct stat sb;                     // used to
    stat(input, &sb);                   // read input file
    char *line = malloc(sb.st_size);    // line-by-line
    char *ch;
    int i,linecount=0;
    bool errorword=false;
    FILE *opfp = fopen(output,"w");                 // open output file for overwriting
    while(fscanf(fp, "%[^\n] ", line) != EOF){      // read line-by-line
        linecount++;
        ch = strtok(line, " ");                     // read morse code
        while(ch != NULL){
            if(strcmp(ch,"/")==0) {
                fprintf(opfp, " ");
                wcount++;         // increment word count when '/' occur
                if(errorword) ewcount++;
                errorword=false;
            }
            // following morse code are not defined in the above array
            else if(strcmp(ch,"...-.")==0){
                fprintf(opfp, "Understood");
                ccount++;
            }
            else if(strcmp(ch,"........")==0){
                fprintf(opfp, "Error");
                ccount++;
            }
            else if(strcmp(ch,"-.-")==0){
                fprintf(opfp, "Invitation to transmit");
                ccount++;
            }
            else if(strcmp(ch,".-...")==0){
                fprintf(opfp, "Wait");
                ccount++;
            }

            else if(strcmp(ch,"...-.-")==0){
                fprintf(opfp, "End of work");
                ccount++;
            }
            else if(strcmp(ch,"-.-.-")==0){
                fprintf(opfp, "Starting signal");
                ccount++;
            }
            else if(sizeof(char)<=7){
                bool found=false;
                for(i=0;i<morselen;i++){                // iterate for all pre-defined morse code
                    if(strcmp(ch,morse[i])==0){         // if morse code matched
                        fprintf(opfp, "%c", chr[i]);    // write corresponding character
                        ccount++;                       // increment converted character count
                        found=true;
                        break;
                    }
                }
                if(found==false){
                    fprintf(opfp, "*");
                    fprintf(templog,"Error 04: Invalid Morse code %s on line %d\n" , ch,linecount);
                    ecount++;
                    errorword=true;
                }
            }    
            else{
                fprintf(opfp, "#");
                fprintf(templog,"Error 04: Invalid Morse code %s on line %d\n" , ch,linecount);
                ecount++;
                errorword=true;
            }
            ch = strtok(NULL, " ");     // read next morse code
        }
        fprintf(opfp, "\n");            // one line ends. print new line 
        if(errorword) {
		ewcount++;
		errorword=false;
	}
        wcount++;
    }
                           // count last word
    chrcount=ccount+ecount;         // count total characters
    fclose(fp);                 
    fclose(opfp);                   // close files
    fclose(templog);
    return (EXIT_SUCCESS);
}


// USED TO CHECK WHETHER THE INPUT FILE IS MORSE CODE OR A TEXT FILE

int selector(char* input,char* output){
    FILE *fp = fopen(input, "r");
    char ch;
    char str[] = {'.','-','/',' ','\n','\t'};		// only these characters can exists on a Morse code file
    bool text = false;
    while((ch = fgetc(fp)) != EOF){
        if (strchr(str,ch)==NULL){	// if anothe character exists
            text = true;		// mark it as a text file
            break;
        }
    }
    fclose(fp);
    if (text == false)			// if Morse code, decode it
        decoder(input,output);
    else
        encoder(input,output);		// if text, encode it
    return (EXIT_SUCCESS);
}


// CHECK VALIDITY OF COMMAND LINE ARGUMENTS AND INPUT,OUTOUT FILES 

int validityChecker(int argc, char *argv[]){
    if (argc>5)
        printf("Too many arguments. Type \"morse -h\" for help");
    else if (argc<2)
        printf("Error 01: Unrecognized Command. Type \"morse -h\" for help");
    else if (argc==2){
        if (strcmp(argv[1],"-h")==0)
            printhelp();
        else
            printf("Error 00: Unknown Command. Type \"morse -h\" for help");
    }
    else{
        if(argc>3){
            if (strcmp(argv[3],"-m")==0)
                mflag = true;
            else if (strcmp(argv[3],"-t")==0)
                tflag = true;
            else if (strcmp(argv[3],"-c")==0)
                cflag = true;
            else{
                printf("Error 00: Unknown Command. Type \"morse -h\" for help");
                return 1;
            }
        }
        if(argc>4){
            if (strcmp(argv[4],"-m")==0)
                mflag = true;
            else if (strcmp(argv[4],"-t")==0)
                tflag = true;
            else if (strcmp(argv[4],"-c")==0)
                cflag = true;
            else if (mflag==tflag==true){
                printf("Error 00: Unknown Command.\nBoth \'-m\' and \'-t\' arguments cannot be used together.\nType \"morse -h\" for help");
                return 1;
            }
            else{
                printf("Error 00: Unknown Command. Type \"morse -h\" for help");
                return 1;
            }
        }
        FILE *fp = fopen(argv[1], "r");
        if (fp==NULL){
            if(strstr("-m -h -c -t",argv[1]) != 0)
                printf("Error 00: Unknown Command.\nOptional arguments should be parsed after <inputfile> <outputfile>.\nType \"morse -h\" for help");
            else
                printf("Error 02: %s could not be opened",  argv[1] );
            return 1;
        }
        fclose(fp);
        FILE *opfp = fopen(argv[2], "r");
        if (opfp!=NULL){
            char c;
            fclose(opfp);
            printf("Warning: %s already exists. Do you wish to overwrite (y,n)?", argv[2]);
            scanf("%c", &c);
            if (c=='y'){
                FILE *opfp = fopen(argv[2], "w");
                if (opfp==NULL){
                    printf("Error 03: %s is not allowed to be overwrited", argv[2]);
                    return 1;
                }
                fclose(opfp);
            }
            else{
                printf("Exiting without processing");
                return 1;
            }
        }
        if(mflag==true)
            decoder(argv[1],argv[2]);
        else if(tflag==true)
            encoder(argv[1],argv[2]);
        else
            selector(argv[1],argv[2]);
        printstat(argv[1],argv[2]);
    }
    return (EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    start = time(NULL);
    validityChecker(argc,argv);
    return (EXIT_SUCCESS);
}

