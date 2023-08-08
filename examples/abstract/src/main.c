#include <scmdp.h>

char *op1, *op2, *arg1, *arg2 = "";

int main(int argc, char* argv[])
{
    // Инициализируем рабочее пространство
    scmdp_workspace workspace = addwrkspc("USAGE: app [-OPTIONS] arg1 arg2\nOR: app arg1 arg2 [-OPTIONS]",
                                          "App for testing 'scmdp' library.\nIt shows functionality for group\nof some abstract options");

    // Инициализируем аргументы, которые могут нам понадобиться при выполнении программы
    addhelparg(&workspace, "--help", "-h");
    addoptarg(&workspace, "--option1", "-op1", "Optional argument 1\nwith value", "VALUE", &op1);
    addoptarg(&workspace, "--option2", "-op2", "Optional argument 2\nwithout value", "", &op2);
    addnoptarg(&workspace, "arg1", "Neccessary argument 1\nOther description", &arg1);
    addnoptarg(&workspace, "arg2", "Neccessary argument 2", &arg2);

    // Если парсинг завершился ошибкой, то очищаем память и выходим из программы
    if (!parseargs(&workspace, argc, argv)) return 0;

    /* Тело программы */
    printf(">>CMD_VALS:\n-op1: %s\n-op2: %s\narg1: %s\narg2: %s", op1, op2, arg1, arg2);

    return 0;
}