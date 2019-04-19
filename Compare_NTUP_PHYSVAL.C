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

void compare_TH1F(TKey *key)
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
    hist->GetYaxis()->SetTitle("a.u.");
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
    hist->Scale(1 / hist->Integral());
    hist2->Scale(1 / hist2->Integral());
    if (hist3)
        hist3->Scale(1 / hist3->Integral());

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

    fcanvas->cd();
    c1->Write();
    c1->SaveAs((outDir + string(h_name) + ".png").c_str());

    c1->Close();
}
void Compare_NTUP_PHYSVAL()
{

#ifdef __CINT__
    gROOT->LoadMacro("atlasstyle/AtlasLabels.C");
    gROOT->LoadMacro("atlasstyle/AtlasUtils.C");
#endif

    gROOT->SetBatch(kTRUE);

    SetAtlasStyle();

    // Geant4
    TFile *f = TFile::Open("NTUP_PHYSVAL_April2019/G4/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3432_r11322_p3746_p3747/NTUP_PHYSVAL.17554556._000001.pool.root.1");

    // AF2
    TFile *f2 = TFile::Open("NTUP_PHYSVAL_April2019/FCSV2/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3434_r11322_p3746_p3747/NTUP_PHYSVAL.17554749._000001.pool.root.1");

    // G4FastCalo
    TFile *f3 = nullptr;
    f3 = TFile::Open("NTUP_PHYSVAL_April2019/AF2/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.NTUP_PHYSVAL.e4993_s3433_r11322_p3746_p3747/NTUP_PHYSVAL.17554651._000001.pool.root.1");

    signature = "Jets/AntiKt4EMTopoJets";

    outDir = "ttbar/";
    outFile = "physval2_ttbar";
    sample = "t#bar{t} #rightarrow l#nuqq(ll#nu#nu)";
    object = "AntiKT4EMTopoJets";

    file = (TDirectoryFile *)f->Get(signature.c_str());
    file2 = (TDirectoryFile *)f2->Get(signature.c_str());
    file3 = (TDirectoryFile *)f3->Get(signature.c_str());

    // iterate over Geant4 sample keys
    int n_his = file->GetNkeys();
    TIter _next(file->GetListOfKeys());
    TKey *key;

    system(("mkdir -p " + outDir).c_str());

    fcanvas = new TFile((outDir + outFile + ".root").c_str(), "recreate");

    for (int i = 0; (key = (TKey *)_next()); i++)
    {

        if (strcmp(key->GetClassName(), "TTree") == 0)
        {
            cout << " Found TTree, skipping..." << endl;
            continue;
        }

        if (strcmp(key->GetClassName(), "TH1F") == 0)
        {
            compare_TH1F(key);
        }
    }
    fcanvas->Close();
}
