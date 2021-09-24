#include "/nashome/s/sgwon/edep-sim/io/TG4Event.h"

TVector3 GetMaxPositionX(const TG4Event& inputEvent);
TVector3 GetMinPositionX(const TG4Event& inputEvent);

void checkVertex(std::string inputFileName) 
{
    TFile* file = new TFile(inputFileName.c_str(),"READ");
    if (!file->IsOpen()) {
        std::cout << inputFileName << " is not exist" << std::endl;
        exit(0);
    }
    TTree* tree = (TTree*)file->Get("EDepSimEvents");
    TG4Event* event = NULL; tree->SetBranchAddress("Event",&event);

    tree->GetEntry(0);
    TVector3 max = GetMaxPositionX(*event);
    TVector3 min = GetMinPositionX(*event);

    std::cout << "max: " << max.X() << ", " << max.Y() << ", " << max.Z() << std::endl;
    std::cout << "min: " << min.X() << ", " << min.Y() << ", " << min.Z() << std::endl;

    TH2D* xy = new TH2D("xy", "hit xy", 100, min.X(), max.X(), 100, min.Y(), max.Y());
    TH2D* xz = new TH2D("xy", "hit xz", 100, min.X(), max.X(), 100, min.Z(), max.Z());
    TH2D* yz = new TH2D("xy", "hit yz", 100, min.Y(), max.Y(), 100, min.Z(), max.Z());

    for (int i = 0; i < tree->GetEntries(); ++i) {
        std::cout << "event#" << i << std::endl;
        tree->GetEntry(i);
        for (auto sd : event->SegmentDetectors) {
            for (Int_t j = 0; j < (int)sd.second.size(); j++) {
                //std::cout << "x: " << sd.second[j].GetStart().X() << std::endl;
                xy->Fill(sd.second[j].GetStart().X(), sd.second[j].GetStart().X());
                yz->Fill(sd.second[j].GetStart().Y(), sd.second[j].GetStart().Z());
                xz->Fill(sd.second[j].GetStart().X(), sd.second[j].GetStart().Z());
            }
        }
    }

    TCanvas* can = new TCanvas;
    can->Divide(2,2);
    can->cd(1);
    xy->Draw("colz");
    can->cd(2);
    yz->Draw("colz");
    can->cd(3);
    xz->Draw("colz");
}

TVector3 GetMaxPositionX(const TG4Event& inputEvent) {
    TVector3 tempMaxVector;
    for (auto sd : inputEvent.SegmentDetectors) {
        for (Int_t i = 0; i < (int)sd.second.size(); i++) {
            if (tempMaxVector.X() < sd.second[i].GetStart().X()) {
                tempMaxVector.SetX(sd.second[i].GetStart().X());
            }
            if (tempMaxVector.Y() < sd.second[i].GetStart().Y()) {
                tempMaxVector.SetY(sd.second[i].GetStart().Y());
            }
            if (tempMaxVector.Z() < sd.second[i].GetStart().Z()) {
                tempMaxVector.SetZ(sd.second[i].GetStart().Z());
            }
        }
    }

    return tempMaxVector;
}

TVector3 GetMinPositionX(const TG4Event& inputEvent) {
    TVector3 tempMinVector;
    for (auto sd : inputEvent.SegmentDetectors) {
        for (Int_t i = 0; i < (int)sd.second.size(); i++) {
            if (tempMinVector.X() > sd.second[i].GetStart().X()) {
                tempMinVector.SetX(sd.second[i].GetStart().X());
            }
            if (tempMinVector.Y() > sd.second[i].GetStart().Y()) {
                tempMinVector.SetY(sd.second[i].GetStart().Y());
            }
            if (tempMinVector.Z() > sd.second[i].GetStart().Z()) {
                tempMinVector.SetZ(sd.second[i].GetStart().Z());
            }
        }
    }

    return tempMinVector;
}
