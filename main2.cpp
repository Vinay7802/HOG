/* Example of using the algorithm */
#include <bits/stdc++.h>
#include "Aho.hpp"
using namespace std;
#define ROOT 0
class HOG : public Aho{
    vector<bool> inH;
    vector<list<int>> list_of_leaves;
    vector<stack<int>> stack_of_exposed_suffix;
    vector<list<int>::iterator> list_of_stacks_it;
    list<int> list_of_stacks;
    vector<bool> inS;
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
            update_tree();
            print_h();
        } 
    private:
        void erase_from_list(int leafVertex){
            if(inS[leafVertex]){
                inS[leafVertex] = 0;
                auto it = list_of_stacks_it[leafVertex];
                list_of_stacks.erase(it);
            }
        }
        void add_to_list(int leafVertex){
            if(!inS[leafVertex]){
                inS[leafVertex] = 1;
                list_of_stacks.push_back(leafVertex);
                auto it = list_of_stacks.end();
                it--;
                list_of_stacks_it[leafVertex] = it;
            }
        }
        void print_stacks_list(){
            for(auto it : list_of_stacks){
                cout << it << " ";
            }
            cout << endl;
        }
        void print_h(){
            for(bool x : inH){
                cout << x << " ";
            }
            cout << endl;
        }
        void markH(){
            inH.resize(node_cnt);
            for(int i = 0; i < leaves_cnt; i++){
                inH[leaves[i]] = 1;
            }
            inH[ROOT] = 1;
            stack_of_exposed_suffix.resize(leaves_cnt);
            inS.resize(leaves_cnt);
            list_of_stacks_it.resize(leaves_cnt);
            dfs_markH(ROOT);
        }
        void print_s(stack<int> s){
            while(!s.empty()){
                cout << s.top() <<" ";
                s.pop();
            }
            cout << endl;
        }
        void print_stacks(){
            int i = 0;
            for(auto it : stack_of_exposed_suffix){
                cout << "\t";
                cout << "Stack " << i++ << " ";
                print_s(it);
            }
        }
        void print_leaves(list<int> curList){
            cout << "list of leaves ";
            for(auto it = curList.begin(); it != curList.end(); it++){
                cout << *it << ", ";
            }
            cout << endl;
        }
        void print_ins(){
            for(int x : inS){
                cout << x << " ";
            }
            cout << endl;
        }
        void dfs_markH(int curVertex){
            // internal node curVertex first visited
            list<int> &curList = list_of_leaves[curVertex];
            for(auto it = curList.begin(); it != curList.end(); it++){
                int leafVertex = *it;
                leafVertex = trie[leafVertex].word_ID;
                stack_of_exposed_suffix[leafVertex].push(curVertex);
                add_to_list(leafVertex);
            }
            for(auto it : trie[curVertex].children){
                int nextVertex = it.second;
                dfs_markH(nextVertex);
            }
            // internal node curVertex last visited
            for(auto it = curList.begin(); it != curList.end(); it++){
                int leafVertex = *it;
                leafVertex = trie[leafVertex].word_ID;
                stack<int> &curStack = stack_of_exposed_suffix[leafVertex];
                curStack.pop();
                if(!curStack.empty() && inH[curStack.top()] == 0){
                    add_to_list(leafVertex);
                }
                else{
                    erase_from_list(leafVertex);
                }
            }
            // internal node curVertex is a leaf
            if(trie[curVertex].is_leaf){
                for(auto leafVertex: list_of_stacks){
                    stack<int> &curStack = stack_of_exposed_suffix[leafVertex];
                    inH[curStack.top()] = 1;
                    erase_from_list(leafVertex);
                }
            }
        }
        void update_tree(){
            dfs_update_tree(ROOT);
        }
        void dfs_update_tree(int curVertex){
            if(!inH[curVertex]){
                
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