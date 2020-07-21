#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/AutoPtr.h>
#include <iostream>
#include <string>

using namespace std;

int main(){
    cout << 111111 << endl;
    Poco::XML::DOMParser parser;
    parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, false);
    Poco::XML::AutoPtr<Poco::XML::Document> pDoc = parser.parse("/root/file.xml");

    string node1 = pDoc -> getNodeByPath("/html/body/div") -> firstChild() -> getNodeValue();
    cout << node1 << endl;
    return 0;



}

