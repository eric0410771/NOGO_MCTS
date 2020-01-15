#ifndef MCTS_H
#define MCTS_H
#include "ucbnode.h"
class MCTStree
{
public:
	int prior_child[BOARDSSIZE];
	int num_prior;
	ucbnode* root;
	board rboard;
        

	double rave_num[2][BOARDSSIZE];
	double rave_wnum[2][BOARDSSIZE];
	
        vector<ucbnode*> path;
	int bsize,wsize,tsize;
	int bone[BOARDSSIZE],wone[BOARDSSIZE],two[BOARDSSIZE];
	int totalnode;

        MCTStree();
        double getvalue( ucbnode* nodeptr, int child_index);
        ucbnode* get_best_child(ucbnode* nodeptr);
    
	void select(board &b);
        void update(double result,board& b);
        void run_a_cycle();
        void reset(board &b);
        void clear();
	void show_path();
        string inttostring(int i);
        string inttoGTPstring(int i);
        ~MCTStree();
};


#endif // MCTS_H
