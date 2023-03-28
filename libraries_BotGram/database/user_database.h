#ifndef USER_DATABASE_H
#define USER_DATABASE_H
#include "libraries_BotGram/database/database_complex.h"
class User_DataBase : public dataBase_complex
{
public:
    User_DataBase();
    void addNode(string,const string=" ");
    xml_node<>* search(string);
private:

};

#endif // USER_DATABASE_H
