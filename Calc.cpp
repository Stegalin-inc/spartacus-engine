#ifndef CALC
#define CALC
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <ctype.h>
#include <sstream>
#include <cmath>
#include "Lex.cpp"
#include "Gobj.cpp"
using namespace std;

class Calc
{
    public:
    static map<string, double> staticVars;
    map<string, double*> dVars;
	map<string, int*> iVars;
	map<string, float*> fVars;
    enum code{EQ,THISS,RET,REM,DUP,LOCAL,IGET,FGET,DGET,CALL,GET};
    double stak[20];
    
    vector<Token> vect;
    map<string, int> funcs;
    Lex l;
    Calc(Lex& ll):l(ll)
	{
 
        for(Token t = l.nextToken();t.type!='\0';t = l.nextToken())
        {
            if(t.type=='n')
            t.dval=stox<double>(t.val);
            else if(t.type=='w')
            {
                if(t.val=="eq") t.type=code::EQ;
                else if(t.val=="this") t.type=code::THISS;
				else if(t.val=="ret")t.type=code::RET;
                else if(t.val=="rem") t.type=code::REM;
                else if(t.val=="dup") t.type=code::DUP;
                else if(t.val=="local") t.type=code::LOCAL;
                else if(funcs.find(t.val)!=funcs.end()) t.type=code::CALL;
                else if(iVars.find(t.val)!=iVars.end()) t.type=code::IGET;
				else if(fVars.find(t.val)!=fVars.end()) t.type=code::FGET;
				else if(dVars.find(t.val)!=dVars.end()) t.type=code::DGET;
                //else if(par!=nullptr&&par->vars.find(vect[p].val)!=par->vars.end()){int loc=par->get<int>(vect[p].val);stak[i++]=loc;}
                //if(vect[p].val=="print") {stak[i-2] = stak[i-2] == stak[i-1]; --i;}
                //else throw invalid_argument(vect[p].val+" this token not allowed");
            }
            if(t.type==':') 
            {
                vect.push_back(t);
                t = l.nextToken();
                funcs[t.val] = vect.size();
            }
            vect.push_back(t);
        }
	}
	template<typename T> T stox(string s){stringstream ss;ss<<s;T d;ss>>d;return d;}
    template<typename T> string xtos(T s){stringstream ss;ss<<s;string d;ss>>d;return d;}
	void addvar(int* v, string name){iVars[name]=(v);}
	void addvar(float* v, string name){fVars[name]=(v);}
	void addvar(double* v, string name){dVars[name]=(v);}
    double calc(Gobj* par)
    {
		int i=0;
        vector<int> callret;
        vector<int> local;
		local.push_back(0);
        for(int p = 0; p < vect.size(); ++p)
        {
            switch (vect[p].type)
            {
            case 'n': stak[i++] = vect[p].dval; break;
            case '+': stak[i-2] += stak[i-1]; --i; break;
            case '-': stak[i-2] -= stak[i-1]; --i; break;
            case '*': stak[i-2] *= stak[i-1]; --i; break;
            case '/': stak[i-2] /= stak[i-1]; --i; break;
            case '<': stak[i-2] = stak[i-2] < stak[i-1]; --i; break;
            case '>': stak[i-2] = stak[i-2] > stak[i-1]; --i; break;
            case '^': stak[i-2] = pow(stak[i-2], stak[i-1]); --i; break;
            case '=': stak[(int)stak[i-2]] = stak[i-1]; i-=2; break;
            case 'b': if((bool)stak[--i])while(vect[p].type!='@')--p; break;
            case 'f': if((bool)stak[--i])while(vect[p].type!='@')++p; break;
			case '?': stak[i-3]=(bool)stak[i-3]?stak[i-2]:stak[i-1];i-=2;  break;
            case '$': stak[i-1] = stak[(int)stak[i-1]]; break;
            case '!': stak[i-1] = !(bool)stak[i-1]; break;
            case ':': while(vect[p].type!=';')++p; break;
            case ';': p=callret[callret.size()-1]; callret.pop_back(); local.pop_back(); break;
            case '@': break;
            case code::EQ: {stak[i-2] = stak[i-2] == stak[i-1]; --i;}break;
            case code::THISS: {stak[i++]=i;}break;
			case code::RET: {return stak[i-1];}break;
            case code::REM: {--i;}break;
            case code::DUP: {stak[i]=stak[i-1];++i;}break;
            case code::LOCAL: {stak[i++]=local[local.size()-1];}break;
            case code::CALL: {local.push_back(i-1);callret.push_back(p);p=funcs[vect[p].val];}break;
            case code::IGET: {stak[i++]=*iVars[vect[p].val];}break;
			case code::FGET: {stak[i++]=*fVars[vect[p].val];}break;
			case code::DGET: {stak[i++]=*dVars[vect[p].val];}break;
            case 'w':
                
                if(par!=nullptr&&par->vars.find(vect[p].val)!=par->vars.end()){int loc=par->get<int>(vect[p].val);stak[i++]=loc;}
                //if(vect[p].val=="print") {stak[i-2] = stak[i-2] == stak[i-1]; --i;}
                else throw invalid_argument(vect[p].val+" this token not allowed");
                break;
            default: throw invalid_argument(xtos(vect[p].type)+" this type not allowed"); break;
            };//for(int a=0;a<i;++a)cout<<stak[a]<<", ";cout<<endl;
        }
        return stak[i-1];
    }
}; 
#endif