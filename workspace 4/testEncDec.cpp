#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*

To run do ./test

*/


int main(){
    
    string plainText = "Hello lucas, welcome to the world of the living";
    string passphrase = "abcdef"; // only working with a single char as the passphrase for now
    string encrypt;
    string decrypt;
    string temp;
    temp = min(plainText, passphrase);
    int min;
    min = temp.size();
    
    encrypt = "";
    decrypt = "";
    
    
    cout << "Origional message: " << plainText << "\n" << endl;
    
    for (int i = 0; i < plainText.size(); i++){
        encrypt += plainText[i] ^ ((passphrase[i % min]) ); // % 15 doesnt matter much, just needs to be the same as below
        
    }
    cout << "Encrypted message: " << encrypt << endl;
    
    
    
    
    for (int i = 0; i < plainText.size(); i++){
        
        decrypt += encrypt[i] ^ ((passphrase[i % min]) );
        
    }
    
    cout << "Decrypted message: " << decrypt << endl;
    

    return 0;
    
}