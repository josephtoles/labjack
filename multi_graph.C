void multi_graph(){
    gStyle->SetOptFit();
    TCanvas *c1 = new TCanvas("c1","multigraph",700,500);
    c1->SetGrid();

        // draw a frame to define the range
    TMultiGraph *mg = new TMultiGraph();

        // create first graph
    const Int_t n1 = 10;
    Double_t x1[]  = {-0.1, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95};
    Double_t y1[]  = {-1,2.9,5.6,7.4,9,9.6,8.7,6.3,4.5,1};
    TGraph *gr1 = new TGraph(n1,x1,y1);
    gr1->SetMarkerColor(kBlue);
    gr1->SetMarkerStyle(21);
    gr1->Fit("pol6","q");
    mg->Add(gr1);

        // create second graph
    const Int_t n2 = 10;
    Float_t x2[]  = {-0.28, 0.005, 0.19, 0.29, 0.45, 0.56,0.65,0.80,0.90,1.01};
    Float_t y2[]  = {2.1,3.86,7,9,10,10.55,9.64,7.26,5.42,2};
    TGraph *gr2 = new TGraph(n2,x2,y2);
    gr2->SetMarkerColor(kRed);
    gr2->SetMarkerStyle(20);
    gr2->Fit("pol5","q");
    
    mg->Add(gr2);
    
    mg->Draw("ap");
    
      //force drawing of canvas to generate the fit TPaveStats
    nt create_temp_script(double x[], double y[], int n, int update_delay);
    int create_pres_script(double x[], double y[], int n, int update_delay);
    void INThandler(int);
    void cleanup();
    c1->Update();
    return c1;
}
