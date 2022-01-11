//#include "/Users/gwon/beamtest/neutronselection/utils/CommonHeader.hh"
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

//in MeV unit
double Weight(double inputEnergy) {
    double output;
    auto bFlux = std::make_unique<TGraph> (41);
    bFlux->SetPoint(1, 0 ,  1.26E-001);
    bFlux->SetPoint(2, 1.42E-001 ,  1.35E-001);
    bFlux->SetPoint(3, 1.79E-001 ,  1.35E-001);
    bFlux->SetPoint(4, 2.25E-001 ,  1.40E-001);
    bFlux->SetPoint(5, 2.84E-001 ,  1.44E-001);
    bFlux->SetPoint(6, 3.57E-001 ,  1.42E-001);
    bFlux->SetPoint(7, 4.50E-001 ,  1.41E-001);
    bFlux->SetPoint(8, 5.66E-001 ,  1.42E-001);
    bFlux->SetPoint(9, 7.13E-001 ,  1.30E-001);
    bFlux->SetPoint(10,8.97E-001 ,  1.17E-001);
    bFlux->SetPoint(11,1.13E+000 ,  9.66E-002);
    bFlux->SetPoint(12,1.42E+000 ,  7.64E-002);
    bFlux->SetPoint(13,1.79E+000 ,  6.59E-002);
    bFlux->SetPoint(14,2.25E+000 ,  5.34E-002);
    bFlux->SetPoint(15,2.84E+000 ,  4.27E-002);
    bFlux->SetPoint(16,3.57E+000 ,  3.36E-002);
    bFlux->SetPoint(17,4.50E+000 ,  2.73E-002);
    bFlux->SetPoint(18,5.66E+000 ,  1.97E-002);
    bFlux->SetPoint(19,7.13E+000 ,  1.39E-002);
    bFlux->SetPoint(20,8.97E+000 ,  9.51E-003);
    bFlux->SetPoint(21,1.13E+001 ,  6.26E-003);
    bFlux->SetPoint(22,1.42E+001 ,  4.00E-003);
    bFlux->SetPoint(23,1.79E+001 ,  2.61E-003);
    bFlux->SetPoint(24,2.26E+001 ,  2.13E-003);
    bFlux->SetPoint(25,2.84E+001 ,  1.59E-003);
    bFlux->SetPoint(26,3.57E+001 ,  1.30E-003);
    bFlux->SetPoint(27,4.50E+001 ,  1.05E-003);
    bFlux->SetPoint(28,5.66E+001 ,  8.59E-004);
    bFlux->SetPoint(29,7.13E+001 ,  7.47E-004);
    bFlux->SetPoint(30,8.97E+001 ,  5.93E-004);
    bFlux->SetPoint(31,1.13E+002 ,  5.53E-004);
    bFlux->SetPoint(32,1.42E+002 ,  4.69E-004);
    bFlux->SetPoint(33,1.79E+002 ,  3.93E-004);
    bFlux->SetPoint(34,2.25E+002 ,  3.24E-004);
    bFlux->SetPoint(35,2.84E+002 ,  2.67E-004);
    bFlux->SetPoint(36,3.57E+002 ,  1.88E-004);
    bFlux->SetPoint(37,4.50E+002 ,  9.76E-005);
    bFlux->SetPoint(38,5.38E+002 ,  9.08E-005);
    bFlux->SetPoint(39,6.13E+002 ,  1.38E-004);
    bFlux->SetPoint(40,6.88E+002 ,  9.64E-005);
    bFlux->SetPoint(41,7.63E+002 ,  1.57E-005);
    output = bFlux->Eval(inputEnergy);

    return output;
}

bool ApplyingCuts(const vector<VoxelManager*>& inVoxels, bool isUSJ) {
    if (inVoxels.size() < 3) return false;

    //don't require usj
    //single track condition
    if (!isUSJ) {
        //second : method, third: minimum voxel require, max distance  to find cluster
        std::vector<vector<VoxelManager*>> clusters = FindClusters(inVoxels, 1, 0, 1.8);
        if(clusters.size()!= 1) return false;

        vector<double> geop = FindGeometricProperties(clusters[0], 3); //3 linear
        if (geop[0] < 0.9 ) return false;

        vector<double> voxs = FindGeometricProperties(clusters[0], 1); //1=fat
        if (voxs[0] > 2) return false;
    }

    return true;
}

