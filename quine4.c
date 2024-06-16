#define _POSIX_C_SOURCE 2008'19L
#define _XOPEN_SOURCE   700

#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static char *clones[26];
static size_t clones_count;

void free_clones(void)
{
    while (clones_count--) {
        free(clones[clones_count]);
    }
}

char *caeser_cipher(unsigned int key, 
                    char dest[restrict static 1],
                    const char src[restrict static 1])
{
    char *p = dest;

    for (const unsigned char *c = (const unsigned char *)src; *c; ++c, ++p) {
        if (isalpha(*c)) {
            char const pivot = isupper(*c) ? 'A' : 'a';
            *p = (*c - pivot + key) % 26 + pivot;
        } else {
            *p = *c;
        }
    }

    *p = '\0';
    return dest;
}

bool copy_file(FILE dst[restrict static 1], FILE src[restrict static 1])
{
    struct stat st;
    
    /* Copy permissions, then copy contents. */
    if (fstat(fileno(src), &st) == -1
        || fchmod(fileno(dst), st.st_mode) == -1) {
        return false;
    }

    char buf[65536];
    size_t n;

    while ((n = fread(buf, 1, sizeof buf, src)) > 0) {
        if (fwrite(buf, 1, n, dst) != n) {
            return false;
        }
    }

    return true;
}

FILE *open_new_clone(const char fpath[static 1])
{
    /* Allocate 2 extra bytes for adding "./" to the cloned binaries for passing
     * to execvp() later, and 1 more for the null terminator. */
    char *clone = malloc(strlen(fpath) + 3);    
    
    if (clone == nullptr) {
        return nullptr;
    }

    for (size_t i = 1; i < 26; ++i) {
        FILE *const f = fopen(caeser_cipher(i, clone + 2, fpath), "wx");

        if (f == nullptr) {
            if (errno != EEXIST) {
                return f;
            }

            clone[0] = '.';
            clone[1] = '/';
            clones[clones_count] = strdup(clone);
            
            if (clones[clones_count] == nullptr) {
            /* No recovery possible, so cleanup and exit. */
                perror("malloc()");
                free_clones();
                free(clone);
                exit(EXIT_FAILURE);
            }
            ++clones_count;
            continue;
        }
        free(clone);
        return f;
    }
    return nullptr;
}

bool clone_file(const char fpath[static 1])
{
    FILE *file = {};
    FILE *clone_file = {};
    bool status = (file = fopen(fpath, "r"))
                  && (clone_file = open_new_clone(fpath))
                  && copy_file(clone_file, file);

    if (file)       fclose(file);
    if (clone_file) fclose(clone_file);
    return status;
}

void exec_clones(void)
{
    for (size_t i = 0; i < clones_count; ++i) {
        pid_t cpid = fork();

        switch (cpid) {
            case -1: 
                perror("fork()"); 
                return;  /* Ignore error. */

            case 0 : 
                execvp(clones[i], (char *[]) {clones[i], nullptr}); 
                _Exit(EXIT_FAILURE);

            case 1 : 
                wait(nullptr);
        }
    }
}

