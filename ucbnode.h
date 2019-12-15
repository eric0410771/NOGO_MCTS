#ifndef UCBNODE_H
#define UCBNODE_H
#include "board.h"
#include <math.h>
#define initial_v 0.0
#define initial_rave_v 10

#define minusnum (initial_v - 2)//¤£¯à´î¤@
const double UCB_WEIGHT = 0.25;
class ucbnode
{

public:
	char place;
	bool player;
	char legal_action[BOARDSSIZE+1];
	int num_child;
	ucbnode* childptrs;
	double count;
	double mean;
	double rave_count;
	double rave_mean;
	double logc;

    	void init_node(int i,bool j, double rave_count, double rave_mean);
    	void update_node(double result);
	void update_rave(double result);

    	int get_best_child_index();
    	vector<float> return_values();
    	void show_child();
    	float show_inf(int child);
    	void expansion(board &b);
    
    	string inttostring(int i);
    	string inttoGTPstring(int i);
    	ucbnode();
    	~ucbnode();
    	void setlogc();
};
#endif //UCBNODE_H
