// interpreter.c

#include "interpreter.h"
#include "executor.h"
#include "../data/field.h"
#include "../utils/errors.h"
#include "../utils/common.h"
#include "../utils/validator.h"
#include <stdio.h>
#include <stdlib.h>
#include "../parser/parser.h"
#include <synchapi.h>

void sleep_ms(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}


Interpreter* create_interpreter(void) {
    Interpreter* interpreter = (Interpreter*) malloc(sizeof(Interpreter));
    if (!interpreter) return NULL;
    
    interpreter->field = NULL;
    interpreter->command = NULL;
    interpreter->command_list = NULL;
    interpreter->current_command = 0;
    interpreter->execution_success = 1;
    
    // Конфигурация по умолчанию
    interpreter->config.should_display = 1;
    interpreter->config.should_save = 1;
    interpreter->config.display_interval = 1;
    
    return interpreter;
}

void free_interpreter(Interpreter* interpreter) {
    if (!interpreter) return;
    
    if (interpreter->field) {
        free_field(interpreter->field);
    }
    
    if (interpreter->command_list) {
        free_command_list(interpreter->command_list);
    }
    
    free(interpreter);
}

int execute_command(Interpreter* interpreter, Command *cmd) {
    if (!interpreter) return EXECUTION_ERROR;
    
    ExecutionResult result = EXECUTION_SUCCESS;
    
    switch (cmd->type) {
        case CMD_SIZE:
            if (!execute_size(&interpreter->field, &interpreter->history_field, cmd->parametrs.size.width, cmd->parametrs.size.height, cmd->line_number)) {
                result = EXECUTION_ERROR;
            }
            break;
            
        case CMD_START:
            if (!execute_start(interpreter->field, interpreter->history_field, cmd->parametrs.position.x, cmd->parametrs.position.y, cmd->line_number)) {
                result = EXECUTION_ERROR;
            }
            break;
            
        case CMD_MOVE:
            if (!execute_move(interpreter->field, interpreter->history_field, cmd->parametrs.direction, cmd->line_number))
                result = EXECUTION_ERROR;
            break;

        case CMD_PAINT:
            if (!execute_paint(interpreter->field, interpreter->history_field, cmd->parametrs.color_char, cmd->line_number)) 
                result = EXECUTION_ERROR;
            break;
            
        case CMD_JUMP:
            if (!execute_jump(interpreter->field, interpreter->history_field, cmd->parametrs.direction, cmd->jump_distance, cmd->line_number))
                result = EXECUTION_ERROR;
            break;
        case CMD_DIG:
            if (!execute_dig(interpreter->field,interpreter->history_field, cmd->parametrs.direction, cmd->line_number))
                result = EXECUTION_ERROR;
            break;

        case CMD_MOUND:
            if (!execute_mound(interpreter->field, interpreter->history_field, cmd->parametrs.direction, cmd->line_number))
                result = EXECUTION_ERROR;
            break;
        case CMD_GROW:
            if (!execute_grow(interpreter->field, interpreter->history_field, cmd->parametrs.direction, cmd->line_number))
                result = EXECUTION_ERROR;
            break;
        case CMD_CUT:
            if (!execute_cut(interpreter->field, interpreter->history_field, cmd->parametrs.direction, cmd->line_number))
                result = EXECUTION_ERROR;
            break;
        case CMD_MAKE:
            if (!execute_make(interpreter->field, interpreter->history_field, cmd->parametrs.direction, cmd->line_number))
                result = EXECUTION_ERROR;
            break;
        case CMD_PUSH:
            if (!execute_push(interpreter->field, interpreter->history_field, cmd->parametrs.direction, cmd->line_number))
                result = EXECUTION_ERROR;
            break;
        case CMD_LOAD:
            if (!execute_load(&interpreter->field, &interpreter->history_field, cmd->parametrs.filename, cmd->line_number))
                result = EXECUTION_ERROR;
            break;
        case CMD_IF:
            if (!execute_if(interpreter, interpreter->field, cmd))
                result = EXECUTION_ERROR;
            break;
        case CMD_UNDO:
            if (!execute_undo(interpreter->field, interpreter->history_field, cmd->line_number));
                result = EXECUTION_ERROR;
            break;
        case CMD_UNKNOWN:
            handle_warning("Skipping unknown command", cmd->line_number);
            result = EXECUTION_ERROR;
            break;
            
        case CMD_COMMENT:
            result = EXECUTION_SUCCESS; // Тихий пропуск
            break;
            
        default:
            handle_warning("Unimplemented command", cmd->line_number);
            result = EXECUTION_WARNING;
            break;
    }
    
    // Обновление дисплея после команды
    if (interpreter->config.should_display && interpreter->field) {
        print_field(interpreter->field);
        if (interpreter->config.display_interval > 0) {
            sleep_ms(interpreter->config.display_interval * 1000);
        }
    }
    
    return result;
}

int execute_program(Interpreter* interpreter, const char* input_file, const char* output_file) {
    if (!interpreter) {
        handle_error("Interpreter is NULL", 0);
    }
    
    // Сохраняем имена файлов в конфигурации
    strncpy(interpreter->config.input_file, input_file, MAX_FILENAME - 1);
    strncpy(interpreter->config.output_file, output_file, MAX_FILENAME - 1);
    
    // Парсинг и выполнение команд
    interpreter->command_list = parse_file(input_file);
    for (int i = 0; i < interpreter->command_list->count; i++) {
        Command cmd = interpreter->command_list->commands[i];
        ExecutionResult result = (ExecutionResult) execute_command(interpreter, &cmd);
        
        if (result == EXECUTION_ERROR) {
            // При критической ошибке всё равно сохраняем текущее состояние
            if (interpreter->config.should_save && interpreter->field) {
                save_field(interpreter->field, output_file);
            }
            return EXECUTION_ERROR;
        }
    }

    // СОХРАНЕНИЕ ФИНАЛЬНОГО СОСТОЯНИЯ
    if (interpreter->config.should_save && interpreter->field) {
        if (!save_field(interpreter->field, output_file)) {
            handle_error("Failed to save result to output file", 0);
            return EXECUTION_ERROR;
        }
    } else if (interpreter->config.should_save && !interpreter->field) {
        handle_warning("No field to save - creating empty field", 0);
        // Можно создать пустое поле и сохранить
        Field* empty_field = create_field(10, 10);
        if (empty_field) {
            save_field(empty_field, output_file);
            free_field(empty_field);
        }
    }
    
    return interpreter->execution_success ? EXECUTION_SUCCESS : EXECUTION_ERROR;
}
