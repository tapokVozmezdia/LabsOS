#include "chmfn.h"

int symMode(string mode_str, mode_t * f_mode)
{
    mode_t add = 0, rm = 0;
    char type = mode_str[0];
    int shift = 1;

    if (type != 'u' && type != 'g' && type != 'o' && type != 'a')
    {
        type = 'a';
        shift = 0;
    }

    char op = mode_str[shift];
    string prmns = &mode_str[shift + 1];

    mode_t user_bit = 0, group_bit = 0, other_bit = 0;

    switch (type)
    {
        case 'u':
        {
            user_bit = S_IRWXU;
            break;
        }
        case 'g':
        {
            group_bit = S_IRWXG;
            break;
        }
        case 'o':
        {
            other_bit = S_IRWXO;
            break;
        }
        case 'a':
        {
            user_bit = S_IRWXU;
            group_bit = S_IRWXG;
            other_bit = S_IRWXO;
            break;
        }
        default:
        {
            break;
        }
    }

    for (string p = prmns; *p; p++)
    {
        switch (*p)
        {
            case 'r':
            {
                add |= (S_IRUSR * !!(user_bit) | S_IRGRP * 
                !!(group_bit) | S_IROTH * !!(other_bit));
                break;
            }
            case 'w':
            {   
                add |= (S_IWUSR * !!(user_bit) | S_IWGRP * 
                    !!(group_bit) | S_IWOTH * !!(other_bit));
                break;
            }
            case 'x':
            {   
                add |= (S_IXUSR * !!(user_bit) | S_IXGRP * 
                !!(group_bit) | S_IXOTH * !!(other_bit));
                break;
            }
            case '-':
            {   
                rm |= (S_IRWXU * !!(user_bit) | S_IRWXG * 
                !!(group_bit) | S_IRWXO * !!(other_bit));
                break;
            }
            default:
            {
                fprintf(stderr, "permission error: %c\n", *p);
                return -1;
            }
        }
    }

    if (op == '+')
        *f_mode |= add;
    else if (op == '-')
        *f_mode &= ~add;
    else if (op == '=')
    {
        *f_mode &= ~(user_bit | group_bit | other_bit);
        *f_mode |= add;
    }
    else
    {
        fprintf(stderr, "Invalid operation: %c\n", op);
        return -1;
    }

    return 0;
}

int customChmod(string mode_str, string f_name)
{
    struct stat file_stat;

    if (stat(f_name, &file_stat) < 0)
    {
        fprintf(stderr, "stat error\n");
        return 1;
    }

    mode_t n_mode = file_stat.st_mode;

    int ch = atoi(mode_str);

    if (strcmp(mode_str, "000") == 0 || (ch != 0 && strlen(mode_str) == 3))
    {
        mode_t mode = strtol(mode_str, NULL, 8);

        if (chmod(f_name, mode) < 0)
        {
            fprintf(stderr, "chmod error\n");
            return 1;
        }
    }

    else
    {
        if (symMode(mode_str, &n_mode) < 0)
        {
            fprintf(stderr, "symbol mode error\n");
            return 1;
        }

        if (chmod(f_name, n_mode) < 0)
        {
            fprintf(stderr, "chmod error\n");
            return 1;
        }
    }

    return 0;
}