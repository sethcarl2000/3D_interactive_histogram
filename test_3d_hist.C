//TGMainFrame windows
#include <ROOT/RDataFrame.hxx>
#include <TRandom3.h> 
#include <TGClient.h> 
#include <iostream> 
#include "InteractiveHistApp.hxx"

using namespace std; 

int test_3d_hist()
{
    ROOT::EnableImplicitMT(); 
    ROOT::RDataFrame df(1e5);

    TRandom3 rand; 

    auto df_test = df
        .Define("x", [&rand](){ return rand.Gaus(); }, {}) 
        .Define("y", [&rand](){ return rand.Gaus(); }, {}) 
        .Define("z", [&rand](){ return rand.Gaus(); }, {}); 

    cout << "making interactive app..." << endl; 

    auto hist = df_test.Histo2D<double>({"h", "test",  100, -4, 4,  100, -4, 4}, "x", "y"); 
    
    new InteractiveHistApp(gClient->GetRoot(), 800, 800, df_test, {"x", 125, -4., +4.}, {"y", 125, -4., +4.}, {"z", 125, -4., +4.}); 

    return 0; 
}
