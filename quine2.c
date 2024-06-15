int main(void) {
    static constexpr char code[] = "int main(void) {\n    static constexpr char code[] = \"?\";\n\n    for (size_t i = 0; i < sizeof code - 1; ++i)\n        if (code[i] == 0x3F)\n            for (size_t j = 0; j < sizeof code - 1; ++j)\n                switch(code[j]) {\n                    case '\\n': putchar('\\\\'); putchar('n'); break;\n                    case '\\\\': putchar('\\\\'); putchar('\\\\'); break;\n                    case '\\\"' : putchar('\\\\'); putchar('\"'); break;\n                    default  : putchar(code[j]);\n                }\n        else putchar(code[i]);\n}\n";

    for (size_t i = 0; i < sizeof code - 1; ++i)
        if (code[i] == 0x3F)
            for (size_t j = 0; j < sizeof code - 1; ++j)
                switch(code[j]) {
                    case '\n': putchar('\\'); putchar('n'); break;
                    case '\\': putchar('\\'); putchar('\\'); break;
                    case '\"' : putchar('\\'); putchar('"'); break;
                    default  : putchar(code[j]);
                }
        else putchar(code[i]);
}
