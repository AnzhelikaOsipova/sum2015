/* FILE NAME: T04DETERM
*  PROGRAMMER: AO5
*  DATE: 04.06.2015
*  PURPOSE: */

#include <stdio.h>

#define MAX 10

double A[MAX][MAX], SUM = 0;
int P[MAX];
int N, Par;


void Swap(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void Load( char *FileName )
{
  FILE *F;
   int i, j;
  if ((F = fopen(FileName, "r")) != NULL)
  {
    fscanf(F, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(F, "%lf", &A[i][j]);
    fclose(F);
  }
}

void Count(void)
{
  int i;
  double Prod = 1;
  for(i = 0; i < N; i++)
    Prod *= A[i][P[i] - 1];
  if(Par == 1)
    SUM += Prod;
  else
    SUM -= Prod;
}

void Go(int Pos)
{
  int i;
  if(Pos == N)
  {
    Count();
    return;
  }
  else
  {
    for(i = Pos; i < N; i++)
    {
      if(Pos != i)
        Par = !Par;
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
      if(Pos != i)
        Par = !Par;
      Swap(&P[Pos], &P[i]);
    } 
  }
}

void main(void)
{
  int i;
  Par = 1;
  Load("m4.txt");
  for(i = 0; i < N; i++)
    P[i] = i + 1;
  Go(0);
  printf("%lf", SUM);
  _getch();
}