#ifndef InteractiveHistApp_HXX
#define InteractiveHistApp_HXX

//TGMainFrame windows
#include <TGWindow.h> 
#include <TGFrame.h> 
#include <TRootEmbeddedCanvas.h>
#include <TGSlider.h> 
#include <TGButton.h>
#include <TGClient.h>  
#include <TCanvas.h> 
#include <TBox.h> 
#include <string> 
#include <TH1D.h> 
#include <TH2D.h> 
#include <ROOT/RDataFrame.hxx> 
#include <vector> 
#include <array> 
#include <TColor.h> 


struct HistAxis_t {
    std::string name; 
    int nbins;
    double x0, x1; 
}; 

class InteractiveHistApp : public TGMainFrame {
private: 
    //frame containing both sub-frames
    TGVerticalFrame     *fFrame_main; 

    //frame of 2d hist, and associated objects
    TGVerticalFrame     *fFrame_h2d; 
    TRootEmbeddedCanvas *fECanv_h2d; 
    TH2D                *f_h2d; 
    
    TGVerticalFrame     *fFrame_hcut; 
    TRootEmbeddedCanvas *fECanv_hcut; 
    TGHSlider           *fCutSlider;
    TH1D                *f_h1d;  
    TBox                *fCutBox{nullptr}; 

    //called at construction time. beware! all data will be filled in vectors (in memory)!. 
    //make sure you have enough ram. 
    void FillVectors(ROOT::RDF::RNode df); 

    //BEWARE! for the sake of speed, all data will be stores in vectors, in memory. 
    std::vector<std::array<double,3>> fData;

    const HistAxis_t fXax, fYax, fZax; 

    const UInt_t fPalette; 
    
public: 

    InteractiveHistApp( ROOT::RDF::RNode df, 
                        const HistAxis_t& x_ax, 
                        const HistAxis_t& y_ax, 
                        const HistAxis_t& z_ax, 
                        UInt_t w=800, 
                        UInt_t h=600, 
                        UInt_t palette=kSunset,
                        const TGWindow* p=nullptr );

    ~InteractiveHistApp(); 

    //slider has been updated
    void SliderMoved(); 

    //close window and terminate application
    void CloseWindow(); 

    ClassDef(InteractiveHistApp,1); 
};

#endif