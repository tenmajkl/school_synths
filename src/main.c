#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

#define DATA_FORMAT "%s|%s|%d|%d|%d"
#define SWAP(T, X, Y) do { T _help = X;  X = Y; Y = _help; } while (0);

typedef struct {
    char name[16];
    char manufacturer[16];
    int year;
    int voices;
    bool analog;
} Synthesizer;

typedef struct {
    Synthesizer* array;
    int size;
} SynthesizerArray;

typedef struct {
    char* description;
    void (function*)(SynthesizerArray list);
} MenuItem;

const MenuItem menu_items[] = {
    {"Vypsat data"},
    {"Seradit podle jmena"},
    {"Seradit podle roku vydani"},
    {"Vypsat pocet modelu urcite znacky"},
    {"Najit nejstarsi syntezator"},
};

SynthesizerArray load(FILE* input) 
{
    SynthesizerArray array;
    array.array = malloc(16 * sizeof(Synthesizer));
    array.size = 0;
    int max_size = 16;
    Synthesizer* new;
    int analog;

    while (fscanf(input, "%15[^|]|%15[^|]|%d|%d|%d", 
                array.array[array.size].name, 
                array.array[array.size].manufacturer, 
                &array.array[array.size].year, 
                &array.array[array.size].voices, 
                &analog
            ) == 5
    ) {
        array.array[array.size].analog = analog != 0;
        array.size++;
        if (array.size == max_size) {
            max_size += 16;
            new = realloc(array.array, max_size);
            if (new == NULL) {
                // ERROR
                return array;
            }
            array.array = new;
        }
    }

    return array;
}

void write(FILE* output, SynthesizerArray array, char* format)
{
    Synthesizer item;
    for (int index = 0; index < array.size; index++) {
        item = array.array[index];
        fprintf(output, format, item.name, item.manufacturer, item.year, item.voices, item.analog != 0);
    }
}

int main(void) 
{
    FILE* file = fopen("data.txt", "r");
    if (file == NULL) {
        return -1;
    }
    SynthesizerArray list = load(file);
    write(stdout, list, DATA_FORMAT);
    fclose(file);
    return 0;
}
