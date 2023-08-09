/* At first check if SCMDP library is connected yet */
#ifndef H_SCMDP
#define H_SCMDP

#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*  Codes of errors */
#define ERR_UNKNOWN_PARAMETER 0
#define ERR_NO_VAL_OPT_PARAMETER 1
#define ERR_NO_VAL_NOPT_PARAMETER 2
#define ERR_WRONG_PARS_NUMBER 3
#define ERR_WRONG_SYNTAX 4

/* Structure of optional argument */
typedef struct
{
    char *longKey; /* Long parameter (ex.: --help) */
    char *shortKey; /* Short parameter (ex.: -h), seems it's also called a key */
    char **valPlace; /* Pointer of variable to write the value of parameter */
    char *valName; /* The name of parameter's value (ex.: -option VALUE) */
    bool isValuable; /* If the val_name is not "" or NULL, then the parameter is a valuable and it is TRUE */
    char *help; /* The string with parameter description */
} ScmdpOptArg;

/* Structure of non-optional (neccessary) argument */
typedef struct
{
    char **valPlace; /* Pointer of variable to write the value of parameter */
    char *key; /* The name of parameter (ex.: ARG1) */
    char *help; /* Description of parameter */
} ScmdpNonOptArg;

/* Structure of argument that calls help */
typedef struct
{
    char *longHelpKey; /* Long parameter (ex.: --help) */
    char *shortHelpKey; /* Short parameter (ex.: -h) */
} ScmdpHelpArg;

/**
 * Structure of workspace. The main object of this library
 * Workspace is a structure contains all initialized arguments, full information
 * about them and their numbers and also help strings and full app description
 * 
 * The mechanism of script is fully based on work with workspace object
*/
typedef struct
{
    ScmdpOptArg *optArgs; /* Dynamic array pointer for optional parameters */ 
    ScmdpNonOptArg *nonOptArgs; /* Dynamic array pointer for non-optional parameters */
    ScmdpHelpArg helpArg; /* Variable to write signature of argument calls help */
    int numOfOptArgs; /* Number of initialized optional parameters */
    int numOfValOptArgs; /* Number of valuable optional parameters */
    int numOfNonOptArgs; /* Number of initialized non-optional parameters */
    char *syntaxHelp; /* Syntax help string (ex.: USAGE app ARG1 ARG2 [-OPT]) */
    char *descriptHelp; /* Description of application purpose */
} ScmdpWorkspace;

/**
 * @brief Initialize a new workspace object
 * 
 * @param syntaxHelp String for syntax help
 * @param description String for global parameter description
 *  
 * @returns New workspace object
 */
ScmdpWorkspace AddWorkspace(char *syntaxHelp, char *description);

/**
 * @brief Initializes a new help argument.
 * 
 * @warning The new help argument initialization will overwrite the previous one.
 * 
 * @param syntaxHelp String for syntax help
 * @param description String for global parameter description
 * 
 * @returns Nothing
 */
void AddHelpArg(ScmdpWorkspace *workspace, char *longKey, char *shortKey);

/**
 * @brief Initializes a new optional argument
 * It is added to the end of the 'workspace->optional' array with memory reallocation
 * 
 * @warning Usage "" instead of NULL in valname is preferred because it can lead to a segmentation error in UNIX-systems
 * 
 * @param workspace Initialized workspace pointer
 * @param longKey String contains long parameter key
 * @param shortKey String contains short parameter key
 * @param help String contains description of parameter
 * @param valName The name of value (ex. -option OPTION), use "" or NULL to make an argument non-valuable
 * @param valPlace The pointer to drop value. If it's non-valuable it drops a "true" string.
 * 
 * @returns Nothing
*/
void AddOptArg(ScmdpWorkspace *workspace, char *longKey, char *shortKey, char *help, char *valName, char **valPlace);

/**
 * @brief Initializes a new non-optional argument
 * It is added to the end of the 'workspace->non-optional' array with memory reallocation
 * 
 * @warning Usage "" instead of NULL in valname is preferred because it can lead to a segmentation error in UNIX-systems
 * 
 * @param workspace Initialized workspace pointer
 * @param key String contains the name of non optional parameter
 * @param help String contains description of parameter
 * @param valPlace The string's pointer to drop value. The value of non-optional argument is neccessary!
 * 
 * @returns Nothing
*/
void AddNonOptArg(ScmdpWorkspace *workspace, char *key, char *help, char **valPlace);

/**
 * @brief Parses the entered arguments and scatters them at the addresses (**place) for the given arguments
 * This function is calculation core of this library
 * 
 * @param workspace Initialized workspace pointer
 * @param argc The same is argc in int main(...)
 * @param argv The same is argv[] in int main(...)
 * 
 * @return TRUE, if the parsing was successful. Otherwise, FALSE.
*/
bool ParseArgs(ScmdpWorkspace *workspace, int argc, char *argv[]);

/* If scmdp is not yet connected it will be compiled */
#endif