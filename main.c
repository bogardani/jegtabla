#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "lake.c"
#include "islands.c"
#include "result.c"

void scanIslands(struct IslandManager *islandManager, struct LakeManager *lakeManager)
{
    for (int i = 0; i < lakeManager->LakeWidth; i++)
    {
        for (int j = 0; j < lakeManager->LakeWidth; j++)
        {
            int *foe = lakeManager->Lake + i * (lakeManager->LakeWidth) + j;

            if (*foe == 1)
            {
                int *upperFoe = NULL, *leftFoe = NULL;
                int upperIndex = -1, leftIndex = -1;

                if (i != 0)
                {
                    upperFoe = lakeManager->Lake + (i - 1) * (lakeManager->LakeWidth) + j;
                    if (*upperFoe == 1)
                    {
                        upperIndex = GetIslandIndex(islandManager, upperFoe);
                    }
                }
                if (j != 0)
                {
                    leftFoe = lakeManager->Lake + i * lakeManager->LakeWidth + j - 1;
                    if (*leftFoe == 1)
                    {
                        leftIndex = GetIslandIndex(islandManager, leftFoe);
                    }
                }

                if (upperIndex == -1 && leftIndex == -1)
                {
                    AddNewIsland(islandManager, foe);
                }
                else
                {
                    if (upperIndex == leftIndex)
                    {
                        AddToIsland(islandManager, foe, upperIndex);
                    }
                    else if (upperIndex == -1)
                    {
                        AddToIsland(islandManager, foe, leftIndex);
                    }
                    else if (leftIndex == -1)
                    {
                        AddToIsland(islandManager, foe, upperIndex);
                    }
                    else
                    {
                        MergeIslands(islandManager, leftIndex, upperIndex);
                        if (leftIndex > upperIndex)
                        {
                            AddToIsland(islandManager, foe, upperIndex);
                        }
                        else
                        {
                            AddToIsland(islandManager, foe, upperIndex - 1);
                        }
                    }
                }
            }
        }
    }
}

void printIslands(struct IslandManager *islandManager, struct LakeManager *lakeManager)
{
    int i, j;
    for (i = 0; i < lakeManager->LakeWidth; i++)
    {
        for (j = 0; j < lakeManager->LakeWidth; j++)
        {
            int index = GetIslandIndex(islandManager, lakeManager->Lake + i * lakeManager->LakeWidth + j);
            if (index > 0)
            {
                if (index < 10)
                {
                    printf("%d  ", index);
                }
                else
                {
                    printf("%d ", index);
                }
            }
            else
            {
                printf("0  ");
            }
        }
        printf("\n");
    }
}

bool canCross(struct IslandManager *islandManager, struct LakeManager *lakeManager)
{
    for (int i = 0; i < islandManager->Width; i++)
    {
        if (islandManager->Islands[i]->Width > 0)
        {
            bool isTopRow = false;
            bool isBottomRow = false;
            for (int j = 0; j < islandManager->Islands[i]->Width; j++)
            {
                int yPosition = YPositionInLake(lakeManager, islandManager->Islands[i]->Foes[j]);
                if (yPosition == 0)
                {
                    isTopRow = true;
                }
                else if (yPosition == lakeManager->LakeWidth - 1)
                {
                    isBottomRow = true;
                }

                if (isTopRow && isBottomRow)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

double getCrossProbability(bool *results, int simCount)
{
    int crossed = 0;
    for (int i = 0; i < simCount; i++)
    {
        if (results[i])
        {
            crossed++;
        }
    }

    return (double)crossed / (double)simCount;
}

struct CalculateResultsArgs
{
    int LakeWidth;
    int SimCount;
    double Resolution;
    char *FileName;
};

void calculateResults(struct CalculateResultsArgs *args)
{
    bool *crosses = NULL;
    int resultWidth = (int)(1 / args->Resolution);
    struct Result **results = NULL;
    FILE *fp;
    results = malloc(resultWidth * sizeof(struct Result *));

    for (int j = 0; j < resultWidth; j++)
    {
        crosses = malloc(args->SimCount * sizeof(bool));
        for (int i = 0; i < args->SimCount; i++)
        {
            struct IslandManager *islandManager = CreateIslandManager(10, 10);
            struct LakeManager *lakeManager = CreateLakeManager(args->LakeWidth, j * args->Resolution, args->Resolution);
            scanIslands(islandManager, lakeManager);

            crosses[i] = canCross(islandManager, lakeManager);
            DestroyLakeManager(lakeManager);
            DestroyIslandManager(islandManager);
        }
        results[j] = CreateResult(j * args->Resolution, getCrossProbability(crosses, args->SimCount));
        free(crosses);
    }

    fp = fopen(args->FileName, "w+");
    for (int i = 0; i < resultWidth; i++)
    {
        WriteResultToFile(results[i], fp);
        DestroyResult(results[i]);
    }
    free(results);
    fclose(fp);
}

int main()
{
    srand((unsigned)time(NULL));
    struct CalculateResultsArgs *args = malloc(sizeof(struct CalculateResultsArgs));
    args->LakeWidth = 5;
    args->SimCount = 10000;
    args->Resolution = 0.001;
    args->FileName = "results5fast.txt";
    calculateResults(args);

    // args->LakeWidth = 10;
    // args->SimCount = 1000000;
    // args->Resolution = 0.001;
    // args->FileName = "results10fast.txt";
    // calculateResults(args);

    // args->LakeWidth = 20;
    // args->SimCount = 1000000;
    // args->Resolution = 0.001;
    // args->FileName = "results20fast.txt";
    // calculateResults(args);

    return 0;
}
