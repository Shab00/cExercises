/* File: test_comments.c
   Top-level block comment that should be removed.
   It spans multiple lines.
*/

#include <stdio.h> // include line comment

int main(void) {
    // Simple single-line comment
    printf("Hello, world!\n"); /* Trailing block comment */

    printf("This is not a comment: /* inside string */\n");
    printf("This is not a comment: // inside string\n");

    char quote = '\''; // char with escaped single quote
    char slash = '/';  // char slash

    /* Block comment containing code-looking text:
       if (x /* nested? */ > 0) { do_something(); }
       (should not confuse the remover; comments do not nest)
    */

    printf("Escaped quote: \"\\\" // not a comment\"\n");
    printf("Backslash then quote: \"\\\\\" /* comment after string */\n");

    // A line comment that ends at EOF in the next example will be handled
    printf("End of tests\n"); // final line comment
    return 0;
}
// A final single-line comment after the program
/* Block comment at EOF that may not be followed by a newline */
