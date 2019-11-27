#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>

struct Result
{
    double FoeProbability;
    double CrossProbability;
};

struct Result *CreateResult(double foeProbability, double crossProbability)
{
    struct Result *tmp = NULL;

    tmp = malloc(sizeof(struct Result));
    tmp->FoeProbability = foeProbability;
    tmp->CrossProbability = crossProbability;

    return tmp;
}

void PrintResult(struct Result *this)
{
    printf("FoeProbability: %.3f CrossProbability: %.3f\n", this->FoeProbability, this->CrossProbability);
}

void WriteResultToFile(struct Result *this, FILE *fp)
{
    fprintf(fp, "%.3f\t%.3f\n", this->FoeProbability, this->CrossProbability);
}

void DestroyResult(struct Result *this)
{
    free(this);
}