int main(int, char **argv) {
    clone_file(argv[0][0] == '.' && argv[0][1] == '/' ? argv[0] + 2 : argv[0]);

    static constexpr char code[] = "#define _POSIX_C_SOURCE 2008'19L\n#define _XOPEN_SOURCE   700\n\n#include <errno.h>\n#include <ctype.h>\n#include <stdio.h>\n#include <stdbool.h>\n#include <string.h>\n#include <stdlib.h>\n#include <sys/stat.h>\n#include <stddef.h>\n#include <sys/types.h>\n#include <unistd.h>\n#include <sys/wait.h>\n\nstatic char *clones[26];\nstatic size_t clones_count;\n\nvoid free_clones(void)\n{\n    while (clones_count--) {\n        free(clones[clones_count]);\n    }\n}\n\nchar *caeser_cipher(unsigned int key, \n                    char dest[restrict static 1],\n                    const char src[restrict static 1])\n{\n    char *p = dest;\n\n    for (const unsigned char *c = (const unsigned char *)src; *c; ++c, ++p) {\n        if (isalpha(*c)) {\n            char const pivot = isupper(*c) ? 'A' : 'a';\n            *p = (*c - pivot + key) % 26 + pivot;\n        } else {\n            *p = *c;\n        }\n    }\n\n    *p = '\\0';\n    return dest;\n}\n\nbool copy_file(FILE dst[restrict static 1], FILE src[restrict static 1])\n{\n    struct stat st;\n    \n    /* Copy permissions, then copy contents. */\n    if (fstat(fileno(src), &st) == -1\n        || fchmod(fileno(dst), st.st_mode) == -1) {\n        return false;\n    }\n\n    char buf[65536];\n    size_t n;\n\n    while ((n = fread(buf, 1, sizeof buf, src)) > 0) {\n        if (fwrite(buf, 1, n, dst) != n) {\n            return false;\n        }\n    }\n\n    return true;\n}\n\nFILE *open_new_clone(const char fpath[static 1])\n{\n    /* Allocate 2 extra bytes for adding \"./\" to the cloned binaries for passing\n     * to execvp() later, and 1 more for the null terminator. */\n    char *clone = malloc(strlen(fpath) + 3);    \n    \n    if (clone == nullptr) {\n        return nullptr;\n    }\n\n    for (size_t i = 1; i < 26; ++i) {\n        FILE *const f = fopen(caeser_cipher(i, clone + 2, fpath), \"wx\");\n\n        if (f == nullptr) {\n            if (errno != EEXIST) {\n                return f;\n            }\n\n            clone[0] = '.';\n            clone[1] = '/';\n            clones[clones_count] = strdup(clone);\n            \n            if (clones[clones_count] == nullptr) {\n                \n            /* No recovery possible, so cleanup and exit. */\n                perror(\"malloc()\");\n                free_clones();\n                free(clone);\n                exit(EXIT_FAILURE);\n            }\n            ++clones_count;\n            continue;\n        }\n        free(clone);\n        return f;\n    }\n    return nullptr;\n}\n\nbool clone_file(const char fpath[static 1])\n{\n    FILE *file = {};\n    FILE *clone_file = {};\n    bool status = (file = fopen(fpath, \"r\"))\n                  && (clone_file = open_new_clone(fpath))\n                  && copy_file(clone_file, file);\n\n    if (file)       fclose(file);\n    if (clone_file) fclose(clone_file);\n    return status;\n}\n\nvoid exec_clones(void)\n{\n    for (size_t i = 0; i < clones_count; ++i) {\n        pid_t cpid = fork();\n\n        switch (cpid) {\n            case -1: \n                perror(\"fork()\"); \n                return;  /* Ignore error. */\n\n            case 0 : \n                execvp(clones[i], (char *[]) {clones[i], nullptr}); \n                _Exit(EXIT_FAILURE);\n\n            case 1 : \n                wait(nullptr);\n        }\n    }\n}\n\nint main(int, char **argv) {\n    clone_file(argv[0][0] == '.' && argv[0][1] == '/' ? argv[0] + 2 : argv[0]);\n\n    static constexpr char code[] = \"~\";\n\n    for (const char *p = code; *p; ++p)\n        if (*p == 0x7E) /* bitwise complement op */\n            for (const char *c = code; *c; ++c)\n                switch(*c) {\n                    case '\\n': putchar('\\\\'); putchar('n'); break;\n                    case '\\\\': putchar('\\\\'); putchar('\\\\'); break;\n                    case '\\\"' : putchar('\\\\'); putchar('\"'); break;\n                    default  : putchar(*c);\n                }\n        else putchar(*p);\n    exec_clones();\n    free_clones();\n    return EXIT_SUCCESS;\n}\n";

    for (const char *p = code; *p; ++p)
        if (*p == 0x7E) /* bitwise complement op */
            for (const char *c = code; *c; ++c)
                switch(*c) {
                    case '\n': putchar('\\'); putchar('n'); break;
                    case '\\': putchar('\\'); putchar('\\'); break;
                    case '\"' : putchar('\\'); putchar('"'); break;
                    default  : putchar(*c);
                }
        else putchar(*p);
    exec_clones();
    free_clones();
    return EXIT_SUCCESS;
}
