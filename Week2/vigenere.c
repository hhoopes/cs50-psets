#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])

{
   // check that the user only uses 1 command line argument for the key
   if ((argc != 2))
        {
        printf("usage: ./caeser <keyword>\n");
        return 1;
        } 
    
    // convert the key from a string of chars to array of integers
    
    string keyalpha = argv[1];
    int keylength=strlen(keyalpha);
    int keynum[strlen(keyalpha)];
    for (int i=0; i < keylength; i++)  
        {
        if isalpha(keyalpha[i])
            {
            if isupper(keyalpha[i])
                keynum[i] = keyalpha[i]-65;
            if islower(keyalpha[i])
                keynum[i] = keyalpha[i]-97;
            }
        else 
            {
            
            printf("usage: ./caeser <keyword>\n");
            return 1;
            }
        //printf("%i\n", keynum[i]);
        }
       
    
    // ask for the plaintext
    string plain=NULL;
    do plain = GetString();
    while (plain==NULL);
    
    
    // convert plaintext from ASCII to alpha, shift by k, convert back to ASCII, leave nonalpha characters alone
    int keyposition = 0;
    for (int i = 0, n = strlen(plain); i < n; i++)
    
    {
        if (isupper(plain[i]))
        {
        printf("%c", (((plain[i]-65) + keynum[keyposition])%26)+65);
        keyposition = (keyposition + 1)%keylength;
        }
        
        else if (islower(plain[i]))
        {
        printf("%c", (((plain[i]-97) + keynum[keyposition])%26)+97);
        keyposition = (keyposition + 1)%keylength;
        }
        
        else 
        {
         printf("%c",plain[i]);
        }
   }
    printf("\n");
    
}
