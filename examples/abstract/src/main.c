#include <scmdp.h>

int main(int argc, char* argv[])
{
    /* Variables to drop argument values */
    char *op1, *op2, *arg1, *arg2 = "";

    /* Initialize workspace */
    ScmdpWorkspace workspace = AddWorkspace("USAGE: app [-OPTIONS] arg1 arg2\nOR: app arg1 arg2 [-OPTIONS]",
                                          "App for testing 'scmdp' library.\nIt shows functionality for group\nof some abstract options");

    /* We initialize the arguments that may be needed for the program to work */
    AddHelpArg(&workspace, "--help", "-h");

    /* Optional arguments (not neccessary) */
    AddOptArg(&workspace, "--option1", "-op1", "Optional argument 1\nwith value", "VALUE", &op1);
    AddOptArg(&workspace, "--option2", "-op2", "Optional argument 2\nwithout value", "", &op2);

    /* Non-optional arguments (neccessary) */
    AddNonOptArg(&workspace, "arg1", "Neccessary argument 1\nOther description", &arg1);
    AddNonOptArg(&workspace, "arg2", "Neccessary argument 2", &arg2);

    /* If the parsing fails, then clear the memory and exit the program */
    if (!ParseArgs(&workspace, argc, argv)) return 0;

    /* Body of program */
    printf(">>CMD_VALS:\n-op1: %s\n-op2: %s\narg1: %s\narg2: %s", op1, op2, arg1, arg2);

    return 0;
}