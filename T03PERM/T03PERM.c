/* FILE NAME: T03PERM
*  PROGRAMMER: AO5
*  DATE: 03.06.2015
*  PURPOSE: */

#include <stdio.h>

#define N 3

int P[N];
int Par, save, x;

void Swap(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void Write(void)
{
  FILE *F;
  int i;
  if((F = fopen("a.log", "a")) != NULL)
  {                     
    for(i = 0; i < N; i++)
      fprintf(F, "%i", P[i]);
    fprintf(F, " - %s\n", !Par ? "odd" : "even");
    fclose(F);
  }
}

void Go(int Pos)
{
  int i;
  if(Pos == N)
  {
    Write();
    return;
  }
  else
  {
    /*save = Par;
    Go(Pos + 1);
    for(i = Pos + 1; i < N; i++)
    {
      Par = !Par;
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
    }
    Par = save;
    x = P[Pos];
    for(i = Pos + 1; i < N; i++)
      P[i - 1] = P[i];
    P[N - 1] = x;*/
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
  FILE *F;
  if((F = fopen("a.log", "w")) != NULL)
    fclose(F);
  Par = 1;
  for(i = 0; i < N; i++)
    P[i] = i + 1;
  Go(0);
}