#include "graph.h"

int update_delay = 3;
const char* ROOT_FILE_NAME = "graph.C";

//The graph will be displayed for a tiem equal to the update_delay
int flash_animation(double x[], double y[], int n, int update_delay)
{
    signal(SIGINT, INThandler);
    pID = fork();
    if(pID==0)
    {
        create_script(x,y,n, update_delay);
        exit(0);
    }
    return 0;
}

const char BEGINNING[] = 
    "#include<unistd.h>\n"
    "void graph() {\n"
    "TCanvas *c1 = new TCanvas(\"c1\", \"Temperature\", 200, 10, 700, 500);\n"
    "c1->SetFillColor(42);\n"
    "c1->SetGrid();\n";
//arrays
const char MIDDLE[] =
    "gr = new TGraph(n, x, y);\n"
    "gr->SetLineColor(2);\n"
    "gr->SetLineWidth(4);\n"
    "gr->SetMarkerColor(4);\n"
    "gr->SetMarkerStyle(21);\n"
    "gr->SetTitle(\"a simple graph\");\n"
    "gr->GetXaxis()->SetTitle(\"Time\");\n"
    "gr->GetYaxis()->SetTitle(\"Temperature\");\n"
    "gr->Draw(\"ACP\");\n"
    // TCanvas::Update() draws the frame, after which one can change it
    "c1->Update();\n"
    "c1->GetFrame()->SetFillColor(21);\n"
    "c1->GetFrame()->SetBorderSize(12);\n"
    "c1->Modified();\n";
//sleep
const char END[] = 
    "exit();\n"
    "}\n";

int create_script(double x[], double y[], int n, int update_delay)
{
    FILE *f = fopen(ROOT_FILE_NAME, "w");
    if (f == NULL)
    {
            printf("Error opening file!\n");
            return 1;
    }
    fprintf(f, BEGINNING);
    fprintf(f, "const Int_t n = %d;\n", n);
    fprintf(f, "Double_t x[%d], y[%d];\n", n, n);
    int i=0;
    while(i<n){
        fprintf(f, "x[%d] = %f;\n", i, x[i]);
        fprintf(f, "y[%d] = %f;\n", i, y[i]); 
        ++i;};
    fprintf(f, MIDDLE);
    fprintf(f, "sleep(%d);\n", update_delay);
    fprintf(f, END);
    fclose(f);

    char com[100] = "root -l ";
    strcat(com, ROOT_FILE_NAME);
    strcat(com, " &> /dev/null");
    system(com);
    cleanup();
    return 0;
}

void INThandler(int sig)
{
    signal(sig, SIG_IGN);
    cleanup();
    printf("premature termination\n");
    printf("death to the child\n");
    kill(pID, 1);
    exit(0);
}

void cleanup()
{
    unlink(ROOT_FILE_NAME);
}
