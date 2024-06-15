#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    static constexpr char code[] = "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n\nint main(void)\n{\n    static constexpr char code[] = \"?\";\n\n    for (size_t i = 0; i < sizeof code - 1; ++i) {\n        if (code[i] == 63) {\n            for (size_t j = 0; j < sizeof code - 1; ++j) {\n                switch(code[j]) {\n                    case '\\n': printf(\"\\\\n\");  break;\n                    case '\\\\': printf(\"\\\\\\\\\"); break;\n                    case '\"' : printf(\"\\\\\\\"\"); break;\n                    default  : printf(\"%c\", code[j]);\n                }\n            }\n        } else {\n            printf(\"%c\", code[i]);\n        }\n    }\n\n    return EXIT_SUCCESS;\n}\n";

    for (size_t i = 0; i < sizeof code - 1; ++i) {
        if (code[i] == 63) {
            for (size_t j = 0; j < sizeof code - 1; ++j) {
                switch(code[j]) {
                    case '\n': printf("\\n");  break;
                    case '\\': printf("\\\\"); break;
                    case '"' : printf("\\\""); break;
                    default  : printf("%c", code[j]);
                }
            }
        } else {
            printf("%c", code[i]);
        }
    }

    return EXIT_SUCCESS;
}