double GetEnergy(double inPE) {
    return 7./8*inPE + 100 - 125*7;
}

//simulation data
//ntuple
void angle_energy(std::string inputFileName) {  
    std::cout.precision(12);
    TFile* inputFile = new TFile(inputFileName.c_str(), "READ");
    TTree* inputTree = (TTree*)inputFile->Get("tree");

    int    ifileNo = {}; inputTree->SetBranchAddress("ifileNo", &ifileNo);
    int    ievt = {}; inputTree->SetBranchAddress("ievt", &ievt);
    float  vtx[3] = {}; inputTree->SetBranchAddress("vtx", &vtx);
    int    nFS = {}; inputTree->SetBranchAddress("nFS", &nFS);
    int    fsPdg[1000] = {}; inputTree->SetBranchAddress("fsPdg", &fsPdg);
    float  fsPx[1000] = {}; inputTree->SetBranchAddress("fsPx", &fsPx);
    float  fsPy[1000] = {}; inputTree->SetBranchAddress("fsPy", &fsPy);
    float  fsPz[1000] = {}; inputTree->SetBranchAddress("fsPz", &fsPz);
    float  fsE[1000] = {}; inputTree->SetBranchAddress("fsE", &fsE);
    float  neutronHitX[1000] = {}; inputTree->SetBranchAddress("neutronHitX", &neutronHitX);
    float  neutronHitY[1000] = {}; inputTree->SetBranchAddress("neutronHitY", &neutronHitY);
    float  neutronHitZ[1000] = {}; inputTree->SetBranchAddress("neutronHitZ", &neutronHitZ);
    float  neutronHitT[1000] = {}; inputTree->SetBranchAddress("neutronHitT", &neutronHitT);
    float  neutronHitE[1000] = {}; inputTree->SetBranchAddress("neutronHitE", &neutronHitE);
    float  neutronHitPDG[1000] = {}; inputTree->SetBranchAddress("neutronHitPDG", &neutronHitPDG);
    float  initialE[1000] = {}; inputTree->SetBranchAddress("initialE", &initialE);
    float  daughterPx[1000] = {}; inputTree->SetBranchAddress("daughterPx", &daughterPx);
    float  daughterPy[1000] = {}; inputTree->SetBranchAddress("daughterPy", &daughterPy);
    float  daughterPz[1000] = {}; inputTree->SetBranchAddress("daughterPz", &daughterPz);
    float  daughterPDG[1000] = {}; inputTree->SetBranchAddress("daughterPDG", &daughterPDG);
    int    node[1000] = {}; inputTree->SetBranchAddress("node", &node);
    float  neutronPx[1000] = {}; inputTree->SetBranchAddress("neutronPx", &neutronPx);
    float  neutronPy[1000] = {}; inputTree->SetBranchAddress("neutronPy", &neutronPy);
    float  neutronPz[1000] = {}; inputTree->SetBranchAddress("neutronPz", &neutronPz);
    float  neutronId[1000] = {}; inputTree->SetBranchAddress("neutronId", &neutronId);
    float  daughterParentId[1000] = {}; inputTree->SetBranchAddress("daughterParentId", &daughterParentId);
    int    delay[1000] = {}; inputTree->SetBranchAddress("delay", &delay);
    float  t_t0[1000] = {}; inputTree->SetBranchAddress("t0", &t_t0);
    float  t_spill[1000] = {}; inputTree->SetBranchAddress("spill", &t_spill);
    float  t_gamma_time[1000] = {}; inputTree->SetBranchAddress("gamma_time", &t_gamma_time);
    float  t_neutron_time = {}; inputTree->SetBranchAddress("neutron_time", &t_neutron_time);
    float  neutronParentId[1000] = {}; inputTree->SetBranchAddress("neutronParentId", &neutronParentId);
    float  neutronParentPDG[1000] = {}; inputTree->SetBranchAddress("neutronParentPDG", &neutronParentPDG);
    float  neutronTrackId[1000] = {}; inputTree->SetBranchAddress("neutronTrackId", &neutronTrackId);
    float  neutronTrackPDG[1000] = {}; inputTree->SetBranchAddress("neutronTrackPDG", &neutronTrackPDG);
    float  neutronTrackAng[1000] = {}; inputTree->SetBranchAddress("neutronTrackAng", &neutronTrackAng);
    int    t_id[1000] = {}; inputTree->SetBranchAddress("id", &t_id);
    float  t_length[1000] = {}; inputTree->SetBranchAddress("length", &t_length);
    int    nTrackId[1000] = {}; inputTree->SetBranchAddress("nTrackId", &nTrackId);
    int    nParentId[1000] = {}; inputTree->SetBranchAddress("nParentId", &nParentId);
    float  nTrackEnergy[1000] = {}; inputTree->SetBranchAddress("nTrackEnergy", &nTrackEnergy);
    float  nTrackAng[1000] = {}; inputTree->SetBranchAddress("nTrackAng", &nTrackAng);
    int    nTrackPdg[1000] = {}; inputTree->SetBranchAddress("nTrackPdg", &nTrackPdg);
    int    nParentPdg[1000] = {}; inputTree->SetBranchAddress("nParentPdg", &nParentPdg);
    float  pointPositionX[100] = {}; inputTree->SetBranchAddress("pointPositionX", &pointPositionX);
    float  pointPositionY[100] = {}; inputTree->SetBranchAddress("pointPositionY", &pointPositionY);
    float  pointPositionZ[100] = {}; inputTree->SetBranchAddress("pointPositionZ", &pointPositionZ);
    int    pointProcess[100] = {}; inputTree->SetBranchAddress("pointProcess", &pointProcess);
    int    pointSubProcess[100] = {}; inputTree->SetBranchAddress("pointSubProcess", &pointSubProcess);
    float  nElastic[100] = {}; inputTree->SetBranchAddress("nElastic", &nElastic);

    TH1D* angle = new TH1D("angle", "angle", 50,0,0);
    TH1D* energy = new TH1D("energy", "energy", 20,0,0);
    TH1D* energy_inMeV = new TH1D("energy_inMeV", "energy_inMeV", 20,0,0);


    TVector3 beamLine;
    beamLine.SetX(0);
    beamLine.SetY(0);
    beamLine.SetZ(1);

    for (int event = 0; event < inputTree->GetEntries(); ++event) {
        inputTree->GetEntry(event);
        TVector3 point1;
        point1.SetX(999);
        point1.SetY(999);
        point1.SetZ(999);
        TVector3 point2;
        point2.SetX(999);
        point2.SetY(999);
        point2.SetZ(999);
        bool isHadron = false;
        for (int i = 0; i < 100; ++i) {
            if (pointProcess[i] == 0) continue;
            if (isHadron && point1.X() == 999) {
                point1.SetX(pointPositionX[i]);
                std::cout << "point1 X: " << pointPositionX[i] << std::endl;
                std::cout << "point1 Y: " << pointPositionY[i] << std::endl;
                std::cout << "point1 Z: " << pointPositionZ[i] << std::endl;
                point1.SetY(pointPositionY[i]);
                point1.SetZ(pointPositionZ[i]);
            }
            if (isHadron && point2.X() == 999 && pointPositionX[i] != point1.X()) {
                std::cout << "point2 X: " << pointPositionX[i] << std::endl;
                std::cout << "point2 Y: " << pointPositionY[i] << std::endl;
                std::cout << "point2 Z: " << pointPositionZ[i] << std::endl;
                point2.SetX(pointPositionX[i]);
                point2.SetY(pointPositionY[i]);
                point2.SetZ(pointPositionZ[i]);
            }
            if (pointProcess[i] == 4) {
                isHadron = true;
            } else {
                isHadron = false;
            }
        }
        TVector3 neutronLine;
        if (point2.Z() > point1.Z()) {
            neutronLine.SetX(point2.X() - point1.X());
            neutronLine.SetY(point2.Y() - point1.Y());
            neutronLine.SetZ(point2.Z() - point1.Z());
        } else {
            neutronLine.SetX(point1.X() - point2.X());
            neutronLine.SetY(point1.Y() - point2.Y());
            neutronLine.SetZ(point1.Z() - point2.Z());
        }
        double ang = beamLine.Angle(neutronLine);
        //std::cout << "ang: " << ang << std::endl;
        double tempE = GetEnergy(fsE[0]);
        double tempWeight = Weight(tempE);
        //double tempWeight = 1;
        std::cout << "tempE: " << tempE << std::endl;
        std::cout << "tempWeight: " << tempWeight << std::endl;
        angle->Fill(ang, tempWeight);
        energy->Fill(tempE, tempWeight);
        energy_inMeV->Fill(tempE);
    }

    angle->Sumw2(0);
    energy->Sumw2(0);

    TCanvas* can = new TCanvas();
    can->Divide(2,1);
    can->cd(1);
    angle->Draw();
    can->cd(2);
    energy->Draw();


    TCanvas* can1 = new TCanvas();
    energy_inMeV->Draw();

}

