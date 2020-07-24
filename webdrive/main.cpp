#include <webdriverxx/webdriverxx.h>
using namespace webdriverxx;


int main(){
    //const char* url = "http://192.168.50.74:4444/wd/hub";
    //WebDriver gc = Start(Chrome(), Capilities()/* required */, url);
    WebDriver gc = Start(Chrome());

    gc.Navigate("http://google.com").FindElement(ByCss("input[name=q]")).SendKeys("Hello, world!").Submit();
    return 0;






}
