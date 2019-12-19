#ifndef UCBNODE_H
#define UCBNODE_H
#include "board.h"
#include <math.h>
#define initial_v 0
#define initial_rave_v 20

#define minusnum (initial_v - 2)
#define minusrnum (initial_rave_vmake - 2)
const double UCB_WEIGHT = 0.5;
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
	double logc;

	void init_node(int i,bool j);
	void update_node(double result);
	void expansion(board &b);
	int get_best_child_index();
	vector<float> return_values();

	void show_child();
	float show_inf(int child);
	
	string inttostring(int i);
	string inttoGTPstring(int i);
	ucbnode();
	~ucbnode();
	void setlogc();

	//void update_rave(double result);
};
#endif //UCBNODE_H
