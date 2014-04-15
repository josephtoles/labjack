#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

//constants
const char* ROOT_FILE_NAME = "graph.C";
const char BEGINNING[];
const char END[];
const int NUM_RTDS = 4;

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
        samples[n] = s;
        ++n;
    }
    fclose(f);
    for(int i=0; i<n; ++i)
    {
        printf("Datapoint %d\n", i);
        printf("Timestamp %s\n", samples[i].timestamp);
        for(int j=0; j<4; ++j)
            printf("RTD%d @ %f\n", j, samples[i].rtd[j]);
        printf("Pressure %f\n", samples[i].pressure);

    }

    f = fopen(ROOT_FILE_NAME, "w");
    if(f==NULL)
    {
        printf("Error opening file to write \"%s\"\n", ROOT_FILE_NAME);
        return 1;
    }

    fprintf(f, BEGINNING);

    //input arrays here
    fprintf(f, "const Int_t n = %d;\n", n);
    for(int i=0; i<NUM_RTDS; ++i)
    {
        fprintf(f, "Double_t x%d[] = {", i);
        for(int j=0; j<n; ++j)
        {
            //Make this better once you implement timestamps
            fprintf(f, "%d", j);
            if(j!=n-1)
                fprintf(f, ",");
        }
        fprintf(f, "};\n");

        fprintf(f, "Double_t y%d[] = {", i);
        for(int j=0; j<n; ++j)
        {
            //Make this better once you implement timestamps
            fprintf(f, "%f", samples[j].rtd[i]);
            if(j!=n-1)
                fprintf(f, ",");
        }
        fprintf(f, "};\n");
        fprintf(f, "TGraph *gr%d = new TGraph(n,x%d,y%d);\n", i, i, i);
        fprintf(f, "gr%d->SetMarkerColor(kBlue);\n", i); //make better
        fprintf(f, "gr%d->SetMarkerStyle(%d);\n", i, i+20); //make better
        fprintf(f, "gr%d->Fit(\"pol5\", \"q\");\n", i);
        fprintf(f, "mg->Add(gr%d);\n", i);
    }

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
    "}\n";
