#include "database_complex.h"
#include <QMessageBox>
dataBase_complex::dataBase_complex()
{

    file<> f("C:\\Users\\mhkap\\Documents\\BotGram\\DataBases\\sample.xml");
    DATA_str=f.data();
    doc.parse<parse_no_data_nodes>(&DATA_str[0]);
    root=doc.first_node("dataroot");

}

bool dataBase_complex::createDataBase(string nameDataBase)
{
    try {
        ofstream newfile(nameDataBase,ios_base::in);
        name=nameDataBase;
        newfile.close();
        return  true;
    } catch (int) {
        return  false;
    }

}

//root node must be :dataroot
xml_node<>* dataBase_complex::connectToXml(string nameFile)
{
//returns Root
    file<> f("C:\\Users\\mhkap\\Documents\\BotGram\\DataBases\\sample.xml");
    DATA_str=f.data();
    doc.parse<parse_no_data_nodes>(&DATA_str[0]);
    root=doc.first_node("dataroot");
    return  root;

}

string* dataBase_complex::intoNode()
{
    xml_node<>* tempNode=pointer;

    pointer=pointer->first_node();
    if(!pointer)
    {
        pointer=tempNode;
        return  NULL;
    }

    string *temp = new string();
    *temp = pointer->name();
    return  temp;
}

string *dataBase_complex::nextNode()
{
    xml_node<>* tempNode=pointer;
    pointer=pointer->next_sibling();
    if(!pointer)
    {
        pointer=tempNode;
        return  NULL;
    }
    string *temp = new string();
    *temp = pointer->name();
    return  temp;
}

void dataBase_complex::addNode(string nameNode, string valueNode)
{
    xml_node<>* temp=doc.allocate_node(node_element,nameNode.c_str(),valueNode.c_str());
    pointer->append_node(temp);
}

void dataBase_complex::addAttribute(string nameAttr, string valueAttr)
{
    xml_attribute<>* Temp=doc.allocate_attribute(nameAttr.c_str(),valueAttr.c_str());
    pointer->append_attribute(Temp);
}

string dataBase_complex::get_nameNode()
{
    return  pointer->name();
}

string* dataBase_complex::get_valueAttribute(string nameAttr)
{
    for (xml_attribute<>* att=pointer->first_attribute();att;att=att->next_attribute()) {
        if(att->name()==nameAttr)
        {
            string *Temp =new string();
            *Temp=(att->value());
            return  Temp;
        }

    }
    return  NULL;
}

string *dataBase_complex::get_valueNode()
{
    if(pointer->first_node()) return  NULL;
    else
    {
        string *temp=new string();
        *temp=pointer->value();
        return temp;
    }
}

vector<list<string>>* dataBase_complex::get_AttributesOfNode()
{

    vector<list<string>> *temp=new vector<list<string>>();

    for (xml_attribute<>* att=pointer->first_attribute();att;att=att->next_attribute()) {
        list<string> l;
        string nameAttr=att->name();
        string valueAttr=att->value();
        l.push_back(nameAttr);
        l.push_back(valueAttr);
        temp->push_back(l);
    }
    return  temp;
}

bool dataBase_complex::set_valueNode(string valueNode)
{
    if(!pointer->first_node()){
        pointer->value(valueNode.c_str());
        return true;
    }
    else return  false;
}

bool dataBase_complex::set_valueAttribute(string nameAttr, string valueAttr)
{
    for (xml_attribute<>* att=pointer->first_attribute();att;att=att->next_attribute()) {
        if(att->name()==nameAttr)
        {
            att->value(valueAttr.c_str());
            return true;
        }
    }
    return false;
}

xml_node<> *dataBase_complex::getPointer()
{
    return pointer;
}

xml_node<>* dataBase_complex::findNode(string str, int typeFind)
{
    switch (typeFind) {
    case FIND_ROOT:
        pointer=root;
        break;
    case FIND_BY_NAME:

        for (xml_node<>* tempNode=pointer->first_node();tempNode;tempNode=tempNode->next_sibling()) {
            string SS=tempNode->name();
            if(tempNode->name()==SS)
            {

                pointer=tempNode;
                break;
            }

        }
        return NULL;
    case FIND_BY_VALUE:

        if(pointer->first_node()) return  NULL;
        for (xml_node<>* tempNode=pointer->first_node();tempNode;tempNode=tempNode->next_sibling()) {
            if(str==tempNode->value())
            {
                pointer=tempNode;
                break;
            }
        }
        break;
    default: return  NULL;
    }
    return pointer;
}

//vector<string> *dataBase_complex::get_NodesOfNode()
//{
//    vector<string> *temp=new vector<string>();
//    for (xml_node<>* tempNode=pointer->first_node();tempNode;tempNode=tempNode->next_sibling()) {

//        temp->push_back(tempNode->name());
//    }
//    if(!temp->size())return  NULL;
//    return temp;
//}

bool dataBase_complex::save_modifies()
{
    ofstream f("C:\\Users\\mhkap\\Documents\\BotGram\\sample.xml");
    f << "<?xml version=\"1.0\" encoding=\"utf-8\"?>"<<endl;
    f << doc;
    f.close();
    return true;


}

