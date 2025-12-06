#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_scores(const void* score_a, const void* score_b)
{
    int a = * (int*)score_a;
    int b = * (int*)score_b;
    return a - b;
}

int compare_scores_desc(const void* score_a, const void* score_b)
{
    int a = * (int*)score_a;
    int b = * (int*)score_b;
    return b - a;
}

typedef struct {
    int width;
    int height;
} rectangle;

int compare_area(const void* a, const void* b)
{
    rectangle* ra = (rectangle*)a;
    rectangle* rb = (rectangle*)b;
    int area_a = (ra->height * ra->width);
    int area_b = (rb->height * rb->width);
    return area_a - area_b;
}

int compare_names(const void* a, const void* b)
{
    char ** sa = (char**)a;
    char ** sb = (char**)b;
    return strcmp(*sa, *sb);
}

int compare_area_desc(const void* a, const void*  b)
{
    return compare_area(b,a);
}

int compare_names_desc(const void* a, const void* b)
{
    return compare_names(b,a);
}

int main()
{
    int scores[] = {43,323,32,554,11,3,112};
    int i;
    qsort(scores, 7, sizeof(int), compare_scores);
    puts("These are the scores in order:");
    for (i = 0; i < 7; i++) {
        printf("Scores = %i\n", scores[i]);
    }
    
    char *names[] = {"Karen", "Mark", "Bert", "Molly"};
    qsort(names, 4, sizeof(char*), compare_names);
    puts("These are the names in order:");
    for (i = 0; i < 4; i++) {
        printf("%s\n", names[i]);
    }

    rectangle rects[] = {{2,3}, {5,2}, {1,10}, {4,4}};
    int num_rects = sizeof(rects) / sizeof(rectangle);
    qsort(rects, num_rects, sizeof(rectangle), compare_area);
    puts("Rectangles by area (smallest first):");
    for (i = 0; i < num_rects; i++) {
        printf("Rectangle %d: %dx%d, Area=%d\n", i, rects[i].width, rects[i].height, rects[i].width * rects[i].height);
    }

    return 0;
}
