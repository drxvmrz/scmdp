#include <scmdp.h>

/**
 * @brief Issues an error if something went wrong during the parsing
 * 
 * @param workspace Initialized workspace pointer
 * @param errorCode The code of occured error (see 'enum errors' in scmdp.h file)
 * 
 * @returns Nothing
*/
void _ShowError(ScmdpWorkspace *workspace, int errorCode);

/**
 * @brief Assemble and show the help for all initialized arguments.
 * The assembly comes from the descriptions entered during initialization of the arguments and the workspace
 * 
 * @warning All error messages and formatting are in this function
 * 
 * @param workspace Initialized workspace pointer
 * @param showFullHelp Checks if full help should be shown or only syntax help
 * 
 * @return Nothing
 */
void _ShowHelp(ScmdpWorkspace *workspace, bool showFullHelp);

/** 
 * @brief Remove the whole workspace with all initialized arguments from memory
 * 
 * @warning By default this function is called automatically when  function parsearg() ends
 * 
 * @param workspace Initialized workspace pointer
 * 
 * @returns Nothing
 */
void _DeleteWorkspace(ScmdpWorkspace *workspace);

ScmdpWorkspace AddWorkspace(char *syntaxHelp, char *description)
{
    ScmdpWorkspace newWorkspace;
    newWorkspace.optArgs = malloc(sizeof(ScmdpOptArg));
    newWorkspace.nonOptArgs = malloc(sizeof(ScmdpNonOptArg));
    newWorkspace.numOfNonOptArgs = 0;
    newWorkspace.numOfOptArgs = 0;
    newWorkspace.numOfValOptArgs = 0;
    newWorkspace.helpArg.longHelpKey = "";
    newWorkspace.helpArg.shortHelpKey = "";
    newWorkspace.syntaxHelp = syntaxHelp;
    newWorkspace.descriptHelp = description;
    return newWorkspace;
}

void AddHelpArg(ScmdpWorkspace *workspace, char *longKey, char *shortKey)
{
    ScmdpHelpArg newHelpArg;
    newHelpArg.longHelpKey = longKey;
    newHelpArg.shortHelpKey = shortKey;
    
    workspace->helpArg = newHelpArg;
}

void AddOptArg(ScmdpWorkspace *workspace, char *longKey, char *shortKey, char *help, char *valName, char **valPlace)
{
    ScmdpOptArg newOptArg;
    newOptArg.longKey = longKey;
    newOptArg.shortKey = shortKey;
    newOptArg.help = help;
    newOptArg.valPlace = valPlace;
    newOptArg.valName = valName;

    if (strcmp(valName, "") != 0 && valName != NULL)
    {
        workspace->numOfValOptArgs += 1;
        newOptArg.isValuable = true;
    }
    else
    {
        newOptArg.isValuable = false;
    }

    workspace->numOfOptArgs += 1;
    workspace->optArgs = realloc(workspace->optArgs, sizeof(ScmdpOptArg) * workspace->numOfOptArgs);
    workspace->optArgs[workspace->numOfOptArgs-1] = newOptArg;
}

void AddNonOptArg(ScmdpWorkspace *workspace, char *key, char *help, char **valPlace)
{
    ScmdpNonOptArg new_nopt_arg;
    new_nopt_arg.key = key;
    new_nopt_arg.help = help;
    new_nopt_arg.valPlace = valPlace;

    workspace->numOfNonOptArgs += 1;
    workspace->nonOptArgs = realloc(workspace->nonOptArgs, sizeof(ScmdpNonOptArg) * workspace->numOfNonOptArgs);
    workspace->nonOptArgs[workspace->numOfNonOptArgs-1] = new_nopt_arg;
}

