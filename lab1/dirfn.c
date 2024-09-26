#include "chsort.h"
#include "dirfn.h"

int __n_digits(lint num)
{
    int ans = (num != 0 ? floor(log10(abs(num))) + 1 : 1);
    return ans;
}

// needs to be freed after!
char * __make_fp(const char * path, const char * entry_name)
{
    char * fp_entry;

    if (strlen(path) == 1 && path[0] == '.')
    {
        // printf("WORKED\n");
        fp_entry = (char*)malloc(strlen(entry_name) + sizeof(char));
        strcpy(fp_entry, entry_name);
    }
    else 
    {
        // printf("SHIFT: %d\n", last_se);

        // fp_entry = (char*)malloc(strlen(path) * sizeof(char) + 
        //     strlen(entry_name) * sizeof(char) + sizeof(char));
        // memcpy(fp_entry, path, strlen(path) * sizeof(char));
        // fp_entry[strlen(path)] = '/';
        // memcpy(&fp_entry[strlen(path) + 1], entry_name, strlen(entry_name) * sizeof(char));
    
        fp_entry = (char*)malloc(strlen(path) + strlen(entry_name) + 2);
        strcpy(fp_entry, path);
        fp_entry[strlen(path)] = '/';
        strcpy(&fp_entry[strlen(path) + 1], entry_name);

    }

    return fp_entry;
}

void __calc_lengths(const char * path, char ** entries,
    int num_of_entries, struct prm_lens * lns, 
    blkcnt_t * total_blocks)
{

    lns->grp_len = 0;
    lns->lks_len = 0;
    lns->own_len = 0;
    lns->s_len = 0;

    *total_blocks = 0;

    int i = 0;
    for (; i < num_of_entries; ++i)
    {

        struct stat file_stat;

        char * fp_entry = __make_fp(path, entries[i]);

        if (lstat(fp_entry, &file_stat) == -1)
        {
            fprintf(stderr, "critical error in lstat\n");
            exit(EXIT_FAILURE);
        }

        *total_blocks += file_stat.st_blocks;

        struct passwd *pwd = getpwuid(file_stat.st_uid);
        struct group *grp = getgrgid(file_stat.st_gid);

        int l_len = __n_digits((long)file_stat.st_nlink);
        int st_len = __n_digits((long)file_stat.st_size);
        int o_len = pwd ? strlen(pwd->pw_name) : __n_digits((long)file_stat.st_uid);
        int g_len = grp ? strlen(grp->gr_name) : __n_digits((long)file_stat.st_gid);

        if (l_len > lns->lks_len)
            lns->lks_len = l_len;
        if (o_len > lns->own_len)
            lns->own_len = o_len;
        if (g_len > lns->grp_len)
            lns->grp_len = g_len;
        if (st_len > lns->s_len)
            lns->s_len = st_len;

        free(fp_entry);

    }

}

// does not skip a line (no \n)
void print_entry(const char * path, const char * entry_name, bool long_form,
    struct prm_lens * lns)
{
      
    struct stat file_stat;

    // printf("%s\n", entry_name);
    // printf("%d\n", strlen(entry_name));  
    // int last_se = (path[strlen(path) - 1] == '/' ? 1 : 0); 
    // char * n_path = (char*)malloc(strlen(path) * sizeof(char) - last_se * sizeof(char));
    // memcpy(n_path, path, strlen(path) * sizeof(char) - last_se * sizeof(char));
    
    char * fp_entry = __make_fp(path, entry_name);

    // free(n_path1);

    // printf("%d\n", strlen(fp_entry));
    // printf("%s\n", fp_entry);
    
    // printf("BEFORE ERROR\n");

    // char lstat_path[strlen(fp_entry) + 1];
    // strcpy(lstat_path, fp_entry);

    if (lstat(fp_entry, &file_stat) == -1)
    {
        fprintf(stderr, "critical error in lstat\n");
        exit(EXIT_FAILURE);
    }

    // printf("AFTER ERROR\n");

    char color[strlen(COL_BLUE) + 1];
    strcpy(color, COL_RESET);
    
    bool isdir = false;
    bool bad_link = false;
    bool bold_flag = false;

    if (S_ISDIR(file_stat.st_mode)) 
    {    
        isdir = true;
        bold_flag = true;
        strcpy(color, COL_BLUE);
    }
    
    else if (S_ISLNK(file_stat.st_mode))
    {
        bold_flag = true;
        char link_target[1024];
        ssize_t len = readlink(fp_entry, link_target, sizeof(link_target) - 1);
        if (len != -1)
        {
            link_target[len] = '\0';

            struct stat target_stat;
            if (stat(fp_entry, &target_stat) == -1)
            {
                strcpy(color, COL_RED);
                bad_link = true;
            }
            else
            {
                strcpy(color, COL_CYAN);
            }
        }
    }     

