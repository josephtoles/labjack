#include<unistd.h>
void graph() {
TCanvas *c1 = new TCanvas("c1", "Graph title", 10, 40, 700, 450);
c1->SetGrid();
TMultiGraph *mg = new TMultiGraph();
const Int_t n = 11;
mg->Draw("ALP");
c1->Update();
}
