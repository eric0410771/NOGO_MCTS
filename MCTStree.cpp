#include "MCTStree.h"

double MCTStree::getvalue( ucbnode* nodeptr, int child_index)
{
	/*
	ucbnode *tmp = (nodeptr->childptrs)+child_index;
	char &p = tmp->place;
	bool &c = tmp->player;
	double &N = tmp->count ;
	//double &NR = tmp->ravecount;
	//double ret = tmp->ravemean*NR + tmp->mean*N +  sqrt( nodeptr->logc * N )* UCB_WEIGHT;
	//cout<<tmp->ravemean<<' '<<ret/(N+NR)<<' '<<N<<' '<<NR<<' '<<nodeptr->logc<<endl;
	return tmp->mean + UCB_WEIGHT * sqrt(nodeptr->logc / (N + 1));
	//return ret / (N+NR);
	*/
	ucbnode *child = (nodeptr->childptrs) + child_index;
	return (child->mean * child->count + child->rave_mean * child->rave_count + UCB_WEIGHT * sqrt(nodeptr->logc/(child->count + 1)))/(child->rave_count + child->count);
	//return child->mean + UCB_WEIGHT * sqrt(nodeptr->logc/(child->count + 1));
}

ucbnode* MCTStree::get_best_child(ucbnode* nodeptr)
{
	/*
	if(nodeptr->num_child == 0)return NULL;
	int i,ret=0;
	double ans,tmp = getvalue(nodeptr,0),tma;//tmp minus anwser
	ans=tmp;
	selectlist[0]=0;
	slsize = 1;
	for(i=1;i<(nodeptr->num_child);i++)
	{
		tmp = getvalue(nodeptr,i);
		tma = tmp-ans;
		if( tma > -0.0001 )//tmp >= ans
		{
			if(tma > 0.0001) // tmp > ans
			{
				selectlist[0]=i;
				slsize = 1;
				ans = tmp;
			}else  //tmp == ans
			{
				selectlist[slsize]=i;
				slsize ++ ;
			}
		}
	}
	//for(i=0;i<slsize;i++)
	//{
	//	cout<<selectlist[i]<<' ';
	//}
	//cout<<endl;
	ret = selectlist[ rand() % slsize ];
	return (nodeptr->childptrs +ret);
	*/

	if(nodeptr->childptrs ==NULL){
		return NULL;
	}else{
		int child_index = 0;
		double value = getvalue(nodeptr, child_index);
		double gap;
		double tmp = value;
		selectlist[0] = child_index;
		slsize = 1;	
		for(child_index = 0; child_index < (nodeptr->num_child); child_index++){
			value = getvalue(nodeptr, child_index);
			gap = value - tmp;
			if(gap > 0.005){
				selectlist[0] = child_index;
				slsize = 1;
				tmp = value;		
			}else if(gap > -0.005){
				selectlist[slsize] = child_index;
				slsize += 1;
			}
		}
		child_index = selectlist[rand() % slsize];
		return (nodeptr->childptrs) + child_index;
	}
}