    else if (file_stat.st_mode & S_IXUSR)
    {
        bold_flag = true;
        strcpy(color, COL_GREEN);
    }

    if (!long_form)
    {
        if (bold_flag)
            printf("%s", BOLD_ON);
        printf("%s%s%s", color, entry_name, COL_RESET);
        if (bold_flag)
            printf("%s", BOLD_OFF);
        return;
    }

    char prmns[11];
    const char d_prmns[] = "rwxrwxrwx";

    // prmns[0] = S_ISDIR(file_stat.st_mode) ? 'd' : '-';
    prmns[0] = S_ISDIR(file_stat.st_mode) ? 'd' : 
        (S_ISLNK(file_stat.st_mode) ? 'l' : '-');

    int i = 0;
    for (; i < 9; ++i)
        prmns[1 + i] = (file_stat.st_mode & (1 << (8-i))) ? d_prmns[i] : '-';   

    prmns[10] = '\0';    

    printf("%s %*ld ", prmns, lns->lks_len, (long)file_stat.st_nlink);    

    struct passwd *pwd = getpwuid(file_stat.st_uid);
    struct group *grp = getgrgid(file_stat.st_gid);
    
    if (pwd && grp)
    {
        printf("%-*s %-*s ", lns->own_len, pwd->pw_name, 
            lns->grp_len, grp->gr_name);
    }
    else
    {
        printf(" %-*ld %-*ld ", lns->own_len, (long)file_stat.st_uid, 
            lns->grp_len, (long)file_stat.st_gid);
    }

    printf(" %*ld", lns->s_len, (long)file_stat.st_size);

    char timebuf[80];
    struct tm lt;
    localtime_r(&file_stat.st_mtime, &lt);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", &lt);
    printf(" %s ", timebuf);

    if (bold_flag)
        printf("%s", BOLD_ON);
    printf("%s%s%s", color, entry_name, COL_RESET);
    if (bold_flag)
        printf("%s", BOLD_OFF);

    if (S_ISLNK(file_stat.st_mode))
    {
        char link_target[1024];
        ssize_t len = readlink(fp_entry, link_target, sizeof(link_target) - 1);
        if (len != -1)
        {
            link_target[len] = '\0';
            printf(" -> %s%s%s%s%s", BOLD_ON, bad_link ? COL_RED : COL_CYAN, 
                link_target, COL_RESET, BOLD_OFF);
        }
    }    

    free(fp_entry);
}

void list_dir(const char * path, bool long_form, bool all)
{

    // for debug purposes 
    // printf("I WORK WHILE YOU SLEEP\n");

    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stderr, "failed to open directory\n");
        exit(EXIT_FAILURE);
    }

    struct dirent* d_info = readdir(dir);
    char ** entries = (char**)malloc(sizeof(char*) * 2048);
    int num_of_entries = 0;

    while(d_info != NULL && num_of_entries < 2048)
    {
    
        //printf("%s\n", d_info->d_name);
        if (!all && d_info->d_name[0] == '.')
        {
            d_info = readdir(dir);
            continue;
        }
        /*
        print_entry(d_info->d_name, long_form);
        if (long_form)
            printf("\n");
        else
            printf(" ");
        */
        int n_size = strlen(d_info->d_name) + 1;
 
        entries[num_of_entries] = (char*)malloc(n_size * sizeof(char));
        strcpy(entries[num_of_entries], d_info->d_name);
        num_of_entries++;

        d_info = readdir(dir);
    
    }
    
    sort_entries(entries, num_of_entries);

    struct prm_lens lns;
    blkcnt_t total_blocks;

    __calc_lengths(path, entries, num_of_entries, &lns, &total_blocks);

    if (long_form)
        printf("total %ld\n", (long)total_blocks / 2);

    int cntr = 0;
    for (;cntr < num_of_entries; cntr++)
    {
        //printf("%s\n", entries[cntr]);
    
        print_entry(path, entries[cntr], long_form, &lns);
        if (long_form)
            printf("\n");
        else
            printf("  ");
    }

    if (!long_form)
        printf("\n");
    
    cntr = 0;
    for (; cntr < num_of_entries; cntr++)
        free(entries[cntr]);    
    free(entries);

    if (closedir(dir) == -1)
    {
        fprintf(stderr, "failed to close directory\n");
        exit(EXIT_FAILURE);
    }
}
