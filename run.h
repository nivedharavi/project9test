//
// Created by Nivedha on 8/4/2017.
//
#include "Parse.h"
#include<iostream>
#include "express.h"
#include <vector>

String cmdText = String("text");
String cmdVar = String("var");
String cmdSet  = String("set");
String cmdOutput = String("output");
String cmdSkip = String("//");
String cmdDo = String("do");
String cmdOd = String("od");
String cmdIf = String("if");
String cmdElse = String("else");
String cmdFi = String("fi");
vector<saveExpression> parsing;
vector<saveExpression> ifloop;
vector<saveExpression> elseLoop;
int checkName(String val){ //will return 1 if it is any of the commands
    if (val == cmdText){
        return 1;
    }
    else if (val == cmdVar){
        return 2;
    } else if (val ==cmdSet){
        return 3;
    } else if (val==cmdOutput){
        return 4;
    } else if (val == cmdSkip){
        return 5;
    } else if (val == cmdDo){
        return 6;
    } else if (val == cmdOd){
        return 7;
    } else if (val == cmdIf){
        return 8;
    } else if (val == cmdFi){
        return 9;
    } else if (val==cmdElse){
        return 10;
    }
    else {return 0;}
}

void textOut(void){
    read_next_token();
    cout<<next_token();
    return;
}


  expressParse* output(void){
    expressParse* myTree = new expressParse();
    String next = String (peek_next_token());
    while(checkName(next)==0&&next_token_type!=END) {
        read_next_token();
        myTree->insert(next_token());
        next = String(peek_next_token());
    }
    return myTree;
}

void setVar(void){
    int result = 0;
    read_next_token();
    string newname;
    newname = next_token();
    if(vartable.find(newname)!= vartable.end()){
        cout<<"variable "<<newname<< " is incorrectly reinitialized"<<endl;
    }
    expressParse* checkTree = output();
    expressParse::node* resultNode = checkTree->getRoot();
    result = checkTree->eval(resultNode);
    vartable[newname]=result;
    delete(checkTree);
}
void set(void){
    int res3 = 0;
    read_next_token();
    string new2name;
    new2name = next_token();
    expressParse* myTree = output();
    expressParse::node* checkNode = myTree->getRoot();
    res3 = myTree->eval(checkNode);
    if(vartable.find(new2name)==vartable.end()){
        cout<< "variable "<<new2name<<" not delcared"<<endl;
        vartable[new2name] = res3;
    }
    else{
        vartable.find(new2name)->second = res3;
    }
    delete(myTree);
}



int checkDo(vector<saveExpression>& vecpass){
    int valcheck =0;
    String valmem = vecpass.front().rootfind->val;
    if ((valmem=="+")||(valmem == "-" )|| (valmem=="/")||(valmem=="*")||(valmem=="%")){
        valcheck = 0;
    }
    else{
        valcheck =vecpass.front().rootfind->evalulate();
    }
    return valcheck;

}
void outputFi( ){
    if (checkDo(ifloop)){
        for (vector<saveExpression>::size_type i = 1; i != ifloop.size(); i++) {
            String commandCheck = ifloop[i].cmdType;
            if (commandCheck==cmdOutput){
                int actResult = ifloop[i].rootfind->evalulate();
                cout<<actResult;
            }
            if (commandCheck==cmdText){
                cout<<ifloop[i].var;
            }
            if (commandCheck == cmdVar){
                string newname = ifloop[i].var;
                if(vartable.find(newname)!= vartable.end()){
                    cout<<"variable "<<newname<< " is incorrectly reinitialized"<<endl;
                }
                int rescheck = 0;
                rescheck = ifloop[i].rootfind->evalulate();
                vartable[newname]=rescheck;
            }
            if (commandCheck==cmdSet){
                int res3 = 0;
                string new2name = ifloop[i].var;
                res3 = ifloop[i].rootfind->evalulate();
                if(vartable.find(new2name)==vartable.end()){
                    cout<< "variable "<<new2name<<" not delcared"<<endl;
                    vartable[new2name] = res3;
                }
                else{
                    vartable.find(new2name)->second = res3;
                }
            }
        }
    }
    else{
        if(elseLoop.size()==0){ return;}
        for (vector<saveExpression>::size_type i = 1; i != elseLoop.size(); i++) {
            String commandCheck = elseLoop[i].cmdType;
            if (commandCheck==cmdOutput){
                int actResult = elseLoop[i].rootfind->evalulate();
                cout<<actResult;
            }
            if (commandCheck==cmdText){
                cout<<elseLoop[i].var;
            }
            if (commandCheck == cmdVar){
                string newname = elseLoop[i].var;
                if(vartable.find(newname)!= vartable.end()){
                    cout<<"variable "<<newname<< " is incorrectly reinitialized"<<endl;
                }
                int rescheck = 0;
                rescheck = elseLoop[i].rootfind->evalulate();
                vartable[newname]=rescheck;
            }
            if (commandCheck==cmdSet){
                int res3 = 0;
                string new2name = elseLoop[i].var;
                res3 = elseLoop[i].rootfind->evalulate();
                if(vartable.find(new2name)==vartable.end()){
                    cout<< "variable "<<new2name<<" not delcared"<<endl;
                    vartable[new2name] = res3;
                }
                else{
                    vartable.find(new2name)->second = res3;
                }
            }
        }
    }
    for (vector<saveExpression>::size_type i = 0; i != elseLoop.size(); i++) {
        delete (elseLoop[i].rootfind);
    }
    elseLoop.clear();
    for (vector<saveExpression>::size_type i = 0; i != ifloop.size(); i++) {
        delete (ifloop[i].rootfind);
    }
    ifloop.clear();

}

