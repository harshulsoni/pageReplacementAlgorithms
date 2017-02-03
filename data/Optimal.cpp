// #include<stdio.h>
// #include<vector>
// using namespace std;
struct OPT{
    int val;
    int nextuse;
};
struct OUTPUTOPT{
    int stages;
    int swapstages;
    int page_table_size;
    vector<int> swaploc;
    vector<vector<int> > pagetable;
};
void initializeOPT(struct OPT opt[],int page_table_size)
{
    int i;
    for(i=0;i<page_table_size;i++)
    {
        opt[i].val=-1;
        opt[i].nextuse=-1;
    }
}
void printOPT(struct OPT opt[],int page_table_size)
{
    int i;
    printf("Page Table :\n");
    for(i=0;i<page_table_size;i++)
    {
        if(opt[i].val==-1)
        {
            printf("NIL");
        }
        else
        {
            printf("%d", opt[i].val);
        }
        printf("\n");
    }
}
void printframesOPT(int frame[],int frames)
{
    int i;
    printf("Frames: \n");
    for(i=0;i<frames;i++)
    printf("%d ", frame[i]);
    printf("\n");
}
int locateOPT(struct OPT opt[],int page_table_index,int N)
{
    int i;
    for(i=0;i<page_table_index;i++)
    {
        if(opt[i].val==N)
        {
            return i;
        }
    }
    return -1;
}
int findvictimOPT(struct OPT opt[],int page_table_size,int framelist[],int frames,int curr)
{
    int i,j;
    for(i=0;i<page_table_size;i++)
    {
        if(opt[i].val==-1)
            return i;
    }
    for(i=0;i<page_table_size;i++)
    {
        for(j=curr+1;j<frames;j++)
        {
            if(framelist[j]==opt[i].val)
                break;
        }
        opt[i].nextuse=j;
    }
    int usedlate=-2,pos=-1;
    for(i=0;i<page_table_size;i++)
    {
        if(opt[i].nextuse>usedlate)
        {
            usedlate=opt[i].nextuse;
            pos=i;
        }
    }
    return pos;
}
void changeOPT(struct OPT opt[],int index,int N)
{
    opt[index].val=N;
}
vector<int> extractpagetableOPT(struct OPT opt[],int page_table_size)
{
    vector<int> v;
    int i;
    for(i=0;i<page_table_size;i++)
    {
        v.push_back(opt[i].val);
    }
    return v;
}
int OPT_allocate(int page_table_size,int framelist[],int frames,struct OUTPUTOPT *op)
{
    int i;
    int count=0;
    OPT opt[page_table_size];
    initializeOPT(opt,page_table_size);
    printOPT(opt,page_table_size);

    op->pagetable.push_back(extractpagetableOPT(opt,page_table_size));
    op->stages=1;
    op->swapstages=0;
    op->page_table_size=page_table_size;

    printframesOPT(framelist,frames);
    printf("\n\nAllocating Frames :");
    for(i=0;i<frames;i++)
    {
        int index;
        printf("\n\nAllocating Frame %d : \n", framelist[i]);
        index=locateOPT(opt,page_table_size,framelist[i]);
        if(index!=-1)
        {
            op->swaploc.push_back(0);
            op->stages++;

            printf("Frame %d is already allocated\n", framelist[i]);
        }
        else
        {
            op->swaploc.push_back(1);
            op->stages++;
            op->swapstages++;

            printf("Frame %d is not allocated, finding victim page which would be used late...\n", framelist[i]);
            index=findvictimOPT(opt,page_table_size,framelist,frames,i);
            printf("Frame %d allocated to page table index : %d\n", framelist[i],index);
            changeOPT(opt,index,framelist[i]);
            count++;
        }
        printOPT(opt,page_table_size);

        op->pagetable.push_back(extractpagetableOPT(opt,page_table_size));
    }
    return count;
}
void CreateJsonOPT(struct OUTPUTOPT op)
{
    //FILE *fp=fopen("output.json", "a");
    fprintf(fp, "{" );
    fprintf(fp, "\"algoname\": \"OPT\"," ); 
    fprintf(fp,"\"numberofstages\": %d,", op.stages);
    fprintf(fp,"\"numberofstageswithswap\": %d,", op.swapstages);
    fprintf(fp,"\"stageswithswap\": [");
    int i,j;
    fprintf(fp, " 1" );
    for(i=0;i<op.stages-1;i++)
    {
        fprintf(fp,", %d",op.swaploc[i] );
        /*if(i+1!=op.stages-1)
            fprintf(fp,",");*/
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
    fprintf(fp, "}" );
    //fclose(fp);
}
int OPT(int page_table_size,int framelist[],int frames)
{
    struct OUTPUTOPT op;
    int replacements=0;
    replacements=OPT_allocate(page_table_size,framelist,frames,&op);
    printf("Total page replacements with OPT algorithm : %d\n", replacements);

    CreateJsonOPT(op);
}
/*int main()
{
    int page_table_size=3;
    int framelist[]={7    , 0    , 1   ,  2    , 0     ,3  ,  0  ,   4   ,  2    , 3   ,  0    , 3     ,2  ,   1 ,    2  ,   0   ,  1  ,   7  ,   0 ,    1};
    int frames=sizeof(framelist)/sizeof(int);

    OPT(page_table_size,framelist,frames);
}*/
