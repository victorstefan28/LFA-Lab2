#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;


map <string, set<pair<string, string>>> nfa;
//map <string, set<string>> inchideri_set;
vector<string> finals;


void comp_inchideri_stare(map <string, set<pair<string, string>>> &nfa, string state, set<string>& inchideri)
{
    inchideri.insert(state);

    for(auto tr : nfa[state])
        if(tr.first == "$")
        {
            string next = tr.second;
            comp_inchideri_stare(nfa, next, inchideri);
        }
}


int main()
{
    int finals_num = 0;
    string init_state, aux;
    ifstream f("../nfa.in");
    f>>init_state;
    f>>finals_num;
    for(int i = 0; i<finals_num; i++){
        f>>aux;
        finals.push_back(aux);
    }
    string from,c,to;
    while(f>>from>>c>>to)
    {
        if(nfa[from].empty())
            nfa.insert({from, set<pair<string,string>>{}});
        nfa[from].insert({c,to});
        //cout<<from<<' '<<c<<' '<<to<<'\n';
    }
    set<string> Q; /// stare initiala DFA
    map <set<string>, map<string, set<string>>> dfa;
    map <set<string>, bool> dfa_finals;
    set<set<string>> keys;
    //Q.insert(init_state);
    //dfa[init_state] = nfa[init_state];
    comp_inchideri_stare(nfa, init_state, Q);

//    for(auto x : nfa){
//        cout<<x.first<<':';
//        for(auto y : x.second)
//            cout<<y.first<<' '<<y.second<<'\n';
//    }
    set<string> initiale_DFA;
    initiale_DFA = Q;
    cout<<"Stare initiale:[";
    for(auto x : initiale_DFA)
    {
        cout<<x<<' ';
    }
    cout<<"]\n";
    /// starea initiala
    set<set<string>> v; // visited
    queue<set<string>> q;/// bfs
    q.push(Q);
    while(!q.empty())
    {
        set<string> c_state = q.front();
        q.pop();
        if(v.count(c_state) > 0)
            continue;
        v.insert(c_state);
        set<string> sym;
        for(auto state : c_state)
            for(auto trans : nfa[state])
                if(trans.first != "$")
                    sym.insert(trans.first); /// sym - toate simbolurile nevide
        for(auto c_sym : sym)
        {
            set<string> next_s; /// next_s va contine toate tranzitiile in care se poate ajunge din starea curenta
            for(auto state : c_state) {
                for (auto trans: nfa[state]) {
                    if (trans.first == c_sym)
                    {

                        next_s.insert(trans.second);
                        comp_inchideri_stare(nfa, trans.second, next_s);
                    }

                }
            }
            if(!next_s.empty())
            {
                dfa[c_state][c_sym] = next_s;
                keys.insert(c_state);
                keys.insert(next_s);
                dfa_finals[c_state] = false;
                dfa_finals[next_s] = false;
                q.push(next_s); /// pt starea curenta si simbolul curent actualizam tranzitia si trecem la urmatoarea stare
            }
        }
    }
    for(auto x : dfa)
    {

//        for(int i = 0; i<x.second.size(); i++) {
//            cout<<"[";
//            for (auto y: x.first) {
//                cout << y << ' ';
//            }
//            cout << "]--";
//            for(auto z : x.second)
//            {
//                cout<<z.first<<"-->[";
//                for(auto to : z.second)
//                    cout<<to<<' ';
//                cout<<"]";
//                cout<<'\n';
//            }
//        }
        for(auto z : x.second)
        {
            cout<<"[";
            for (auto y: x.first) {
                cout << y << ' ';
            }
            cout << "]--";
            cout<<z.first<<"-->[";
            for(auto to : z.second)
                cout<<to<<' ';
            cout<<"]";
            cout<<'\n';
        }

    }
    for(auto s : keys)
        for(auto x : finals)
            if(s.count(x) != 0)
                dfa_finals[s] = true;
    cout<<"Stari Finale: ";
    for(auto s : keys) {
        if (dfa_finals[s]) {
            cout<<"[";
            for (auto stare: s)
                cout << stare << ' ';
            cout<<"] ";
        }
    }
    return 0;
}