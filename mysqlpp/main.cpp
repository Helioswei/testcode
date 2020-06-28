#include <iostream>
#include <mysql++.h>

using namespace std;
int main(int argc, char* argv[]) {
    string dbname("test");
    string ip("192.168.50.74");
    string user("root");
    string pwd("helios2020");
    int port = 3306;
    try {
        mysqlpp::Connection conn(false);
        mysqlpp::SetCharsetNameOption* opt =
            new mysqlpp::SetCharsetNameOption("utf8");

        conn.set_option(opt);
        if (conn.connect(dbname.c_str(), ip.c_str(), user.c_str(), pwd.c_str(),
                         port)) {
            cout << "connect successfule" << endl;
        } else {
            cout << "connect failed" << endl;
            return -1;
        }
    }
    catch (const mysqlpp::BadQuery& er) {
        cerr << "Error: " << er.what() << endl;
    }
    catch (const mysqlpp::BadConversion& er) {
        cerr << "Error: " << er.what() << endl;
    }
    catch (const mysqlpp::Exception& er) {
        cerr << "Error: " << er.what() << endl;
    }
    // DMySql mysql(dbname, ip, user, pwd, port);
    // if (mysql.isConnected()){
    //     cout << "database test connect success" << endl;
    // }else {
    //     cout << "connect failed" << endl;
    //     return -1;
    // }
    return 0;
}
