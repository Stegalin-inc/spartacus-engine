#ifndef LEX
#define LEX
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <ctype.h>
#include <sstream>
using namespace std;
struct Token
{
    char type;
    string val;
    double dval;
};
class Lex{
    public:
    string str ;
	istream* istt;
    char lookAhead;
    char currentChar;
	int mode;
	int pos;
	char nextChara()
	{
		
		switch(mode)
		{
			case 0:
				
				if(pos==str.size()) return '\0';
				return str[pos++];
			case 1:
				if(istt->eof()) return '\0';
				return (char)istt->get();
		}		
	}
    Lex(string s)
    {
        str=s;
        mode=0;
		pos=0;
		lookAhead=nextChara();
    }
	Lex(istream &ist):istt(&ist)
    {
        mode=1;
        lookAhead=nextChara();
    }
    char nextChar()
    {
        /*static  int pos = 0;
        if(pos==str.size()) return '\0';
        return str[pos++];*/
        currentChar = lookAhead;
        lookAhead = nextChara();
        return currentChar;
    }

    string nextNum()
    {
        int i = 1;
        bool dot1 = true;
        string buf;
        buf.push_back(currentChar);
        while(1)
        {
            switch (lookAhead)
            {
                case '0':case '1':case '2':case '3':case '4':
                case '5':case '6':case '7':case '8':case '9':
                buf.push_back (nextChar());
                break;
                case '.':
                if(dot1)buf.push_back (nextChar());
                dot1=false;
                break;
                default: return buf;
            }
        }
    }
    string nextWord()
    {
        int i = 1;
        string buf;
        buf.push_back(currentChar);
        while(1)
        {
            if(isalnum(lookAhead))
                buf.push_back(nextChar());
            else
            {
                return buf;
            }
        }
    }
    string nextString(char bra)
    {
        int i = 0;
        string buf;
        //buf[0] = currentChar();
        while(1)
        {
            char c = nextChar();
            if(c==bra)
                return buf;
            else if(c=='\\')
            {
                char c = nextChar();
                switch (c)
                {
                case 't': buf.push_back('\t'); break;
                case 'n': buf.push_back('\n'); break;
                case '0': buf.push_back('\0'); break;
                case '\'': buf.push_back('\''); break;
                case '"': buf.push_back('"'); break;
                case '\\': buf.push_back('\\'); break;
                case '\n': buf.push_back('\n'); break;
                default:
                    printf("error uncnown escape sequence '%c' at %d", c, -1);
                    break;
                }
            }
            else
                buf.push_back(c);
        }
    }
    Token nextToken()
    {
        Token t;
        char c = nextChar();
        while(1)
        switch (c)
        {
        case '.': t.type='.'; return t;
        case '+': t.type='+'; return t;
        case ',': t.type=','; return t;
        case ':': t.type=':'; return t;
        case '-': t.type='-'; return t;
        case '*': t.type='*'; return t;
        case '/': t.type='/'; return t;
        case '=': t.type='='; return t;
        case '(': t.type='('; return t;
        case ')': t.type=')'; return t;
        case '{': t.type='{'; return t;
        case '}': t.type='}'; return t;
        case '<': t.type='<'; return t;
        case '>': t.type='>'; return t;
        case '[': t.type='['; return t;
        case ']': t.type=']'; return t;
        case '@': t.type='@'; return t;
        case '$': t.type='$'; return t;
        case '!': t.type='!'; return t;
        case '^': t.type='^'; return t;
        case ';': t.type=';'; return t;
        case '?': if(lookAhead=='>'){t.type='f';nextChar();}else if(lookAhead=='<'){t.type='b';nextChar();}else t.type='?'; return t;
        case '\0': t.type='\0'; return t;
        case '"':case '\'': t.type='s'; t.val=nextString(c); return t;
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
        t.type='n';
        t.val=nextNum();
        return t;
        default:
        if(isalpha(c)){
        t.type='w';
        t.val=nextWord();
        return t;}
        c=nextChar();
            break;
        }
    }
};
#endif