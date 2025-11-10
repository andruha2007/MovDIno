// movdino.c

#include "core/interpreter.h"
#include "utils/errors.h"
#include "utils/validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char* program_name) {
    printf("Usage: %s input.txt output.txt [options]\n", program_name);
    printf("Options:\n");
    printf("  --interval N    Set display interval in seconds (default: 1)\n");
    printf("  --no-display    Disable console visualization\n");
    printf("  --no-save       Disable saving to output file\n");
    printf("  --help          Show this help message\n");
}

int parse_arguments(int argc, char* argv[], InterpreterConfig* interpreter) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 0;
    }
    
    strncpy(interpreter->output_file, argv[2], MAX_FILENAME - 1);
    interpreter->should_display = 1;
    interpreter->should_save = 1;
    interpreter->display_interval = 1;
    
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--interval") == 0 && i + 1 < argc) {
            if (is_strict_integer(argv[++i]) == -1){
                handle_error("Interval should be positive ", 0);
            }
            if (atoi(argv[i]) == 0){
                handle_error("Interval should be positive ", 0);
            }
            interpreter->display_interval = atoi(argv[i]);   
        }
        else if (strcmp(argv[i], "--no-display") == 0) {
            interpreter->should_display = 0;
        }
        else if (strcmp(argv[i], "--no-save") == 0) {
            interpreter->should_save = 0;
        }
        else if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }
    
    return 1;
}

int main(int argc, char* argv[]) {
    printf("=== MovDino Interpreter ===\n");
    
    // Парсинг аргументов
    InterpreterConfig config;
    if (!parse_arguments(argc, argv, &config)) {
        return EXIT_FAILURE;  // Обычная ошибка - корректный выход
    }
    
    // Создание интерпретатора
    Interpreter* interpreter = create_interpreter();
    if (!interpreter) {
        handle_error("Failed to create interpreter", 0);  // Критическая ошибка
    }
    interpreter->config = config;

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    // Выполнение программы
    int result = execute_program(interpreter, input_file, output_file);
    
    free_interpreter(interpreter);

    if (result == EXECUTION_SUCCESS) {
        printf("Program completed successfully!\n");
        return EXIT_SUCCESS;
    } else {
        printf("Program execution completed with errors\n");
        return EXIT_FAILURE;
    }
}

// cd /c/Code_Territory/C_Code/movdino_lab
// 
// gcc -I. movdino.c core/interpreter.c core/executor.c data/field.c parser/parser.c parser/specific_parser.c utils/errors.c utils/validator.c -o movdino.exe
//
// ./movdino.exe input.txt output.txt --no-display  
