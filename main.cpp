/* Example of using the algorithm */
#include <bits/stdc++.h>
#include "Aho.hpp"
using namespace std;
#define ROOT 0
class HOG : public Aho{
    vector<bool> inH;
    vector<list<int>> list_of_leaves;
    vector<stack<int>> stack_of_exposed_suffix;
    int node_cnt;
    int leaves_cnt;
    public:
        void prepare(){
            Aho::prepare();
            node_cnt = size;
            leaves_cnt = wordID;
            list_of_leaves.resize(node_cnt);
            for(int i = 0; i < leaves_cnt; i++){
                int y = get_suffix_link(leaves[i]);
                while(y != ROOT){
                    list_of_leaves[y].push_back(leaves[i]);
                    y = get_suffix_link(y);
                }
            }
            markH();
            for(bool x : inH){
                cout << x << " ";
            }
            cout << endl;
        } 
    private:
        void markH(){
            inH.resize(node_cnt);
            for(int i = 0; i < leaves_cnt; i++){
                inH[leaves[i]] = 1;
            }
            inH[ROOT] = 1;
            stack_of_exposed_suffix.resize(leaves_cnt);
            dfs_markH(ROOT);
        }
        void dfs_markH(int curVertex){
            // internal node curVertex first visited

            list<int> &curList = list_of_leaves[curVertex];
            for(auto it = curList.begin(); it != curList.end(); it++){
                int leafVertex = *it;
                stack_of_exposed_suffix[trie[leafVertex].word_ID].push(curVertex);
            }

            // internal node curVertex is a leaf
            if(trie[curVertex].is_leaf){
                for(int leafVertex : leaves){
                    stack<int> &curStack = stack_of_exposed_suffix[trie[leafVertex].word_ID];
                    if(!curStack.empty()){
                        inH[curStack.top()] = 1;
                    }
                }
            }
            for(auto it : trie[curVertex].children){
                int nextVertex = it.second;
                dfs_markH(nextVertex);
            }
            // internal node curVertex last visited
            for(auto it = curList.begin(); it != curList.end(); it++){
                int leafVertex = *it;
                stack_of_exposed_suffix[trie[leafVertex].word_ID].pop();
            }
        }
};
void dfs(Aho &ahotrie, int curVertex, vector<bool> &inH){
    if(!inH[curVertex]){
        // merge_curVertex_with_its_parent();
    }
    vector<int> children = ahotrie.get_children(curVertex);
    for(int nextVertex : children){
        dfs(ahotrie, nextVertex, inH);
    }
}
int main()
{
    vector<string> dictionary {"aabaa", "aadbd", "dbdaa"};
    
    HOG HOGtrie {};
    for(auto &x: dictionary){
        HOGtrie.add_string(x);
    }
    HOGtrie.prepare();
    return 0;
}