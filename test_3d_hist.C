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
    ROOT::RDataFrame df(1e6);

    TRandom3 rand; 

    auto df_test = df
        .Define("z", [&rand](){ return rand.Gaus(); }, {}) 
        .Define("x", [&rand](double z){ return z; }, {"z"})   //add some fun little correlations with 'z' 
        .Define("y", [&rand](double z){ return z; }, {"z"}); 
        
    cout << "making interactive app..." << endl; 

    new InteractiveHistApp(df_test, {"x", 125, -4., +4.}, {"y", 125, -4., +4.}, {"z", 125, -4., +4.}, 900, 750, kSunset); 

    return 0; 
}
