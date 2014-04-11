#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

//constants
const char* ROOT_FILE_NAME = "graph.C";
const char BEGINNING[];
const char END[];

struct sample
{
    char timestamp[10];
    double rtd[4];
    double rtd_v[4];
    double pressure;
    double pressure_v;
};

int main(int argc, char** argv)
{
    //flags
    int c;
    extern char *optarg;
    extern int optind, optopt, opterr;

    bool disp_temperature = false;
    bool disp_pressure = false;


    while ((c = getopt(argc, argv, ":t")) != -1) {
    switch(c) {
        case 't':
            disp_temperature = true;
            break;
        case  'p':
            disp_pressure = true;
            break;
        case '?':
            printf("unknown arg %c\n", optopt);
            break;
        }
    }

    //file access
    char* FILE_NAME = "2014-04-08_12:41:25";
    FILE* f = fopen(FILE_NAME, "r");
    if(f == NULL)
        printf("Could not open file %s.\n", FILE_NAME);

    //ignore first line //works
    char mark = getc(f);
    while(mark != '\n') {
        mark = getc(f);
    }

    int samples_len = 10;
    struct sample* samples;
    samples = (struct sample*)malloc(sizeof(struct sample)*samples_len);

    //read data
    int n = 0;
    while(!feof(f))
    {
        //extend array size
        if(n==samples_len)
        {
            struct sample* new_samples;
            new_samples = (struct sample*)malloc(sizeof(struct sample)*samples_len*2);
            for(int i=0; i<n; ++i)
                new_samples[i] = samples[i];
            free(samples);
            samples = new_samples;
            samples_len *= 2;
        }

        //append data
        printf("Scanning datapoint %d\n", n);
        struct sample s;
        fscanf(f,
            "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
            s.timestamp,
            s.rtd, s.rtd+1, s.rtd+2, s.rtd+3,
            &s.pressure,
            s.rtd_v, s.rtd_v+1, s.rtd_v+2, s.rtd_v+3,
            &s.pressure_v);
        ++n;
    }
    fclose(f);

    f = fopen(ROOT_FILE_NAME, "w");
    if(f==NULL)
    {
        printf("Error opening file to write \"%s\"\n", ROOT_FILE_NAME);
        return 1;
    }

    fprintf(f, BEGINNING);

    //input arrays here

    fprintf(f, END);

 
    return 0;
}

const char BEGINNING[] = 
    "#include<unistd.h>\n"
    "void graph() {\n"
    "TCanvas *c1 = new TCanvas(\"c1\", \"Graph title\", 10, 40, 700, 450);\n"
    "c1->SetGrid();\n"
    "TMultiGraph *mg = new TMultiGraph();\n";

const char END[] = 
    "mg->Draw(\"ALP\");\n"
    "c1->Update();\n"
    "exit();\n"
    "}\n";
