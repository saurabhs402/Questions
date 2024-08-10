#include <bits/stdc++.h>

using namespace std;

struct TreeNode
{                                    
    string name;                       
    int lockedByUid;                     
    bool isLocked;                     
    vector<TreeNode *> childrens;         
    TreeNode *parent;                  
    set<TreeNode *> lockedDescendants; 

    TreeNode(string nodeName, TreeNode *p)
    { 
        name = nodeName;
        lockedByUid = -1; 
        parent = p;
        isLocked = false; 
    }

    void addChildren(vector<string> &childNames) 
    { 
        for (string &nodeName : childNames)
        {
            childrens.push_back(new TreeNode(nodeName, this));
        }
    }
};

struct Tree
{
    TreeNode *root;                                          
    unordered_map<string, TreeNode *> nameToNodeMap; 
    Tree(string nodeName)
    {
        root = new TreeNode(nodeName, nullptr);
    }

    void makeTree(vector<string> &nodeNames, int m)
    {                        
        queue<TreeNode *> q; 
        int k = 1, i, n = nodeNames.size();
        q.push(root); 
        while (!q.empty())
        { 
            TreeNode *node = q.front();
            q.pop();
            nameToNodeMap[node->name] = node;
            vector<string> childNames;
            for (i = k; i < min(n, k + m); i++)
            {
                childNames.push_back(nodeNames[i]);
            }
            node->addChildren(childNames);
            for (TreeNode *children : node->childrens)
            { 
                q.push(children);
            }
            k = i;
        }
    }

    void print(TreeNode *r)
    { // method is used to print tree (for debug perpose).
        if (!r)
            return;
        cout << "TreeNode -> " << r->name << " " << r->lockedByUid << " " << "\n";
        cout << "childrens -> \n";
        for (TreeNode *child : r->childrens)
        {
            cout << "       " << child->name << "\n";
        }
        cout << "Locked -> \n";
        for (TreeNode *child : r->lockedDescendants)
        {
            cout << "       " << child->name << "\n";
        }
        for (TreeNode *child : r->childrens)
        {
            print(child);
        }
    }
   
   
    void updateParentDescendants(TreeNode *node, TreeNode *curr)
    { 
        while (node)
        {
            node->lockedDescendants.insert(curr);
            node = node->parent;
        }
    }

    bool lock(string name, int uid)
    {                                             
        TreeNode *node = nameToNodeMap[name];
        if (node->isLocked || node->lockedDescendants.size())
            return false; 
        TreeNode *p = node->parent;

        // Ancestor check
        while (p)
        { 
            if (p->isLocked)
                return false;
            p = p->parent;
        }
        updateParentDescendants(node->parent, node); // inform all ancestor about its locking in O(logm(n) time)
        node->isLocked = true;          // lock the node
        node->lockedByUid = uid;
        return true;
    }

    bool unlock(string name, int uid)
    {
        TreeNode *node = nameToNodeMap[name];
        if (!node->isLocked || node->lockedByUid != uid)
            return false; 
        TreeNode *p = node->parent;
        while (p)
        { 
            p->lockedDescendants.erase(node);
            p = p->parent;
        }
        node->isLocked = false; 
        node->lockedByUid = -1;
        return true;
    }
    bool upgradeLock(string name, int uid)
    {
        TreeNode *node = nameToNodeMap[name];
        if (node->isLocked || node->lockedDescendants.size() == 0)
            return false; 
        for (TreeNode *lockedDescendant : node->lockedDescendants)
        { 
            if (lockedDescendant->lockedByUid != uid)
                return false;
        }
        TreeNode *p = node->parent;
        while (p)
        { 
            if (p->isLocked)
                return false;
            p = p->parent;
        }
        set<TreeNode *> stt = node->lockedDescendants;
        for (TreeNode *lockedDescendant : stt)
        { 
            unlock(lockedDescendant->name, uid);
        }
        lock(name, uid); 
        return true;
    }
};
int main()
{
    int n, m, q, i, opType, uid;
    string name;
    cin >> n >> m >> q;
    vector<string> nodeNames(n);

    for (i = 0; i < n; i++){
         cin >> nodeNames[i];
    }
    
    Tree *tree = new Tree(nodeNames[0]);

    tree->makeTree(nodeNames, m);

    tree->print(tree->root);
 
    while (q--)
    {
        cin >> opType >> name >> uid;
        if (opType == 1)
        {
            if (tree->lock(name, uid))
            {
                cout << "true";
            }
            else
                cout << "false";
        }
        else if (opType == 2)
        {
            if (tree->unlock(name, uid))
            {
                cout << "true";
            }
            else
                cout << "false";
        }
        else if (opType == 3)
        {
            if (tree->upgradeLock(name, uid))
            {
                cout << "true";
            }
            else
                cout << "false";
        }
        cout << "\n";
    }
}