void ifParse(){
    expressParse* newTree = output(); // need to save expression regardless
    expressParse::node* roocheck = newTree->getRoot();
    saveExpression express;
    express.cmdType = cmdIf;
    express.rootfind = roocheck;
    ifloop.push_back(express);
    peek_next_token();
    String checkNext = String(next_token());
    while(checkNext!= cmdFi&& checkNext!=cmdElse){
        read_next_token();
        String command = String(next_token());
        if (command == cmdSkip){
            skip_line();
        }
        if (command==cmdVar || command == cmdSet||command ==cmdText){
            read_next_token();
            express.var = next_token();
        }
        expressParse* newTree = output();
        expressParse::node* rootcheck = newTree->getRoot();
        express.cmdType = command;
        express.rootfind = rootcheck;
        ifloop.push_back(express);
        peek_next_token();
        checkNext = String(next_token());
    }
    if (checkNext==cmdFi){
        return;
    }
    else if (checkNext==cmdElse) {
        expressParse *newTree = output(); // need to save expression regardless
        expressParse::node *roocheck = newTree->getRoot();
        saveExpression express;
        express.cmdType = cmdElse;
        express.rootfind = roocheck;
        elseLoop.push_back(express);
        peek_next_token();
        String checkNext = String(next_token());
        while (checkNext != cmdFi) {
            read_next_token();
            saveExpression express;
            String command = String(next_token());
            if (command == cmdSkip){
                skip_line();
                continue;
            }
            if (command == cmdVar || command == cmdSet || command == cmdText) {
                read_next_token();
                express.var = next_token();
            }
            expressParse *newTree = output();
            expressParse::node *rootcheck = newTree->getRoot();
            express.cmdType = command;
            express.rootfind = rootcheck;
            elseLoop.push_back(express);
            peek_next_token();
            checkNext = String(next_token());
        }

    }
}

void outputAll(){
    while(checkDo(parsing)) {
        for (vector<saveExpression>::size_type i = 1; i != parsing.size(); i++) {
            String commandCheck = parsing[i].cmdType;
            if (commandCheck==cmdOutput){
                int actResult = parsing[i].rootfind->evalulate();
                cout<<actResult;
            }
           else if (commandCheck==cmdText){
                cout<<parsing[i].var;
            }
            else if (commandCheck == cmdVar){
                string newname = parsing[i].var;
                if(vartable.find(newname)!= vartable.end()){
                    cout<<"variable "<<newname<< " is incorrectly reinitialized"<<endl;
                }
                int rescheck = 0;
                rescheck = parsing[i].rootfind->evalulate();
                vartable[newname]=rescheck;
            }
            else if (commandCheck==cmdSet){
                int res3 = 0;
                string new2name = parsing[i].var;
                res3 = parsing[i].rootfind->evalulate();
                if(vartable.find(new2name)==vartable.end()){
                    cout<< "variable "<<new2name<<" not delcared"<<endl;
                    vartable[new2name] = res3;
                }
                else{
                    vartable.find(new2name)->second = res3;
                }
            }
            else if (commandCheck == cmdIf){
                outputFi();
            }
        }
    }
    for (vector<saveExpression>::size_type i = 0; i != parsing.size(); i++){
        delete(parsing[i].rootfind); // free all the trees that were made in the tree
    }
    parsing.clear();
    for (vector<saveExpression>::size_type i = 0; i != elseLoop.size(); i++){
        delete(elseLoop[i].rootfind);
    }
    elseLoop.clear();
    for (vector<saveExpression>::size_type i = 0; i != ifloop.size(); i++){
        delete(ifloop[i].rootfind);
    }
    ifloop.clear();
    return;
}

void parseWhole(void){
    expressParse* newTree = output(); // need to save expression regardless
    expressParse::node* roocheck = newTree->getRoot();
     saveExpression express;
    express.cmdType = cmdDo;
    express.rootfind = roocheck;
    parsing.push_back(express);
    peek_next_token();
    String checkNext = String(next_token());
    while(checkNext!=cmdOd){
        read_next_token();
        saveExpression express;
        String command = String(next_token());
        if (command == cmdIf){
            ifParse();
        }
        if (command == cmdSkip){
            skip_line();
            continue;
        }
        if (command==cmdVar || command == cmdSet|| command == cmdText){
            read_next_token();
            express.var = next_token();
        }
        expressParse* newTree = output();
        expressParse::node* rootcheck = newTree->getRoot();
        express.cmdType = command;
        express.rootfind = rootcheck;
        parsing.push_back(express);
        peek_next_token();
        checkNext = String(next_token());
    }
    outputAll();
}



void run(){
    read_next_token();
    while (next_token_type!=END) {
        if (next_token_type == NAME) {
            String check = String(next_token());
            if (check == cmdText) {
                textOut();
            } else if (check == cmdVar) {
                setVar();
            } else if (check == cmdSet) {
                set();
            } else if (check == cmdOutput) {
                int res = 0;
                expressParse* rootfind = output();
                expressParse::node* rootcheck = rootfind->getRoot();
                res = rootfind->eval(rootcheck);
                cout << res;
                delete(rootfind);

            } else if (check == cmdDo) {
                parseWhole();

            } else if (check == cmdSkip){
                skip_line();
            } else if (check==cmdOd){

            } else if (check==cmdIf) {
                ifParse();
                outputFi();
            } else if (check == cmdFi){

            }

        }
        else if (next_token_type==SYMBOL){
            String skipcheck = String(next_token());
             if (skipcheck == cmdSkip){
                skip_line();
            }
        }
        read_next_token();

    }
}
