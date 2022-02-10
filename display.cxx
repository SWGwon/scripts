#include "/Users/gwon/beamtest/neutronselection/IO/Event.cc"
#include "/Users/gwon/beamtest/neutronselection/IO/VoxelManager.cc"
#include "/Users/gwon/beamtest/neutronselection/IO/Hit.cc"
#include "/Users/gwon/beamtest/neutronselection/utils/RecoUtils.cc"
#include "/Users/gwon/beamtest/neutronselection/utils/FindGeometricProperties.cc"
#include "/Users/gwon/beamtest/neutronselection/utils/FindClusters.cc"

//#include "/nashome/s/sgwon/beamtest2020/neutronselection/IO/Event.cc"
//#include "/nashome/s/sgwon/beamtest2020/neutronselection/IO/VoxelManager.cc"
//#include "/nashome/s/sgwon/beamtest2020/neutronselection/IO/Hit.cc"
//#include "/nashome/s/sgwon/beamtest2020/neutronselection/utils/RecoUtils.cc"
//#include "/nashome/s/sgwon/beamtest2020/neutronselection/utils/FindGeometricProperties.cc"
//#include "/nashome/s/sgwon/beamtest2020/neutronselection/utils/FindClusters.cc"

TH2F *hUnifiedZY = new TH2F("unified ZY view", "sfgd ZY view; Z position [cm];Y position [cm]; Hit charge [pe]", 48, 0, 48, 24, -8, 16);
TH2F *hUnifiedZX = new TH2F("unified ZX view", "sfgd ZX view; Z position [cm];X position [cm]; Hit charge [pe]", 48, 0, 48, 24, 0, 24);

double threshold = 20;

