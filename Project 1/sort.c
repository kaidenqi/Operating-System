#include <stdio.h>
#include <string.h>

int main()
{
    char name[10][10];
    char temp[10];
    int num = 0;
    int i,j,k;
    int x;
    
    //printf("Please input a list of state abbreviations:\n");
    while (scanf("%s", name[num]) != EOF){
        num++;
    }
    
    for(i = 0; i < num; i++)
    {
        for (j = i + 1; j < num; j++)
        {
            if(strcmp(name[i],name[j]) > 0)
            {
                strcpy(temp, name[i]);
                strcpy(name[i], name[j]);
                strcpy(name[j], temp);
            }
        }
    }
    //printf("Sorted state abbreviations are:\n");
    for (k = 0; k < num; ++k)
        printf("%s\n", name[k]);
    
    return 0;
}
