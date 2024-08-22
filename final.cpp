struct TreeNode{
	string name; 
	int lockedBy; 
	int lockedCount; // changes isLocked ( bool) to lockedCount(int)
	vector<TreeNode*> childs;
	TreeNode* parent; 
	set<TreeNode*> lockedDescendents;
}
bool lock(string name, int id){
		TreeNode* r= nameToTreeNodeMapping[name];
        if(r->lockedCount>0 || r->lockedDescendents.size())return false;
        r->lockedCount++;
        if(r->lockedCount>1){
            r->lockedCount--;
            return false;
        }
        TreeNode* par= r->parent;
        while(par){
            if(par->lockedCount>0 || r->lockedDescendents.size()>0){
                r->lockedCount--;
                TreeNode *par1= r->parent;
                while(par1!=par){
                    par1->lockedDescendents.erase(r);
                    par1=par1->parent;
                }
                return false;
            }
            par->lockedDescendents.insert(r);
            par=par->parent;
        }
        r->lockedBy=id;
        return true; 
    }
