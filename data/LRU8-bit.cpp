
//structure for page table entry
struct LRU{
    int pgno;
    vector<int> usage_history;
};
struct OUTPUTLRU{
    int stages;
    int swapstages;
    int page_table_size;
    vector<int> swaploc;
    vector<vector<int> > pagetable;
};
//intializing page table with empty entries
void initializeLRU(struct LRU L[],int page_table_size)
{
    int i,j;
    for(i=0;i<page_table_size;i++)
    {
        L[i].pgno=-1;
        for(j=0;j<8;j++)
        {
            L[i].usage_history.push_back(0);
        }
    }
}
//print contents of LRU alongwith usage history bits
void printLRU(struct LRU L[],int page_table_size)
{
    int i,j;
    printf("Page Table: \n");
    for(i=0;i<page_table_size;i++)
    {
        if(L[i].pgno!=-1)
        {
            printf("%d, ", L[i].pgno);
            for(j=0;j<8;j++)
            {
                printf("%d", L[i].usage_history[j]);
            }
        }
        else
        {
            printf("NIL");
        }
        printf("\n");
    }
}
//print frames to be allocated
void printframesLRU(int frame[],int frames)
{
    int i;
    printf("Frames: \n");
    for(i=0;i<frames;i++)
    printf("%d ", frame[i]);
    printf("\n");
}
//to check if page is already allocated
bool isallocatedLRU(struct LRU L[],int page_table_size,int N)
{
    int i;
    for(i=0;i<page_table_size;i++)
    {
        if(L[i].pgno==N)
            return true;
    }
    return false;
}
//to convert usage history bits to corresponding integer value for comparison of LRU
int to_intLRU(vector<int> v)
{
    int i,val=0;
    for(i=7;i>=0;i--)
    {
        val=val+ v[i]*((int) pow(2,7-i));
    }
    return val;
}
//to find the page which is least recently used
int findLRU(struct LRU L[],int page_table_size)
{
    int i;
    int val=INT_MAX,pos=-1;
    for(i=0;i<page_table_size;i++)
    {
        int Lused;
        Lused=to_intLRU(L[i].usage_history);
        if(Lused<val)
        {
            val=Lused;
            pos=i;
        }
    }
    return pos;
}
//to deallocate least recently used page and allocate that entry to new page
void modifyLRU(struct LRU L[], int page_table_size, int index,int N)
{
    int i;
    L[index].pgno=N;
    for(i=0;i<8;i++)
    {
        L[index].usage_history[i]=0;
    }
}
//to mark the history bits as used or not used after certain allocation of a frame
void updateLRU(struct LRU L[],int page_table_size,int N)
{
    int i;
    for(i=0;i<page_table_size;i++)
    {
        vector<int>::iterator it;
        it=L[i].usage_history.begin();
        if(L[i].pgno==N)
        {
            L[i].usage_history.insert(it,1);
        }
        else
        {
            L[i].usage_history.insert(it,0);
        }
        L[i].usage_history.resize(8);
    }
}
vector<int> extractpagetableLRU(struct LRU L[],int page_table_size)
{
    vector<int> v;
    int i;
    for(i=0;i<page_table_size;i++)
    {
        v.push_back(L[i].pgno);
    }
    return v;
}
//Main function to allocate frames in a page table
int LRU8bit_allocate(int page_table_size,int framelist[],int frames,struct OUTPUTLRU *op)
{
    int i,count=0;
    LRU L[page_table_size];
    initializeLRU(L,page_table_size);
    printLRU(L,page_table_size);

    op->pagetable.push_back(extractpagetableLRU(L,page_table_size));
    op->stages=1;
    op->swapstages=0;
    op->page_table_size=page_table_size;

    printframesLRU(framelist,frames);
    printf("\nAllocating Frames:\n\n");
    for(i=0;i<frames;i++)
    {
        bool allocated=isallocatedLRU(L,page_table_size,framelist[i]);
        if(allocated==false)
        {
            op->swaploc.push_back(1);
            op->stages++;
            op->swapstages++;

            printf("Frame %d is not allocated\n", framelist[i]);
            int index=findLRU(L,page_table_size);
            printf("Frame %d is allocated to Page Table Index :  %d\n", framelist[i],index);
            modifyLRU(L,page_table_size,index,framelist[i]);
            count++;
        }
        else
        {
            op->swaploc.push_back(0);
            op->stages++;

            printf("Frame %d is already allocated in Page Table\n", framelist[i]);
        }
        updateLRU(L,page_table_size,framelist[i]);
        printLRU(L,page_table_size);

        op->pagetable.push_back(extractpagetableLRU(L,page_table_size));

        printf("\n\n");
    }
    return count;
}
void CreateJsonLRU(struct OUTPUTLRU op)
{
    //FILE *fp=fopen("outputLRU.json", "w");
    fprintf(fp, "{");
    fprintf(fp, "\"algoname\": \"LRU8\"," ); 
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
    fprintf(fp, " }" );
    //fclose(fp);
}
int LRU(int page_table_size,int framelist[],int frames)
{
    struct OUTPUTLRU op;
    int replacements=0;
    replacements=LRU8bit_allocate(page_table_size,framelist,frames,&op);
    printf("Total page replacements with LRU 8-bit page table : %d\n", replacements);

    CreateJsonLRU(op);
}
/*int main()
{
    int page_table_size=3;
    int framelist[]={1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,3,3,3,3};
    int frames=sizeof(framelist)/sizeof(int);

    LRU(page_table_size,framelist,frames);
}*/
