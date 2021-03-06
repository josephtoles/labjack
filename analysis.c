#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "calculations.h"

//constants
const char* ROOT_FILE_NAME = "graph.C";
const char BEGINNING[];
const char END[];
const int NUM_RTDS = 4;
const int TEMP_MARKER_STYLES[4] = {20, 20, 20, 20};
const int TEMP_MARKER_COLORS[4] = {2, 3, 4, 28};

//Command line options
//Default behavior: display temperature only
// -v display voltages
// -t display temperatures
// -p display pressures
// -o display resistance (always calculated, never from file)
// -f display fraction of pressure/temperature
// -r recalculate all data from voltages (use this when a new equation has been implemented)
//        Note: doing so does not alter the original file
//User must also specify a file with data

static int pID;

struct sample
{
    char timestamp[10];
    double rtd[4];
    double rtd_v[4];
    double rtd_r[4]; //resistance
    double pressure;
    double pressure_v;
};

int main(int argc, char** argv)
{
    //file access
    char file_name[100] = "";
    for(int i=1; i<argc; ++i)
    {
        if(argv[i][0] != '-')
        {
            strcpy(file_name, argv[i]);
            break;
        }
    }
    if(strcmp(file_name, "")==0)
    {
        printf("Please name a file to display.\n");
        return -1;
    }
 
    //flags
    int c;
    extern char *optarg;
    extern int optind, optopt, opterr;

    bool disp_temperature = false;
    bool disp_pressure = false;
    bool disp_voltage = false;
    bool disp_fraction = false;
    bool disp_resistance = false;
    bool recalculate = false;

    while ((c = getopt(argc, argv, ":otpvfr")) != -1) {
    switch(c) {
        case 't':
            disp_temperature = true;
            break;
        case  'p':
            disp_pressure = true;
            break;
        case 'v':
            disp_voltage = true;
            break;
        case 'f':
            disp_fraction = true;
            break;
        case 'r':
            recalculate = true;
            break;
        case 'o':
            disp_resistance = true;
            break;
        case '?':
            printf("unknown arg %c\n", optopt);
            break;
        }
    }
    //Default no options setting
    if(!disp_temperature && !disp_pressure && !disp_fraction && !disp_resistance)
        disp_temperature = true;

    FILE* f = fopen(file_name, "r");
    if(f == NULL)
        printf("Could not open file %s.\n", file_name);

    //ignore first line
    char mark = getc(f);
    while(mark != '\n') {
        mark = getc(f);
    }

    int samples_len = 10;
    struct sample* samples;
    samples = (struct sample*)malloc(sizeof(struct sample)*samples_len);

    //read data
    int n = 0; //number of datums so far in sample array
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
        struct sample s;
        fscanf(f,
            "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
            s.timestamp,
            s.rtd, s.rtd+1, s.rtd+2, s.rtd+3,
            &s.pressure,
            s.rtd_v, s.rtd_v+1, s.rtd_v+2, s.rtd_v+3,
            &s.pressure_v);
        for(int i=0; i<NUM_RTDS; ++i)
            s.rtd_r[i] = resistance(s.rtd_v[i], i);
        samples[n] = s;
        ++n;
    }
    fclose(f);

    if(recalculate)
    {
        for(int i=0; i<n; ++i)
        {
            for(int j=0; j<NUM_RTDS; ++j)
            {
                samples[i].rtd[j] = temperature(samples[i].rtd_v[j], j);
            }
            samples[i].pressure = pressure(samples[i].pressure_v);
        }
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
    if(disp_temperature || disp_resistance)
    {
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
                if(disp_voltage)
                    fprintf(f, "%f", samples[j].rtd_v[i]);
                else if(disp_resistance)
                    fprintf(f, "%f", samples[j].rtd_r[i]);
                else
                    fprintf(f, "%f", samples[j].rtd[i]);
                if(j!=n-1)
                    fprintf(f, ",");
            }
            fprintf(f, "};\n");
            fprintf(f, "TGraph *gr%d = new TGraph(n,x%d,y%d);\n", i, i, i);
            fprintf(f, "gr%d->SetMarkerColor(%d);\n", i, TEMP_MARKER_COLORS[i]);
            fprintf(f, "gr%d->SetMarkerStyle(%d);\n", i, TEMP_MARKER_STYLES[i]);
            fprintf(f, "mg->Add(gr%d);\n", i);
        }
    }
    if(disp_fraction)
    {
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
                fprintf(f, "%f", samples[j].pressure / samples[j].rtd[i]);
                if(j!=n-1)
                    fprintf(f, ",");
            }
            fprintf(f, "};\n");
            fprintf(f, "TGraph *gr%d = new TGraph(n,x%d,y%d);\n", i, i, i);
            fprintf(f, "gr%d->SetMarkerColor(%d);\n", i, TEMP_MARKER_COLORS[i]);
            fprintf(f, "gr%d->SetMarkerStyle(%d);\n", i, TEMP_MARKER_STYLES[i]);
            fprintf(f, "mg->Add(gr%d);\n", i);
        }
    }
    if(disp_pressure)
    {
        fprintf(f, "Double_t xp[] = {");
        for(int j=0; j<n; ++j)
        {
            //Make this better once you implement timestamps
            fprintf(f, "%d", j);
            if(j!=n-1)
                fprintf(f, ",");
        }
        fprintf(f, "};\n");

        fprintf(f, "Double_t yp[] = {");
        for(int j=0; j<n; ++j)
        {
            if(disp_voltage)
                fprintf(f, "%f", samples[j].pressure_v);
            else
                fprintf(f, "%f", samples[j].pressure);
            if(j!=n-1)
                fprintf(f, ",");
        }
        fprintf(f, "};\n");
        fprintf(f, "TGraph *grp = new TGraph(n,xp,yp);\n");
        fprintf(f, "grp->SetMarkerColor(%d);\n", 6);
        fprintf(f, "grp->SetMarkerStyle(%d);\n", 20);
        fprintf(f, "mg->Add(grp);\n");
    }

    fprintf(f, END);
    fclose(f);

    char root_command[30] = "root -l ";
    strcat(root_command, ROOT_FILE_NAME);
    strcat(root_command, " &> /dev/null");
    //system(root_command);
   
    //signal(SIGINT, int_handler);
    pID = fork();
    if(pID==0)
    {
        printf("Initializing root. This may take a moment.\n");
        system(root_command);
        printf("Root terminated.\n");
        exit(0);
    }
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
