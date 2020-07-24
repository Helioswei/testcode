#include <htmlcxx/html/ParserDom.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
using namespace htmlcxx;

int main(){
    //读取文件
    ifstream in("/root/bet365.html", ios::in);
    stringstream buffer;
    buffer << in.rdbuf();
    string file(buffer.str());
    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(file);
    tree<HTML::Node>::iterator it = dom.begin();
    for(;it != dom.end(); it++){
        
    }
    cout << dom << endl;
    
    return 0;



}
