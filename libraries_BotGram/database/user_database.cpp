#include "user_database.h"
#include <QString>
User_DataBase::User_DataBase()
{

}
xml_node<>* User_DataBase::addUser(const string nameUser,const string passwordUser,const string emailUser)
{
    if(!root)
    {
        root=connectToXml("sample.xml");
        if(!root) exit(1);
    }
    xml_node<>* newUser=doc.allocate_node(node_element,"user"," ");
    xml_node<>* users=root->first_node("users");
    users->append_node(newUser);
    newUser=NULL;
    newUser=users->last_node("user");

    xml_node<>* temp=newUser->previous_sibling();
    string tempStr=temp->first_attribute("token")->value();
    sendMessage(tempStr);
    int previosToken=QString::fromStdString(tempStr).toInt();
    previosToken++;
    tempStr=to_string(previosToken);
    const char* ch=tempStr.c_str();
    sendMessage(ch);
    xml_attribute<>* newAttForNewUser=doc.allocate_attribute("token",ch);
    newUser->append_attribute(newAttForNewUser);
    newAttForNewUser=NULL;
    newAttForNewUser=doc.allocate_attribute("id","@");
    newUser->append_attribute(newAttForNewUser);

    xml_node<>* newUserInfo=doc.allocate_node(node_element,"username",nameUser.c_str());
    newUser->append_node(newUserInfo);
    newUserInfo=NULL;
    newUserInfo=doc.allocate_node(node_element,"password",passwordUser.c_str());
    newUser->append_node(newUserInfo);
    newUserInfo=NULL;
    newUserInfo=doc.allocate_node(node_element,"email",emailUser.c_str());
    newUser->append_node(newUserInfo);
    newUserInfo=NULL;
    newUserInfo=doc.allocate_node(node_element,"logined","0");
    newUser->append_node(newUserInfo);

    return newUser;

}

xml_node<> *User_DataBase::search(const string usernameTemp,string type)
{

    if(!root)
    {
        root=connectToXml("sample.xml");
        if(!root) exit(1);
    }
    xml_node<>* users=root->first_node("users");
    for (xml_node<>* cur=users->first_node("user");cur;cur=cur->next_sibling()) {
        if(strcmp(cur->first_node(type.c_str())->value(),usernameTemp.c_str())==0)
        {
            return cur;
        }
    }
    return NULL;

}
