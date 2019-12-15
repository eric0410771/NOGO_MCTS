#ifndef MCTS_H
#define MCTS_H
#include "ucbnode.h"
#define raveb 0.0001
class MCTStree
{
public:
	int selectlist[BOARDSSIZE];
	int slsize;
	int bsize,wsize,tsize;
	int bone[BOARDSSIZE],wone[BOARDSSIZE],two[BOARDSSIZE];
	int sbnum,swnum;//select num
	int total;
	int totalnode;

	ucbnode* root;
	board rboard;
    	MCTStree();
    	~MCTStree();
    	vector<ucbnode*> path;
	

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
};


#endif // MCTS_H
