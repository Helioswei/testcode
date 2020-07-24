#include <htmlcxx/html/ParserDom.h>
#include <mycore/mystring.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
using namespace htmlcxx;

int main() {
    //读取文件
    ifstream in("/root/bet365.html", ios::in);
    stringstream buffer;
    buffer << in.rdbuf();
    string file(buffer.str());
    setlocale(LC_ALL, ".OCP");
    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(file);
    tree<HTML::Node>::iterator it = dom.begin();
    for (; it != dom.end(); it++) {
        // if (it->isComment()) {
        //    continue;  //没有搞懂这个参数
        //}
        if (it->isTag()) {
            if (it->tagName() == "li") {
                //解析标签属性的值例如 <font id = "111"></font> 可以获取id 的值
                it->parseAttributes();
                if (it -> attribute("description").first){
                    cout << it -> text() << endl;
                    cout << it -> attribute("date").second << endl;
                    cout << it -> attribute("epoch_date").second << endl;
                    cout << it -> attribute("str_date").second << endl;
                    cout << it -> attribute("sport_id").second << endl;
                    cout << it -> attribute("place_terms_description").second << endl;
                    cout << it -> attribute("meeting_name").second << endl;
                    cout << it -> attribute("class").second << endl;
                    cout << it -> attribute("description").second << endl;
                
                }

               // std::pair<bool, std::string> description =
               //     it->attribute("description");
            }
        }
    }
    // cout << dom << endl;

    return 0;
}
