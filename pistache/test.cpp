#include <iostream>
#include <string>
#include <memory>
using namespace std;


class A{
    public:
        A(string name, int age):_name(name),_age(age){}
        const string& name() const{
            return _name;
        }
        int age() const{
            return _age;
        }
    private:
        string _name;
        int _age;
};
int main(){
    std::shared_ptr<A> a(new A("weiyang", 23));
    cout << a -> name() << endl;
    cout << a -> age() << endl;


}