void display(TString inputFileName) {
    TFile* inputFile = new TFile(inputFileName, "READ");
    TTree* inputTree = (TTree*)inputFile->Get("AllEvents");
    Event* unpackEvent = nullptr;
    unpackEvent = new Event(); inputTree->SetBranchAddress("Event", &unpackEvent);

    Int_t prevEventTime = -999;
    double prevMaxCharge = -999;
    double prevRange = -999;

    for (int iev = 0; iev < inputTree->GetEntries(); iev++) {
        inputTree->GetEntry(iev);

        if (abs(unpackEvent->GetFEB12hitTfromSpill() - prevEventTime) < 700) continue;
        if (prevMaxCharge == unpackEvent->GetMaxCharge() && prevRange == unpackEvent->GetRange()) continue;
        prevEventTime = unpackEvent->GetFEB12hitTfromSpill();
        prevMaxCharge = unpackEvent->GetMaxCharge();
        prevRange = unpackEvent->GetRange();

        TClonesArray* unpackHits = unpackEvent->GetHits();
        int N = unpackEvent->GetNHits();
        vector<Hit*> sfgdHits = {};
        for (int i = 0; i < N; ++i) {
            Hit* tempHit = (Hit*)unpackEvent->GetHits()->At(i);

            if (tempHit->GetFEB() > 27) continue;

            if (tempHit->GetPE() > threshold) {
                sfgdHits.push_back(tempHit);
            }
        }

        vector<VoxelManager*> sfgdVoxels;
        sfgdVoxels = MakeHitsToVoxels(sfgdHits, 1);
        std::vector<vector<VoxelManager*>> sfgdClusters = FindClusters(sfgdVoxels, 1, 0, 1.8);
        std::vector<vector<VoxelManager*>> sfgdClustersVector;
        for (const auto& c : sfgdClusters) {
            if (c.size() < 3) continue;

            vector<double> geop = FindGeometricProperties(c, 3); //3 linear
            if (geop[0] < 0.9 ) continue;

            vector<double> voxs = FindGeometricProperties(c, 1); //1=fat
            if (voxs[0] > 2) continue;

            sfgdClustersVector.push_back(c);
        }
        if (sfgdClustersVector.size() != 2) continue;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "sfgdClustersVector.size(): " << sfgdClustersVector.size() << std::endl;

        for (auto v:sfgdVoxels) {
            std::cout << "sfgd  voxel: " << v->GetX() << ", " << v->GetY() << ", " << v->GetZ() << std::endl;
            hUnifiedZY->Fill(48 + (-1)*v->GetZ(), v->GetY());
            hUnifiedZX->Fill(48 + (-1)*v->GetZ(), v->GetX());
        }
        //DrawEventDisplay();

        TLine* zy1 = new TLine(48,0, 48,8);
        TLine* zy2 = new TLine(0,0, 48,0);
        TLine* zy3 = new TLine(0,8, 48,8);
        TLine* zy4 = new TLine(48+112,0, 48+112,8);
        TLine* zy5 = new TLine(48+112,0, 48+112+32,0);
        TLine* zy6 = new TLine(48+112,8, 48+112+32,8);
        TArrow* zyArrow1 = new TArrow(0,4, 48+112+32,4, 0.01, "|>");
        zyArrow1->SetLineColor(2);
        zyArrow1->SetFillColor(2);
        TText* zyText1 = new TText(-10, 3.5, "beam");
        TText* zyText2 = new TText(20, 9, "sfgd");
        TText* zyText3 = new TText(48+112+16, 9, "usj");

        TLine* zx1 = new TLine(48,0, 48,24);
        TLine* zx2 = new TLine(48+112,16, 48+112+32,16);
        TLine* zx3 = new TLine(48+112,8, 48+112+32,8);
        TLine* zx4 = new TLine(48+112,8, 48+112,16);
        TArrow* zxArrow1 = new TArrow(0,12, 48+112+32,12, 0.01, "|>");
        zxArrow1->SetLineColor(2);
        zxArrow1->SetFillColor(2);
        TText* zxText1 = new TText(-10, 11.5, "beam");
        TText* zxText2 = new TText(20, 23, "sfgd");
        TText* zxText3 = new TText(48+112+16, 17, "usj");

        TCanvas* unifiedCan = new TCanvas("unifiedCan", "unifiedCan", 960, 960); 
        //unifiedCan->SetBatch(true);
        unifiedCan->Divide(1,2);
        unifiedCan->cd(1);
        hUnifiedZY->SetStats(0);
        hUnifiedZY->Draw("colz");
        zy1->Draw();
        zy2->Draw("same");
        zy3->Draw("same");
        zy4->Draw("same");
        zy5->Draw("same");
        zy6->Draw("same");
        zyArrow1->Draw();
        zyText1->Draw();
        zyText2->Draw("same");
        zyText3->Draw("same");

        unifiedCan->cd(2);
        hUnifiedZX->SetStats(0);
        hUnifiedZX->Draw("colz");
        zx1->Draw();
        zx2->Draw("same");
        zx3->Draw("same");
        zx4->Draw("same");
        zxArrow1->Draw();
        zxText1->Draw();
        zxText2->Draw("same");
        zxText3->Draw("same");

        unifiedCan->Update();
        //unifiedCan->SaveAs(Form("%d.pdf", eventNum));
        unifiedCan->WaitPrimitive();
        hUnifiedZX->Reset();
        hUnifiedZY->Reset();
    }
}

