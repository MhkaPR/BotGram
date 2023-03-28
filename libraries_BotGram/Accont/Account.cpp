#include "Account.h"

Account::Account()
{

}

int Account::setUsername(std::string str)
{
    int ErrorType=checkCorrect_Text(str,USERNAME);
    if(!ErrorType)
    {
        username=str;
        return IS_CORRECT;
    }
    username="";
    return ErrorType;
}

std::string Account::getUsername()
{
    return  username;
}

int Account::setPassword(std::string str)
{
    int ErrorType=checkCorrect_Text(str,PASSWORD);
    if(!ErrorType){
        password=str;
        return IS_CORRECT;
    }

    password="";
    return ErrorType;
}

std::string Account::getPassword()
{
    return password;
}

int Account::setEmail(std::string str)
{
    int ErrorType=checkCorrect_Text(str,EMAIL);
    if(!ErrorType)
    {
        email=str;
        return IS_CORRECT;
    }
    email="";
    return ErrorType;
}

std::string Account::getEmail()
{
    return email;
}

bool Account::setLogined(bool valueLogined)
{
    logined=valueLogined;
    return true;
}

bool Account::getLogined()
{
    return logined;
}

int Account::checkCorrect_Text(std::string str, int type_str)
{
    switch (type_str) {
    case USERNAME: return checkUsername(str);
    case PASSWORD: return  checkPassword(str);
    case EMAIL: return  checkEmail(str);
    default: return ERROR_IN_INPUT;
    }
}

int Account::checkUsername(std::string username_temp)
{
    if(username_temp.length()<USERNAME_LENGHT)return  USERNAME_IS_SHORT;
    else if(!IsStandardUsername(username_temp)) return  USERNAME_IS_NOT_STANDARD;
    else return IS_CORRECT;
}

int Account::checkPassword(std::string password_temp)
{
    if(password_temp.length()<PASSWORD_LENGHT)return  PASSWORD_IS_SHORT;
    if(!IsWeakPassword(password_temp)) return PASSWORD_IS_WEAK;
    if(!IsStandardPassword(password_temp)) return  PASSWORD_IS_NOT_STANDARD;
    else return IS_CORRECT;

}

int Account::checkEmail(std::string str)
{
if(!IsEndOfString(str,"@gmail.com"))
   if(!IsEndOfString(str,"@outlook.com"))
       if(!IsEndOfString(str,"@icloud.com"))
           if(!IsEndOfString(str,"@yahoo.com"))
           {
               return EMAIL_IS_NOT_EMAIL;
           }
return IS_CORRECT;

}

bool Account::IsStandardUsername(std::string str)
{
    int lenght=int(str.length());

    for (int i=lenght-1;i>=0;i--) {
        bool sw=false;
        if(str[i]>='A' && str[i] <= 'Z')sw=true;
        else if(str[i]>='a' && str[i] <= 'z')sw=true;
        else if(str[i] >= '0' && str[i] <='9')sw=true;
        else if(str[i]=='_')sw=true;
        if(!sw) return false;
    }
    return true;
}

bool Account::IsStandardPassword(std::string str)
{
    for (int i=0;str[i];i++) {
        switch (str[i]) {
        case '<':
        case '>':
        case '\"':
        case '\'':
        case '\\':
        case '/':return false;
        }
    }
    return true;
}

bool Account::IsWeakPassword(std::string str)
{
    if(IsStandardUsername(str)) return false;
    return  true;
}

bool Account::IsEndOfString(std::string str, std::string strEnd)
{
    int len=int(str.length());
    for (int i=len-1;len-i <= int(strEnd.length());i--) {

    if(str[i]!=strEnd[i])
    {
        return false;
    }
    }
    return  true;
}


