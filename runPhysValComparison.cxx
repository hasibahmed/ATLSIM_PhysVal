#include "TFile.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TKey.h"
#include "TString.h"
#include "TLine.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TLatex.h"

#include "atlasstyle/AtlasStyle.C"

#ifdef __CLING__
// these are not headers - do not treat them as such - needed for ROOT6
#include "atlasstyle/AtlasLabels.C"
#include "atlasstyle/AtlasUtils.C"
#endif

bool is_norm = true; 
std::string leg1 = "Geant4";
std::string leg2 = "G4FastCalo";
std::string leg3 = "AF2";
std::string xlabel = "";
std::string ratio_title = "X / " + leg1;

std::string signature = "";
std::string outDir = "";
std::string outFile = "";
std::string sample = "";
std::string object = "";

TDirectoryFile *file = nullptr;
TDirectoryFile *file2 = nullptr;
TDirectoryFile *file3 = nullptr;
TFile *fcanvas = nullptr;
TFile *fout = nullptr; 

std::vector<std::string> loadSamples(std::string object)
{
    std::vector<std::string> v_samples;

    if (object == "largeRJets") {
        v_samples = {"AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets"};

    } else if (object == "Jets" or object == "TopoClusters")
    {
        v_samples = {"ttbar",
                     "JZ3W",
                     "JZ7W",
                     "minbias"};
    }
    else if (object == "MET")
    {
        v_samples = {"ttbar"};
    }
    else if (object == "Electron")
    {
        v_samples = {"ttbar",
                     "Zee",
                     "electron"};
    }
    else if (object == "Photon")
    {
        v_samples = {"photon", "Hyy", "Zee"};
    }
    else if (object == "Tau")
    {
        v_samples = {"ttbar",
                     "Ztautau",
                     "DYtautau"};
    }

    return v_samples;
}

std::map<std::string, std::string> loadInputs(std::string inputDir, std::string sample)
{
    std::map<std::string, std::string> inputFiles;

    if (sample == "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets") {

        inputFiles["G4"] = inputDir + "/G4/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3432_r11403_p3851_p3861_p3747/NTUP_PHYSVAL.18034540._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "FCSV2/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3434_r11403_p3851_p3861_p3747/NTUP_PHYSVAL.18049722._000001.pool.root.1"; 

        inputFiles["AF2"] = inputDir + "AF2/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3433_r11403_p3851_p3861_p3747/NTUP_PHYSVAL.18049691._000001.pool.root.1";     
    }
    if (sample == "ttbar")
    {
        inputFiles["G4"] = inputDir + "/G4/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554556._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554749._000001.pool.root.1";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554651._000001.pool.root.1";
    } else if (sample == "JZ3W")
    {
        inputFiles["G4"] = inputDir + "/G4/valid1.361023.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ3W.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554477._000002.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.361023.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ3W.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554712.pool.root";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.361023.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ3W.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554582._000001.pool.root.1";

    } else if (sample ==   "JZ7W")
    {
        inputFiles["G4"] = inputDir + "/G4/valid1.361027.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ7W.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554325._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.361027.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ7W.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554635.pool.root";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.361027.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ7W.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554430._000001.pool.root.1";

    } else if(sample == "minbias") {

        inputFiles["G4"] = inputDir + "/G4/valid1.361033.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554292._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.361033.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554600.pool.root";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.361033.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554393._000001.pool.root.1";

    } else if(sample == "Zee") {

        inputFiles["G4"] = inputDir + "/G4/valid1.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554165._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554492.pool.root";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554287._000001.pool.root.1";

    } else if(sample == "electron") {

        inputFiles["G4"] = inputDir + "/G4/valid1.423000.ParticleGun_single_electron_egammaET.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554514._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.423000.ParticleGun_single_electron_egammaET.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554728.pool.root";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.423000.ParticleGun_single_electron_egammaET.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554615._000001.pool.root.1";

    } else if(sample == "Ztautau") {

        inputFiles["G4"] = inputDir + "/G4/valid1.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554251._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.pool.root";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554356._000001.pool.root.1";

    } else if(sample == "DYtautau") {

        inputFiles["G4"] = inputDir + "/G4/valid1.301050.PowhegPythia8EvtGen_AZNLOCTEQ6L1_DYtautau_2000M2250.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554358._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.301050.PowhegPythia8EvtGen_AZNLOCTEQ6L1_DYtautau_2000M2250.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554659._000001.pool.root.1";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.301050.PowhegPythia8EvtGen_AZNLOCTEQ6L1_DYtautau_2000M2250.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554462._000001.pool.root.1";

    } else if(sample == "photon") {

        inputFiles["G4"] = inputDir + "/G4/valid1.423001.ParticleGun_single_photon_egammaET.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554102._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.423001.ParticleGun_single_photon_egammaET.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554420.pool.root";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.423001.ParticleGun_single_photon_egammaET.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554205._000001.pool.root.1";

    } else if(sample == "Hyy") {

        inputFiles["G4"] = inputDir + "/G4/valid1.341000.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_ggH125_gamgam.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554136._000001.pool.root.1";

        inputFiles["G4FastCalo"] = inputDir + "/FCSV2/valid1.341000.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_ggH125_gamgam.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554459.pool.root";

        inputFiles["AF2"] = inputDir + "/AF2/valid1.341000.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_ggH125_gamgam.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554247._000001.pool.root.1";
    }

    return inputFiles; 
}

