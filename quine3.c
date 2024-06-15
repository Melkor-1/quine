int main(void) {
    static constexpr char code[] = "int main(void) {\n    static constexpr char code[] = \"?\";\n\n    for (const char *p = code; *p; ++p)\n        if (*p == 0x3F) /* question mark */\n            for (const char *c = code; *c; ++c)\n                switch(*c) {\n                    case '\\n': putchar('\\\\'); putchar('n'); break;\n                    case '\\\\': putchar('\\\\'); putchar('\\\\'); break;\n                    case '\\\"' : putchar('\\\\'); putchar('\"'); break;\n                    default  : putchar(*c);\n                }\n        else putchar(*p);\n}\n";

    for (const char *p = code; *p; ++p)
        if (*p == 0x3F) /* question mark */
            for (const char *c = code; *c; ++c)
                switch(*c) {
                    case '\n': putchar('\\'); putchar('n'); break;
                    case '\\': putchar('\\'); putchar('\\'); break;
                    case '\"' : putchar('\\'); putchar('"'); break;
                    default  : putchar(*c);
                }
        else putchar(*p);
}
