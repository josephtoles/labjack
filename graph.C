#include<unistd.h>
void graph() {
TCanvas *c1 = new TCanvas("c1", "Graph title", 10, 40, 700, 450);
c1->SetGrid();
TMultiGraph *mg = new TMultiGraph();
const Int_t n = 11;
Double_t xp[] = {0,1,2,3,4,5,6,7,8,9,10};
Double_t yp[] = {0.612735,0.613929,0.612735,0.612735,0.613332,0.613929,0.613929,0.613929,0.613332,0.613332,0.613332};
TGraph *grp = new TGraph(n,xp,yp);
grp->SetMarkerColor(6);
grp->SetMarkerStyle(20);
mg->Add(grp);
mg->Draw("ALP");
c1->Update();
}