std::vector<std::string> loadDirectorties(std::string object) {

    std::vector<std::string> v_dir; 

    if(object == "largeRJets") {

        v_dir = {"Jets/AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets"}; 

    } else if (object == "Jets") {

        v_dir = {"Jets/AntiKt4EMTopoJets"};

    } else if (object == "TopoClusters") {

        v_dir = {"TopoClusters/TopoClusters_JetETMiss/JetETMiss_LCOriginTopoClustersPFO_JetETMiss"}; 
    } else if (object == "Electron") {

        v_dir = {"Electron/Central/All/KinPlots", "Electron/Central/All/ShowerShapesPlots", "Electron/Central/All/IsolationPlots", "Electron/Frwd/All/KinPlots", "Electron/Frwd/All/ClusMomentPlots", "Electron/Central/Iso/KinPlots", "Electron/Central/Iso/ShowerShapesPlots", "Electron/Central/Iso/IsolationPlots", "Electron/Frwd/Iso/KinPlots", "Electron/Frwd/Iso/ClusMomentPlots", "Electron/Central/IsoLoosePP/KinPlots", "Electron/Central/IsoMediumPP/KinPlots", "Electron/Central/IsoTightPP/KinPlots", "Electron/Central/IsoLHLoose/KinPlots", "Electron/Central/IsoLHMedium/KinPlots", "Electron/Central/IsoLHTight/KinPlots"}; 
    } else if (object == "MET") {

        v_dir = {"MET/MET_Rebuilt_AntiKt4EMTopo/Terms/RefEle", "MET/MET_Rebuilt_AntiKt4EMTopo/Terms/RefGamma", "MET/MET_Rebuilt_AntiKt4EMTopo/Terms/RefTau", "MET/MET_Rebuilt_AntiKt4EMTopo/Terms/Muons", "MET/MET_Rebuilt_AntiKt4EMTopo/Terms/RefJet", "MET/MET_Rebuilt_AntiKt4EMTopo/Terms/SoftClus", "MET/MET_Rebuilt_AntiKt4EMTopo/Terms/PVSoftTrk", "MET/MET_Rebuilt_AntiKt4EMTopo/Terms/FinalTrk", "MET/MET_Rebuilt_AntiKt4EMTopo/Residuals", "MET/MET_Rebuilt_AntiKt4EMTopo/Significance", "MET/MET_Rebuilt_AntiKt4EMTopo/dPhi", "MET/MET_Calo"}; 

    } else if (object == "Tau") {

        v_dir = {"Tau/Nominal/Matched/Tau1P", "Tau/Nominal/Matched/Tau3P", "Tau/Nominal/Matched/PFOs", "Tau/Nominal/Fake/Jet1P", "Tau/Nominal/Fake/Jet3P", "Tau/Nominal/Fake/PFOs"}; 

    } else if (object =="Photon") {

        v_dir = {"Photon/PhotAll/All/KinPlots", "Photon/PhotAll/All/ShowerShapesPlots","Photon/PhotAll/Iso/KinPlots", "Photon/PhotAll/All/ShowerShapesPlots", "Photon/PhotAll/Iso/KinPlotsUnc", "Photon/PhotAll/Iso/KinPlotsConv"}; 


    }

    return v_dir; 
}