bool ParseArgs(ScmdpWorkspace *workspace, int argc, char *argv[])
{
    bool helpArgFound = false;
    bool optArgFound = false;
    bool success = true;    

    /**
    *   First you need to check if there is an argument that calls the help.
    *   If it is present, then we call the _ShowHelp() function and exit the program
    */
    for (int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], workspace->helpArg.longHelpKey) == 0 || strcmp(argv[i], workspace->helpArg.shortHelpKey) == 0)
        {
            helpArgFound = true;
            break;
        }
    }

    if (helpArgFound)
    {
        _ShowHelp(workspace, true);
        success = false;
    }
    /* If help was not called, we go to analyze all the entered arguments */
    else if (argc-1 > workspace->numOfNonOptArgs + workspace->numOfOptArgs + workspace->numOfValOptArgs)
    {
        _ShowHelp(workspace, false);
        success = false;
    }
    else if (argc-1 < workspace->numOfNonOptArgs)
    {
        _ShowHelp(workspace, false);
        success = false;
    }
    else
    {   
        /**
        *   Check if the first argument is optional or not 
        * 
        *   You can choose the notation by yourself (for example '@')
        *   By default there are presents UNIX notation '-' and Windows notation '/'
        */
        if (*argv[1] == '-' || *argv[1] == '/')
        {
            /* If the first argument is optional, we go from the end to parse non-optional at first */
            for (int i = argc - 1; i > argc - workspace->numOfNonOptArgs - 1; i--)
            {
                if (*argv[i] == '-' || *argv[i] == '/')
                {
                    _ShowError(workspace, ERR_NO_VAL_NOPT_PARAMETER);
                    success = false;
                    break;
                }
                else
                {
                    *(workspace->nonOptArgs[i - argc + workspace->numOfNonOptArgs].valPlace) = argv[i];
                }
            }
            /* After non-optional arguments if parsing was succesful we come back to begin and parse optional ones */
            if (success)
            {
                for(int i = 1; i < argc - workspace->numOfNonOptArgs; i++)
                {
                    if (*argv[i] == '-' || *argv[i] == '/')
                    {
                        optArgFound = false;
                        for (int j = 0; j < workspace->numOfOptArgs; j++)
                        {
                            if(strcmp(argv[i], workspace->optArgs[j].longKey) == 0 || strcmp(argv[i], workspace->optArgs[j].shortKey) == 0)
                            {
                                optArgFound = true;
                                if(workspace->optArgs[j].isValuable)
                                {
                                    if((i + 1 > argc - workspace->numOfNonOptArgs - 1) || (*argv[i+1] == '-' || *argv[i+1] == '/'))
                                    {
                                        _ShowError(workspace, ERR_NO_VAL_OPT_PARAMETER);
                                        success = false;
                                        break;
                                    }
                                    else
                                    {
                                        *(workspace->optArgs[j].valPlace) = argv[i+1];
                                        ++i;
                                    }
                                }
                                else
                                {
                                    *(workspace->optArgs[j].valPlace) = "true";
                                }
                            }
                        }
                        if(success)
                        {
                            if (!optArgFound)
                            {
                                _ShowError(workspace, ERR_UNKNOWN_PARAMETER);
                                success = false;
                                break;
                            }
                        }
                    }
                    else
                    {
                        _ShowError(workspace, ERR_WRONG_SYNTAX);
                        success = false;
                        break;
                    }
                }
            }
        }
        else
        {
            /* If the first argument is non-optional, we go parse arguments in direct order */
            for (int i = 1; i < workspace->numOfNonOptArgs+1; i++)
            {
                if (*argv[i] == '-' || *argv[i] == '/')
                {
                    _ShowError(workspace, ERR_NO_VAL_NOPT_PARAMETER);
                    success = false;
                    break;
                }
                else
                {
                    *(workspace->nonOptArgs[i-1].valPlace) = argv[i];
                }
            }
            if (success)
            {
                /* Separate loop for parsing optional arguments in direct order */
                for (int i = workspace->numOfNonOptArgs+1; i < argc; i++)
                {
                    optArgFound = false;
                    for (int j = 0; j < workspace->numOfOptArgs; j++)
                    {
                        if(strcmp(argv[i], workspace->optArgs[j].longKey) == 0 || strcmp(argv[i], workspace->optArgs[j].shortKey) == 0)
                        {
                            optArgFound = true;
                            if(workspace->optArgs[j].isValuable)
                            {
                                if((i+1 >= argc) || (*argv[i+1] == '-' || *argv[i+1] == '/'))
                                {
                                    _ShowError(workspace, ERR_NO_VAL_OPT_PARAMETER);
                                    success = false;
                                    break;
                                }
                                else
                                {
                                    *(workspace->optArgs[j].valPlace) = argv[i+1];
                                    ++i;
                                }
                            }
                            else
                            {
                                *(workspace->optArgs[j].valPlace) = "true";
                            }
                        }
                    }
                    if(success)
                    {
                        if (!optArgFound)
                        {
                            _ShowError(workspace, ERR_UNKNOWN_PARAMETER);
                            success = false;
                            break;
                        }
                    }
                }
            }
        }
    }
    /** Now release the memory! 
     * Everything is scattered into variables or error message was issued
     * We don't need workspace anymore! */
    _DeleteWorkspace(workspace);
    return success;
}    

void _ShowError(ScmdpWorkspace *workspace, int errorCode)
{
    char *error;
    switch (errorCode)
    {
    case ERR_NO_VAL_NOPT_PARAMETER:
        error = "Error: Need a value for non-optional parameter!";
        break;
    case ERR_NO_VAL_OPT_PARAMETER:
        error = "Error: Need a value for optional parameter!";
        break;
    case ERR_UNKNOWN_PARAMETER:
        error = "Error: Unknown argument or option!";
        break;
    case ERR_WRONG_PARS_NUMBER:
        error = "Error: Wrong number of parameters!";
        break;
    case ERR_WRONG_SYNTAX:
        error = "Error: Wrong command line syntax!";
        break;
    default:
        break;
    }
    printf("%s\nprint -h or --help to see help\n", error);
}

void _ShowHelp(ScmdpWorkspace *workspace, bool showFullHelp)
{
    char checkSymbol = 0;
    if(!showFullHelp)
    {
        printf("%s\n", workspace->syntaxHelp);
    }
    else
    {
        printf("%s\n\n%s\n", workspace->syntaxHelp, workspace->descriptHelp);
        
        printf("\n>> Non-optional arguments:\n");
        for (int i = 0; i < workspace->numOfNonOptArgs; i++)
        {
            printf("\n%3s%-10s", " ", workspace->nonOptArgs[i].key);
            while (checkSymbol = *workspace->nonOptArgs[i].help++)
            {
                if (checkSymbol == '\n')
                {
                    printf("\n%-13s", " ");
                }
                else
                {
                    printf("%c", checkSymbol);
                }
            }
        }
        printf("\n\n>> Optional agruments:\n");
        for (int i = 0; i < workspace->numOfOptArgs; i++)
        {
            printf("\n%3s%-10s,%-5s%-7s", " ", workspace->optArgs[i].longKey, workspace->optArgs[i].shortKey, workspace->optArgs[i].valName);
            while (checkSymbol = *workspace->optArgs[i].help++)
            {
                if (checkSymbol == '\n')
                {
                    printf("\n%-26s", " ");
                }
                else
                {
                    printf("%c", checkSymbol);
                }
            }
        }
    }
}

void _DeleteWorkspace(ScmdpWorkspace *workspace)
{
    free(workspace->optArgs);
    free(workspace->nonOptArgs);
}