//for MC
void display() {
    TFile* inputFile = new TFile(inputFileName, "READ");
    TTree* inputTree = (TTree*)inputFile->Get("AllEvents");
    Event* unpackEvent = nullptr;
    unpackEvent = new Event(); inputTree->SetBranchAddress("Event", &unpackEvent);

    Int_t prevEventTime = -999;
    double prevMaxCharge = -999;
    double prevRange = -999;

    for (int iev = 0; iev < inputTree->GetEntries(); iev++) {
        inputTree->GetEntry(iev);

        if (abs(unpackEvent->GetFEB12hitTfromSpill() - prevEventTime) < 700) continue;
        if (prevMaxCharge == unpackEvent->GetMaxCharge() && prevRange == unpackEvent->GetRange()) continue;
        prevEventTime = unpackEvent->GetFEB12hitTfromSpill();
        prevMaxCharge = unpackEvent->GetMaxCharge();
        prevRange = unpackEvent->GetRange();

        TClonesArray* unpackHits = unpackEvent->GetHits();
        int N = unpackEvent->GetNHits();
        vector<Hit*> sfgdHits = {};
        for (int i = 0; i < N; ++i) {
            Hit* tempHit = (Hit*)unpackEvent->GetHits()->At(i);

            if (tempHit->GetFEB() > 27) continue;

            if (tempHit->GetPE() > threshold) {
                sfgdHits.push_back(tempHit);
            }
        }

        vector<VoxelManager*> sfgdVoxels;
        sfgdVoxels = MakeHitsToVoxels(sfgdHits, 1);
        std::vector<vector<VoxelManager*>> sfgdClusters = FindClusters(sfgdVoxels, 1, 0, 1.8);
        std::vector<vector<VoxelManager*>> sfgdClustersVector;
        for (const auto& c : sfgdClusters) {
            if (c.size() < 3) continue;

            vector<double> geop = FindGeometricProperties(c, 3); //3 linear
            if (geop[0] < 0.9 ) continue;

            vector<double> voxs = FindGeometricProperties(c, 1); //1=fat
            if (voxs[0] > 2) continue;

            sfgdClustersVector.push_back(c);
        }
        if (sfgdClustersVector.size() != 2) continue;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "sfgdClustersVector.size(): " << sfgdClustersVector.size() << std::endl;

        for (auto v:sfgdVoxels) {
            std::cout << "sfgd  voxel: " << v->GetX() << ", " << v->GetY() << ", " << v->GetZ() << std::endl;
            hUnifiedZY->Fill(48 + (-1)*v->GetZ(), v->GetY());
            hUnifiedZX->Fill(48 + (-1)*v->GetZ(), v->GetX());
        }
        //DrawEventDisplay();

        TLine* zy1 = new TLine(48,0, 48,8);
        TLine* zy2 = new TLine(0,0, 48,0);
        TLine* zy3 = new TLine(0,8, 48,8);
        TLine* zy4 = new TLine(48+112,0, 48+112,8);
        TLine* zy5 = new TLine(48+112,0, 48+112+32,0);
        TLine* zy6 = new TLine(48+112,8, 48+112+32,8);
        TArrow* zyArrow1 = new TArrow(0,4, 48+112+32,4, 0.01, "|>");
        zyArrow1->SetLineColor(2);
        zyArrow1->SetFillColor(2);
        TText* zyText1 = new TText(-10, 3.5, "beam");
        TText* zyText2 = new TText(20, 9, "sfgd");
        TText* zyText3 = new TText(48+112+16, 9, "usj");

        TLine* zx1 = new TLine(48,0, 48,24);
        TLine* zx2 = new TLine(48+112,16, 48+112+32,16);
        TLine* zx3 = new TLine(48+112,8, 48+112+32,8);
        TLine* zx4 = new TLine(48+112,8, 48+112,16);
        TArrow* zxArrow1 = new TArrow(0,12, 48+112+32,12, 0.01, "|>");
        zxArrow1->SetLineColor(2);
        zxArrow1->SetFillColor(2);
        TText* zxText1 = new TText(-10, 11.5, "beam");
        TText* zxText2 = new TText(20, 23, "sfgd");
        TText* zxText3 = new TText(48+112+16, 17, "usj");

        TCanvas* unifiedCan = new TCanvas("unifiedCan", "unifiedCan", 960, 960); 
        //unifiedCan->SetBatch(true);
        unifiedCan->Divide(1,2);
        unifiedCan->cd(1);
        hUnifiedZY->SetStats(0);
        hUnifiedZY->Draw("colz");
        zy1->Draw();
        zy2->Draw("same");
        zy3->Draw("same");
        zy4->Draw("same");
        zy5->Draw("same");
        zy6->Draw("same");
        zyArrow1->Draw();
        zyText1->Draw();
        zyText2->Draw("same");
        zyText3->Draw("same");

        unifiedCan->cd(2);
        hUnifiedZX->SetStats(0);
        hUnifiedZX->Draw("colz");
        zx1->Draw();
        zx2->Draw("same");
        zx3->Draw("same");
        zx4->Draw("same");
        zxArrow1->Draw();
        zxText1->Draw();
        zxText2->Draw("same");
        zxText3->Draw("same");

        unifiedCan->Update();
        //unifiedCan->SaveAs(Form("%d.pdf", eventNum));
        unifiedCan->WaitPrimitive();
        hUnifiedZX->Reset();
        hUnifiedZY->Reset();
    }
}
