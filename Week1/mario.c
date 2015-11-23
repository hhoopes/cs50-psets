#include <cs50.h>
#include <stdio.h>

int main(void) {
    int height=0;

    do {
        printf("Height:");
        height = GetInt();
    }
    while ((height<0) || (height > 23));

    int i;
    for (i=0; i<height; i++) {
        
        //spaces
        int spaces; 
        for (spaces=0; spaces < height-i-1; spaces++) {
            printf(" ");
        }
    
        //hashes
        int hashes;
        for (hashes = 0; hashes < i+2; hashes++) {
            printf("#");
        }
    
        //new lines
        printf("\n");              

    }
}

