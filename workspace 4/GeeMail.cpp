#include <stdio.h>
#include <iostream>
#include "sha256.h"
#include <string.h>
#include <sqlite3.h>
#include <ctime>
using namespace std;

char choice;
sqlite3* db;
string signedInUser;



void promptUser(){
    start:
    cout << "Type 's' for Sign-In or 'r' for Register New User: " << endl;
    cin >> choice;
    if ((choice != 's') && (choice !='r')){
        goto start;
    }
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void registerUser(){
    regStart:
    string username;
    string password;
    char *zErrMsg = 0;
    int rc;
    
    cout << "Enter a New Username: " << endl;
    getline(cin,username);
    
    char * selectQuery = "SELECT USERNAME FROM USERS;";
    sqlite3_stmt *selectStmt;
    sqlite3_prepare(db, selectQuery, strlen(selectQuery)+1, &selectStmt, NULL);
    while (1) {
        int s;
        //printf("in select while\n");
        s = sqlite3_step(selectStmt);
        if (s == SQLITE_ROW) {
            const unsigned char * text;
            //text = sqlite3_column_text (selectStmt, 0);
            string stext = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt,0)));
            if(username==stext){
                printf("Username already used.\n");
                sqlite3_finalize(selectStmt);
                goto regStart;
                break;
            }
            
        }
        else if (s == SQLITE_DONE) {
            break;
        }
    }
    sqlite3_finalize(selectStmt);
    
    cout << "Enter a Password: " << endl;
    getline(cin,password);
    string hashPass = sha256(password);
    
    char str1[1000];
    //char* sql = "INSERT INTO USERS(USERNAME, PASSWORD) VALUES('";
    char* q = "','";
    char* g = "');";
    strcpy(str1,"INSERT INTO USERS(USERNAME, PASSWORD) VALUES('");
    strcat(str1,username.c_str());
    strcat(str1,q);
    strcat(str1,hashPass.c_str());
    strcat(str1,g);
    rc = sqlite3_exec(db, str1, callback, 0, NULL);
    
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      //fprintf(stdout, "Table created successfully\n");
    } 
    //cout << "register complete" << endl;
    signedInUser = username;
    
}

void signIn() {
    signStart:
    string username;
    string password;
    char *zErrMsg = 0;
    int rc;
    
    
    cout << "Please Enter Username: " << endl;
    getline(cin,username);
    cout << "Please Enter Password: " << endl;
    getline(cin,password);
    string hashPass = sha256(password);
    password = "";
   
    char * selectQuery = "SELECT * FROM USERS;";
    sqlite3_stmt *selectStmt;
    sqlite3_prepare(db, selectQuery, strlen(selectQuery)+1, &selectStmt, NULL);
    while (1) {
        int s;
        s = sqlite3_step(selectStmt);
        if (s == SQLITE_ROW) {
            //const unsigned char * text;
            //text = sqlite3_column_text (selectStmt, 0);
            string d_user = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt,0)));
            string d_pass = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt,1)));
            if(username==d_user && hashPass==d_pass){
                signedInUser = username;
                printf("Sign in successful\n");
                break;
            }
            
        }
        else if (s == SQLITE_DONE) {
            printf("Invalid Username/Password\n");
            sqlite3_finalize(selectStmt);
            goto signStart;
        }
    }
    sqlite3_finalize(selectStmt);
    
    //sign in to database
    
}

void showMessages() {
    
    char *zErrMsg = 0;
    int rc;
    char str1[1000];
    //char * selectQuery = "select count(receiver) from messages where read = 0 and receiver = ";
    char* g = "';";
    sqlite3_stmt *selectStmt;
    
    
    strcpy(str1,"select count(receiver) from messages where receiver = '");
    strcat(str1,signedInUser.c_str());
    strcat(str1,g);

    sqlite3_prepare(db, str1, strlen(str1)+1, &selectStmt, NULL);
    while (1) {
        int s;
        s = sqlite3_step(selectStmt);
        if (s == SQLITE_ROW) {
            //const unsigned char * text;
            //text = sqlite3_column_text (selectStmt, 0);
            int total = sqlite3_column_int(selectStmt, 0);
            cout << "You have " << total << " total message(s). " << endl;
        }
        else if (s == SQLITE_DONE) {
            break;
           
        }
    }
    sqlite3_finalize(selectStmt);
    
    strcpy(str1,"select sender from messages where receiver = '");
    strcat(str1,signedInUser.c_str());
    strcat(str1,g);
    
    cout << "Messages from:" << endl;
    sqlite3_prepare(db, str1, strlen(str1)+1, &selectStmt, NULL);
    while (1) {
        int s;
        s = sqlite3_step(selectStmt);
        if (s == SQLITE_ROW) {
            const unsigned char * sender;
            sender = sqlite3_column_text (selectStmt, 0);
            
            cout << sender << endl;
        }
        else if (s == SQLITE_DONE) {
            break;
           
        }
    }
    sqlite3_finalize(selectStmt);
}


