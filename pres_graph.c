#include "pres_graph.h"

const char* PRES_ROOT_FILE_NAME = "pres_graph.C";
int pID;

//private functions
int create_pres_script(double x[], double y[], int n, int update_delay);
void pres_int_handler(int);
void pres_cleanup();

//The graph will be displayed for a time equal to the update_delay
int flash_pres_animation(double x[], double y[], int n, int update_delay)
{
    signal(SIGINT, pres_int_handler);
    int pID = fork();
    if(pID==0)
    {
        create_pres_script(x,y,n, update_delay);
        exit(0);
    }
    return 0;
}

//Constant strings for writing root file
const char PRES_BEGINNING[] = 
    "#include<unistd.h>\n"
    "void pres_graph() {\n"
    "TCanvas *c1 = new TCanvas(\"c1\", \"Pressure\", 10, 530, 700, 450);\n"
    //"c1->SetFillColor(42);\n"
    "c1->SetGrid();\n";
    //root file arrays go here
const char PRES_MIDDLE[] =
    "gr = new TGraph(n, x, y);\n"
    //"gr->SetLineColor(2);\n"
    //"gr->SetLineWidth(4);\n"
    "gr->SetMarkerColor(1);\n"
    "gr->SetMarkerStyle(20);\n"
    "gr->SetTitle(\"Graph of pressure\");\n"
    "gr->GetXaxis()->SetTitle(\"Time\");\n"
    "gr->GetYaxis()->SetTitle(\"Pressure\");\n"
    "gr->Draw(\"ALP\");\n"
    "c1->Update();\n" //this command draws the frame, after which one can change it
    "c1->Modified();\n";
    //root file sleep command goes here
const char PRES_END[] = 
    "exit();\n"
    "}\n";

int create_pres_script(double x[], double y[], int n, int update_delay)
{
    FILE *f = fopen(PRES_ROOT_FILE_NAME, "w");
    if (f == NULL)
    {
            printf("Error opening file!\n");
            return 1;
    }
    fprintf(f, PRES_BEGINNING);
    fprintf(f, "const Int_t n = %d;\n", n);
    fprintf(f, "Double_t x[%d], y[%d];\n", n, n);
    int i=0;
    while(i<n){
        fprintf(f, "x[%d] = %f;\n", i, x[i]);
        fprintf(f, "y[%d] = %f;\n", i, y[i]); 
        ++i;};
    fprintf(f, PRES_MIDDLE);
    fprintf(f, "sleep(%d);\n", update_delay); //A "/2" here sort of helps.
    fprintf(f, PRES_END);
    fclose(f);

    char com[100] = "root -l ";
    strcat(com, PRES_ROOT_FILE_NAME);
    strcat(com, " &> /dev/null");
    system(com);
    pres_cleanup();
    return 0;
}

void pres_int_handler(int sig)
{
    signal(sig, SIG_IGN);
    if(sig == 2)
        kill(pID, 1);
    pres_cleanup();
    exit(0);
}

void pres_cleanup()
{
    unlink(PRES_ROOT_FILE_NAME);
}
