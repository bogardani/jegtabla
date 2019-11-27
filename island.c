#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>

struct Island
{
    int **Foes;
    int Width;
    int _buffSize;
};

struct Island *CreateIsland(int initBuffSize)
{
    if (initBuffSize == 0)
    {
        printf("Invalid buffer size");
        exit(1);
    }
    struct Island *island = NULL;
    island = malloc(sizeof(struct Island));
    island->Foes = NULL;
    island->Foes = malloc(initBuffSize * sizeof(int *));
    for (int i = 0; i < initBuffSize; i++)
    {
        island->Foes[i] = NULL;
    }
    island->Width = 0;
    island->_buffSize = initBuffSize;

    return island;
}

void _reallocIsland(struct Island *this, int buffSize)
{
    if (this->_buffSize > buffSize)
    {
        printf("Small buffSize");
        return;
    }
    int **tmp = NULL;
    tmp = realloc(this->Foes, buffSize * sizeof(int *));
    if (tmp != NULL)
    {
        this->Foes = tmp;
        for (int i = this->Width; i < buffSize; i++)
        {
            this->Foes[i] = NULL;
        }
        this->_buffSize = buffSize;
    }
    else
    {
        printf("Island realloc failed");
        exit(1);
    }
}

void AddFoe(struct Island *this, int *foe)
{
    if (this->Width == this->_buffSize)
    {
        _reallocIsland(this, this->_buffSize * 2);
    }
    this->Foes[this->Width] = foe;
    this->Width++;
}

bool ContainsFoe(struct Island *this, int *foe)
{
    for (int i = 0; i < this->Width; i++)
    {
        if (this->Foes[i] == foe)
        {
            return true;
        }
    }
    return false;
}

void DestroyIsland(struct Island *this)
{
    free(this->Foes);
    free(this);
}