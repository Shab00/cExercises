# include <stdio.h>
# include <string.h>

int main(int argc, char *argv[])
{
    int numbers[4] = { 'a' };
    char name[4] = { 'a', 'd', 'c', 'd' };
    int *num = (int*) name; // this interpets char array as int
    int value;

    // first, print them out raw
    printf("numbers: %d %d %d %d\n",
            numbers[0], numbers[1], numbers[2], numbers[3]);

    printf("name each: %c %c %c %c\n",
            name[0], name[1], name[2], name[3]);

    printf("name: %s\n", name);

    // set up the numbers
    numbers[0] = 1;
    numbers[1] = 2;
    numbers[2] = 3;
    numbers[3] = 4;
    
    // set up the name
    name[0] = 'Z';
    name[1] = 'e';
    name[2] = 'd';
    name[3] = '\0';

    // memememem 
    printf("%d %d %d\n", name[0], name[1], name [2]);
    printf("%o %o %o\n", name[0], name[1], name[2]);
    
    memcpy(&value, name, sizeof(int));

    printf("interpreted as an integer: %d\n", value);

    printf("%d\n", *num);

    // then print them out initialize
    printf("numbers: %d %d %d %d\n",
            numbers[0], numbers[1], numbers[2], numbers[3]);

    printf("name each: %c %c %c %c\n",
            name[0], name[1], name[2], name[3]);
    
    // print the like a string
    printf("numbers: %s\n", name);

    // another way to use name
    char *another = "Zed";
    
    printf("another: %s\n", another);

    printf("another each: %c %c %c %C\n",
            another[0], another[1], another[2], another[3]);

    return 0;
}
