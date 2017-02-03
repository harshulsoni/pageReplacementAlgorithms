#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <utility>
#include <iostream>
using namespace std;
FILE *fp=fopen("outputmainprogram.json", "w");
#include "LRU8-bit.cpp"
#include "SecondChance.cpp"
#include "LFU.cpp"
#include "Optimal.cpp"
#include "FIFO.cpp"


int main(int argc, char const *argv[])
{
	freopen("outputcache.out", "w", stdout);
	freopen("input.in", "r", stdin);
	int page_table_size=3, frames;
    cin>>page_table_size>>frames;
    int *framelist= new int[frames+5];
    for (int i = 0; i < frames; ++i)
    {
    	cin>>framelist[i];
    }
    fprintf(fp, "[");
    LRU(page_table_size,framelist,frames);
	fprintf(fp, ",");
	LFU(page_table_size,framelist,frames);
	fprintf(fp, ",");
	SC(page_table_size,framelist,frames);
	fprintf(fp, ",");
	OPT(page_table_size,framelist,frames);
	fprintf(fp, ",");
	FIFO(page_table_size,framelist,frames);
	fprintf(fp, "]");
	fclose(fp);
	return 0;
}