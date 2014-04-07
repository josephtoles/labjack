#include "temp_graph.h"

const char* TEMP_ROOT_FILE_NAME = "temp_graph.C";
int pID;

//private functions
int create_temp_script(double x[], double y[], int n, int update_delay);
void temp_int_handler(int);
void temp_cleanup();

//The graph will be displayed for a time equal to the update_delay
int flash_temp_animation(double x[], double y[], int n, int update_delay)
{
    signal(SIGINT, temp_int_handler);
    int pID = fork();
    if(pID==0)
    {
        create_temp_script(x,y,n, update_delay);
        exit(0);
    }
    return 0;
}

//Constant strings for writing root file
const char TEMP_BEGINNING[] = 
    "#include<unistd.h>\n"
    "void temp_graph() {\n"
    "TCanvas *c1 = new TCanvas(\"c1\", \"Temperature\", 10, 10, 700, 500);\n"
    "c1->SetFillColor(42);\n"
    "c1->SetGrid();\n";
    //root file arrays go here
const char TEMP_MIDDLE[] =
    "gr = new TGraph(n, x, y);\n"
    "gr->SetLineColor(2);\n"
    "gr->SetLineWidth(4);\n"
    "gr->SetMarkerColor(4);\n"
    "gr->SetMarkerStyle(21);\n"
    "gr->SetTitle(\"Graph of temperature\");\n"
    "gr->GetXaxis()->SetTitle(\"Time\");\n"
    "gr->GetYaxis()->SetTitle(\"Temperature\");\n"
    "gr->Draw(\"ACP\");\n"
    "c1->Update();\n" //this command draws the frame, after which one can change it
    "c1->GetFrame()->SetFillColor(21);\n"
    "c1->GetFrame()->SetBorderSize(12);\n"
    "c1->Modified();\n";
    //root file sleep command goes here
const char TEMP_END[] = 
    "exit();\n"
    "}\n";

int create_temp_script(double x[], double y[], int n, int update_delay)
{
    FILE *f = fopen(TEMP_ROOT_FILE_NAME, "w");
    if (f == NULL)
    {
            printf("Error opening file!\n");
            return 1;
    }
    fprintf(f, TEMP_BEGINNING);
    fprintf(f, "const Int_t n = %d;\n", n);
    fprintf(f, "Double_t x[%d], y[%d];\n", n, n);
    int i=0;
    while(i<n){
        fprintf(f, "x[%d] = %f;\n", i, x[i]);
        fprintf(f, "y[%d] = %f;\n", i, y[i]); 
        ++i;};
    fprintf(f, TEMP_MIDDLE);
    fprintf(f, "sleep(%d);\n", update_delay);
    fprintf(f, TEMP_END);
    fclose(f);

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
    unlink(TEMP_ROOT_FILE_NAME);
}
