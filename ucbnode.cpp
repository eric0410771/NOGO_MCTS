#include "ucbnode.h"


void ucbnode::setlogc()
{
    logc = log(count);
}
ucbnode::ucbnode()
{
    
}
ucbnode::~ucbnode()
{
    if(childptrs != NULL) 
    {
        delete [] childptrs;
        childptrs=NULL;
    }
}


void ucbnode::init_node(int i,bool j)
{
    	player=j;
    	place=i;
    	mean = 0.5;
    	count = initial_v;
    	num_child=0;
    	memset(legal_action,-1,sizeof(legal_action));
    	childptrs=NULL;
    	logc=1;
}

void ucbnode::update_node(double result)
{
    if((result > 0 && player==BLACK)||(result < 0 && player == WHITE))
    {
        mean = (mean*count+1.00)/(count+1);
    }else
    {
        mean = (mean*count)/(count+1);
    }
    count += 1;
    setlogc();
}

/*
void ucbnode::update_rave(double result)
{
    if((result > 0 && player==BLACK) || (result < 0 && player==WHITE) )
    {
        ravemean = (ravemean*ravecount+1.00)/(ravecount+1);
    }else
    {
        ravemean = (ravemean*ravecount)/(ravecount+1);
    }
    ravecount+=1;
}*/

void ucbnode::expansion(board &b)
{

	bool next_player = ! b.just_play_color();
	int num_legal_a = 0;	
	for(int i = 0;i<BOARDSSIZE; i++){
		if(b.check(i, next_player)){
			num_legal_a += 1;
		}
	}

	num_child = num_legal_a;
	if(num_legal_a == 0){
		return;
	}else{
		childptrs = new ucbnode[num_legal_a];
		for(int i = 0;i<BOARDSSIZE; i++){
			if(b.check(i, next_player)){
				num_legal_a --;
				legal_action[i] = num_legal_a;
				childptrs[num_legal_a].init_node(i, next_player);
			}
		}	
	}
}

int ucbnode::get_best_child_index()
{
	int child_index = -1;
	double tmp_winrate = 0;
	double tmp_count = -10;
	for(int i = 0;i<num_child;i++)
	{
		
		/*if(tmp_winrate < childptrs[i].mean || tmp_count < childptrs[i].count)
		{
			if(tmp_count *0.9 < childptrs[i].count)
			{
				tmp_winrate = childptrs[i].mean;
				tmp_count = childptrs[i].count;
				child_index = i;
			}
		}*/
		
		if(childptrs[i].count > tmp_count){
			tmp_count = childptrs[i].count;
			child_index = i;
		}	
	}

	return child_index;
}


vector<float> ucbnode::return_values()
{
	vector<float> values(BOARDSSIZE, 0);
	for (int i = 0; i < num_child; i++)
	{
		values[childptrs[i].place] = childptrs[i].count / count;
	}
	return values;
}


void ucbnode::show_child()
{
	for(int i=0;i<num_child;i++)
	{
		if(childptrs[i].count<10)continue;
		cerr<< inttoGTPstring((int)childptrs[i].place)<<' '<<childptrs[i].mean<<' '<<childptrs[i].count<<' ';
		//cerr<<sqrt( logc / ((childptr[i].count) - minusnum))* UCB_WEIGHT<<' ' ;
		//cerr<<(childptrs[i].ravemean)<<' '<<(childptrs[i].ravecount)<<endl;
	}
}
float ucbnode::show_inf(int best)
{
		cerr<<"total : "<< count -initial_v <<endl<<"winrate :"<<(childptrs[best].mean);
		//cerr<<"\nlog : "<<sqrt( logc / ((childptr[best].count) - minusnum))* UCB_WEIGHT ;
		//cerr<<"\nrave : "<<(childptrs[best].ravemean)<<' '<<(childptrs[best].ravecount)<<endl;
		return childptrs[best].mean;
}

string ucbnode::inttoGTPstring(int i)
{
    string s = "A1";
    s[0]+= i / 9;
    if(s[0]>='I')s[0]++;
    s[1]+= i % 9;
    return s;
}
string ucbnode::inttostring(int i)
{
    return inttoGTPstring(i);
}