void readMessage() {
    char *zErrMsg = 0;
    int rc;
    char str1[1000];
    char* a = "' and sender = '";
    char* g = "';'";
    sqlite3_stmt *selectStmt;
    string readMessageFrom;
    string pPhrase;
    readStart:
    getline(cin,readMessageFrom);
    cout << "Who's message would you like to read?" << endl;
    getline(cin,readMessageFrom);
    //check to make sure this person has a message to be read.
    strcpy(str1,"select message,passphrase,mlength from messages where receiver = '");
    strcat(str1,signedInUser.c_str());
    strcat(str1,a);
    strcat(str1,readMessageFrom.c_str());
    strcat(str1,g);

    sqlite3_prepare(db, str1, strlen(str1)+1, &selectStmt, NULL);
    while (1) {
        int s;
        s = sqlite3_step(selectStmt);
        if (s == SQLITE_ROW) {
            string message;
            // const unsigned char * passphrase;
            //decrypt here
            message = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 0)));
            string passphrase = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt,1)));
            int mLength;
            mLength = sqlite3_column_int(selectStmt, 2);
            cout << "Enter Pin Number: ";
            getline(cin,pPhrase);
            if(passphrase == sha256(pPhrase)){
                cout << "Message from " << readMessageFrom << ":" << endl;
                
                //////////
                string decrypt;
                decrypt = "";
                int lower;
                lower = min((int)(pPhrase.size()), mLength);
                for (int i = 0; i < mLength; i++){
                    decrypt += message[i] ^ ((pPhrase[i % lower]) );
                }
                //////////
                //message = decrypt;
                cout << decrypt << endl;
            }else{
                cout << "Passphrase does not match" << endl;
                goto readStart;
            }
        }
        else if (s == SQLITE_DONE) {
            break;
        }
    }
    sqlite3_finalize(selectStmt);
    
}

void writeMessage() {
    char *zErrMsg = 0;
    int rc;
    char str1[1000];
    char* c = "','";
    char* g = "');";
    sqlite3_stmt *selectStmt;
    string sendMessageTo;
    string message;
    int pinNumber;
    string passphrase;
    
    getline(cin,sendMessageTo);
    cout << "To: ";
    getline(cin,sendMessageTo);
    cout << "Message: ";
    getline(cin,message);
    cout << "Pin Number [Must be positive integer]: ";
    //getline(cin,passphrase);
    cin >> pinNumber;
   
   if(cin.fail()){
        cout << "Pin invalid." << endl;
        exit(EXIT_FAILURE);
    }
    else if(pinNumber <= 0){
         cout << "Pin invalid." << endl;
        exit(EXIT_FAILURE);
    }
    else{
        passphrase = std::to_string(pinNumber);
    } 
    //for(i = 0; i < passphrase.size(); i++){
    //    if(passphrase[i] !='1')
    //}
    
    string hashPassphrase = sha256(passphrase);

    //encrypt message
    //check to make sure this person is in the system.
    int mLength = message.length();
    char mLen[100];
    sprintf(mLen, "%d", mLength);
    //////////
    string encrypt;
    string temp;
    temp = min(message, passphrase);
    int min;
    min = temp.size();
    encrypt = "";
    
    for (int i = 0; i < message.size(); i++){
        encrypt += message[i] ^ ((passphrase[i % min]) );
    }
    message = "";
    ///////////
    char* a1 = "',";
    char* a2 = ",'";
    strcpy(str1,"insert into messages(sender,receiver,message,mlength,passphrase) values('");
    strcat(str1,signedInUser.c_str());
    strcat(str1,c);
    strcat(str1,sendMessageTo.c_str());
    strcat(str1,c);
    strcat(str1,encrypt.c_str());
    strcat(str1,a1);
    strcat(str1,mLen);
    strcat(str1,a2);
    strcat(str1,hashPassphrase.c_str());
    strcat(str1,g);
    
    rc = sqlite3_exec(db, str1, callback, 0, NULL);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", "Message full for this user.");
      sqlite3_free(zErrMsg);
    }else{
      cout << "Message Sent" << endl;
    } 

    
    
}

int main(){
    
    char *zErrMsg = 0;
    int rc;
    
    rc = sqlite3_open("mail.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    };

    string username;
    promptUser();
    // dummy get line needed.
    getline(cin,username);
    if (choice == 's'){
        signIn();
    }
    else if(choice == 'r'){
        registerUser();
    }
    
    choice = 'a';
    showMessages();
    bool readWrite;
    mark1:
    cout << "Type 'r' to Read a Message, 'w' to Write a Message, or 'q' to Sign Out: " << endl;
    cin >> choice;
    cout << endl;
    if ((choice != 'r') && (choice !='w') && (choice != 'q')){
        goto mark1;
    }
    if (choice == 'r'){
        readMessage();
    }
    else if(choice == 'w'){
        writeMessage();
    }
    else if(choice == 'q'){
        return 1;
    }
    goto mark1;
    
    sqlite3_close(db);
}