void MCTStree::select(board &b)
{
	
	bool j = ! b.just_play_color();//next to play
	ucbnode* nodeptr = root;
	b.bpsize=0;
	b.wpsize=0;
	path.clear();
	path.push_back(nodeptr);
	while(nodeptr->childptrs != NULL && nodeptr->num_child != 0)
	{
		nodeptr = get_best_child(nodeptr);
		path.push_back(nodeptr);
	//	cout<<inttostring(nodeptr->place)<<' ';
		if(nodeptr->player == BLACK)
		{
			b.addbp(nodeptr->place);
			sbnum++;
		}else
		{
			b.addwp(nodeptr->place);
			swnum++;
		}
		b.add(nodeptr->place,nodeptr->player);
	}
//	b.showboard();
	//system("pause");
}
void MCTStree::update(double result,board& b)
{
	int branch_child_index;
	for(int i=0;i<path.size();i++)
	{
		path[i]->update_node(result);

		if(path[i]->player != BLACK){
			for(int k = 0;k<sbnum;k++){
				branch_child_index = path[i]->legal_action[b.bpath[k]];	
				if(branch_child_index >=0 && branch_child_index<81){
					((path[i]->childptrs)+branch_child_index)->update_rave(result);
				}
			}
		}else{
			for(int k = 0; k<swnum; k++){
				branch_child_index = path[i]->legal_action[b.wpath[k]];
				if(branch_child_index >=0 && branch_child_index < 81){
					((path[i]->childptrs)+branch_child_index)->update_rave(result);
				}
			}
		}		
	}
}
void MCTStree::run_a_cycle()
{
	board b;
	double result;
	b =rboard;
	sbnum=swnum=0;
	select(b);
	ucbnode &last=(*(path.back()));
	ucbnode *nodeptr;
	if(last.num_child==0 && last.count > initial_v )//¦Ü¤Ösimulate 1 ¦¸
	{
		last.expansion(b);
		if(last.num_child!=0)
		{
			totalnode+=last.num_child;
			nodeptr = get_best_child(&last);
			path.push_back(nodeptr);
			if(nodeptr->player == 0)
			{
				b.addbp(nodeptr->place);
				sbnum++;
			}else
			{
				b.addwp(nodeptr->place);
				swnum++;
			}
			b.add(nodeptr->place,nodeptr->player);
			
		}
	}
	total += sbnum;
	total += swnum;
	b.getv(bone,wone,two,bsize,wsize,tsize);
	
	if((b.just_play_color()==BLACK) && (wsize + tsize)==0)
	{
		result = 1;
	}else if(b.just_play_color()==WHITE && (bsize + tsize)==0)
	{
		result = -1;
	}else
	{
		result=b.simulate(!b.just_play_color(),bone,wone,two,bsize,wsize,tsize);
	}
	update(result,b);
}

void MCTStree::reset(board &b)
{

	/*bool get_simulated = false;
	if(action1 != -1 && action2 != -1 && root != NULL){	
		int child_index = -1;
		ucbnode* tmp = root;
		for(int index = 0;index < root->num_child;index++){
			if(int(((root->childptrs)+index)->place) == action2){
				child_index = index;
			}
		}

		if(child_index != -1 && ((root->childptrs)+child_index) != NULL){
			root = (root->childptrs) + child_index;
		}

		child_index = -1;
		for(int index = 0;index < root->num_child;index++){
			if(int(((root->childptrs)+index)->place) == action1){
				child_index = index;
			}
		}

		if(child_index != -1&&((root->childptrs) + child_index) != NULL){
			root = (root->childptrs) + child_index;
			get_simulated = true;
		}
		//delete tmp;
	}*/
	rboard=b;
		
	root = new ucbnode;		
	root -> player = rboard.just_play_color();
		
	root -> place = 81;
	root -> count = initial_v;
	root -> logc = 1;
	memset(root -> legal_action,-1,sizeof(root -> legal_action)  );
	root -> expansion(b);
	total = 0;
	totalnode = 0;

}

void MCTStree::show_path()
{
	ucbnode* nodeptr = root;
	int i,k;
	i=0;
	while( nodeptr->childptrs != NULL && i<10)
	{
		k=nodeptr ->get_best_child_index();
		nodeptr = nodeptr->childptrs +k;
		i++;
		if(nodeptr != NULL){
			if(nodeptr->player == BLACK)
				cerr<<"B(";
			else
				cerr<<"W(";
			cerr<<inttostring(nodeptr -> place)<<") ";
		}
	}
	cerr<<endl;
}
void MCTStree::clear()
{
	if( root != NULL)delete root;
	cout<<"Done clear"<<endl;
	if(root != NULL){
		cout<<"I am not NULL"<<endl;
	}
}
string MCTStree::inttoGTPstring(int i)
{
    string s = "A1";
    s[0]+= i / 9;
    if(s[0]>='I')s[0]++;
    s[1]+= i % 9;
    return s;
}
string MCTStree::inttostring(int i)
{
    return inttoGTPstring(i);
}

MCTStree::MCTStree()
{
	;
}

MCTStree::~MCTStree()
{
	;
}

