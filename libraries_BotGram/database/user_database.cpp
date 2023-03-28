#include "user_database.h"

User_DataBase::User_DataBase()
{

}
void User_DataBase::addNode(string nameNode, string valueNode)
{
    if(!root)
    {
        root=connectToXml("sample.xml");
        if(!root) exit(1);
    }
    xml_node<>* newUser=doc.allocate_node(node_element,nameNode.c_str(),valueNode.c_str());
    xml_node<>* users=root->first_node("users");
    users->append_node(newUser);
}

xml_node<> *User_DataBase::search(const string usernameTemp)
{
    if(!root)
    {
        root=connectToXml("sample.xml");
        if(!root) exit(1);
    }
    xml_node<>* users=root->first_node("users");
    for (xml_node<>* cur=users->first_node("user");cur;cur=cur->next_sibling()) {
        if(strcmp(cur->first_node("username")->value(),usernameTemp.c_str())==0)
        {
            return cur;
        }
    }
    return NULL;

}
