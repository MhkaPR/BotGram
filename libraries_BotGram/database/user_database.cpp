#include "user_database.h"
#include <QString>
User_DataBase::User_DataBase()
{

}
xml_node<>* User_DataBase::addUser(const string& nameUser,const string& passwordUser,const string& emailUser)
{

        if(!root)
        {

            root=connectToXml("sample.xml");

            if(!root) exit(1);
        }

    //------------------------------------------
    xml_node<>* newUser=doc.allocate_node(node_element,"user"," ");
    xml_node<>* users=root->first_node("users");
    users->append_node(newUser);
    newUser=NULL;
    newUser=users->last_node("user");

    xml_attribute<>* newAtt=doc.allocate_attribute("id","@");
    newUser->append_attribute(newAtt);
    newAtt=NULL;


    string* previousTokenStr=new string();
    *previousTokenStr=newUser->previous_sibling()->value();
    int previousToken=Toint(*previousTokenStr);
    previousToken++;
    *previousTokenStr=to_string(previousToken);

    newAtt=doc.allocate_attribute("token",previousTokenStr->c_str());
    newUser->prepend_attribute(newAtt);



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
