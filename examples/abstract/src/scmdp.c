#include <scmdp.h>

/**
 * @brief Issues an error if something went wrong during the parsing
 * 
 * @param workspace Initialized workspace pointer
 * @param error_code The code of occured error (see 'enum errors' in scmdp.h file)
 * 
 * @returns Nothing
*/
void showerr(scmdp_workspace *workspace, int error_code);

/**
 * @brief Assemble and show the help for all initialized arguments.
 * The assembly comes from the descriptions entered during initialization of the arguments and the workspace
 * 
 * @warning All error messages and formatting are in this function
 * 
 * @param workspace Initialized workspace pointer
 * @param full_help Checks if full help should be shown or only syntax help
 * 
 * @return Nothing
 */
void showhelp(scmdp_workspace *workspace, bool full_help);

/** 
 * @brief Remove the whole workspace with all initialized arguments from memory
 * 
 * @warning By default this function is called automatically when  function parsearg() ends
 * 
 * @param workspace Initialized workspace pointer
 * 
 * @returns Nothing
 */
void delwrkspc(scmdp_workspace *workspace);

scmdp_workspace addwrkspc(char *syntax_help, char *description)
{
    scmdp_workspace new_workspace;
    new_workspace.optional = malloc(sizeof(scmpd_opt_arg));
    new_workspace.non_optional = malloc(sizeof(scmpd_nopt_arg));
    new_workspace.non_opt_pars_count = 0;
    new_workspace.opt_pars_count = 0;
    new_workspace.val_opt_pars = 0;
    new_workspace.help_arg.long_par = "";
    new_workspace.help_arg.short_par = "";
    new_workspace.syntax_help = syntax_help;
    new_workspace.description = description;
    return new_workspace;
}

void addhelparg(scmdp_workspace *workspace, char *long_par, char *short_par)
{
    scmpd_help_arg new_help_arg;
    new_help_arg.long_par = long_par;
    new_help_arg.short_par = short_par;
    
    workspace->help_arg = new_help_arg;
}

void addoptarg(scmdp_workspace *workspace, char *long_par, char *short_par, char *help, char *val_name, char **place)
{
    scmpd_opt_arg new_opt_arg;
    new_opt_arg.long_par = long_par;
    new_opt_arg.short_par = short_par;
    new_opt_arg.help = help;
    new_opt_arg.place = place;
    new_opt_arg.val_name = val_name;

    if (strcmp(val_name, "") != 0 && val_name != NULL)
    {
        workspace->val_opt_pars += 1;
        new_opt_arg.valuable = true;
    }
    else
    {
        new_opt_arg.valuable = false;
    }

    workspace->opt_pars_count += 1;
    workspace->optional = realloc(workspace->optional, sizeof(scmpd_opt_arg) * workspace->opt_pars_count);
    workspace->optional[workspace->opt_pars_count-1] = new_opt_arg;
}

void addnoptarg(scmdp_workspace *workspace, char *par, char *help, char **place)
{
    scmpd_nopt_arg new_nopt_arg;
    new_nopt_arg.par = par;
    new_nopt_arg.help = help;
    new_nopt_arg.place = place;

    workspace->non_opt_pars_count += 1;
    workspace->non_optional = realloc(workspace->non_optional, sizeof(scmpd_nopt_arg) * workspace->non_opt_pars_count);
    workspace->non_optional[workspace->non_opt_pars_count-1] = new_nopt_arg;
}

