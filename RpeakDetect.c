/*****************************************************************************************/
/* This code takes the cleaned signals as the input parameter. This will detect all the 
R-peaks and write the output of the peaks is written into the output file.                                                    
/*****************************************************************************************/
#include<stdio.h>
int main(int argc, char * argv[]){
	if(argc != 3){
		printf("Check input arguments - ./RpeakDetect inputfile outputfile");
	}
	FILE *inputfile, *outputfile; /*file handlers*/
	long double ecg_filtered[150], temp1[149], output[148], reader; /*array for double differentiation*/
	int iter1 = 0, iter2 = 0, iter3 = 0;
	int window_curr = 0, max = 0;
	inputfile = fopen(argv[1],"r");
	outputfile = fopen(argv[2],"w");
	fprintf(outputfile,"Following samples are associated with R-peaks\n");
	while(fscanf(inputfile,"%Lf",&reader) != EOF){ /*iterate through the input samples*/
		ecg_filtered[iter1] = reader*10000000; /*amplify the sample value for ease of differentiation*/
		iter1 = iter1 + 1;
		if(iter1 == 150){ /*once a window size of samples has been read in*/
			for(iter2 = 0; iter2 < 149; iter2++){
				temp1[iter2] = ecg_filtered[iter2 + 1] - ecg_filtered[iter2]; /*first difference*/
			}
			for(iter3 = 0; iter3 < 148; iter3++){
				output[iter3] = (temp1[iter3 + 1] - temp1[iter3])*(temp1[iter3 + 1] - temp1[iter3]); /*second difference and squaring*/
			}
			max = 0;
			for(iter3 = 0; iter3 < 148; iter3++){ /*determine peak value in each window*/
				if(output[iter3] > output[max]){
					max = iter3;
				}
			}
			fprintf(outputfile,"%d \n",150*window_curr + max +1); /*output the peak value*/
			iter1 = 0;
			window_curr = window_curr + 1;
		}
	}
	fprintf(outputfile,"The heart-beat rate is: %d beats per minute",100); /*time in seconds for 1 heartbeat = 250/150, 1 minute - 100beats*/
	fclose(inputfile);
	fclose(outputfile);
}
