#include <iostream>
#include <vector>

#include <Poco/Exception.h>
#include <Poco/Format.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Tuple.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/MySQL/Utility.h>
#include <Poco/Data/MySQL/MySQL.h>
#include <Poco/Data/StatementImpl.h>
#include <Poco/Data/SessionPool.h>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>


#include "Poco/File.h"
#include "Poco/Path.h"


using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::format;
using Poco::DateTime;
using Poco::NumberParser;
using Poco::Any;
using Poco::AnyCast;
using Poco::Int32;
using Poco::Nullable;
using Poco::Tuple;
using Poco::DynamicAny;


int main(int argc, char* argv[]) {

    Poco::File tmp("/root/deployvim.sh");
    tmp.copyTo("./deddd.sh");
    return 0;
    string dbConnString =
        "host=192.168.50.237;port=3306;user=root;password=helios2020;db=test;"
        "compress=true;auto-reconnect=true";
    Poco::Data::MySQL::Connector::registerConnector();
    //与数据库建立一个连接池
    Poco::Data::SessionPool pool(Poco::Data::MySQL::Connector::KEY,
                                 dbConnString, 1, 32, 10);
    //从数据库连接池中获取一个数据库连接
    Poco::Data::Session ses(pool.get());

    //如果月数据库建立会话成功，输出连接信息
    if (ses.isConnected())
        cout << "*** Connected to " << '(' << dbConnString << ')' << endl;

    //如果有表user，则删除
    ses << "drop table if exists user", now ;

    //把查询结果存储到容器中
    std::vector<string> names;

    ses << "show databases", into(names), now;
    //输出查询结果，此处列出数据库的名称
    for (std::vector<string>::const_iterator it = names.begin();
         it != names.end(); it++) {
        cout << *it << endl;
    }
    //创建一个表，名字为user,字段名为：name,birth
    ses << "create table  user(name VARCHAR(20),birth VARCHAR(20));", now;
    //实现数据记录的插入
    DateTime bd(1980, 4, 1);
    DateTime ld(1982, 5, 9);
    ses << "insert into user values('weiyang',?)", use(bd), now;
    ses << "insert into user values('suming',?)", use(ld), now;

    //实现查询的方法，并输出查询的结果
    std::vector<string> res;
    ses << "select * from user where name like 'suming'", into(res), now;
    for (std::vector<string>::const_iterator it = res.begin(); it != res.end();
         it++)
        cout << *it << endl;

    Statement select(ses);
    select << "select * from user";
    select.execute();

    //创建记录表
    RecordSet rs(select);
    std::size_t cols = rs.columnCount();
    //输出列名
    for(std::size_t col = 0; col < cols; ++col)
        cout << rs.columnName(col) << endl;

    //输出所有的查询结果
    bool more = rs.moveFirst();
    while(more){
    
        for(std::size_t col = 0; col < cols; ++col)
            cout << rs[col].convert<std::string>() << "\t";
        more = rs.moveNext();
    
    
    }
    ses.close();
    Poco::Data::MySQL::Connector::unregisterConnector();
    return 0;
}
