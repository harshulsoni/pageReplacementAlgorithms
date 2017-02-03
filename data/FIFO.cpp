/*#include<bits/stdc++.h>
using namespace std;*/

int findnum(vector<pair<int, int> > &page){
	int min1=INT_MAX, ans=-1;
	for (int i = 0; i < page.size(); ++i)
	{
		if(page[i].second<min1)min1=page[i].second, ans=i;
		if(page[i].second==-1)return i;
	}
	return ans;
}

bool ifpresent(int c, vector<pair<int, int> > &page){
	for (int i = 0; i < page.size(); ++i)
	{
		if(page[i].first==c)return true;
	}
	return false;
}

void FIFO(int page_size, int *frameslist, int frames){
	vector<pair<int, int> > page;
	vector<int> swap_list;
	vector<vector<pair<int, int> > > ans;
	int i, j, k;
	for(i=0;i<page_size;i++)page.push_back(make_pair(-1, -1));
		//cout<<"pagesize: "<<page.size()<<endl;
	for(i=0;i<frames;i++){
		if(ifpresent(frameslist[i], page)){
			swap_list.push_back(0);
		}
		else{
			j=findnum(page);
			cout<<"page: "<<j<<endl;
			page[j]=make_pair(frameslist[i], i);
			swap_list.push_back(1);
		}
		ans.push_back(page);
	}
	k=0;
	for(i=0;i<swap_list.size();i++){
		if(swap_list[i]==1)k++;
	}
	/*cout<<"No of stages: "<<ans.size()<<endl;
	cout<<"numberofstageswithswap: "<<k<<endl;
	for(i=0;i<ans.size();i++){
		for(j=0;j<ans[i].size();j++){
			cout<<ans[i][j].first<<" ";
		}
		cout<<endl;
	}*/
	//FILE *fp=fopen("FIFOoutput.json", "w");
	/*fprintf(fp, "{" );
	fprintf(fp, "\"algoname: \"FIFO\",");
	fprintf(fp, "\"numberofstages\": %d,", ans.size());
	fprintf(fp, "\"numberofstageswithswap\": %d,",k );*/
	fprintf(fp, "{" );
    fprintf(fp, "\"algoname\": \"FIFO\"," ); 
    fprintf(fp,"\"numberofstages\": %d,", (int)ans.size());
    fprintf(fp,"\"numberofstageswithswap\": %d,", k);
    fprintf(fp,"\"stageswithswap\": [");
    //int i,j;
    fprintf(fp,"%d",swap_list[0] );
    for(i=1;i<swap_list.size();i++)
    {
        fprintf(fp,", %d",swap_list[i] );
    }
    fprintf(fp," ],");
    fprintf(fp,"\"allstages\":[");
    for(i=0;i<ans.size();i++)
    {
        fprintf(fp," [");
        for(j=0;j<ans[i].size();j++)
        {
            fprintf(fp,"%d",ans[i][j].first);
            if(j+1!=ans[i].size())
                fprintf(fp,", ");
        }
        fprintf(fp,"]");
        if(i+1!=ans.size())
            fprintf(fp,", ");
    }
    fprintf(fp," ]");
    fprintf(fp, "}" );
    //fclose(fp);

}
/*

int main(){
	freopen("FIFO.in", "r", stdin);
	int n, k;
	cin>>n>>k;
	int arr[k+5];
	for (int i = 0; i < k; ++i)
	{
		cin>>arr[i];
	}
	FIFO(n, arr, k);
}*/