#include<iostream>//including required header files
#include<conio.h>
#include<cstdlib>
#include<ctime>
#include<math.h>
#include<fstream>
using namespace std;
int iter;
double cdef,mdef;
double costMatrix[48][48];
double rewardMatrix[48][48];
int chromosomes[48][288];
int combinations[48][48];//stores decoded chormosomes
double fitness[48];//stores fitness of decoded chromosomes
int cityBuffer[48];//buffer to identify the cities decoded and used for each chromosomes
int elite[100000][48];//stores elite
double elscore[100000];
double cost[100000];
int elcount=0;
int tempchrom[48][288];
int coords[48][2];
int genChrom()//generating binary chromosomes
{
	int i,j;
	for(i=0;i<48;i++)
	{
		for(j=0;j<288;j++)
		{
			chromosomes[i][j]=(rand()%2);
		}
	}
}
double findDist(int i, int j)// udf to find reward between two cities
{
	double r;
	if(i==j)
		return -1.0;
	else
	{
		r=costMatrix[i][j];
		return r;
	}
}
double dist(int i, int j)
{
	double sum,a,b;
	a=pow((coords[j][0]-coords[i][0]),2);
	b=pow((coords[j][1]-coords[i][1]),2);
	sum=sqrt((a+b)/10.0);
	return sum;
}
int genCost()
{
	int i,j;
	double max=0.0,d=0;
	for(i=0;i<48;i++)
	{
		for(j=i+1;j<48;j++)
		{
			d=dist(i,j);
			costMatrix[i][j]=d;
			costMatrix[j][i]=d;
		}
	}
	for(i=0;i<48;i++)
	{
		for(j=0;j<48;j++)
		{
			if(max<costMatrix[i][j])
				max=costMatrix[i][j];
		}
	}
	cout<<endl<<"Max: "<<max<<endl;
	for(int i=0;i<48;i++)
	{
		for(int j=0;j<48;j++)
		{
			if(i!=j)
				rewardMatrix[i][j]=max-costMatrix[i][j];
		}
	}
}
int printCost()
{
	int i,j,c=0;
	cout<<"Generated Cost Matrix:\n \n";
	for(i=0;i<49;i++)
	{
		for(j=0;j<49;j++)
		{
			if(i==0)
			{
				if(i==0 && j==0)
					cout<<" \t";
				else
					cout<<j-1<<"\t";
			}
			else
			{
				if(j==0)
					cout<<c<<"\t";
				else
					cout<<costMatrix[i-1][j-1]<<"\t";
			}
		}
		if(i>=1)
			++c;
		if(i==0)
			cout<<endl;
		cout<<endl;
	}
}
int printReward()
{
	int i,j,c=0;
	cout<<endl<<"Reward Matrix:\n \n";
	for(i=0;i<49;i++)
	{
		for(j=0;j<49;j++)
		{
			if(i==0)
			{
				if(i==0 && j==0)
					cout<<" \t";
				else
					cout<<j-1<<"\t";
			}
			else
			{
				if(j==0)
					cout<<c<<"\t";
				else
					cout<<rewardMatrix[i-1][j-1]<<"\t";
			}
		}
		if(i>=1)
			++c;
		if(i==0)
			cout<<endl;
		cout<<endl;
	}
	cout<<endl;
}
double findReward(int i, int j)// udf to find reward between two cities
{
	double r;
	if(i==j)
		return -1.0;
	else
	{
		r=rewardMatrix[i][j];
		return r;
	}
}
void elitism()
{
	int i,ind;
	double max=0;
	for(i=0;i<48;i++)
	{
		if(max<fitness[i])
		{
			max=fitness[i];
			ind=i;
		}
	}
	for(i=0;i<48;i++)
		elite[elcount][i]=combinations[ind][i];
	elscore[elcount]=max;
	elcount++;
}
int decode() //decoding the chromosomes using the strategy mentioned by the book
{
	int i,j,d,c,k;
	for(k=0;k<48;k++)
	{
		c=0;
		for(i=0;i<48;i++)
		{
			c=i*6;
			d=0;
			for(j=5;j>=0;j--)
			{
				d=d+(chromosomes[k][c+(5-j)]*pow(2,j));
			}
			if(d<48)
			{
				while(cityBuffer[d]!=0)
					d=(d+1)%48;
			}
			else
			{
				d=0;
				while(cityBuffer[d]!=0)
					d=(d+1)%48;
			}
			cityBuffer[d]=1;
			combinations[k][i]=d;
			c++;
		}
		for(i=0;i<48;i++)
			cityBuffer[i]=0;
	}
}
void fitneScore() //finding and storing fitness scores of each chromosomes
{
	int i,j;
	double r;
	decode();
	for(i=0;i<48;i++)
	{
		r=0.0;
		for(j=0;j<47;j++)
			r+=findReward(combinations[i][j],combinations[i][j+1]);
		r+=findReward(combinations[i][47],combinations[i][0]);
		fitness[i]=r;
	}
}
void selection()
{
	int i,j,r1,r2,select;
	for(i=0;i<48;i++)
	{
		r1=rand()%48;
		r2=rand()%48;
		while(r1==r2)
			r1=rand()%48;
		if(fitness[r1]>fitness[r2])
			select=r1;
		else
			select=r2;
		for(j=0;j<288;j++)
		{
			tempchrom[i][j]=chromosomes[select][j];
		}
	}
	for(i=0;i<48;i++)
	{
		for(j=0;j<288;j++)
		{
			chromosomes[i][j]=tempchrom[i][j];
		}
	}
}
void crossover()
{
	int i,j,k,swap,pos1,pos2,chr1,chr2;
	double cp;
	for(i=0;i<24;i++)
	{
		cp=(rand()%1000)/1000.0;
		if(cp<cdef)
		{
			chr1=rand()%48;
			chr2=rand()%48;
			while(chr1==chr2)
				chr1=rand()%48;
			pos1=rand()%288;
			pos2=rand()%288;
			while(pos1==pos2)
				pos1=rand()%288;
			if(pos1>pos2)
			{
				swap=pos1;
				pos1=pos2;
				pos2=swap;
			}	
			for(k=pos1;k<=pos2;k++)
			{
				swap=chromosomes[chr1][i];
				chromosomes[chr1][i]=chromosomes[chr2][i];
				chromosomes[chr2][i]=swap;
			}
		}
	}
}
void mutation()
{
	int i,pos;
	double mp;
	for(i=0;i<48;i++)
	{
		mp=(rand()%1000)/1000.0;
		if(mp<mdef)
		{
			pos=rand()%288;
			if(chromosomes[i][pos]==0)
				chromosomes[i][pos]=1;
			else
				chromosomes[i][pos]=0;
		}
	}
}
void GeneticAlgorithm(int n,double c=0.7,double m=0.01)
{
	int i,j;
	iter=n;
	cdef=c;
	mdef=m;
	genChrom();
	cout<<"Generation : 0"<<endl;
	fitneScore();
	elitism();
	cout<<"Elite: ";
	for(j=0;j<48;j++)
		cout<<elite[elcount-1][j]<<" ";
	cout<<":\t"<<elscore[elcount-1];
	cout<<endl<<endl;
	for(i=1;i<iter;i++)
	{
		if(i%500==0)
			srand(time(NULL));
		selection();
		crossover();
		mutation();
		fitneScore();
		elitism();
	}
}
int main()
{ 
	int n=15000;
	double c=0.7,m=0.05,dist=0.0;
	ifstream inData("tspdistance.txt");
    for (int i=0; i<48; i++) {
    	inData >> coords[i][0] >> coords[i][1]; //Read x and y coordinates at position i
    	cout << coords[i][0] <<"\t"<< coords[i][1] << endl; //Output the x and y coordinates read above
    	cityBuffer[i]=0;
	}
    inData.close();
    genCost();
    printCost();
    printReward();
    //cout<<"No. of Iteration"<<endl;
    //cin>>n;
    //cout<<"Enter Crossover Probability"<<endl;
	//cin>>c;
	//cout<<"Enter Mutation Probability"<<endl;
	//cin>>m;
	srand(time(NULL));
	cout<<endl;
	GeneticAlgorithm(n,c,m);
	cout<<"Generation : "<<n-1<<endl;
	cout<<"Elite: ";
	for(int j=0;j<48;j++)
		cout<<elite[elcount-1][j]<<" ";
	cout<<":\t"<<elscore[elcount-1]<<endl;
	cout<<"Cost: ";
	for(int j=0;j<47;j++)
		dist+=findDist(elite[elcount-1][j],elite[elcount-1][j+1]);
	dist+=findDist(elite[elcount-1][47],elite[elcount-1][0]);
	cout<<dist;
    return 0;
}
