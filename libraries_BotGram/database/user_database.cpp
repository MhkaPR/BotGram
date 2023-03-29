#include "user_database.h"
#include <QString>
User_DataBase::User_DataBase()
{

}
xml_node<>* User_DataBase::addUser(const string nameUser,const string passwordUser,const string emailUser)
{

    //    if(!root)
    //    {
    //   root=connectToXml("sample.xml");
    //        if(!root) exit(1);
    //    }
    xml_document<> docME;
    xml_node<>* rootME;
    string tempAdd="C:\\Users\\mhkap\\Documents\\botGram\\DataBases\\sample.xml";
    QFile f;
    f.setFileName(tempAdd.c_str());

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return  NULL;
    }
    string DATA_strME=f.readAll().toStdString();
    docME.parse<parse_full>(&DATA_strME[0]);
    rootME=docME.first_node("dataroot");

    //        name=nameFile;
    f.close();
    //        return  root;


    //------------------------------------------
    xml_node<>* newUser=docME.allocate_node(node_element,"user"," ");
    xml_node<>* users=rootME->first_node("users");
    users->append_node(newUser);
    newUser=NULL;
    newUser=users->last_node("user");

    xml_attribute<>* newAtt=docME.allocate_attribute("id","@");
    newUser->append_attribute(newAtt);
    newAtt=NULL;





    xml_node<>* newUserInfo=docME.allocate_node(node_element,"username",nameUser.c_str());
    newUser->append_node(newUserInfo);
    newUserInfo=NULL;

    newUserInfo=docME.allocate_node(node_element,"password",passwordUser.c_str());
    newUser->append_node(newUserInfo);
    newUserInfo=NULL;

    newUserInfo=docME.allocate_node(node_element,"email",emailUser.c_str());
    newUser->append_node(newUserInfo);
    newUserInfo=NULL;

    newUserInfo=docME.allocate_node(node_element,"logined","0");
    newUser->append_node(newUserInfo);

    ofstream FF("C:\\Users\\mhkap\\Documents\\botGram\\DataBases\\sample.xml");
    FF << docME;
    FF.close();
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
