#include <stdio.h>
#include <stdlib.h>


struct strip_element {
    int value;
    struct strip_element *next;
    struct strip_element *prev;	
};

struct stack_element {
    int value;
    struct stack_element *next;
};

typedef struct stack_element *stack_entry;

typedef struct strip_element *entry;

entry strip;
stack_entry stack;

//Step forward once in the Turing strip
void stepForward() {
    if (strip->next == NULL) {
        entry next_entry = malloc(sizeof(entry));
        next_entry->prev = strip;
        next_entry->value = 0;
        next_entry->next = NULL;
        strip->next = next_entry;
        strip = next_entry;
    } else {
        strip = strip->next;
    }
}

//Step backwards once in the Turing strip
void stepBackward() {
    if (strip->prev == NULL) {
        entry prev_entry = malloc(sizeof(entry));
        prev_entry->next = strip;
        prev_entry->value = 0;
        prev_entry->prev = NULL;
        strip->prev = prev_entry;
        strip = prev_entry;
    } else {
        strip = strip->prev;
    }
}

//Skips amount steps forward in the Turing strip
void skipForward(int amount) {
    for (int i = 0; i < amount; i++) {
        stepForward();
    }
}

//Skips amount steps backward in the Turing strip
void skipBackward(int amount) {
    for (int i = 0; i < amount; i++) {
        stepBackward();
    }
}

//Writes to the strip
void write(int value) { 
    strip->value = value;
}

//Reads from the strip
int read() {
    return strip->value;
}

//Pushes to the stack
void push(int value) {
    stack_entry new_head = malloc(sizeof(stack_entry));
    new_head->value = value;
    new_head->next = stack;
    stack = new_head;
}

//Pops from the stack
int pop() {
    if (stack->next == NULL) {
        return 0;
    }
    int out = stack->value;
    stack_entry new_head = stack->next;
    free(stack);
    stack = new_head;
    return out;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage : crash program_string\n");
        return -1;
    }
   
    char *program = argv[1]; 
    //Initialize stack and strip
    strip = malloc(sizeof(entry));
    strip->next = NULL;
    strip->prev = NULL;
    strip->value = 0;

    stack = malloc(sizeof(stack_entry));
    stack->next = NULL;
    stack->value = 0;

    int program_head = 0;
    for (;;) {
        if (program_head < 0) {
            program_head = 0;
        }
        switch (program[program_head]) {
            //CORE OPERATIONS
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                //Read value, push to stack
                ;
                int converted = 0;
                while ((program[program_head] >= 48)
                    && (program[program_head] <= 57)) {
                    converted = (converted * 10) + (program[program_head] - 48);
                    program_head++;
                }
                program_head--;
                push(converted);
                break;
            case 'p':
                printf("%c", (char) pop()); 
                break;
            case ']':
                stepForward();
                break;    
            case '[':
                stepBackward();
                break;
            case '.':
                //Write 
                write(pop());
                break;
            case ',':
                //Read
                if (program[program_head + 1] == '}') {
                    program_head++;
                    int limit = pop();
                    for (int i = 0; i < limit; i++) {
                        stepForward();
                        push(read());
                    }
                    continue;
                }
                if (program[program_head + 1] == '}') {
                    program_head++;
                    int limit = pop();
                    for (int i = 0; i < limit; i++) {
                        stepBackward();
                        push(read());
                    }
                    continue;
                }
                push(read());
                break;
            case '?':
                if (pop() == 1) {
                    program_head++;
                    int converted = 0;
                    while ((program[program_head] >= 48)
                        && (program[program_head] <= 57)) {
                        converted = (converted * 10) + (program[program_head] - 48);
                        program_head++;
                    }
                    program_head = converted;
                    program_head--;
                    continue;
                }
                program_head++;
                while ((program[program_head] >= 48)
                    && (program[program_head] <= 57)) {
                    program_head++;
                }
                program_head--;
                break; 
            case '{':
                skipForward(pop());
                break;
            case '}':
                skipBackward(pop());
                break;
            case 'q':
            case '\0':
                return 0;
            case '~':
                //Discard
                pop();
                break;
            case ' ':
                //Relative jump
                program_head += pop();
                break;
            case 'n':
                //Negative
                push(0 - pop());
                break;
            case '!':
                //Invert
                if (pop() == 0) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            case 'r':
                //Repeat the top value of the stack
                ;
                int value = pop();
                push(value);
                push(value);
                break;



            //LOGIC OPERATIONS
            case '=':
                if (pop() == pop()) {
                    push(1);
                } else {
                    push(0);
                }
                break;
    }
        program_head++;
    } 
}

