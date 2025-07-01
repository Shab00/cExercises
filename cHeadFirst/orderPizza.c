#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    char *delivery = "";
    int thick = 0;
    int count = 0;
    char ch;

    while((ch = getopt(argc, argv, "dt:")) ! = EOF)
        switch (ch) {
            case 'd':
                delivery = optarg;
                break;
            case 't':
                thick = ;
                break;
            default:
                fprintf(stderr, "Unknown option: '%s'\n", optarg);
                return 1;
        }
        
        argc -= optind;
        argv += optind;

        if (thick)
            puts("Think Crust.");
        if (delivery[0])
            printf("To be delivered %s.\n", delivery);

        puts("Ingreients:");

        for (count = 0; count < argc; count++)
            puts(argv[count]);
        return 0;
}
