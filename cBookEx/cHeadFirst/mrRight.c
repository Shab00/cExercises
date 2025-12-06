#include <stdio.h>
#include <string.h>

void find(int num_ads, char *ads[])
{
    int i;
    puts("Search results:");
    puts("------------------------------");

    for (i = 0; i < num_ads; i++) {
        if(strstr(ads[i], "sports") && ! strstr(ads[i], "bieber")) {
            printf("%s\n", ads[i]);
        }
    }
    puts("------------------------------");
}

int main()
{
    int NUM_ADS = 7;
    char *ADS[] = {
    "William: SBM GSOH likes sports, TV, dining",
    "Matt: SWM NS likes art, movies, theater",
    "Luis: SLM ND likes books, theater, art",
    "Mike: DWM DS likes trucks, sports and bieber",
    "Peter: SAM likes chess, working out and art",
    "Josh: SJM likes sports, movies and theater",
    "Jed: DBM likes theater, books and dining"
    };
    find(NUM_ADS, ADS);
    return 0;
}
