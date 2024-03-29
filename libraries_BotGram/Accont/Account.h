#ifndef ACCONTVALIDITY_H
#define ACCONTVALIDITY_H
#include <string>
#include<iostream>
using std::string;

class Account
{
public:

    Account();
    enum information_
    {
        USERNAME,
        PASSWORD,
        EMAIL,
        USERNAME_LENGHT=8,
        PASSWORD_LENGHT=10
    };
    enum ERROR_ISCORRECT_FUNC{
        IS_CORRECT,
        USERNAME_IS_SHORT,
        USERNAME_IS_REPETITIVE,
        USERNAME_IS_NOT_STANDARD,
        USERNAME_NOT_FOUND,
        PASSWORD_IS_SHORT,
        PASSWORD_IS_WEAK,
        PASSWORD_IS_NOT_STANDARD,
        EMAIL_IS_NOT_EMAIL,
        EMAIL_IS_REPETITIVE,
        EMAIL_HAVE_BAD_CHARS,
        ERROR_IN_INPUT,

    };

    int setUsername(string);
    string getUsername();

    int setPassword(string);
    string getPassword();

    int setEmail(string);
    string getEmail();

    bool setLogined(bool);
    bool getLogined();

    string ErrorStr(int);

    int checkCorrect_Text(string,int);

private:
    string username;
    string password;
    string  email;
    bool logined;




    int checkUsername(string);
    int checkPassword(string);
    int checkEmail(string);

    bool IsStandardUsername(string);
    bool IsStandardPassword(string);
    bool IsWeakPassword(string);

    bool IsEndOfString(string,string);

};

#endif // ACCONTVALIDITY_H
