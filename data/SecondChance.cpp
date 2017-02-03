// #include<stdio.h>
// #include<vector>
//using namespace std;
struct SCT{
    int val;
    int sec_chance;
};
struct OUTPUTSCT{
    int stages;
    int swapstages;
    int page_table_size;
    vector<int> swaploc;
    vector<vector<int> > pagetable;
};
//to initialize SC Table
void initializeSCT(struct SCT s[],int page_table_size)
{
    int i;
    for(i=0;i<page_table_size;i++)
    {
        s[i].val=-1;
        s[i].sec_chance=0;
    }
}
//to print SC Table
void printSCT(struct SCT s[],int page_table_size,int curr)
{
    int i;
    printf("Page Table : \n");
    for(i=0;i<page_table_size;i++)
    {
        if(s[i].val==-1)
        {
            printf("NIL");
        }
        else
        {
            printf("%d, %d", s[i].val,s[i].sec_chance);
        }
        if(curr==i)
            printf("<-");
        printf("\n");
    }
}
//print frames to be allocated
void printframesSCT(int frame[],int frames)
{
    int i;
    printf("Frames: \n");
    for(i=0;i<frames;i++)
    printf("%d ", frame[i]);
    printf("\n");
}
//to find if the frame is already allocated
int locateSCT(struct SCT s[],int page_table_size,int N)
{
    int i;
    for(i=0;i<page_table_size;i++)
    {
        if(s[i].val==N)
        {
            return i;
        }
    }
    return -1;
}
//to update the second chance bit
void updateSCT(struct SCT s[], int index)
{
    s[index].sec_chance=1;
}
//to find index of page to be replaced
int findSCTvictim(struct SCT s[], int page_table_size,int curr)
{
    int i;
    while(s[curr].sec_chance!=0)
    {
        s[curr].sec_chance=0;
        curr=(curr+1)%page_table_size;
    }
    return curr;
}
//to deallocate victim page and reallocate to new frame
void changeSCT(struct SCT s[],int index,int N)
{
    s[index].val=N;
}
vector<int> extractpagetableSCT(struct SCT s[],int page_table_size)
{
    vector<int> v;
    int i;
    for(i=0;i<page_table_size;i++)
    {
        v.push_back(s[i].val);
    }
    return v;
}
//main function to implement Second Chance algorithm
int SCT_allocate(int page_table_size,int framelist[],int frames, struct OUTPUTSCT *op)
{
    int count=0;
    int i,curr=0;
    struct SCT s[page_table_size];
    initializeSCT(s,page_table_size);
    printSCT(s,page_table_size,curr);

    op->pagetable.push_back(extractpagetableSCT(s,page_table_size));
    op->stages=1;
    op->swapstages=0;
    op->page_table_size=page_table_size;

    printframesSCT(framelist,frames);
    printf("\nAllocating Frames :");
    for(i=0;i<frames;i++)
    {
        int index;
        index=locateSCT(s,page_table_size,framelist[i]);
        printf("\n\nAllocating frame %d\n", framelist[i]);
        if(index!=-1)
        {
            op->swaploc.push_back(0);
            op->stages++;

            printf("Frame %d is already allocated in page table, updating its second chance bit\n", framelist[i]);
            updateSCT(s,index);
        }
        else
        {
            op->swaploc.push_back(1);
            op->stages++;
            op->swapstages++;

            //printf("Frame %d is not allocated, Finding victim...\n", );
            index=findSCTvictim(s,page_table_size,curr);
            printf("Frame %d is allocated to page table index: %d\n", framelist[i],index);
            changeSCT(s,index,framelist[i]);
            curr=(index+1)%page_table_size;
            count++;
        }
        printSCT(s,page_table_size,curr);
        op->pagetable.push_back(extractpagetableSCT(s,page_table_size));
    }
    return count;
}
void CreateJsonSCT(struct OUTPUTSCT op)
{
    //FILE *fp=fopen("outputSCT.json", "w");
    fprintf(fp," {");
    fprintf(fp, "\"algoname\": \"SCT\"," );
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
    fprintf(fp," }");
    //fclose(fp);
}
int SC(int page_table_size,int framelist[],int frames)
{
    struct OUTPUTSCT op;
    int replacements=0;
    replacements=SCT_allocate(page_table_size,framelist,frames,&op);

    printf("\n\nTotal page replacements with Second Chance algorithm : %d\n", replacements);
    CreateJsonSCT(op);
}
/*int main()
{
    int page_table_size=3;
    int framelist[]={0,4,1,4,2,4,3,4,2,4,0,4,1,4,2,4,3,4};
    int frames=sizeof(framelist)/sizeof(int);

    SC(page_table_size,framelist,frames);
}*/
