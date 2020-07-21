#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void match() {
    // regex_match,匹配整个字符串符合某个规则
    string text = "Date:2020-10-10";
    // string text = "Date:2020-10-10,date:2010-11-11,DAta:2010-22-22";
    //构造正则表达式
    string pattern = "[a-zA-Z]*:\\d{4}-\\d{2}-\\d{2}";
    std::regex express(pattern);

    //匹配
    std::smatch result;
    // bool res = regex_match(text.cbegin(), text.cend(), result, express);
    bool res = regex_match(text, result, express);
    if (res) {
        cout << "匹配成功 " << endl;
        for (int i = 0; i < result.size(); i++) {
            cout << result[i] << endl;
        }

    } else {
        cout << "匹配失败 " << endl;
    }
}

void search() {
    // regex_search
    // 搜素正则表达式参数，但它不要求整个字符序列完全匹配。而且它只进行单次搜索，搜索到即停止继续搜索，不进行重复多次搜索
    string text =
        "Date:2020-10-10,date:2010-11-11,DAta:2010-22-22,0Date:2020-23-23,Date:"
        "2020-12-1a";
    //构造正则表达式
    // string pattern = "^[a-zA-Z]*:\\d{4}-\\d{2}-\\d{2}$";
    string pattern = "[a-zA-Z]*:\\d{4}-\\d{2}-\\d{2}";
    std::regex express(pattern);

    //匹配
    std::smatch result;
    while (regex_search(text, result, express)) {
        for (int i = 0; i < result.size(); i++) {
            cout << result[i] << endl;
        }
        text = result.suffix().str();
        cout << "text: " << text << endl;
    }
    // bool res = regex_search(text, result, express);
    // if (res) {
    //    cout << "匹配成功 " << endl;
    //    for (int i = 0; i < result.size(); i++) {
    //        cout << result[i] << endl;
    //    }

    //} else {
    //    cout << "匹配失败 " << endl;
    //}
}
void searchUrl() {
    // regex_search
    // 搜素正则表达式参数，但它不要求整个字符序列完全匹配。而且它只进行单次搜索，搜索到即停止继续搜索，不进行重复多次搜索
    ifstream in("/root/testa", ios::in);
    stringstream buffer;
    buffer << in.rdbuf();
    string text(buffer.str());
    //构造正则表达式
    // string pattern = "^[a-zA-Z]*:\\d{4}-\\d{2}-\\d{2}$";
    //string pattern = "[a-zA-Z]*:\\d{4}-\\d{2}-\\d{2}";
    //string pattern = "((ht|f)tps?):\/\/[\w\-]+(\.[\w\-]+)+([\w\-.,@?^=%&:\/~+#]*[\w\-@?^=%&\/~+#])";
    string xieyi = "(ht|f)tps?:\/\/";
    string yuming = "[a-zA-Z0-9][-a-zA-Z0-9]{0,62}(\.[a-zA-Z0-9][-a-zA-Z0-9]{0,62})+";
    string port = "(:[0-9]{1,5})?";
    string ziyuan = "[-a-zA-Z0-9()@:%_\\\+\.~#?&//=]*";
    string pattern = xieyi + yuming + port + ziyuan; 

    std::regex express(pattern);
    vector<string> urls;

    //匹配
    std::smatch result;
    while (regex_search(text, result, express)) {
        urls.push_back(result[0]);
        //for (int i = 0; i < result.size(); i++) {
        //    cout << result[i] << endl;
        //}
        text = result.suffix().str();
    }
    ofstream out("/root/url1", ios::app);
    for(int i = 0 ; i < urls.size(); i++){
        out << urls[i] << endl;
    }
}

int main() {

    // match();
    searchUrl();
   // search();
    // regex_search1
    // regex_replace

    return 0;
}
