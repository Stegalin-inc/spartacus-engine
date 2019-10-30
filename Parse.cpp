#ifndef PARSE
#define PARSE
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <ctype.h>
#include <sstream>
#include "Lex.cpp"
;using namespace std;
struct Node
{
    string name;
    vector<Node*> children;
    map<string, string> values;
    void print(string space=" ", int l = 0)
    {
        for(int i=0;i<l;++i)cout<<space;
        cout<<name<<" ";
        cout << "{\n";
		map<string, string>::iterator it = values.begin();
		for (;it!=values.end();++it)
        {
            for(int i=0;i<l+1;++i)cout<<space;
			cout<<it->first<<":"<<it->second<<endl;
        }
		for (int i = 0; i< children.size();++i)
        {
            //for(int i=0;i<l+1;++i)cout<<space;
            //PRV(key);
            children[i]->print(space, l+1);
        }
        for(int i=0;i<l;++i)cout<<space;
        cout << "}\n";
    }
};
/* pair:=name:value
node:=name{[pair|node ]} */
class Parse
{
    public:
    Lex l;
    Token currentToken;
    Token lookAHead;
    Node root;
    Parse(Lex& ll):l(ll)
    {
        lookAHead = l.nextToken();
    }
    Token nextToken()
    {
        currentToken = lookAHead;
        lookAHead = l.nextToken();
        //cout << "Readed "<<currentToken.val<<" type "<<currentToken.type<<endl;
        return currentToken;
    }
    Node* readNode(bool isRoot=true)
    {
        static int ent = 0;
        //cout<<"ent # "<<ent++<<endl;
        Token t;
        if(isRoot)
        {
            t = nextToken();
            if(t.type!='w') throw invalid_argument("type != w");
        }else{t=currentToken;}
        Node* n = new Node();
        string namen = t.val;
        n->name = namen;
        t = nextToken();
        if(t.type!='{') throw invalid_argument("type != {");
        for (t=nextToken(); ; t=nextToken())
        {
            if(lookAHead.type==':')
            {
                string namev = t.val;
                nextToken();
                n->values[namev] =nextToken().val;
            } else if(lookAHead.type=='{')
            {
                string namec = t.val;
                n->children.push_back(readNode(false));
            }
            else /* if(t.type=='\0') */ throw invalid_argument("type == 0");
            if(lookAHead.type=='}') break;
        }
        t=nextToken();
        if(t.type!='}') throw invalid_argument("type != }");
        return n; 
    }
};
#endif