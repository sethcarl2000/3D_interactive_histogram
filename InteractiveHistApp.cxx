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
InteractiveHistApp::InteractiveHistApp( const TGWindow* p, 
                                        UInt_t w, 
                                        UInt_t h,
                                        ROOT::RDF::RNode df, 
                                        const HistAxis_t& x_ax, 
                                        const HistAxis_t& y_ax, 
                                        const HistAxis_t& z_ax )
 : TGMainFrame( p, w, h ), 
    fXax{x_ax}, fYax{y_ax}, fZax{z_ax}
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
    gStyle->SetOptStat(0); 
    
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
    
    const double z_center   = 

    printf("new slider position: %.4f", slider_val/slider_amplitude); 
    cout << flush; 
}   