TCanvas* compare_TProfile(TKey *key)
{
    char *h_name = (char *)key->GetName();
    TProfile *hist = (TProfile *)file->Get(h_name);
    TProfile *hist2 = (TProfile *)file2->Get(h_name);
    TProfile *hist3 = nullptr;
    if (file3)
        hist3 = (TProfile *)file3->Get(h_name);

    // hist->Sumw2();
    // hist2->Sumw2();
    // if (hist3)
    //     hist3->Sumw2();
    xlabel = hist->GetTitle();
    // hist->GetYaxis()->SetTitle("a.u.");
    hist->SetTitle("");
    hist->SetLineColor(kRed);
    hist->SetMarkerColor(kRed);

    hist2->SetLineColor(kBlack);
    hist2->SetMarkerColor(kBlack);
    hist2->SetMarkerStyle(kOpenTriangleUp);
    hist2->SetLineStyle(9);

    if (hist3)
    {
        hist3->SetLineColor(kGreen);
        hist3->SetMarkerColor(kGreen);
        hist3->SetMarkerStyle(kOpenCircle);
        hist3->SetLineStyle(2);
    }

    // // normalize
    // hist->Scale(1 / hist->Integral());
    // hist2->Scale(1 / hist2->Integral());
    // if (hist3)
    //     hist3->Scale(1 / hist3->Integral());

    // get the ratio histogram

    TProfile *hist_ratio_1_2 = (TProfile *)hist2->Clone();
    hist_ratio_1_2->Divide(hist);
    hist_ratio_1_2->SetLineColor(kBlack);
    hist_ratio_1_2->SetMarkerStyle(kOpenTriangleUp);
    hist_ratio_1_2->SetMarkerColor(kBlack);
    hist_ratio_1_2->SetTitle(""); 
    hist_ratio_1_2->GetYaxis()->SetTitle("");
    hist_ratio_1_2->GetXaxis()->SetLabelSize(0.08);
    // hist_ratio_1_2->GetXaxis()->SetTitle(xlabel.c_str());
    hist_ratio_1_2->GetXaxis()->SetTitleSize(0.1);
    hist_ratio_1_2->GetXaxis()->SetTitleOffset(1.);
    hist_ratio_1_2->GetYaxis()->SetRangeUser(0.7, 1.3); 


    TProfile *hist_ratio_1_3 = nullptr;
    if (hist3)
    {

        hist_ratio_1_3 = (TProfile *)hist3->Clone();
        hist_ratio_1_3->Divide(hist);
        hist_ratio_1_3->SetLineColor(kGreen);
        hist_ratio_1_3->SetMarkerStyle(kOpenCircle);
        hist_ratio_1_3->SetMarkerColor(kGreen);
        

    }

    TCanvas *c1 = new TCanvas(hist->GetName(), hist->GetName(), 1200, 900);
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3525, 1, 1);
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.35 - 0.0015);
    pad2->SetTopMargin(0.001);
    pad1->SetBottomMargin(0.001);
    pad2->SetBottomMargin(0.3);

    pad1->SetLeftMargin(0.13);
    pad2->SetLeftMargin(0.13);

    pad1->SetRightMargin(0.075);
    pad2->SetRightMargin(0.075);

    c1->cd();
    pad1->Draw();
    pad2->Draw();

    pad1->SetLogy();

    double Ymax = hist->GetMaximum();
    if (hist->GetMaximum() < hist2->GetMaximum())
        Ymax = hist2->GetMaximum();

    if (hist3 and (Ymax < hist3->GetMaximum()))
    {
        Ymax = hist3->GetMaximum();
    }

    hist->SetMaximum(10 * Ymax);

    pad1->cd();
    hist->Draw("hist e");
    if (hist3)
        hist3->Draw("hist e same");
    hist2->Draw("hist e same");

    TLegend *leg = new TLegend(0.7, 0.75, 0.95, 0.95);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetFillColor(0);
    //leg->SetTextSize(0.04);
    leg->AddEntry(hist, leg1.c_str(), "lpe");

    if (hist3)
        leg->AddEntry(hist3, leg3.c_str(), "lpe");
    leg->AddEntry(hist2, leg2.c_str(), "lpe");

    leg->Draw();

    ATLASLabel(0.2, 0.9, "Simulation Internal");
    myText(0.2, 0.85, 1, sample.c_str());
    myText(0.2, 0.8, 1, object.c_str());

    pad2->cd();

    hist_ratio_1_2->Draw("ep same");
    if (hist3)
        hist_ratio_1_3->Draw("ep same");

    TLine *line = new TLine(hist_ratio_1_2->GetXaxis()->GetBinLowEdge(1), 1.0, hist_ratio_1_2->GetXaxis()->GetBinUpEdge(hist_ratio_1_2->GetNbinsX()), 1.0);
    line->SetLineColor(kRed);
    line->Draw();
    gPad->RedrawAxis();

    gPad->Update();

    // fcanvas->cd();
    // c1->Write();
    c1->SaveAs((outFile + string(h_name) + ".png").c_str());

    // c1->Close();
    return c1; 
}