bool parseargs(scmdp_workspace *workspace, int argc, char *argv[])
{
    bool help_arg_found = false;
    bool opt_arg_found = false;

    /**
    *   First you need to check if there is an argument that calls the help.
    *   If it is present, then we call the showhelp() function and exit the program
    */
    for (int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], workspace->help_arg.long_par) == 0 || strcmp(argv[i], workspace->help_arg.short_par) == 0)
        {
            help_arg_found = true;
            break;
        }
    }
    if (help_arg_found)
    {
        showhelp(workspace, true);
        return false;
    }

    /*
    *   If help was not called, we go to analyze all the entered arguments
    */
    if (argc-1 > workspace->non_opt_pars_count + workspace->opt_pars_count + workspace->val_opt_pars)
    {
        showhelp(workspace, false);
        return false;
    }
    else if (argc-1 < workspace->non_opt_pars_count)
    {
        showhelp(workspace, false);
        return false;
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
            /**
             * If the first argument is optional, we go from the end to parse non-optional at first
            */
            for (int i = argc - 1; i > argc - workspace->non_opt_pars_count - 1; i--)
            {
                if (*argv[i] == '-' || *argv[i] == '/')
                {
                    showerr(workspace, ERR_NO_VAL_NOPT_PARAMETER);
                    return false;
                }
                else
                {
                    *(workspace->non_optional[i - argc + workspace->non_opt_pars_count].place) = argv[i];
                }
            }
            /*
            *   After non-optional arguments we come back to begin and parse optionals
            */
            for(int i = 1; i < argc - workspace->non_opt_pars_count; i++)
            {
                if (*argv[i] == '-' || *argv[i] == '/')
                {
                    opt_arg_found = false;
                    for (int j = 0; j < workspace->opt_pars_count; j++)
                    {
                        if(strcmp(argv[i], workspace->optional[j].long_par) == 0 || strcmp(argv[i], workspace->optional[j].short_par) == 0)
                        {
                            opt_arg_found = true;
                            if(workspace->optional[j].valuable)
                            {
                                if((i + 1 > argc - workspace->non_opt_pars_count - 1) || (*argv[i+1] == '-' || *argv[i+1] == '/'))
                                {
                                    showerr(workspace, ERR_NO_VAL_OPT_PARAMETER);
                                    return false;
                                }
                                else
                                {
                                    *(workspace->optional[j].place) = argv[i+1];
                                    ++i;
                                }
                            }
                            else
                            {
                                *(workspace->optional[j].place) = "true";
                            }
                        }
                    }
                    if (!opt_arg_found)
                    {
                        showerr(workspace, ERR_UNKNOWN_PARAMETER);
                        return false;
                    }
                }
                else
                {
                    showerr(workspace, ERR_WRONG_SYNTAX);
                    return false;
                }
            }
        }
        else
        {
            /**
             * If the first argument is non-optional, we go parse arguments in direct order
            */
            for (int i = 1; i < workspace->non_opt_pars_count+1; i++)
            {
                if (*argv[i] == '-' || *argv[i] == '/')
                {
                    showerr(workspace, ERR_NO_VAL_NOPT_PARAMETER);
                    return false;
                }
                else
                {
                    *(workspace->non_optional[i-1].place) = argv[i];
                }
            }
            /**
             * Separate loop for parsing optional arguments in direct order
            */
            for (int i = workspace->non_opt_pars_count+1; i < argc; i++)
            {
                opt_arg_found = false;
                for (int j = 0; j < workspace->opt_pars_count; j++)
                {
                    if(strcmp(argv[i], workspace->optional[j].long_par) == 0 || strcmp(argv[i], workspace->optional[j].short_par) == 0)
                    {
                        opt_arg_found = true;
                        if(workspace->optional[j].valuable)
                        {
                            if((i+1 >= argc) || (*argv[i+1] == '-' || *argv[i+1] == '/'))
                            {
                                showerr(workspace, ERR_NO_VAL_OPT_PARAMETER);
                                return false;
                            }
                            else
                            {
                                *(workspace->optional[j].place) = argv[i+1];
                                ++i;
                            }
                        }
                        else
                        {
                            *(workspace->optional[j].place) = "true";
                        }
                    }
                }
                if (!opt_arg_found)
                {
                    showerr(workspace, ERR_UNKNOWN_PARAMETER);
                    return false;
                }
            }
        }
    }
    return true;
}    

void showerr(scmdp_workspace *workspace, int error_code)
{
    char *error;
    switch (error_code)
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
    printf("\n%s\nprint -h or --help to see help\n", error);
}

void showhelp(scmdp_workspace *workspace, bool full_help)
{
    char check_symbol = 0;
    if(!full_help)
    {
        printf("%s\n", workspace->syntax_help);
    }
    else
    {
        printf("%s\n\n%s\n", workspace->syntax_help, workspace->description);
        
        printf("\n>> Non-optional arguments:\n");
        for (int i = 0; i < workspace->non_opt_pars_count; i++)
        {
            printf("\n%3s%-10s", " ", workspace->non_optional[i].par);
            while (check_symbol = *workspace->non_optional[i].help++)
            {
                if (check_symbol == '\n')
                {
                    printf("\n%-13s", " ");
                }
                else
                {
                    printf("%c", check_symbol);
                }
            }
        }
        printf("\n\n>> Optional agruments:\n");
        for (int i = 0; i < workspace->opt_pars_count; i++)
        {
            printf("\n%3s%-10s,%-5s%-7s", " ", workspace->optional[i].long_par, workspace->optional[i].short_par, workspace->optional[i].val_name);
            while (check_symbol = *workspace->optional[i].help++)
            {
                if (check_symbol == '\n')
                {
                    printf("\n%-26s", " ");
                }
                else
                {
                    printf("%c", check_symbol);
                }
            }
        }
    }
}