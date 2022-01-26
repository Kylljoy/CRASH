#include <stdio.h>
#include <stdlib.h>

#define PROGRAM_SIZE = 1000

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
    for (int i = 1; i < amount; i++) {
        stepForward();
    }
}

//Skips amount steps backward in the Turing strip
void skipBackward(int amount) {
    for (int i = 1; i < amount; i++) {
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

FILE *program;

int getchr() {
    return fgetc(program);
}

int putchr() {
    return fseek(program, -1, SEEK_CUR);    
}

int jump(int pos) {
    return fseek(program, pos, SEEK_SET);
}

void cleanup() {
    fclose(program);
    program = NULL;
}






int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage : crash program_file\n");
        return -1;
    }
   
    char *program_file= argv[1];
    if (program_file == NULL) {
        return -1;
    }

    program = fopen(program_file, "r");
    if (!program) {
        return -1;
    }
     
    //Initialize stack and strip
    strip = malloc(sizeof(entry));
    strip->next = NULL;
    strip->prev = NULL;
    strip->value = 0;

    stack = malloc(sizeof(stack_entry));
    stack->next = NULL;
    stack->value = 0;
    char c;
    for (;;) {
        switch (getchr()) {
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
                putchr();
                while (((c = (char) getchr()) >= 48)
                    && (c <= 57)
                    && (c != EOF)) {
                    converted = (converted * 10) + 
                        (c - 48);
                } 
                push(converted);
                putchr();
                break;
            case 39:
                ;
                while ((c = getchr()) != 39) {
                    push((char) c);
                }
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
                c = getchr();
                if (c == '}') {
                    int limit = pop();
                    for (int i = 0; i < limit; i++) {
                        write(pop());
                        stepForward();
                    }
                    continue;   
                }
                if (c == '{') {
                    int limit = pop();
                    for(int i = 0; i < limit; i++) {
                        write(pop());
                        stepBackward();
                    }
                    continue;
                }
                putchr();
                write(pop());
                break;
            case ',':
                //Read
                ;
                c = getchr();
                if (c == '}') {
                    int limit = pop();
                    for (int i = 0; i < limit; i++) {
                        push(read());
                        stepForward();
                    }
                    continue;
                }
                if (c == '{') {
                    int limit = pop();
                    for (int i = 0; i < limit; i++) {
                        push(read());
                        stepBackward();
                    }
                    continue;
                }
                push(read());
                break;
            case '?':
                if (pop() == 1) {
                    int converted = 0;
                    while (((c = getchr()) >= 48)
                        && (c <= 57)) {
                        converted = (converted * 10) + (c - 48);
                    }
                    jump(converted);
                    continue;
                } else {
                    while (((c = getchr()) >= 48)
                        && c <= 57) {
                    }
                    putchr();       
                }
                break; 
            case '{':
                skipBackward(pop());
                break;
            case '}':
                skipForward(pop());
                break;
            case 'q':
            case '\0':
            case EOF:
                cleanup();
                return 0;
            case '~':
                //Discard
                pop();
                break;
            case ' ':
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
            case 'f':
                //Flush
                printf("\n");
                break;
            case '%':
                //Swap top two stack elements
                ;
                int a_ = pop();
                int b_ = pop();
                push(a_);
                push(b_);
                break;

            //LOGIC OPERATIONS
            case '=':
                if (pop() == pop()) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            case '>':
                if (pop() > pop()) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            case '<':
                if (pop() < pop()) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            case '|':
                if ((pop() == 1) || (pop() == 1)) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            case '&':
                if ((pop() == 1) && (pop() == 1)) {
                    push(1);
                } else {
                    push(0);
                }
                break;

            //Math
            case '*':
                push(pop() * pop());
                break;
            case '/':
                ;
                a_ = pop();
                b_ = pop();
                if(b_ == 0) {
                    printf("Division by 0\n");
                    cleanup();
                    return -1;
                } else {
                    push(a_ / b_);
                }
                break;
            case '+':
                push(pop() + pop());
                break;
            case '-':
                push(pop() - pop());
                break;
        }
    } 
}

