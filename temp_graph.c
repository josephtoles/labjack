#include "temp_graph.h"

const char* TEMP_ROOT_FILE_NAME = "temp_graph.C";
int pID;

//private functions
int create_temp_script(double x[], double* y[4], int n, int update_delay);
void temp_int_handler(int);
void temp_cleanup();
void add_graph(FILE* f, int index, double x[], double y[], int n);

//The graph will be displayed for a time equal to the update_delay
int flash_temp_animation(double x[], double* y[4], int n, int update_delay)
{
    signal(SIGINT, temp_int_handler);
    int pID = fork();
    if(pID==0)
    {
        create_temp_script(x,y,n, update_delay); //modify to allow multiple arrays
        exit(0);
    }
    return 0;
}

//Constant strings for writing root file
const char TEMP_BEGINNING[] = 
    "#include<unistd.h>\n"
    "void temp_graph() {\n"
    "TCanvas *c1 = new TCanvas(\"c1\", \"Temperature\", 10, 40, 700, 450);\n"
    //"c1->SetFillColor(42);\n"
    "c1->SetGrid();\n"
    "TMultiGraph *mg = new TMultiGraph();\n";

    /* root file graphs are inserted here */
   

const char TEMP_MIDDLE[] = 
    "mg->Draw(\"ap\");\n"
    "c1->Update();\n";

    /* root file sleep command goes here */

const char TEMP_END[] = 
    "exit();\n"
    "}\n";

//tracer only
const char GRAPHS[] = 
"const Int_t n1 = 10;\n"
"Double_t x1[]  = {-0.1, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95};\n"
"Double_t y1[]  = {-1,2.9,5.6,7.4,9,9.6,8.7,6.3,4.5,1};\n"
"TGraph *gr1 = new TGraph(n1,x1,y1);\n"
"gr1->SetMarkerColor(kBlue);\n"
"gr1->SetMarkerStyle(21);\n"
"gr1->Fit(\"pol6\",\"q\");\n"
"mg->Add(gr1);\n"
""
"const Int_t n2 = 10;"
"Float_t x2[]  = {-0.28, 0.005, 0.19, 0.29, 0.45, 0.56,0.65,0.80,0.90,1.01};"
"Float_t y2[]  = {2.1,3.86,7,9,10,10.55,9.64,7.26,5.42,2};"
"TGraph *gr2 = new TGraph(n2,x2,y2);"
"gr2->SetMarkerColor(kRed);"
"gr2->SetMarkerStyle(20);"
"gr2->Fit(\"pol5\",\"q\");"
"mg->Add(gr2);\n";

void add_graph(FILE* f, int index, double x[], double y[], int n)
{
    fprintf(f, "const Int_t n%d = %d;\n", index, n);
    fprintf(f, "Double_t x%d[] = {", index);
    for(int i=0; i<n; ++i)
    {
        fprintf(f, "%f", x[i]);
        if(i != n-1)
            fprintf(f, ",");
    }
    fprintf(f, "};\n");
    fprintf(f, "Double_t y%d[] = {", index);
    for(int i=0; i<n; ++i)
    {
        fprintf(f, "%f", y[i]);
        if(i != n-1)
            fprintf(f, ",");
    }
    fprintf(f, "};\n");
    fprintf(f, "TGraph *gr%d = new TGraph(n%d,x%d,y%d);\n", index, index, index, index);
    fprintf(f, "gr%d->SetMarkerColor(kBlue);\n", index);
    fprintf(f, "gr%d->SetMarkerStyle(21);\n", index);
    fprintf(f, "gr%d->Fit(\"pol6\",\"q\");\n", index);
    fprintf(f, "mg->Add(gr%d);\n", index);

}


int create_temp_script(double x[], double* y[4], int n, int update_delay)
{
    //open file
    FILE *f = fopen(TEMP_ROOT_FILE_NAME, "w");
    if (f == NULL)
    {
            printf("Error opening file!\n");
            return 1;
    }

    //beginning
    fprintf(f, TEMP_BEGINNING);

    //graphs
    for(int i=0; i<4; ++i)
        add_graph(f, i, x, y[i], n);

    //middle
    fprintf(f, TEMP_MIDDLE);

    //sleep
    fprintf(f, "sleep(%d);\n", update_delay);

    //end
    fprintf(f, TEMP_END);
    fclose(f);

    //execute
    char com[100] = "root -l ";
    strcat(com, TEMP_ROOT_FILE_NAME);
    strcat(com, " &> /dev/null");
    system(com);
    temp_cleanup();
    return 0;
}

void temp_int_handler(int sig)
{
    signal(sig, SIG_IGN);
    temp_cleanup();
    kill(pID, 1);
    exit(0);
}

void temp_cleanup()
{
    //uncomment following line after testing
    //unlink(TEMP_ROOT_FILE_NAME);
}
