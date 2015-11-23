#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)

{
int coins = 0;
int money = 0;

    printf("How much money do you need change for?\n ");
    float x = GetFloat();
    
    
    while (x<=0)
    {
        printf("Please provide a positive sum of money.\n ");
        x = GetFloat();
        
    }
    money = (int)round(x*100);
    //check50 2014/x/pset1/hello hello.cprintf("%d money in pennies.\n",money);

        while (money > 25)
        {
            coins = coins + money/25;
            money = money - money/25*25;
        }   
        while (money >= 10)
        {
            coins = coins + money/10;
            money = money - money/10*10;
        }
        while (money >= 5)
        {
            coins = coins + money/5;
            money = money - money/5*5;
        }
        while (money >= 1)
        {
            coins = coins + money/1;
            money = money - money/1*1;
        }
        printf("%d\n",coins);
  
}
