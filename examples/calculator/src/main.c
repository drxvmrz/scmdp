#include <scmdp.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    /* Variables to drop argument values */
    char *num1, *num2, *act, *verbose = "";

    /* Initialize workspace */
    ScmdpWorkspace workspace = AddWorkspace("USAGE: CALC NUM1 ACT NUM2  [-OPTIONS]\nOR: CALC [-OPTIONS] NUM1 ACT NUM2",
    "Calculate something with two numbers");

    /* We initialize the arguments that may be needed for the program to work */
    AddHelpArg(&workspace, "--help", "-h");
    AddNonOptArg(&workspace, "NUM1", "First Integer number", &num1);
    AddNonOptArg(&workspace, "ACT", "Type the ACTION\nAVAILABLE: [+], [-], [/], [*]", &act);
    AddNonOptArg(&workspace, "NUM2", "Second integer number", &num2);
    AddOptArg(&workspace, "--verbose", "-v", "Duplicates the operation with words", "", &verbose);

    /* If the parsing fails, then clear the memory and exit the program */
    if(!ParseArgs(&workspace, argc, argv)) return 0;

    long number1, number2 = 0;
    bool vrbs = false;

    /* Converting received numbers from strings to int */
    number1 = strtol(num1, NULL, 10);
    number2 = strtol(num2, NULL, 10);

    /* Checking if key -v was in command-line */
    if(strcmp(verbose, "true") == 0)
    {
        vrbs = true;
    }

    /* Carrying out action with numbers, according to the input on the command-line */
    if (strcmp(act, "[+]") == 0)
    {
        if(vrbs)
        {
            printf("The summ of %ld and %ld is %ld", number1, number2, number1+number2);
        }
        else
        {
            printf("Result: %ld", number1+number2);
        }
    }
    else if (strcmp(act, "[-]") == 0)
    {
        if(vrbs)
        {
            printf("Residual of %ld and %ld is %ld", number1, number2, number1-number2);
        }
        else
        {
            printf("Result: %ld", number1-number2);
        }
    }
    else if (strcmp(act, "[/]") == 0)
    {
        if(number2 == 0)
        {
            printf("DIVIDING BY ZERO!");
            return 0;
        }
        if(vrbs)
        {
            printf("Dividing %ld by %ld is %ld", number1, number2, number1/number2);
        }
        else
        {
            printf("Result: %ld", number1/number2);
        }
    }
    else if(strcmp(act, "[*]") == 0)
    {
        if(vrbs)
        {
            printf("Multiplication %ld by %ld is %ld", number1, number2, number1*number2);
        }
        else
        {
            printf("Result: %ld", number1*number2);
        }
    }
    else
    {
        printf("UNKNOWN ACTION!");
    }
    return 0;
}