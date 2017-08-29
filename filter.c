/********************************************************************************/
/* This code will remove the baseline interference by calculating the fft and 
then removing the lower frequencies from the frequency spectrum. After that the
ifft of the plot is calculated.
/********************************************************************************/
#include <stdio.h>
#include <math.h>
#include <complex.h>
 
double PI;
typedef double complex cplx; 


void fft1(cplx buf[], cplx out[], int n, int step) // This is the recursive function which computes the N-point fft
{
	int i = 0;
	if (step < n) {
		fft1(out, buf, n, step * 2);
		fft1(out+step, buf+step, n, step * 2);
		for (i = 0; i < n; i += 2 * step)
		{
			cplx t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}
void fft(cplx buf[], int n) // This is the fft function which internally calls the fft1 function to compute the fft
{
	int i = 0;
	cplx out[n];
	for (i = 0; i < n; i++)
	{
		out[i] = buf[i];
	}
	fft1(buf, out, n, 1);
	
}
 
 
complex conjg(cplx z) // This function returns the conjugate of the given complex number
{
	return (creal(z) -cimag(z));
}

complex rdiv(cplx z, int a) // This function returns complex number by dividing the given complex number with a number
{
	return ( (creal(z)/a) + (cimag(z)/a) );
}

void ifft(complex buf[], int k) // This function computes the inverse fft of the input array
{
	int i = 0;
	for (i = 0; i < k; i++)
	{
		buf[i] = conjg(buf[i]); // calls the conjugate function
		
	}
	fft(buf, k); // calls the fft function 
	
	for (i = 0; i < k; i++)
	{
		buf[i] = rdiv(conjg(buf[i]), k); // divides the complex number and finds its conjugate
	}
}

int main(int argc , char* argv[]) // gets the command line arguments
{
	
	complex X[100000];
	double var;
	PI = atan2(1, 1) * 4; // initializes the PI constant
	int i =0,size =0, org_size=0;
	FILE* fin;
	fin = fopen(argv[1],"r"); // opens the file for reading the input data
	while(!feof(fin))
	{
		fscanf(fin,"%lf",&var);
		X[i] = var + I*0.0; // converting the given float number into a complex number for computing fft of the signal
		i += 1;
	}
	fclose(fin);
	org_size = i-1;
	size = i-1;
	int next = pow(2, ceil(log(size)/log(2))); // For computing the N-point fft we find the next power of 2 from the given input size
	
	for(i = size-1; i < next; i++)
		X[i] = 0.0 + 0.0*I; // appending zeroes to make the size as a power of 2
	
	size = next;
	fft(X,size); // calling the fft function to compute the frequency domain numbers
	
	for (i=0; i < 100 ; i++) //baseline interference removal
	{
		X[i] = 0 + I*0; // The starting lower frequencies are removed
	}
	
	ifft(X,size); // calling the inverse fft function to convert the frequency domain signal back to time domain signal
	size = org_size;
	
	FILE *fin1;
	fin1 = fopen(argv[2],"w"); // opening the file to write the output into it
	for (i = 0; i < size; i++)
	{
		fprintf(fin1,"%g\n", creal(X[i]));
	}
	fclose(fin);
	
	/*------------------------------------------------------------------------------------------------------------------------
	// THE FOLLOWING CODE WILL PLOT THE TIME DOMAIN SIGNAL AFTER BASELINE INTERFERENCE REMOVAL
	FILE *gnuplot = popen("gnuplot -persistent","w");
	fprintf(gnuplot,"plot'-'\n");
	int j= 0,k=0;
	
	for (j=0;j<=size;j++)
	{
		fprintf(gnuplot,"%g \n", creal(X[j]));
		k++;
	}

	fprintf(gnuplot,"e\n");
	fflush(gnuplot);

	return 0;
	--------------------------------------------------------------------------------------------------------------------------*/
}



