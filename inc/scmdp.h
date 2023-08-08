/* At first check if SCMDP library is connected yet */
#ifndef H_SCMDP
#define H_SCMDP

#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

/*  Codes of errors */
enum errors
{
    ERR_UNKNOWN_PARAMETER = 1,
    ERR_NO_VAL_OPT_PARAMETER,
    ERR_NO_VAL_NOPT_PARAMETER,
    ERR_WRONG_PARS_NUMBER,
    ERR_WRONG_SYNTAX
};

/* Structure of optional argument */
typedef struct
{
    char *long_par; /* Long parameter (ex.: --help) */
    char *short_par; /* Short parameter (ex.: -h), seems it's also called a key */
    char **place; /* Pointer of variable to write the value of parameter */
    char *val_name; /* The name of parameter's value (ex.: -option VALUE) */
    bool valuable; /* If the val_name is not "" or NULL, then the parameter is a valuable and it is TRUE */
    char *help; /* The string with parameter description */
} scmpd_opt_arg;

/* Structure of non-optional (neccessary) argument */
typedef struct
{
    char **place; /* Pointer of variable to write the value of parameter */
    char *par; /* The name of parameter (ex.: ARG1) */
    char *help; /* Description of parameter */
} scmpd_nopt_arg;

/* Structure of argument that calls help */
typedef struct
{
    char *long_par; /* Long parameter (ex.: --help) */
    char *short_par; /* Short parameter (ex.: -h) */
} scmpd_help_arg;

/**
 * Structure of workspace. The main object of this library
 * Workspace is a structure contains all initialized arguments, full information
 * about them and their numbers and also help strings and full app description
 * 
 * The mechanism of script is fully based on work with workspace object
*/
typedef struct
{
    scmpd_opt_arg *optional; /* Dynamic array pointer for optional parameters */ 
    scmpd_nopt_arg *non_optional; /* Dynamic array pointer for non-optional parameters */
    scmpd_help_arg help_arg; /* Variable to write signature of help_arg */
    int opt_pars_count; /* Number of initialized optional parameters */
    int val_opt_pars; /* Number of valuable optional parameters */
    int non_opt_pars_count; /* Number of initialized non-optional parameters */
    char *syntax_help; /* Syntax help string (ex.: USAGE app ARG1 ARG2 [-OPT]) */
    char *description; /* Description of application purpose */
} scmdp_workspace;

/**
 * @brief Initialize a new workspace object
 * 
 * @param syntax_help String for syntax help
 * @param description String for global parameter description
 *  
 * @returns New workspace object
 */
scmdp_workspace addwrkspc(char *syntax_help, char *description);

/**
 * @brief Initializes a new help argument.
 * 
 * @warning The new help argument initialization will overwrite the previous one.
 * 
 * @param syntax_help String for syntax help
 * @param description String for global parameter description
 * 
 * @returns Nothing
 */
void addhelparg(scmdp_workspace *workspace, char *long_par, char *short_par);

/**
 * @brief Initializes a new optional argument
 * It is added to the end of the 'workspace->optional' array with memory reallocation
 * 
 * @warning Usage "" instead of NULL in valname is preferred because it can lead to a segmentation error in UNIX-systems
 * 
 * @param workspace Initialized workspace pointer
 * @param long_par String contains long parameter key
 * @param short_par String contains short parameter key
 * @param help String contains description of parameter
 * @param valname The name of value (ex. -option OPTION), use "" or NULL to make an argument non-valuable
 * @param place The pointer to drop value. If it's non-valuable it drops a "true" string.
 * 
 * @returns Nothing
*/
void addoptarg(scmdp_workspace *workspace, char *long_par, char *short_par, char *help, char *valname, char **place);

/**
 * @brief Initializes a new non-optional argument
 * It is added to the end of the 'workspace->non-optional' array with memory reallocation
 * 
 * @warning Usage "" instead of NULL in valname is preferred because it can lead to a segmentation error in UNIX-systems
 * 
 * @param workspace Initialized workspace pointer
 * @param par String contains the name of parameter
 * @param help String contains description of parameter
 * @param place The string's pointer to drop value. The value of non-optional argument is neccessary!
 * 
 * @returns Nothing
*/
void addnoptarg(scmdp_workspace *workspace, char *par, char *help, char **place);

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
bool parseargs(scmdp_workspace *workspace, int argc, char *argv[]);

/* If scmdp is not yet connected it will be compiled */
#endif