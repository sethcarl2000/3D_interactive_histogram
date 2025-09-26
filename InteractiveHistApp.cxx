#include <InteractiveHistApp.hxx> 
#include <TGClient.h> 
#include <TGLayout.h>
#include <TStyle.h>  
#include <cstdio> 
#include <iostream> 
#include <TApplication.h>

using namespace std; 

InteractiveHistApp::~InteractiveHistApp()
{
    Cleanup(); 
}

void InteractiveHistApp::CloseWindow()
{
    gApplication->Terminate(0); 
}

//_______________________________________________________________________________________________________
InteractiveHistApp::InteractiveHistApp( ROOT::RDF::RNode df, 
                                        const HistAxis_t& x_ax, 
                                        const HistAxis_t& y_ax, 
                                        const HistAxis_t& z_ax, 
                                        UInt_t w, 
                                        UInt_t h,
                                        UInt_t palette, 
                                        const TGWindow* p
                                        )
 : TGMainFrame( (p==nullptr) ? gClient->GetRoot() : p, w, h ), 
    fXax{x_ax}, fYax{y_ax}, fZax{z_ax}, fPalette{palette}
{
    const char* const here = "InteractiveHistApp::InteractiveHistApp"; 

    //create the main, vertical frame
    fFrame_main = new TGVerticalFrame(this, w, h); 

    //create the frame for the primary (2D) canvas
    fFrame_h2d  = new TGVerticalFrame(fFrame_main, w, 0.8*h); 
    
    //add the embedded canvas
    fECanv_h2d  = new TRootEmbeddedCanvas("canv_h2d", fFrame_h2d, w, 0.8*h); 

    //create the new, empty histogram 
    f_h2d = new TH2D("h_xy", Form(";%s;%s", fXax.name.c_str(), fYax.name.c_str()),
        fXax.nbins, fXax.x0, fXax.x1, 
        fYax.nbins, fYax.x0, fYax.x1
    ); 
    //disable stats, set the drawing color palette
    gStyle->SetOptStat(0); 
    gStyle->SetPalette(fPalette);

    fECanv_h2d->GetCanvas()->cd(); 
    f_h2d->Draw("col"); 

    fFrame_h2d->AddFrame(fECanv_h2d, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));

    fFrame_main->AddFrame(fFrame_h2d, new TGLayoutHints(kLHintsBottom | kLHintsCenterX | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5)); 


    //create the frame for the secondary 
    fFrame_hcut = new TGVerticalFrame(fFrame_main, w, 0.25*h); 
    
    fCutSlider = new TGHSlider(fFrame_hcut, w); 
    fCutSlider->Connect("Released()", "InteractiveHistApp", this, "SliderMoved()"); 
    fFrame_hcut->AddFrame(fCutSlider, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 0, 0, 0, 5));

    fECanv_hcut = new TRootEmbeddedCanvas("canv_hcut", fFrame_hcut, w, 0.2*h); 
    
    f_h1d = new TH1D("h_z", Form(";%s;", fZax.name.c_str()), 
        fZax.nbins, fZax.x0, fZax.x1
    );
    gStyle->SetOptStat(0); 

    fECanv_hcut->GetCanvas()->cd(); 
    f_h1d->Draw(); 

    fFrame_hcut->AddFrame(fECanv_hcut, new TGLayoutHints(kLHintsBottom | kLHintsExpandX, 0, 0, 5, 0)); 

    fFrame_main->AddFrame(fFrame_hcut, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 5, 5, 5, 5)); 
    //*/

    AddFrame(fFrame_main, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0)); 

    //fill the data vectors
    FillVectors(df);   

    //draw the 'z' histogram
    for (const auto& dat : fData) f_h1d->Fill(dat[2]); 
    fECanv_hcut->GetCanvas()->cd(); 
    f_h1d->Draw(); 
    fECanv_hcut->GetCanvas()->Modified(); 
    fECanv_hcut->GetCanvas()->Update(); 

    //draw the histograms
    SliderMoved(); 

    SetWindowName("2d histogram");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

//_______________________________________________________________________________________________________
void InteractiveHistApp::SliderMoved()
{   
    //slider has been moved. lets update the hist. 

    double slider_amplitude = (double)(fCutSlider->GetMaxPosition() - fCutSlider->GetMinPosition());
    double slider_val       = (double)(fCutSlider->GetPosition()    - fCutSlider->GetMinPosition());
    
    double index = slider_val/slider_amplitude; 

    const double z_center = fZax.x0 + (fZax.x1 - fZax.x0)*index; 

    const double cut_fraction = 0.10; 
    double cut_size = (fZax.x1 - fZax.x0)*(cut_fraction/2.); 

    //now, redraw the histograms and the box 
    if (fCutBox != nullptr) { delete fCutBox; }
    fCutBox = new TBox(
        z_center - cut_size, 0., 
        z_center + cut_size, f_h1d->GetMaximum()*1.05
    ); 
    fCutBox->SetLineStyle(1); 
    fCutBox->SetLineColor(kRed); 
    fCutBox->SetFillStyle(3004); 
    fCutBox->SetFillColor(kRed); 
    
    auto c_cut = fECanv_hcut->GetCanvas(); 
    c_cut->cd(); 
    fCutBox->Draw(); 
    c_cut->Modified(); 
    c_cut->Update(); 

    //now, redraw the histograms
    f_h2d->Reset(); 

    for (const auto& point : fData) {
        if (fabs(point[2]-z_center) < cut_size) { f_h2d->Fill( point[0], point[1] ); } 
    }
    auto c_h2d = fECanv_h2d->GetCanvas(); 
    c_h2d->cd();
    f_h2d->Draw("col2"); 
    c_h2d->Modified();
    c_h2d->Update(); 
}   

//_______________________________________________________________________________________________________
void InteractiveHistApp::FillVectors(ROOT::RDF::RNode df)
{
    //fill the vectors with the data. 
    const size_t n_events = *df.Count(); 

    cout << "making vectors for " << n_events << "events..." << flush;

    fData = *df  
        .Define("data_array", [](double x, double y, double z){ 
            return std::array<double,3>{x, y, z};
        }, {
            fXax.name.c_str(),
            fYax.name.c_str(),
            fZax.name.c_str()
        })
        
        .Take<std::array<double,3>>("data_array");

    cout << "done." << endl; 
}
