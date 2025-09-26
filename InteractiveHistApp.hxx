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
    std::vector<double> fData_x, fData_y, fData_z;


    const HistAxis_t fXax, fYax, fZax; 
    
public: 

    InteractiveHistApp( const TGWindow* p, UInt_t w, UInt_t h,
                        ROOT::RDF::RNode df, 
                        const HistAxis_t& x_ax, 
                        const HistAxis_t& y_ax, 
                        const HistAxis_t& z_ax );

    ~InteractiveHistApp(); 

    //slider has been updated
    void SliderMoved(); 

    //close window and terminate application
    void CloseWindow(); 

    ClassDef(InteractiveHistApp,1); 
};

#endif