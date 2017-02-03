/*#include<stdio.h>
#include<limits.h>
#include<vector>
using namespace std;*/
struct LFU{
    int val;
    int freq;
};
struct OUTPUTLFU{
    int stages;
    int swapstages;
    int page_table_size;
    vector<int> swaploc;
    vector<vector<int> > pagetable;
};

//to initialize LFU Table
void initializeLFU(struct LFU lfu[],int page_table_size)
{
    int i;
    for(i=0;i<page_table_size;i++)
    {
        lfu[i].val=-1;
        lfu[i].freq=0;
    }
}
//to print LFU Table
void printLFU(struct LFU lfu[],int page_table_size)
{
    int i;
    printf("Page Table :\n");
    for(i=0;i<page_table_size;i++)
    {
        if(lfu[i].val==-1)
        {
            printf("NIL");
        }
        else
        {
            printf("%d, %d", lfu[i].val,lfu[i].freq);
        }
        printf("\n");
    }
}
//print frames to be allocated
void printframesLFU(int frame[],int frames)
{
    int i;
    printf("Frames: \n");
    for(i=0;i<frames;i++)
    printf("%d ", frame[i]);
    printf("\n");
}
//to check if the frame is already allocated
int locateLFU(struct LFU lfu[],int page_table_index,int N)
{
    int i;
    for(i=0;i<page_table_index;i++)
    {
        if(lfu[i].val==N)
        {
            return i;
        }
    }
    return -1;
}
//to increase the frequency of allocated frame
void increasefreqLFU(struct LFU lfu[],int index)
{
    lfu[index].freq++;
}
//to find victim page for replacement
int findvictimLFU(struct LFU lfu[],int page_table_size)
{
    int i;
    int freq=INT_MAX,pos=-1;
    for(i=0;i<page_table_size;i++)
    {
        if(lfu[i].freq<freq)
        {
            freq=lfu[i].freq;
            pos=i;
        }
    }
    return pos;
}
//to deallocate victim page and allocate it to new frame
void changeLFU(struct LFU lfu[],int index,int N)
{
    lfu[index].val=N;
    lfu[index].freq=1;
}
vector<int> extractpagetableLFU(struct LFU lfu[],int page_table_size)
{
    vector<int> v;
    int i;
    for(i=0;i<page_table_size;i++)
    {
        v.push_back(lfu[i].val);
    }
    return v;
}

//main function to implement LFU algorithm
int LFU_allocate(int page_table_size,int framelist[],int frames,struct OUTPUTLFU *op)
{
    int i;
    int count=0;
    LFU lfu[page_table_size];
    initializeLFU(lfu,page_table_size);
    printLFU(lfu,page_table_size);

    op->pagetable.push_back(extractpagetableLFU(lfu,page_table_size));
    op->stages=1;
    op->swapstages=0;
    op->page_table_size=page_table_size;

    printframesLFU(framelist,frames);
    printf("\n\nAllocating Frames :");
    for(i=0;i<frames;i++)
    {
        int index;
        printf("\n\nAllocating Frame %d : \n", framelist[i]);
        index=locateLFU(lfu,page_table_size,framelist[i]);
        if(index!=-1)
        {
            op->swaploc.push_back(0);
            op->stages++;

            printf("Frame %d is already allocated, increasing its frequency of usage by 1\n", framelist[i]);
            increasefreqLFU(lfu,index);
        }
        else
        {
            op->swaploc.push_back(1);
            op->stages++;
            op->swapstages++;

            printf("Frame %d is not allocated, finding victim page which is less frequently used...\n", framelist[i]);
            index=findvictimLFU(lfu,page_table_size);
            printf("Frame %d allocated to page table index : %d\n", framelist[i],index);
            changeLFU(lfu,index,framelist[i]);
            count++;
        }
        printLFU(lfu,page_table_size);

        op->pagetable.push_back(extractpagetableLFU(lfu,page_table_size));
    }
    return count;
}
void CreateJsonLFU(struct OUTPUTLFU op)
{
    //FILE *fp=fopen("outputLFU.json", "w");
    fprintf(fp, "{");
    fprintf(fp, "\"algoname\": \"LFU\", ");
    fprintf(fp,"\"numberofstages\": %d,", op.stages);
    fprintf(fp,"\"numberofstageswithswap\": %d,", op.swapstages);
    fprintf(fp,"\"stageswithswap\": [");
    int i,j;
    fprintf(fp, " 1" );
    for(i=0;i<op.stages-1;i++)
    {
        fprintf(fp,", %d",op.swaploc[i] );
        // if(i+1!=op.stages-1)
        //     fprintf(fp,",");
    }
    fprintf(fp," ],");
    fprintf(fp,"\"allstages\":[");
    for(i=0;i<op.stages;i++)
    {
        fprintf(fp," [");
        for(j=0;j<op.page_table_size;j++)
        {
            fprintf(fp,"%d",op.pagetable[i][j]);
            if(j+1!=op.page_table_size)
                fprintf(fp,", ");
        }
        fprintf(fp,"]");
        if(i+1!=op.stages)
            fprintf(fp,", ");
    }
    fprintf(fp," ]");
    fprintf(fp, " }");
    //fclose(fp);
}
int LFU(int page_table_size,int framelist[],int frames)
{
    struct OUTPUTLFU op;
    int replacements=0;
    replacements=LFU_allocate(page_table_size,framelist,frames,&op);
    printf("Total page replacements with LFU page replacement algorithm : %d\n", replacements);

    CreateJsonLFU(op);
}
/*int main()
{
    //freopen("LFUoutput.out", "w", stdout);
    int page_table_size=3;
    int framelist[]={1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,3,3,3,3,4,4,4,6};
    int frames=sizeof(framelist)/sizeof(int);

    LFU(page_table_size,framelist,frames);
}
*/