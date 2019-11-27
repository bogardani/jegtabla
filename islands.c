#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "island.c"

struct IslandManager
{
    struct Island **Islands;
    int Width;
    int _buffSize;
    int _islandWidth;
};

struct IslandManager *CreateIslandManager(int initBuffSize, int islandWidth)
{
    if (initBuffSize == 0 || islandWidth == 0)
    {
        printf("Invalid buffer size");
        exit(1);
    }
    struct IslandManager *this = NULL;
    this = malloc(sizeof(struct IslandManager));
    this->Islands = NULL;
    this->Islands = malloc(initBuffSize * sizeof(struct Island *));
    for (int i = 0; i < initBuffSize; i++)
    {
        this->Islands[i] = CreateIsland(islandWidth);
    }
    this->Width = 0;
    this->_buffSize = initBuffSize;
    this->_islandWidth = islandWidth;

    return this;
}

void _reallocIslands(struct IslandManager *this, int buffSize)
{
    if (buffSize < this->_buffSize)
    {
        return;
    }
    struct Island **tmp = NULL;
    tmp = realloc(this->Islands, buffSize * sizeof(struct Island *));
    if (tmp != NULL)
    {
        this->Islands = tmp;
        for (int i = this->Width; i < buffSize; i++)
        {
            this->Islands[i] = CreateIsland(this->_islandWidth);
        }
    }
    else
    {
        printf("Islands realloc failed");
        exit(1);
    }
    this->_buffSize = buffSize;
}

int GetIslandIndex(struct IslandManager *this, int *foe)
{
    for (int i = 0; i < this->Width; i++)
    {
        if (ContainsFoe(this->Islands[i], foe))
        {
            return i;
        }
    }
    return -1;
}

void AddNewIsland(struct IslandManager *this, int *foe)
{
    if (this->Width == this->_buffSize)
    {
        _reallocIslands(this, this->_buffSize * 2);
    }
    AddFoe(this->Islands[this->Width], foe);
    this->Width++;
}

void AddToIsland(struct IslandManager *this, int *foe, int islandIndex)
{
    if (islandIndex > this->Width - 1)
    {
        printf("No island with index: %d", islandIndex);
        exit(1);
    }
    AddFoe(this->Islands[islandIndex], foe);
}

void RemoveIslandAt(struct IslandManager *this, int index)
{
    if (index > this->Width - 1)
    {
        printf("Index out of range");
        exit(1);
    }
    DestroyIsland(this->Islands[index]);
    for (int i = index; i < this->_buffSize; i++)
    {
        this->Islands[i] = this->Islands[i + 1];
    }
    this->Islands[this->_buffSize - 1] = CreateIsland(this->_islandWidth);
    this->Width--;
}

void MergeIslands(struct IslandManager *this, int fromIndex, int toIndex)
{
    for (int i = 0; i < this->Islands[fromIndex]->Width; i++)
    {
        AddFoe(this->Islands[toIndex], this->Islands[fromIndex]->Foes[i]);
    }
    RemoveIslandAt(this, fromIndex);
}

void DestroyIslandManager(struct IslandManager *this)
{
    for (int i = 0; i < this->_buffSize; i++)
    {
        DestroyIsland(this->Islands[i]);
    }
    free(this->Islands);
    free(this);
}
