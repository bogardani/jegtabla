#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>

struct LakeManager
{
    int *Lake;
    int LakeWidth;
};

struct LakeManager *CreateLakeManager(int lakeWidth, double iceProbability, double resolution)
{
    struct LakeManager *this = NULL;
    this = malloc(sizeof(struct LakeManager));
    this->Lake = malloc(lakeWidth * lakeWidth * sizeof(int));
    this->LakeWidth = lakeWidth;

    for (int i = 0; i < this->LakeWidth; i++)
    {
        for (int j = 0; j < this->LakeWidth; j++)
        {
            int r = rand();
            if (r % (int)(1 / resolution) < iceProbability * (int)(1 / resolution))
            {
                *(this->Lake + i * this->LakeWidth + j) = 1;
            }
            else
            {
                *(this->Lake + i * this->LakeWidth + j) = 0;
            }
        }
    }

    return this;
}

void PrintLake(struct LakeManager *this)
{
    for (int i = 0; i < this->LakeWidth; i++)
    {
        for (int j = 0; j < this->LakeWidth; j++)
        {
            printf("%d  ", *(this->Lake + i * this->LakeWidth + j));
        }
        printf("\n");
    }
    printf("\n");
}

int YPositionInLake(struct LakeManager *this, int *position)
{
    for (int i = 0; i < this->LakeWidth; i++)
    {
        for (int j = 0; j < this->LakeWidth; j++)
        {
            if (this->Lake + i * this->LakeWidth + j == position)
            {
                return i;
            }
        }
    }
    return -1;
}

void DestroyLakeManager(struct LakeManager *this)
{
    free(this->Lake);
    free(this);
}
