#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#define TOL pow(10,-3)

int main()
{
	int L,M,N,P;
	int i,j,k,p;    //for iterations
	int counter=1;
	float eta=0.9;
	
	srand(time(NULL));
		
	FILE *input1, *input2,*output1, *output2;

		input1 = fopen("Input File 1.txt","r");
		input2 = fopen("Input File 2.txt","r");
		output1  = fopen("OUTPUT DATA_0.9.txt","w");
		output2 = fopen("FINAL_DATA_0.9.txt","w");

	double I[100][100],IH[100][100],OH[100][100],IO[100][100],OO[100][100],TO[100][100];
	double V[100][100],W[100][100],del_W[100][100],del_V[100][100];
	double e,MEANSE;
	double min_I[100],max_I[100],min_TO[100],max_TO[100];
	
	fscanf(input1,"%d%d%d%d",&P,&L,&M,&N);
	fprintf(output2,"NUMBER OF PATTERN USED IN ANN  (P)=%d\n",P);
	fprintf(output2,"NUMBER OF INPUT LAYER USED IN ANN  (L)=%d\n",L);	
	fprintf(output2,"NUMBER OF HIDDEN LAYER USED IN ANN  (M)=%d\n",M);	
	fprintf(output2,"NUMBER OF OUTPUT LAYER USED IN ANN  (N)=%d\n",N);
	
// FROM HERE WE HAVE STARTED TO SCAN THE INPUT FILE VALUES
	for(p=1;p<=P;p++)
	{
		for(i=1;i<=L;i++)
		{
			fscanf(input1,"%lf",&I[i][p]);
		}
	}
	
//PRINTING INPUTS FOR INPUT LAYER IN FINAL OUTPUT VALUES
//ONLY FOR BETTER REPRESENATION
	fprintf(output2,"\nI matrix of order %dX%d :\n",L,P);
	
	for(i=1;i<=L;i++)
	{
		for(p=1;p<=P;p++)
		{
			fprintf(output2,"I[%d][%d]=%lf\t",i,p,I[i][p]);
		}
		fprintf(output2,"\n");
	}
	
//FOR PRINTING OF OUTPUT TARGETED VALUE FROM DATA

	fprintf(output2,"\nTO matrix of order %dX%d :\n",P,N);
	
	for(p=1;p<=P;p++)
	{
		for(k=1;k<=N;k++)
		{
			fscanf(input2,"%lf",&TO[k][p]);                //SCANNING OF ARGETED VALUE
			fprintf(output2,"TO[%d][%d]:%lf\t",k,p,TO[k][p]);
		}
		fprintf(output2,"\n");
	}
//STARTED THE CACULATION

// NORMALISATION OF INPUT VALUES FOR INPUT LAYER 

	for(i=1;i<=L;i++)
	{
		max_I[i]=-1000;min_I[i]=1000;
		
		for(p=1;p<=P;p++)
		{
			if(I[i][p]>max_I[i])
			max_I[i]=I[i][p];
			if(I[i][p]<min_I[i])
			min_I[i]=I[i][p];
		}
	}
	
	for(p=1;p<=P;p++)
	{
		for(i=1;i<=L;i++)
		{
			I[i][p]=0.1+0.8*((I[i][p]-min_I[i])/(max_I[i]-min_I[i]));
		}
	}
	
	fprintf(output2,"\nNormalised I matrix of order %dX%d :\n",L,P);
	
	for(i=1;i<=L;i++)
	{
		for(p=1;p<=P;p++)
		{
			fprintf(output2,"%f\t",I[i][p]);
		}
		fprintf(output2,"\n");
	}


//NORMAISATION OF TARGEED VALUE FOR OUPUT LAYER

for(k=1;k<N+1;k++)
	{
		max_TO[k]=-1000;min_TO[k]=1000;
		for(p=1;p<=P;p++)
		{
			if(TO[k][p]>max_TO[k])
			max_TO[k]=TO[k][p];
			if(TO[k][p]<min_TO[k])
			min_TO[k]=TO[k][p];			
		}
	}
	
	for(p=1;p<=P;p++)
	{
		for(k=1;k<=N;k++)
		{
			TO[k][p]=-0.1+(1.0*((TO[k][p]-min_TO[k])/(max_TO[k]-min_TO[k])));
		}
	}

	fprintf(output2,"\nNormalised TO matrix of order %dX%d :\n",P,N);
	
	for(p=1;p<=P;p++)
	{
		for(k=1;k<N+1;k++)
		{
			fprintf(output2,"TO[%d][%d]:%lf\t",k,p,TO[k][p]);
		}
		fprintf(output2,"\n");
	}
	


// ////CALCULATION FOR AND DEFINATION OF W

	fprintf(output2,"\nW matrix of order %dX%d :\n",M+1,N);
	
	for(j=0;j<M+1;j++)
	{
		for(k=1;k<=N;k++)
		{
			if(j==0)
			{
				W[i][j]=0;
			}
			else
			{
			W[j][k]=1.0*rand()/RAND_MAX;
			}
		}
	}
	
	for(j=0;j<M+1;j++)
	{
		for(k=1;k<=N;k++)
		{
			fprintf(output2,"W[%d][%d]:%f\t",j,k,W[j][k]);
		}
		fprintf(output2,"\n");
	}
//STAREDED CALCULATIONS FOR V
//DEFINE V

	fprintf(output2,"\nV matrix of order %dX%d :\n",L+1,M);

	for(i=0;i<L+1;i++)
	{
		for(j=1;j<=M;j++)
		{
			if(i==0)
			{
				V[i][j]=0;
			}
			else
			{
				V[i][j]=1.0*rand()/RAND_MAX;
			}
		}
	}
	
	for(i=0;i<=L;i++)
	{
		for(j=1;j<=M;j++)
		{
			fprintf(output2,"V[%d][%d]:%f\t",i,j,V[i][j]);
		}
		fprintf(output2,"\n");
	}
	fprintf(output2,"\n");

//TRAINNING PATTERN
	do
	{		
		//Calculation for forward pass
		
		for(p=1;p<=P-5;p++)
		{
			IH[j][p]=0;
			for(j=1;j<M+1;j++)
			{
				for(i=1;i<L+1;i++)
				{
					IH[j][p]=IH[j][p]+(I[i][p]*V[i][j]);
				}
				IH[j][p]=IH[j][p]+(1.0);
				OH[j][p]=1/(1+exp(-IH[j][p]));
				IH[j][p]=0;
			}
			
		}
		fprintf(output1,"\n");
		
		//CALCULATION FOR OUTPUT FOR THE OUTPUT LAYER
		for(p=1;p<=P-5;p++)
		{
			IO[k][p]=0;
			for(k=1;k<N+1;k++)
			{
				for(j=1;j<M+1;j++)
				{
					IO[k][p]=IO[k][p]+OH[j][p]*W[j][k];
				}
				IO[k][p]=IO[k][p]+1.0;
				OO[k][p]=(exp(IO[k][p])-exp(-1*IO[k][p]))/(exp(IO[k][p])+exp(-1*IO[k][p]));
				IO[k][p]=0;
			}
		}
		fprintf(output1,"\n");
		
		// FINDING DELTA W_jk
		for(j=1;j<=M;j++)
		{
			for(k=1;k<=N;k++)
			{
				del_W[j][k]=0;
				for(p=1;p<=P-5;p++)
				{
					del_W[j][k]=del_W[j][k]+((eta/P)*(TO[k][p]-OO[k][p])*(1-(OO[k][p]*OO[k][p]))*OH[j][p]);
				}
				fprintf(output1,"del_W[%d][%d]=%lf\t",j,k,del_W[j][k]);
			}
			fprintf(output1,"\n");
		}

		fprintf(output1,"\n");
		
		//FINDING VALUE OF DELTA V_ij
		for(i=1;i<=L;i++)
		{
			for(j=1;j<=M;j++)
			{
				del_V[i][j]=0;
				for(p=1;p<=P-5;p++)
				{
					for(k=1;k<=N;k++)
					{
						del_V[i][j]=del_V[i][j]+((eta/(P*N))*((TO[k][p]-OO[k][p])*(1-(OO[k][p]*OO[k][p]))*W[j][k]*OH[j][p]*(1-OH[j][p])*I[i][p]));
					}
				}
				fprintf(output1,"del_V[%d][%d]=%lf\t",i,j,del_V[i][j]);
			}
			fprintf(output1,"\n");
		}
		
	//here we are starting The value of error
		MEANSE=0;
		for(p=1;p<=P-5;p++)
		{
			for(k=1;k<=N;k++)
			{
				e=pow((TO[k][p]-OO[k][p]),2)/2;
				MEANSE=MEANSE+e;
			}
		}
		MEANSE=MEANSE/P;
		fprintf(output1,"\nMEANSE=%f\tIteration=%d\n",MEANSE,counter);
		
		//UPDAING VALUE OF Vij
		for(i=1;i<=L;i++)
		{
			for(j=1;j<=M;j++)
			{
				V[i][j]=V[i][j]+del_V[i][j];
				fprintf(output1,"V[%d][%d]:%f\t",i,j,V[i][j]);
			}

			fprintf(output1,"\n");
		}
		fprintf(output1,"\n");
		
       //UPDAING VALUE OF Wij
		for(j=1;j<=M;j++)
		{
			for(k=1;k<=N;k++)
			{
				W[j][k]=W[j][k]+del_W[j][k];
				fprintf(output1,"W[%d][%d]:%f\t",j,k,W[j][k]);
			}
			fprintf(output1,"\n");
		}
		
	printf("\nIteration %d completed",counter);
	
	counter++;

	
	}while(MEANSE>TOL);

	for(i=1;i<=L;i++)
		{
			for(j=1;j<=M;j++)
			{
				fprintf(output2,"V[%d][%d]:%f\t",i,j,V[i][j]);
			}
			fprintf(output2,"\n");
		}
		fprintf(output2,"\n");
		
		//UPDAING VALUE  OF Wjk
		for(j=1;j<=M;j++)
		{
			for(k=1;k<=N;k++)
			{
				fprintf(output2,"W[%d][%d]:%f\t",j,k,W[j][k]);
			}
			fprintf(output2,"\n");
		}
		
	//TESING OF PATERN
	
	//HERE WE ARE SARING FORWARD PASS
		for(p=31;p<=35;p++)
		{
			IH[j][p]=0;
			for(j=1;j<M+1;j++)
			{
				for(i=1;i<L+1;i++)
				{
					IH[j][p]=IH[j][p]+(I[i][p]*V[i][j]);
				}
				IH[j][p]=IH[j][p]+1.0;
				OH[j][p]=1/(1+exp(-IH[j][p]));
				fprintf(output2,"\nIH[%d][%d]:%f\tOH[%d][%d]:%f",j,p,IH[j][p],j,p,OH[j][p]);
				IH[j][p]=0;
			}
			
		}

		fprintf(output2,"\n\n");
		
		//Calculation for Output of Output layer
		for(p=31;p<=35;p++)
		{
			IO[k][p]=0;
			for(k=1;k<N+1;k++)
			{
				for(j=1;j<=M+1;j++)
				{
					IO[k][p]=IO[k][p]+OH[j][p]*W[j][k];
				}
				IO[k][p]=IO[k][p]+1.0;
				OO[k][p]=(exp(IO[k][p])-exp(-1*IO[k][p]))/(exp(IO[k][p])+exp(-1*IO[k][p]));
				fprintf(output2,"\nIO[%d][%d]:%f\tOO[%d][%d]:%f\tTO[%d][%d]:%f,\tError=%f",k,p,IO[k][p],k,p,OO[k][p],k,p,TO[k][p],fabs(OO[k][p]-TO[k][p]));
				IO[k][p]=0;
			}
		}
	
	fclose(input1);
	fclose(output1);
	fclose(output2);

	return 0;
}
