#include "MCTStree.h"

double MCTStree::getvalue( ucbnode* nodeptr, int child_index)
{
	ucbnode *tmp = (nodeptr->childptrs) + child_index;
	//return tmp->mean + UCB_WEIGHT * sqrt(nodeptr->logc / (tmp->count + 1));
	return (tmp->mean * tmp->count + tmp->ravemean *tmp -> ravecount + sqrt(nodeptr->logc * tmp -> count) * UCB_WEIGHT)/ (tmp->count + tmp->ravecount);
}

ucbnode* MCTStree::get_best_child(ucbnode* nodeptr)
{
	
	if(nodeptr->childptrs == NULL){
		return NULL;
	}else{
		int child_index = 0; 
		double last = getvalue(nodeptr, child_index);
		double current;
		double gap;
		prior_child[0]=0;
		num_prior = 1;
		for(child_index = 1; child_index<(nodeptr->num_child); child_index++)
		{
			current = getvalue(nodeptr, child_index);
			gap = current - last;
			if(gap > 0.0001)
			{
				prior_child[0] = child_index;
				num_prior = 1;
				last = current;
			}else if(gap > -0.0001) 
			{
				prior_child[num_prior] = child_index;
				num_prior += 1 ;
			}
		}
		child_index = prior_child[ rand() % num_prior];
		return (nodeptr->childptrs + child_index);
	}
}

void MCTStree::select(board &b)
{
	
	bool next_player = !b.just_play_color();
	
	b.bpsize=0;
	b.wpsize=0;
	
	path.clear();	
	path.push_back(root);
	
	ucbnode* tmpptr = root;

	while(tmpptr->childptrs != NULL && tmpptr->num_child != 0)
	{
		
		tmpptr = get_best_child(tmpptr);
		path.push_back(tmpptr);
		if(tmpptr->player == BLACK)
		{
			b.addbp(tmpptr->place);
		}else
		{
			b.addwp(tmpptr->place);
		}
		b.add(tmpptr->place,tmpptr->player);
	}
}


void MCTStree::update(double result,board& b)
{
	for(int i=0;i<path.size();i++)
	{
		path[i]->update_node(result);

		if(path[i] -> player == BLACK){
			for(int j = 0; j < b.wpsize; j++){
				int k = (path[i]->legal_action[b.wpath[j]]);
				if ( k != -1){
					((path[i]->childptrs)+k)->update_rave_node(result);
				}
			}
		}else{
			for(int j = 0; j < b.bpsize; j++){
				int k = (path[i]->legal_action[b.bpath[j]]);
				if (k != -1){
					((path[i]->childptrs)+k)->update_rave_node(result);
				}
			}
		}
	}
}
void MCTStree::run_a_cycle()
{
	board b = rboard;
	select(b);
	ucbnode &last_node=(*(path.back()));

	if(last_node.num_child==0 && last_node.count > initial_v )
	{
		last_node.expansion(b, rave_num, rave_wnum);
		if(last_node.num_child!=0)
		{
			ucbnode * next_node = get_best_child(&last_node);
			path.push_back(next_node);
			if(next_node->player == 0)
			{
				b.addbp(next_node->place);
			}else
			{
				b.addwp(next_node->place);
			}
			b.add(next_node->place, next_node->player);
			
		}
	}
	b.getv(bone,wone,two,bsize,wsize,tsize);
	
	double result = 0;
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
	/*
	bool get_simulated = false;
	vector<double> child_mean;
 	vector<double> child_count;
  	ucbnode* tmp_root = root;
  	if(action1 != -1 && action2 != -1 && root != NULL){	
		int child_index = -1;
		cout<<tmp_root->num_child<<endl;
		for(int index = 0;index < tmp_root->num_child;index++){
			if(int(((tmp_root->childptrs)+index)->place) == action2){
				child_index = index;
      			}
		}

		if(child_index != -1 && ((root->childptrs)+child_index) != NULL){
			tmp_root = (tmp_root->childptrs) + child_index;
      			cout<<tmp_root->num_child<<endl;
		}else{
      			cout<<"Not Find"<<endl;
    		}

		child_index = -1;
		for(int index = 0;index < tmp_root->num_child;index++){
			if(int(((tmp_root->childptrs)+index)->place) == action1){
				child_index = index;
			}
		}
		if(child_index != -1&&((tmp_root->childptrs) + child_index) != NULL){ 
      			tmp_root = (tmp_root->childptrs) + child_index;
      			for(int i = 0; i<tmp_root->num_child; i++){
        			child_mean.push_back((tmp_root->childptrs+i)->mean);
        			child_count.push_back((tmp_root->childptrs+i)->count);
      			}
		}
	}*/

  	delete root;
	rboard=b;
	root = new ucbnode;
	root -> player = rboard.just_play_color();
	root -> place = 81;
	root -> count = initial_v;
	root -> logc = 1;
	memset(root -> legal_action,-1,sizeof(root -> legal_action)  );
	root-> expansion(b, rave_num, rave_wnum);
	totalnode =0;
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