void DoData();

void angle_energy() {
    //DoData();
    TH1D* angle = new TH1D("angle", "angle", 50,0,0);
    TH1D* energy = new TH1D("energy", "energy", 20,0,0);
    double threshold = 20;
    //TFile *SFGDinputFile  = new TFile("/pnfs/dune/persistent/users/sgwon/beamTest2020/MC/edep/PROD3/MC_EventStructure/SFGDoutput.root");
    //TFile *USJinputFile  = new TFile("/pnfs/dune/persistent/users/sgwon/beamTest2020/MC/edep/PROD3/MC_EventStructure/USJoutput.root");
    TFile *SFGDinputFile  = new TFile("/Users/gwon/beamtest/MC/test/neutronShoot/SFGDoutput.root","READ");
    TFile *USJinputFile  = new TFile("/Users/gwon/beamtest/MC/test/neutronShoot/USJoutput.root","READ");
    TTree* SFGDtree = (TTree*) SFGDinputFile->Get("AllEvents");
    Event* SFGDunpackEvent = new Event(); SFGDtree->SetBranchAddress("Event", &SFGDunpackEvent);

    TTree* USJtree = (TTree*) USJinputFile->Get("AllEvents");
    Event* USJunpackEvent = new Event(); USJtree->SetBranchAddress("Event", &USJunpackEvent);
    for (int iev = 0; iev < SFGDtree->GetEntries(); iev++) {
        SFGDtree->GetEntry(iev);
        //std::cout << "SFGDunpackEvent->GetEventID():" << SFGDunpackEvent->GetEventID() << std::endl;
        USJtree->GetEntry(iev);
        //std::cout << "USJunpackEvent->GetEventID():" << USJunpackEvent->GetEventID() << std::endl;
        std::cout << "iev: " << iev << std::endl;

        //vector<Hit*> usjHits = {};
        //TClonesArray * USJunpackHits = USJunpackEvent->GetHits();
        //int USJN = USJunpackEvent->GetNHits();
        //for (int i = 0; i < USJN; ++i) {
        //    Hit* tempHit = (Hit*)USJunpackEvent->GetHits()->At(i);

        //    if (tempHit->GetPE() > threshold) {
        //        usjHits.push_back(tempHit);
        //    }
        //}

        vector<Hit*> sfgdHits = {};
        TClonesArray * SFGDunpackHits = SFGDunpackEvent->GetHits();
        int SFGDN = SFGDunpackEvent->GetNHits();
        for (int i = 0; i < SFGDN; ++i) {
            Hit* tempHit = (Hit*)SFGDunpackEvent->GetHits()->At(i);

            if (tempHit->GetPE() > threshold) {
                sfgdHits.push_back(tempHit);
            }
        }

        //vector<VoxelManager*> usjVoxels;
        vector<VoxelManager*> sfgdVoxels;
        //usjVoxels = MakeHitsToVoxels(usjHits, 1);
        sfgdVoxels = MakeHitsToVoxels(sfgdHits, 1);
        
        //second : method, third: minimum voxel require, max distance  to find cluster
        //std::vector<vector<VoxelManager*>> usjClusters = FindClusters(usjVoxels, 1, 0, 1.8);
        std::vector<vector<VoxelManager*>> sfgdClusters = FindClusters(sfgdVoxels, 1, 0, 1.8);

        //std::cout << "sfgdClusters.size(): " << sfgdClusters.size() << std::endl;

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
        std::cout << "sfgdClustersVector.size(): " << sfgdClustersVector.size() << std::endl;

        TVector3 beamLine;
        beamLine.SetX(0);
        beamLine.SetY(0);
        beamLine.SetZ(1);

        vector<VoxelManager*> tempVoxel1 = sfgdClustersVector[0];
        vector<VoxelManager*> tempVoxel2 = sfgdClustersVector[1];
        TVector3 point1;
        point1.SetX(tempVoxel1.at(0)->GetX());
        point1.SetY(tempVoxel1.at(0)->GetY());
        point1.SetZ(tempVoxel1.at(0)->GetZ());
        TVector3 point2;
        point2.SetX(tempVoxel2.at(0)->GetX());
        point2.SetY(tempVoxel2.at(0)->GetY());
        point2.SetZ(tempVoxel2.at(0)->GetZ());
        std::cout << "point1: " << point1.X() << ", " << point1.Y() << ", " << point1.Z() << std::endl;
        std::cout << "point2: " << point2.X() << ", " << point2.Y() << ", " << point2.Z() << std::endl;
        TVector3 neutronLine;
        if (point2.Z() > point1.Z()) {
            neutronLine.SetX(point2.X() - point1.X());
            neutronLine.SetY(point2.Y() - point1.Y());
            neutronLine.SetZ(point2.Z() - point1.Z());
        } else {
            neutronLine.SetX(point1.X() - point2.X());
            neutronLine.SetY(point1.Y() - point2.Y());
            neutronLine.SetZ(point1.Z() - point2.Z());
        }
        double ang = beamLine.Angle(neutronLine);
        std::cout << "ang: " << ang << std::endl;
        angle->Fill(ang);

        /*
        if (usjClusters.size() + sfgdClusters.size() == 2) {
            if (usjClusters.size() == 2) {
                vector<VoxelManager*> tempVoxel1 = usjClusters[0];
                vector<VoxelManager*> tempVoxel2 = usjClusters[1];
                TVector3 point1;
                point1.SetX(tempVoxel1.at(0)->GetX());
                point1.SetY(tempVoxel1.at(0)->GetY());
                point1.SetZ(tempVoxel1.at(0)->GetZ());
                TVector3 point2;
                point2.SetX(tempVoxel2.at(0)->GetX());
                point2.SetY(tempVoxel2.at(0)->GetY());
                point2.SetZ(tempVoxel2.at(0)->GetZ());
                std::cout << "point1: " << point1.X() << ", " << point1.Y() << ", " << point1.Z() << std::endl;
                std::cout << "point2: " << point2.X() << ", " << point2.Y() << ", " << point2.Z() << std::endl;
                TVector3 neutronLine;
                if (point2.Z() > point1.Z()) {
                    neutronLine.SetX(point2.X() - point1.X());
                    neutronLine.SetY(point2.Y() - point1.Y());
                    neutronLine.SetZ(point2.Z() - point1.Z());
                } else {
                    neutronLine.SetX(point1.X() - point2.X());
                    neutronLine.SetY(point1.Y() - point2.Y());
                    neutronLine.SetZ(point1.Z() - point2.Z());
                }
                double ang = beamLine.Angle(neutronLine);
                std::cout << "ang: " << ang << std::endl;
                angle->Fill(ang);
            } else if (sfgdClusters.size() == 2) {
                vector<VoxelManager*> tempVoxel1 = sfgdClusters[0];
                vector<VoxelManager*> tempVoxel2 = sfgdClusters[1];
                TVector3 point1;
                point1.SetX(tempVoxel1.at(0)->GetX());
                point1.SetY(tempVoxel1.at(0)->GetY());
                point1.SetZ(tempVoxel1.at(0)->GetZ());
                TVector3 point2;
                point2.SetX(tempVoxel2.at(0)->GetX());
                point2.SetY(tempVoxel2.at(0)->GetY());
                point2.SetZ(tempVoxel2.at(0)->GetZ());
                TVector3 neutronLine;
                if (point2.Z() > point1.Z()) {
                    neutronLine.SetX(point2.X() - point1.X());
                    neutronLine.SetY(point2.Y() - point1.Y());
                    neutronLine.SetZ(point2.Z() - point1.Z());
                } else {
                    neutronLine.SetX(point1.X() - point2.X());
                    neutronLine.SetY(point1.Y() - point2.Y());
                    neutronLine.SetZ(point1.Z() - point2.Z());
                }
                double ang = beamLine.Angle(neutronLine);
                std::cout << "point1: " << point1.X() << ", " << point1.Y() << ", " << point1.Z() << std::endl;
                std::cout << "point2: " << point2.X() << ", " << point2.Y() << ", " << point2.Z() << std::endl;
                std::cout << "ang: " << ang << std::endl;
                angle->Fill(ang);
            } else {
                vector<VoxelManager*> tempVoxel1 = sfgdClusters[0];
                vector<VoxelManager*> tempVoxel2 = usjClusters[0];
                TVector3 point1;
                point1.SetX(tempVoxel1.at(0)->GetX());
                point1.SetY(tempVoxel1.at(0)->GetY());
                point1.SetZ(tempVoxel1.at(0)->GetZ());
                TVector3 point2;
                point2.SetX(tempVoxel2.at(0)->GetX()+8);
                point2.SetY(tempVoxel2.at(0)->GetY());
                point2.SetZ(tempVoxel2.at(0)->GetZ()+112+48);
                TVector3 neutronLine;
                if (point2.Z() > point1.Z()) {
                    neutronLine.SetX(point2.X() - point1.X());
                    neutronLine.SetY(point2.Y() - point1.Y());
                    neutronLine.SetZ(point2.Z() - point1.Z());
                } else {
                    neutronLine.SetX(point1.X() - point2.X());
                    neutronLine.SetY(point1.Y() - point2.Y());
                    neutronLine.SetZ(point1.Z() - point2.Z());
                }
                double ang = beamLine.Angle(neutronLine);
                std::cout << "usjClusters.size(): " << usjClusters.size() << std::endl;
                std::cout << "sfgdClusters.size(): " << sfgdClusters.size() << std::endl;
                std::cout << "point1: " << point1.X() << ", " << point1.Y() << ", " << point1.Z() << std::endl;
                std::cout << "point2: " << point2.X() << ", " << point2.Y() << ", " << point2.Z() << std::endl;
                std::cout << "ang: " << ang << std::endl;
                angle->Fill(ang);
            }
        }
    */
    }
    TCanvas* can = new TCanvas();
    can->Divide(2,1);
    can->cd(1);
    angle->Draw();
    can->cd(2);
    //energy->Draw();
}

