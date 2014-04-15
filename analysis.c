#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

    while ((c = getopt(argc, argv, ":tpv")) != -1) {
    switch(c) {
        case 't':
            disp_temperature = true;
            break;
        case  'p':
            disp_pressure = true;
            break;
        case 'v':
            disp_voltage = true;
        case '?':
            printf("unknown arg %c\n", optopt);
            break;
        }
    }
    if(!disp_temperature && !disp_pressure && !disp_voltage)
        disp_temperature = true;

    FILE* f = fopen(file_name, "r");
    if(f == NULL)
        printf("Could not open file %s.\n", file_name);

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

    f = fopen(ROOT_FILE_NAME, "w");
    if(f==NULL)
    {
        printf("Error opening file to write \"%s\"\n", ROOT_FILE_NAME);
        return 1;
    }

    fprintf(f, BEGINNING);

    //input arrays here
    fprintf(f, "const Int_t n = %d;\n", n);
    if(disp_temperature && ! disp_voltage)
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
                //Make this better once you implement timestamps
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
    else if(disp_pressure && ! disp_voltage)
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
            //Make this better once you implement timestamps
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

    char exit_command[30] = "root -l ";
    strcat(exit_command, ROOT_FILE_NAME);
    system(exit_command);
 
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
