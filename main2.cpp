/* Example of using the algorithm */
#include <bits/stdc++.h>
#include "Aho.hpp"
using namespace std;
#define ROOT 0

class HOG : public Aho{
    class Node {
        public:
        int data;
        Node* next; // Pointer to next node in DLL
        Node* prev; // Pointer to previous node in DLL
        Node(){
            next = NULL;
            prev = NULL;
        }
    };
    vector<bool> inH;
    vector<list<int>> list_of_leaves;
    vector<stack<int>> Sv;
    // vector<list<int>::iterator> S_it;
    vector<Node*> S_it;
    Node* S_head = NULL;
    Node* S_tail = NULL;
    // list<int> S;
    vector<bool> inS;
    int node_cnt;
    int leaves_cnt;
    int error_cnt = 0;
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
            print_error_cnt();
        } 
    private:
        void add_to_list(int leafVertex){

            if(!inS[leafVertex]){
                inS[leafVertex] = 1;
                Node* temp = new Node();
                temp->data = leafVertex;
                if(S_tail != NULL){
                    S_tail->next = temp;
                    temp->prev = S_tail;
                    S_tail = temp;
                }
                else{
                    S_tail = temp;
                    S_head = temp;
                }
                    S_it[leafVertex] = temp;
                
            }
        }
        void erase_from_list(int leafVertex){
            if(inS[leafVertex]){
                inS[leafVertex] = 0;
                auto it = S_it[leafVertex];
                if(S_head == it){
                    S_head = S_head->next;
                }
                if(S_tail == it){
                    S_tail = S_tail->prev;
                }
                auto l = it->prev;
                auto r = it->next;
                if(l){
                    l->next = r;
                }
                if(r){
                    r->prev = l;
                }
                // it->next = NULL;
                // it->prev = NULL;
            }
        }
        void print_error_cnt(){
            int cnt = 0;
            for(int x : inH){
                if(x == 1){
                    cnt++;
                }
            }
            cout << cnt << endl;
        }
        // void print_stacks_list(){
        //     for(auto it : S){
        //         cout << it << " ";
        //     }
        //     cout << endl;
        // }
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
            Sv.resize(leaves_cnt);
            inS.resize(leaves_cnt);
            S_it.resize(leaves_cnt);

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
            for(auto it : Sv){
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
            if(!trie[curVertex].is_leaf){
                list<int> &Lv = list_of_leaves[curVertex];
                for(auto it = Lv.begin(); it != Lv.end(); it++){
                    int leafVertex = *it;
                    leafVertex = trie[leafVertex].word_ID;
                    Sv[leafVertex].push(curVertex);
                    add_to_list(leafVertex);
                }
                for(auto it : trie[curVertex].children){
                    int nextVertex = it.second;
                    dfs_markH(nextVertex);
                }
                // internal node curVertex last visited
                for(auto it = Lv.begin(); it != Lv.end(); it++){
                    int leafVertex = *it;
                    leafVertex = trie[leafVertex].word_ID;
                    stack<int> &curStack = Sv[leafVertex];
                    curStack.pop();
                    if(!curStack.empty() && inH[curStack.top()] == 0){
                        add_to_list(leafVertex);
                    }
                    else{
                        erase_from_list(leafVertex);
                    }
                }
            }
            // internal node curVertex is a leaf
            if(trie[curVertex].is_leaf){
                auto temp = S_head;
                int cycle_cnt = 0;
                while(temp != NULL){
                    // cout << cycle_cnt++ << endl;
                    int leafVertex = temp->data;
                    stack<int> &curStack = Sv[leafVertex];
                    inH[curStack.top()] = 1;
                    erase_from_list(leafVertex);
                    temp = temp->next;
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
    vector<string> dictionary ;
    ifstream indata;
    indata.open("cases/case3.txt");
    if(!indata) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      exit(1);
    }
    string str;
    while ( !indata.eof() ) { // keep reading until end-of-file
        indata >> str; // sets EOF flag if no value found
        dictionary.push_back(str);
    }
    indata.close();

    //Measuring start time
    auto begin = std::chrono::high_resolution_clock::now();


    HOG HOGtrie {};
    for(auto &x: dictionary){
        HOGtrie.add_string(x);
    }
    HOGtrie.prepare();

    //Measuring End time
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    printf("Time measured: %.6f seconds.\n", elapsed.count() * 1e-9);
    return 0;
}