void DoData() {
    TH1D* angle = new TH1D("angle", "angle", 50,0,0);
    TH1D* energy = new TH1D("energy", "energy", 20,0,0);
    double threshold = 20;
    TFile *inputFile  = new TFile("/Users/gwon/beamtest/data/2020/232/MCR0_Run_232_SubRun_01_2020_12_12_09_41_27__events.root","READ");
    TTree* tree = (TTree*) inputFile->Get("AllEvents");
    Event* unpackEvent = new Event(); tree->SetBranchAddress("Event", &unpackEvent);

    for (int iev = 0; iev < tree->GetEntries(); iev++) {
        tree->GetEntry(iev);
        std::cout << "iev: " << iev << std::endl;

        TClonesArray * unpackHits = unpackEvent->GetHits();

        vector<Hit*> usjHits = {};
        vector<Hit*> sfgdHits = {};
        int N = unpackEvent->GetNHits();
        for (int i = 0; i < N; ++i) {
            Hit* tempHit = (Hit*)unpackEvent->GetHits()->At(i);

            bool isUSJ = false;
            if (tempHit->GetFEB() > 27)
                isUSJ = true;

            if (!isUSJ && tempHit->GetPE() > threshold) {
                if (tempHit->GetDt() > -340 && tempHit->GetDt() < 360)
                    sfgdHits.push_back(tempHit);
            }
            if (isUSJ && tempHit->GetPE() > threshold) {
                if (tempHit->GetDt() > -325 && tempHit->GetDt() < 360)
                    usjHits.push_back(tempHit);
            }
        }

        vector<VoxelManager*> usjVoxels;
        vector<VoxelManager*> sfgdVoxels;
        usjVoxels = MakeHitsToVoxels(usjHits, 1);
        sfgdVoxels = MakeHitsToVoxels(sfgdHits, 1);
        
        //second : method, third: minimum voxel require, max distance  to find cluster
        std::vector<vector<VoxelManager*>> usjClusters = FindClusters(usjVoxels, 1, 0, 1.8);
        std::vector<vector<VoxelManager*>> sfgdClusters = FindClusters(sfgdVoxels, 1, 0, 1.8);

        TVector3 beamLine;
        beamLine.SetX(0);
        beamLine.SetY(0);
        beamLine.SetZ(1);

        if (usjClusters.size() + sfgdClusters.size() == 2) {
            if (usjClusters.size() == 2) {
                vector<VoxelManager*> tempVoxel1 = usjClusters[0];
                vector<VoxelManager*> tempVoxel2 = usjClusters[1];
                TVector3 point1;
                point1.SetX(tempVoxel1.at(0)->GetX());
                point1.SetY(tempVoxel1.at(0)->GetY());
                point1.SetZ(tempVoxel1.at(0)->GetZ());
                TVector3 point2;
                point2.SetX(tempVoxel2.at(0)->GetX());
                point2.SetY(tempVoxel2.at(0)->GetY());
                point2.SetZ(tempVoxel2.at(0)->GetZ());
                std::cout << "point1: " << point1.X() << ", " << point1.Y() << ", " << point1.Z() << std::endl;
                std::cout << "point2: " << point2.X() << ", " << point2.Y() << ", " << point2.Z() << std::endl;
                TVector3 neutronLine;
                if (point2.Z() > point1.Z()) {
                    neutronLine.SetX(point2.X() - point1.X());
                    neutronLine.SetY(point2.Y() - point1.Y());
                    neutronLine.SetZ(point2.Z() - point1.Z());
                } else {
                    neutronLine.SetX(point1.X() - point2.X());
                    neutronLine.SetY(point1.Y() - point2.Y());
                    neutronLine.SetZ(point1.Z() - point2.Z());
                }
                double ang = beamLine.Angle(neutronLine);
                std::cout << "ang: " << ang << std::endl;
                angle->Fill(ang);
            } else if (sfgdClusters.size() == 2) {
                vector<VoxelManager*> tempVoxel1 = sfgdClusters[0];
                vector<VoxelManager*> tempVoxel2 = sfgdClusters[1];
                TVector3 point1;
                point1.SetX(tempVoxel1.at(0)->GetX());
                point1.SetY(tempVoxel1.at(0)->GetY());
                point1.SetZ(tempVoxel1.at(0)->GetZ());
                TVector3 point2;
                point2.SetX(tempVoxel2.at(0)->GetX());
                point2.SetY(tempVoxel2.at(0)->GetY());
                point2.SetZ(tempVoxel2.at(0)->GetZ());
                TVector3 neutronLine;
                if (point2.Z() > point1.Z()) {
                    neutronLine.SetX(point2.X() - point1.X());
                    neutronLine.SetY(point2.Y() - point1.Y());
                    neutronLine.SetZ(point2.Z() - point1.Z());
                } else {
                    neutronLine.SetX(point1.X() - point2.X());
                    neutronLine.SetY(point1.Y() - point2.Y());
                    neutronLine.SetZ(point1.Z() - point2.Z());
                }
                double ang = beamLine.Angle(neutronLine);
                std::cout << "point1: " << point1.X() << ", " << point1.Y() << ", " << point1.Z() << std::endl;
                std::cout << "point2: " << point2.X() << ", " << point2.Y() << ", " << point2.Z() << std::endl;
                std::cout << "ang: " << ang << std::endl;
                angle->Fill(ang);
            } else {
                vector<VoxelManager*> tempVoxel1 = sfgdClusters[0];
                vector<VoxelManager*> tempVoxel2 = usjClusters[0];
                TVector3 point1;
                point1.SetX(tempVoxel1.at(0)->GetX());
                point1.SetY(tempVoxel1.at(0)->GetY());
                point1.SetZ(tempVoxel1.at(0)->GetZ());
                TVector3 point2;
                point2.SetX(tempVoxel2.at(0)->GetX()+8);
                point2.SetY(tempVoxel2.at(0)->GetY());
                point2.SetZ(tempVoxel2.at(0)->GetZ()+112+48);
                TVector3 neutronLine;
                if (point2.Z() > point1.Z()) {
                    neutronLine.SetX(point2.X() - point1.X());
                    neutronLine.SetY(point2.Y() - point1.Y());
                    neutronLine.SetZ(point2.Z() - point1.Z());
                } else {
                    neutronLine.SetX(point1.X() - point2.X());
                    neutronLine.SetY(point1.Y() - point2.Y());
                    neutronLine.SetZ(point1.Z() - point2.Z());
                }
                double ang = beamLine.Angle(neutronLine);
                std::cout << "usjClusters.size(): " << usjClusters.size() << std::endl;
                std::cout << "sfgdClusters.size(): " << sfgdClusters.size() << std::endl;
                std::cout << "point1: " << point1.X() << ", " << point1.Y() << ", " << point1.Z() << std::endl;
                std::cout << "point2: " << point2.X() << ", " << point2.Y() << ", " << point2.Z() << std::endl;
                std::cout << "ang: " << ang << std::endl;
                angle->Fill(ang);
            }
        }
    }
    TCanvas* can = new TCanvas();
    can->Divide(2,1);
    can->cd(1);
    angle->Draw();
    can->cd(2);
    energy->Draw();
}
