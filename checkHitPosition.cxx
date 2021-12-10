#include "/nashome/s/sgwon/edep-sim/io/TG4Event.h"

TG4Event* event = NULL; 

TVector3 GetMaxPositionX(TTree& inputTree);
TVector3 GetMinPositionX(TTree& inputTree);
std::pair<TVector3, TVector3> GetMaxMinPositionX(TTree& inputTree);
//------------------------------------------------------------------------------
void checkVertex(std::string inputFileName) {
    TFile* file = new TFile(inputFileName.c_str(),"READ");
    if (!file->IsOpen()) {
        std::cout << inputFileName << " is not exist" << std::endl;
        exit(0);
    }
    TTree* tree = (TTree*)file->Get("EDepSimEvents");
    tree->SetBranchAddress("Event",&event);

    std::pair<TVector3, TVector3> max_min = GetMaxMinPositionX(*tree);
    //TVector3 max = GetMaxPositionX(*tree);
    //TVector3 min = GetMinPositionX(*tree);
    TVector3 max = max_min.first;
    TVector3 min = max_min.second;

    std::cout << "max: " << max.X() << ", " << max.Y() << ", " << max.Z() << std::endl;
    std::cout << "min: " << min.X() << ", " << min.Y() << ", " << min.Z() << std::endl;

    TH2D* xy = new TH2D("xy", "hit xy;x;y", 100, 0,0, 100, 0,0);
    TH2D* xz = new TH2D("xz", "hit xz;x;z", 100, 0,0, 100, 0,0);
    TH2D* yz = new TH2D("yz", "hit yz;y;z", 100, 0,0, 100, 0,0);

    for (int i = 0; i < tree->GetEntries(); ++i) {
        std::cout << "event#" << i << std::endl;
        tree->GetEntry(i);
        for (auto sd : event->SegmentDetectors) {
            for (Int_t j = 0; j < (int)sd.second.size(); j++) {
                //std::cout << "x: " << sd.second[j].GetStart().X() << std::endl;
                xy->Fill(sd.second[j].GetStart().X(), sd.second[j].GetStart().Y());
                yz->Fill(sd.second[j].GetStart().Y(), sd.second[j].GetStart().Z());
                xz->Fill(sd.second[j].GetStart().X(), sd.second[j].GetStart().Z());
            }
        }
    }

    TCanvas can;
    can.Divide(2,2);
    can.cd(1);
    xy->Draw("colz");
    can.cd(2);
    yz->Draw("colz");
    can.cd(3);
    xz->Draw("colz");
    can.SaveAs("hitPosition.pdf");
    exit(0);
}
//------------------------------------------------------------------------------
TVector3 GetMaxPositionX(TTree& inputTree) {
    TVector3 tempMaxVector;
    for (int i = 0; i < inputTree.GetEntries(); ++i) {
        std::cout << "event#" << i << std::endl;
        inputTree.GetEntry(i);
        for (auto sd : event->SegmentDetectors) {
            for (Int_t j = 0; j < (int)sd.second.size(); j++) {
                if (tempMaxVector.X() < sd.second[j].GetStart().X()) {
                    tempMaxVector.SetX(sd.second[j].GetStart().X());
                }
                if (tempMaxVector.Y() < sd.second[j].GetStart().Y()) {
                    tempMaxVector.SetY(sd.second[j].GetStart().Y());
                }
                if (tempMaxVector.Z() < sd.second[j].GetStart().Z()) {
                    tempMaxVector.SetZ(sd.second[j].GetStart().Z());
                }
            }
        }
    }

    return tempMaxVector;
}
//------------------------------------------------------------------------------
TVector3 GetMinPositionX(TTree& inputTree) {
    TVector3 tempMinVector;
    for (int i = 0; i < inputTree.GetEntries(); ++i) {
        std::cout << "event#" << i << std::endl;
        inputTree.GetEntry(i);
        for (auto sd : event->SegmentDetectors) {
            for (Int_t j = 0; j < (int)sd.second.size(); j++) {
                if (tempMinVector.X() > sd.second[j].GetStart().X()) {
                    tempMinVector.SetX(sd.second[j].GetStart().X());
                }
                if (tempMinVector.Y() > sd.second[j].GetStart().Y()) {
                    tempMinVector.SetY(sd.second[j].GetStart().Y());
                }
                if (tempMinVector.Z() > sd.second[j].GetStart().Z()) {
                    tempMinVector.SetZ(sd.second[j].GetStart().Z());
                }
            }
        }
    }

    return tempMinVector;
}
//------------------------------------------------------------------------------
std::pair<TVector3, TVector3> GetMaxMinPositionX(TTree& inputTree) {
    TVector3 tempMaxVector;
    TVector3 tempMinVector;

    for (int i = 0; i < inputTree.GetEntries(); ++i) {
        std::cout << "event#" << i << std::endl;
        inputTree.GetEntry(i);
        for (auto sd : event->SegmentDetectors) {
            for (Int_t j = 0; j < (int)sd.second.size(); j++) {
                if (tempMaxVector.X() < sd.second[j].GetStart().X()) {
                    tempMaxVector.SetX(sd.second[j].GetStart().X());
                }
                if (tempMaxVector.Y() < sd.second[j].GetStart().Y()) {
                    tempMaxVector.SetY(sd.second[j].GetStart().Y());
                }
                if (tempMaxVector.Z() < sd.second[j].GetStart().Z()) {
                    tempMaxVector.SetZ(sd.second[j].GetStart().Z());
                }
                if (tempMinVector.X() > sd.second[j].GetStart().X()) {
                    tempMinVector.SetX(sd.second[j].GetStart().X());
                }
                if (tempMinVector.Y() > sd.second[j].GetStart().Y()) {
                    tempMinVector.SetY(sd.second[j].GetStart().Y());
                }
                if (tempMinVector.Z() > sd.second[j].GetStart().Z()) {
                    tempMinVector.SetZ(sd.second[j].GetStart().Z());
                }
            }
        }
    }

    std::pair<TVector3, TVector3> tempPair;
    tempPair.first = tempMaxVector;
    tempPair.second = tempMinVector;

    return tempPair;
}
