#ifndef DATABASE_COMPLEX_H
#define DATABASE_COMPLEX_H

#include <QWidget>
#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include "rapidxml/rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml/rapidxml_ext.hpp"
#include "rapidxml/rapidxml/rapidxml_utils.hpp"
#include <QFile>
using namespace std;
using namespace rapidxml;

class dataBase_complex
{

public:

    enum type_finds
    {
FIND_ROOT,FIND_BY_NAME,FIND_BY_VALUE
    };

    xml_node<>* pointer=NULL;
    string name;

    string DATA_str;
    xml_node<>* root=NULL;
    xml_document<> doc;
    vector<char> Buffer;
    explicit dataBase_complex();

    void sendMessage(string);
    int Toint(string);

    //connection to dataBase
    //root node must be :dataroot
    bool createDataBase(string);
    xml_node<>* connectToXml(const string&);

    //move on nodes
    string* intoNode();
    string* nextNode();

    //add objescts
    void addNode(xml_node<>*,string,string=" ");
    void addAttribute(string,string=" ");

    //get information
    string get_nameNode();
    string* get_valueAttribute(string);
    string* get_valueNode();
    vector<list<string>>* get_AttributesOfNode();
    //     vector<string> *get_NodesOfNode();

    //set funcs
    bool set_valueNode(string);
    bool set_valueAttribute(string,string);
    xml_node<>* getPointer();

    //search funcs
    xml_node<>* findNode(string,int);




    bool save_modifies();




private:



};
#endif // DATABASE_COMPLEX_H