TCanvas* compare_TH1F(TKey *key)
{
    char *h_name = (char *)key->GetName();
    TH1F *hist = (TH1F *)file->Get(h_name);
    TH1F *hist2 = (TH1F *)file2->Get(h_name);
    TH1F *hist3 = nullptr;
    if (file3)
        hist3 = (TH1F *)file3->Get(h_name);

    hist->Sumw2();
    hist2->Sumw2();
    if (hist3)
        hist3->Sumw2();
    xlabel = hist->GetTitle();
    if(is_norm) hist->GetYaxis()->SetTitle("a.u.");
    hist->SetTitle("");
    hist->SetLineColor(kRed);
    hist->SetMarkerColor(kRed);

    hist2->SetLineColor(kBlack);
    hist2->SetMarkerColor(kBlack);
    hist2->SetMarkerStyle(kOpenTriangleUp);
    hist2->SetLineStyle(9);

    if (hist3)
    {
        hist3->SetLineColor(kGreen);
        hist3->SetMarkerColor(kGreen);
        hist3->SetMarkerStyle(kOpenCircle);
        hist3->SetLineStyle(2);
    }

    // normalize
    if(is_norm) {
        hist->Scale(1 / hist->Integral());
        hist2->Scale(1 / hist2->Integral());
        if (hist3)
            hist3->Scale(1 / hist3->Integral());
    }
    // get the ratio histogram

    TH1F *hist_ratio_1_2 = (TH1F *)hist2->Clone();
    hist_ratio_1_2->Divide(hist);
    hist_ratio_1_2->SetLineColor(kBlack);
    hist_ratio_1_2->SetMarkerStyle(kOpenTriangleUp);
    hist_ratio_1_2->SetMarkerColor(kBlack);
    hist_ratio_1_2->SetTitle("");
    hist_ratio_1_2->GetYaxis()->SetTitle("");
    hist_ratio_1_2->GetXaxis()->SetLabelSize(0.08);
    hist_ratio_1_2->GetXaxis()->SetTitle(xlabel.c_str());
    hist_ratio_1_2->GetXaxis()->SetTitleSize(0.1);
    hist_ratio_1_2->GetXaxis()->SetTitleOffset(1.);
    hist_ratio_1_2->GetYaxis()->SetRangeUser(0.7, 1.3); 


    TH1F *hist_ratio_1_3 = nullptr;
    if (hist3)
    {

        hist_ratio_1_3 = (TH1F *)hist3->Clone();
        hist_ratio_1_3->Divide(hist);
        hist_ratio_1_3->SetLineColor(kGreen);
        hist_ratio_1_3->SetMarkerStyle(kOpenCircle);
        hist_ratio_1_3->SetMarkerColor(kGreen);
    }

    TCanvas *c1 = new TCanvas(hist->GetName(), hist->GetName(), 1200, 900);
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3525, 1, 1);
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.35 - 0.0015);
    pad2->SetTopMargin(0.001);
    pad1->SetBottomMargin(0.001);
    pad2->SetBottomMargin(0.3);

    pad1->SetLeftMargin(0.13);
    pad2->SetLeftMargin(0.13);

    pad1->SetRightMargin(0.075);
    pad2->SetRightMargin(0.075);

    c1->cd();
    pad1->Draw();
    pad2->Draw();

    pad1->SetLogy();

    double Ymax = hist->GetMaximum();
    if (hist->GetMaximum() < hist2->GetMaximum())
        Ymax = hist2->GetMaximum();

    if (hist3 and (Ymax < hist3->GetMaximum()))
    {
        Ymax = hist3->GetMaximum();
    }

    hist->SetMaximum(10 * Ymax);

    pad1->cd();
    hist->Draw("hist e");
    if (hist3)
        hist3->Draw("hist e same");
    hist2->Draw("hist e same");

    TLegend *leg = new TLegend(0.7, 0.75, 0.95, 0.95);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetFillColor(0);
    //leg->SetTextSize(0.04);
    leg->AddEntry(hist, leg1.c_str(), "lpe");

    if (hist3)
        leg->AddEntry(hist3, leg3.c_str(), "lpe");
    leg->AddEntry(hist2, leg2.c_str(), "lpe");

    leg->Draw();

    ATLASLabel(0.2, 0.9, "Simulation Internal");
    myText(0.2, 0.85, 1, sample.c_str());
    myText(0.2, 0.8, 1, object.c_str());

    pad2->cd();

    hist_ratio_1_2->Draw("ep same");
    if (hist3)
        hist_ratio_1_3->Draw("ep same");

    TLine *line = new TLine(hist_ratio_1_2->GetXaxis()->GetBinLowEdge(1), 1.0, hist_ratio_1_2->GetXaxis()->GetBinUpEdge(hist_ratio_1_2->GetNbinsX()), 1.0);
    line->SetLineColor(kRed);
    line->Draw();
    gPad->RedrawAxis();

    gPad->Update();

    // fcanvas->cd();
    // c1->Write();
    c1->SaveAs((outFile + string(h_name) + ".png").c_str());

    // c1->Close();
    return c1; 
}
void runPhysValComparison(std::string object="Photon", bool isNorm = true, std::string outDir="./PhysVal2/Shape_Comparison/", std::string inDir="./NTUP_PHYSVAL_April2019/")
{

#ifdef __CINT__
    gROOT->LoadMacro("atlasstyle/AtlasLabels.C");
    gROOT->LoadMacro("atlasstyle/AtlasUtils.C");
#endif

    gROOT->SetBatch(kTRUE);

    SetAtlasStyle();

    is_norm = isNorm; 
    std::vector<std::string> v_samples = loadSamples(object); 
    std::vector<std::string> v_dir = loadDirectorties(object);

    // TFile* fout = new TFile((outDir + object + "/"+object + ".root").c_str(), "recreate");



    for(int i = 0; i < v_samples.size(); i++) {

        sample = v_samples.at(i); 

        system(("mkdir -p " + outDir + object + "/" + sample +"/").c_str());

        fout = new TFile((outDir + object + "/" + sample +"/" +object +"_"+ sample + ".root").c_str(), "recreate"); 

        
        cout<<" sample = " << sample << endl; 
        std::map<std::string, std::string> v_files = loadInputs(inDir, sample); 

        cout<<" files = " << v_files["G4"] <<endl; 
        cout<<" files = " << v_files["G4FastCalo"] <<endl; 
        cout<<" files = " << v_files["AF2"] <<endl; 


        TFile* f = TFile::Open(v_files["G4"].c_str());
        TFile* f2 = TFile::Open(v_files["G4FastCalo"].c_str()); 
        TFile* f3 = TFile::Open(v_files["AF2"].c_str()); 

        

        int ndir = v_dir.size();
        

        for(int j = 0; j < v_dir.size(); j++) {
            std::string dir = v_dir.at(j); 

            cout<<" dir = " << dir << endl; 

            outFile = outDir + object + "/" + sample + "/";
            system(("mkdir -p " + outFile).c_str());

        
            file = (TDirectoryFile*)f->Get(dir.c_str());
            file2 = (TDirectoryFile*)f2->Get(dir.c_str());
            file3 = (TDirectoryFile*)f3->Get(dir.c_str());


             // iterate over Geant4 sample keys
            int n_his = file->GetNkeys();
            cout<<" nKeys = "<< n_his <<endl; 
            TIter _next(file->GetListOfKeys());
            TKey *key;
            for (int k = 0; (key = (TKey *)_next()); k++)
            {
                if (strcmp(key->GetClassName(), "TH1F") == 0 or strcmp(key->GetClassName(), "TH1D") == 0)
                {
                    TCanvas* c1  = compare_TH1F(key);
                    fout->cd(); 
                    c1->Write();
                    c1->Close(); 
                }
                else if (strcmp(key->GetClassName(), "TProfile") == 0)
                {
                    TCanvas* c1 = compare_TProfile(key); 
                    fout->cd(); 
                    c1->Write();
                    c1->Close(); 
                }

                // gDirectory->Print();
                // cdtof->cd(); 
            } // loop over keys
            // file->cd(); 
            // cdtof->cd(); 

        } // over directories
        // fout->cd(); 

        fout->Close(); 
    } // over samples

    // // Geant4
    // TFile *f = TFile::Open("NTUP_PHYSVAL_April2019/G4/valid1.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.NTUP_PHYSVAL.e5112_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554165._000001.pool.root.1");

    // // G4FastCalo
    // TFile *f2 = TFile::Open("NTUP_PHYSVAL_April2019/FCSV2/valid1.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.NTUP_PHYSVAL.e5112_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554492.pool.root");

    // // AF2
    // TFile *f3 = nullptr;
    // f3 = TFile::Open("NTUP_PHYSVAL_April2019/AF2/valid1.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.merge.NTUP_PHYSVAL.e5112_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554287._000001.pool.root.1");

    // signature = "Electron";

    // outDir = "Zee/Electron/";
    // outFile = "Electron_profileplots";
    // sample = "Z #rightarrow ee";
    // object = "Electron";

    // file = (TDirectoryFile *)f->Get(signature.c_str());
    // file2 = (TDirectoryFile *)f2->Get(signature.c_str());
    // file3 = (TDirectoryFile *)f3->Get(signature.c_str());

    // // iterate over Geant4 sample keys
    // int n_his = file->GetNkeys();
    // TIter _next(file->GetListOfKeys());
    // TKey *key;

    // system(("mkdir -p " + outDir).c_str());

    // fcanvas = new TFile((outDir + outFile + ".root").c_str(), "recreate");

    // for (int i = 0; (key = (TKey *)_next()); i++)
    // {

    //     // if (strcmp(key->GetClassName(), "TTree") == 0)
    //     // {
    //     //     cout << " Found TTree, skipping..." << endl;
    //     //     continue;
    //     // }

    //     if (strcmp(key->GetClassName(), "TH1F") == 0 or strcmp(key->GetClassName(), "TH1D") == 0)
    //     {
    //         TCanvas* c1  = compare_TH1F(key);
    //         c1->Write();
    //         c1->Close(); 
    //     }
    //     else if (strcmp(key->GetClassName(), "TProfile") == 0)
    //     {
    //         TCanvas* c1 = compare_TProfile(key);
    //         c1->Write();
    //         c1->Close(); 
    //     }
    // }
    // fcanvas->Close();
}
