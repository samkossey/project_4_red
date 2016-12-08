#Team 2 as Red Team

#Applications
#1 in GeeMail.cpp
#EXP37-C. Call functions with the correct number and type of arguments
#functions are called properly. For example, since the sha256 function takes
#a string as its parameter, passphrase is converted to a string before 
#passed to the function

#2 in GeeMail.cpp and sha256.cpp
#CTR50-CPP. Guarantee that container indices and iterators are 
#within the valid range
#All loops start at 0 and have an ending condition for when they go out 
#of bounds. One example is the loop in GeeMail.cpp that encrpyts a message.

#3 in GeeMail.cpp
#STR50-CPP. Guarantee that storage for strings has sufficient space for 
#character data and the null terminator
#Uses cin with strings so there is enough room to store the values. At one
#point they use an integer pinNumber that they want to be only positive
#and they handle the invalid entry.

#4 in GeeMail.cpp and sha256.cpp
#MEM50-CPP. Do not access freed memory
#They didn't really use new or malloc, so this is does not become a problem. 
#If you don't specifically allocate you don't specifically deallocate and
#therefore are not going to access any freed memory.

#5 in GeeMail and sha256.cpp
#MEM34-C. Only free memory allocated dynamically
#They were correct not to free any memory because they were not 
#allocating dynamically

#6 in sha256.cpp
#FLP30-C. Do not use floating-point variables as loop counters
#Loop counters correctly use integers over floating-point variables

#7 in GeeMail.cpp and sha256.cpp
#DCL04-C. Do not declare more than one variable per declaration
#keep declarations to one per line. In sha256.cpp there is one instance
#where multiple unsigned ints are declared on the same line, but the 
#guidelines say this acceptable as long as they are not also initialized
#on the same line

#8
#EXP15-C. Do not place a semicolon on the same line as an if,
#for, or while statement
#They correctly avoided this behavior throughout

#9 in GeeMail.cpp
#ERR04-C. Choose an appropriate termination strategy
#exit is an appropriate termination that does important things like close 
#open files such as the db

#10 in GeeMail.cpp
#FIO46-C. Do not access a closed file
#Since they close the db file at the end of main or when they exit
#from an invalid pinNumber, they leave no possibility of trying to access
#a closed file


#Exceptions
#1 in GeeMail.cpp
#DCL20-C. Explicitly specify void when a function accepts no arguments
#Functions were passing in no arguments and leaving the
#parameter blank. We have updated the functions to have void as a parameter 
#ex/promptUser(void)

#2 in sha256.cpp
#INT34-C. Do not shift an expression by a negative number of bits or by 
#greater than or equal to the number of bits that exist in the operand
#They did not ensure that their bit shifts were smaller than
#the bits in the left operands

#3 in sha256.h
#PRE03-C. Prefer typedefs to defines for encoding non-pointer types
# #define used for potential non-pointer types (although it is possible
#this function does just return a pointer I don't think it does) so typedef
#would probably have been better to use

#4 in sha256.cpp
#EXP19-C. Use braces for the body of an if, for, or while statement
#in the sha256 function, failed to use braces in a for loop

#5 in GeeMail.cpp
#STR05-C. Use pointers to const when referring to string literals
#In functions for database interactions initialized char*'s with string 
#literals for the db queries that were not marked const

#6 in GeeMail.cpp
#FIO51-CPP. Close files when they are no longer needed
#Although exit is an appropriate way to kill the program when the pinNumber
#is invalid, there should be some error checking to make sure exit definitely
#does close the db without any problems


