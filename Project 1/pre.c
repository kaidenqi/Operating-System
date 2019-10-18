#include <stdio.h>

struct PopuState
{
    char name[5];
    int population;
};

int main()
{
    struct PopuState sample[10];
    int i = 0;
    int j;
    
    //printf("Please input state abbreviations and the population:\n");
    while (scanf("%s %d", sample[i].name,&sample[i].population) != EOF){
        //scanf("%d", &sample[i].population);
        i++;
    }
    
    //printf("States whose population are above 10 minllion are:\n");
    for (j = 0; j < i; ++j)
    {
        if(sample[j].population > 10)
            printf("%s\n", sample[j].name);
    }
    
    return 0;
}
