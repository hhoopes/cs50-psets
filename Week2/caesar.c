#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])

{
   // check that the user only uses 1 command line argument (the key, k)
   while (argc != 2)
        {
        printf("usage: ./caeser <key>\n");
        return 1;
        } 
    
    // convert the key from a string to an integer
    
    int k = atoi(argv[1]);
    
    // ask for the plaintext
    string plain=NULL;
    do (plain = GetString());
    while (plain==NULL);
 
    // convert plaintext from ASCII to alpha, shift by k, convert back to ASCII, leave nonalpha characters alone
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        if (isupper(plain[i]))
        {
        printf("%c", (((plain[i]-65) + k)%26)+65);
        }
        
        else if (islower(plain[i]))
        {
        printf("%c", (((plain[i]-97) + k)%26)+97);
        }
        
        else 
        {
         printf("%c",plain[i]);
        }
   }
    printf("\n");
    
}
