#include <stdio.h>
#include <string.h>

// Define the Person struct
struct Person {
    char name[50];  // Use a fixed-size char array instead of a pointer for the name
    int age;
    int height;
    int weight;
};

// Fix the Person_print function to accept the struct by value
void Person_print(struct Person person)
{
    printf("Name: %s\n", person.name);
    printf("\tAge: %d\n", person.age);
    printf("\tHeight: %d\n", person.height);
    printf("\tWeight: %d\n", person.weight);
}

int main(int argc, char *argv[])
{
    // Create two Person structs directly on the stack (no malloc)
    struct Person joe;
    strcpy(joe.name, "Joe Alex");  // Copy the string directly into the fixed-size array
    joe.age = 32;
    joe.height = 64;
    joe.weight = 140;

    struct Person frank;
    strcpy(frank.name, "Frank Blank");  // Same for Frank
    frank.age = 20;
    frank.height = 72;
    frank.weight = 180;

    // Print out the details of each person
    printf("Joe is at memory location %p:\n", (void*)&joe);
    Person_print(joe);

    printf("Frank is at memory location %p:\n", (void*)&frank);
    Person_print(frank);

    return 0;
}
