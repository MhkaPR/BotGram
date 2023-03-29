#ifndef USER_DATABASE_H
#define USER_DATABASE_H
#include "libraries_BotGram/database/database_complex.h"
class User_DataBase : public dataBase_complex
{
public:
    enum typeSearchUser
    {
        USERNAME,
        PASSWORD,
        EMAIL,
    };
    User_DataBase();
    xml_node<>* addUser(const string,const string,const string);
    xml_node<>* search(string,string);
private:

};

#endif // USER_DATABASE_H
