#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "TFile.h"
#include "TColor.h"
#include "TPaveText.h"
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPad.h"
#include "TMath.h"
#include "TSystem.h"
#include <libgen.h> 
#include <TCollection.h>
#include <TKey.h>
#include <stdio.h>
#include <string.h>
#include "TROOT.h"
//#include "TRatioPlot.h"
#include "TClassRef.h"
#include "TVirtualPad.h"
#include "TBrowser.h"
#include "TH1.h"
#include "TF1.h"
#include "TPad.h"
#include "TString.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TGaxis.h"
#include "TLine.h"
#include "TVirtualFitter.h"
#include "TFitResult.h"
  
using namespace std; 
  
void Plot_macro_plot(){

  char a[20];
  char b[80];
    //cout << "Scrivi istogramma ";
    //cin >> a;
  //a = 'histo_name'
   strcpy(a, "histo_name");
    //string a = histo_name;
    std::vector<string> data;
  
    std::ifstream infile;
    infile.open("input_data.txt");

    std::string inputfilename;
    
    while(std::getline(infile,inputfilename)){
      data.push_back(inputfilename); 
    }
    infile.close();
    
    TList *list = new TList;
    TList *list1 = new TList;
    TList *list2 = new TList;
    TList *list_2P2F = new TList;
    TH1D* hMZ_3 = new TH1D ("","", Nbins, Xmin, Xmax);
    TH1D *total_data = (TH1D*)hMZ_3->Clone("total_data");
    
    TChain *chain = new TChain("HZZ4LeptonsAnalysisReduced");
    
  for (unsigned int datasetIdData=0; datasetIdData<data.size(); datasetIdData++){    
    char dataset[328];
    sprintf(dataset,"%s",data.at(datasetIdData).c_str());
    //cout << "Root-ple= " << dataset << endl;

    TFile *f1 = TFile::Open(dataset);
    TIter next(f1->GetListOfKeys());
    TKey *key;
    
    while ((key = (TKey*)next())) {
      TClass *clsPtr = gROOT->GetClass(key->GetClassName());
      TString name = key->GetClassName();
            
      if(strcmp(key->GetName(), a) == 0 ){
	strcpy(b, key->GetName());  
      }
    }

    TH1D *hMZ_3=(TH1D*)f1->Get(b);
    list->Add(hMZ_3);
    
  }
  total_data->Merge(list);
  

  std::vector<string> bkg;
  std::ifstream infile_bkg;
  infile_bkg.open("input_bkg.txt");
  
  std::string inputfilename_bkg;
    
  while(std::getline(infile_bkg,inputfilename_bkg)){
      bkg.push_back(inputfilename_bkg);                            
  }
  infile_bkg.close();
  
  TList *list_ttV = new TList;
  TList *list_H = new TList;
  TList *list_ZZ = new TList;
  TList *list_HV = new TList;
  TList *list_VV = new TList;
  TList *list_VVV = new TList;
  TList *list_tt = new TList;
  TList *list_DY = new TList;
  TList *list_others = new TList;
  TList *list_all = new TList;
  TList *list_1 = new TList;
  TList *list_2 = new TList;

  TList *list_num = new TList;
  TList *list_den = new TList;
  TH1D *hMZ_3_bkg = new TH1D ("","",Nbins, Xmin, Xmax);
  TH1D *bkg_ttV = (TH1D*)hMZ_3_bkg->Clone("bkg_ttV");
  TH1D *bkg_H = (TH1D*)hMZ_3_bkg->Clone("bkg_H");
  TH1D *bkg_ZZ = (TH1D*)hMZ_3_bkg->Clone("bkg_ZZ");
  TH1D *bkg_HV = (TH1D*)hMZ_3_bkg->Clone("bkg_HV");
  TH1D *bkg_VV = (TH1D*)hMZ_3_bkg->Clone("bkg_VV");
  TH1D *bkg_VVV = (TH1D*)hMZ_3_bkg->Clone("bkg_VVV");
  TH1D *bkg_tt = (TH1D*)hMZ_3_bkg->Clone("bkg_tt");
  TH1D *bkg_DY = (TH1D*)hMZ_3_bkg->Clone("bkg_DY");
  TH1D *bkg_others = (TH1D*)hMZ_3_bkg->Clone("bkg_others");
  TH1D *bkg_all = (TH1D*)hMZ_3_bkg->Clone("bkg_all");


  for (unsigned int datasetIdData=0; datasetIdData<bkg.size(); datasetIdData++){    
    char bkgset[328];
    sprintf(bkgset,"%s",bkg.at(datasetIdData).c_str());
    //cout << "Root-ple= " << bkg.at(datasetIdData) << endl;
    TFile *f1_bkg = TFile::Open(bkgset);
   
    
    TString name = bkg.at(datasetIdData);
        
    if(name.Contains("ttH") || name.Contains("TTW") || name.Contains("TTZ")){
      cout << "Root-ple for ttV " << bkgset << "\n" << endl;
      hMZ_3_bkg = (TH1D*)f1_bkg->Get(b); 
      list_ttV->Add(hMZ_3_bkg);
    }


    if(name.Contains("GluGluHToZZ") || name.Contains("VBF")){
      cout << "Root-ple for H " << bkgset << "\n" << endl; 
      hMZ_3_bkg = (TH1D*)f1_bkg->Get(b); 
      list_H->Add(hMZ_3_bkg);
    }
    			 
    if(name.Contains("ZZTo4L_13") || name.Contains("GluGluToContinToContinToZZ") || name.Contains("ZZTo2L2Nu")){
      cout << "Root-ple for ZZ " << bkgset << "\n" << endl;
      hMZ_3_bkg = (TH1D*)f1_bkg->Get(b); 
      list_ZZ->Add(hMZ_3_bkg);
    }
    
    
    if(name.Contains("_WminusH") || name.Contains("_WplusH") || name.Contains("_ZH") || name.Contains("_HZ")){
      cout << "Root-ple for HV " << bkgset << "\n" << endl;
      hMZ_3_bkg = (TH1D*)f1_bkg->Get(b); 
      list_HV->Add(hMZ_3_bkg);
      list_others->Add(hMZ_3_bkg);
    }
    
     
    if(name.Contains("_WWTo2L") ||  name.Contains("_WZT") || name.Contains("_WJets")){
      cout << "Root-ple for VV " << bkgset << "\n" << endl;
      hMZ_3_bkg = (TH1D*)f1_bkg->Get(b); 
      list_VV->Add(hMZ_3_bkg);
      list_others->Add(hMZ_3_bkg);
    }
      
    if(name.Contains("WZZ_") ||  name.Contains("WWZ_") || name.Contains("ZZZ_")){
      cout << "Root-ple for VVV " << bkgset << "\n" << endl;
      hMZ_3_bkg = (TH1D*)f1_bkg->Get(b); 
      list_VVV->Add(hMZ_3_bkg);
      list_others->Add(hMZ_3_bkg);
    }
    
    
    if(name.Contains("TTTo")){
      cout << "Root-ple for tt " << bkgset << "\n" << endl;
      hMZ_3_bkg = (TH1D*)f1_bkg->Get(b);
      list_tt->Add(hMZ_3_bkg);
    }
    
    if(name.Contains("DY")){
     cout << "Root-ple for DY " << bkgset << "\n" << endl;
     hMZ_3_bkg = (TH1D*)f1_bkg->Get(b);
     list_DY->Add(hMZ_3_bkg);
    }
    
    
    cout << "Root-ple for all bkg " << bkgset << "\n" << endl;
    hMZ_3_bkg = (TH1D*)f1_bkg->Get(b); 
    list_all->Add(hMZ_3_bkg);


  }

  std::vector<string> signal;

  std::ifstream infile_signal;
  infile_signal.open("signal_all.txt");

  std::string inputfilename_signal;

  while(std::getline(infile_signal, inputfilename_signal)){
    cout << "Reading " << inputfilename_signal.c_str() << endl;
    signal.push_back(inputfilename_signal);
  }
  infile_signal.close();

  TList *list_signal = new TList;
  TH1D *hMZ_3_signal = new TH1D ("","", Nbins, Xmin, Xmax);//1200, 4.5, 1204.5);//200 , 0 , 200);
  TH1D *signal_all = (TH1D*)hMZ_3_signal->Clone("signal_all");

  for (unsigned int datasetIdData=0; datasetIdData<signal.size(); datasetIdData++){
    char dataset_signal[328];
    sprintf(dataset_signal,"%s",signal.at(datasetIdData).c_str());
    cout << "Root-ple= " << signal.at(datasetIdData) << endl;  

    TFile *f1_signal = TFile::Open(dataset_signal);
    hMZ_3_signal = (TH1D*)f1_signal->Get(b);
    list_signal->Add(hMZ_3_signal);
  }

  signal_all->Merge(list_signal);
  signal_all->Scale(144);
  signal_all->SetLineStyle(2);
  signal_all->SetLineWidth(4);
  signal_all->SetLineColor(kBlack);

 
  bkg_others->Merge(list_others);
  total_data->SetMarkerStyle(8);
  total_data->SetMarkerColor(kBlack);
  total_data->SetLineColor(kBlack);

  
  bkg_ttV->Merge(list_ttV);
  bkg_ttV->SetFillColor(kBlue);
  cout << "ttV "<< bkg_ttV->GetEntries() << "\n";
  cout << "ttV Integral "<< bkg_ttV->Integral() << "\n";

  bkg_H->Merge(list_H);
  bkg_H->SetFillColor(kOrange);//
  cout << "H "<< bkg_H->GetEntries() << "\n";
  cout << "H Integral "<< bkg_H->Integral() << "\n";

  bkg_ZZ->Merge(list_ZZ);
  bkg_ZZ->SetFillColor(kGreen);
  cout << "ZZ "<< bkg_ZZ->GetEntries() << "\n";
  cout << "ZZ Integral "<< bkg_ZZ->Integral() << "\n";

  bkg_VV->Merge(list_VV);
  bkg_VV->SetFillColor(kYellow);
  cout << "VV "<< bkg_VV->GetEntries() << "\n";
  cout << "VV Integral "<< bkg_VV->Integral() << "\n";

  bkg_VVV->Merge(list_VVV);
  bkg_VVV->SetFillColor(kGreen);
  cout << "VVV "<< bkg_VVV->GetEntries() << "\n";
  cout << "VVV Integral "<< bkg_VVV->Integral() << "\n";

  bkg_HV->Merge(list_HV);
  bkg_HV->SetFillColor(kBlue);
  cout << "HV "<< bkg_HV->GetEntries() << "\n";
  cout << "HV Integral "<< bkg_HV->Integral() << "\n";

  bkg_others->SetFillColor(kMagenta -7);//kYellow
  cout << "others "<< bkg_others->GetEntries() << "\n";


  bkg_tt->Merge(list_tt);
  bkg_tt->SetFillColor(kRed);
  cout << "tt "<< bkg_tt->GetEntries() << "\n";
  cout << "tt Integral "<< bkg_tt->Integral() << "\n";

  bkg_DY->Merge(list_DY);
  bkg_DY->SetFillColor(kCyan);
  cout << "DY "<< bkg_DY->GetEntries() << "\n";
  cout << "DY Integral "<< bkg_DY->Integral() << "\n";

  bkg_all->Merge(list_all);
  bkg_all->SetFillColor(11);
  cout << "bkg all "<<bkg_all->GetEntries()<< "\n";

  cout << "signal all "<<signal_all->GetEntries()<< "\n";


  TPaveText *ll = new TPaveText(0.10, 0.91, 0.95, 0.99, "NDC"); //0.15 0.95 0.95 0.99
  ll->SetTextSize(0.024);
  ll->SetTextFont(42);
  ll->SetFillColor(0);
  ll->SetBorderSize(0);
  ll->SetMargin(0); //0.01
  ll->SetTextAlign(12); //12 align left
  TString text = "CMS Preliminary";
  //TString text = "CMS";
  ll->AddText(0.01,0.5,text); //0.01,0.5

  text = "#sqrt{s} = 13 TeV, L = 40 fb^{-1}";
  ll->AddText(0.67,0.6,text);//0.65,0.6

  TCanvas *ciao = new TCanvas("ciao","ciao",900,900,900,900); 
  ciao->cd();
  ciao->SetLogy();
  
  THStack *mass4l = new THStack("bdisccontrol4mu","");
  TH1D *framemass4l = new TH1D("","", frame_bins, frame_min, frame_max);// 200, 70, 200);//200 , 0 , 200);///280,20,300
  //ciao->SetLogy(1);
  TLine *line = new TLine(0,0,0,10);
  framemass4l->GetXaxis()->SetTitle("labelX");
  framemass4l->GetYaxis()->SetTitle("labelY");
  framemass4l->SetStats(0);
  framemass4l->SetMaximum(6e+6);//120e+2//5e+5//1500///50000//10
  framemass4l->SetMinimum(0.1);  

  bkg_VVV->Rebin(num_rebin);
  bkg_HV->Rebin(num_rebin);
  bkg_ttV->Rebin(num_rebin);
  bkg_H->Rebin(num_rebin);
  bkg_VV->Rebin(num_rebin);
  bkg_tt->Rebin(num_rebin);
  bkg_ZZ->Rebin(num_rebin);
  bkg_DY->Rebin(num_rebin);
  total_data->Rebin(num_rebin);
  bkg_others->Rebin(num_rebin);
  bkg_all->Rebin(num_rebin);
  signal_all->Rebin(num_rebin);

  //mass4l->Add(bkg_tt);
  //mass4l->Add(bkg_ZZ);
  //mass4l->Add(bkg_DY);
  //mass4l->Add(bkg_ttV);
  //mass4l->Add(bkg_others);
  mass4l->Add(bkg_ttV);
  mass4l->Add(bkg_H);
  mass4l->Add(bkg_others);
  mass4l->Add(bkg_ZZ);
  mass4l->Add(bkg_DY);

  framemass4l->Draw();
  mass4l->Draw("same HISTO");
  //line->Draw("same HISTO");
  total_data->Draw("same EP");

  gPad->Update();
  gPad->RedrawAxis();
  
  TH1D *frame2 = new TH1D("","", frame_bins, frame_min, frame_max);//200, 70, 200);//100 , 0, 10);///280,20,300
  frame2->SetMaximum(2);
  frame2->SetMinimum(0);
  frame2->SetStats(0);
  frame2->GetYaxis()->SetTitle("Data / mc");
  frame2->GetYaxis()->SetNdivisions(508);
  TH1D *h3 = (TH1D*)total_data->Clone("h3");
  h3->Sumw2();
  h3->Divide(bkg_all);
  h3->SetMarkerStyle(8);
  h3->SetMarkerColor(kBlack);
  h3->SetLineColor(kBlack);
  double canvasratio = 0.3;
  ciao->SetBottomMargin(canvasratio + (1-canvasratio)*ciao->GetBottomMargin()-canvasratio*ciao->GetTopMargin());
  canvasratio = 0.16;
  TPad *ratioPad = new TPad("BottomPad","",0,0,1,1);
  ratioPad->SetTopMargin((1-canvasratio) - (1-canvasratio)*ratioPad->GetBottomMargin()+canvasratio*ratioPad->GetTopMargin());
  ratioPad->SetFillStyle(4000);
  ratioPad->SetFillColor(4000);
  ratioPad->SetFrameFillColor(4000);
  ratioPad->SetFrameFillStyle(4000);
  ratioPad->SetFrameBorderMode(0);
  //ratioPad->SetTicks(1,1);
  ratioPad->SetGrid(1,1);
  //ratioPad->SetLogx();
  ratioPad->Draw("HISTO");
  frame2->Draw("same HISTO");
  //bkg_all->Draw("EPsame HISTO");
  ratioPad->cd();
  frame2->Draw("same HISTO");
  h3->Draw("EPsame HISTO"); 

  
  TLegend *legendmass4l = new TLegend(0.25,0.70,0.45,0.89);//0.47,0.75,0.67,0.89  ,NULL,"brNDC");//0.47,0.75,0.67,0.89,//0.72,0.7,0.95,0.92 //0.45,0.70,0.65,0.85 //0.1,0.7,0.48,0.9//0.1,0.7,0.48,0.9,//0.47,0.69,0.67,0.86,
  //legendmass4l = new TLegend(0.30,0.50,0.50,0.67,NULL,"brNDC");//0.50,0.70,0.70,0.87
  legendmass4l->SetTextSize(0.030);
  legendmass4l->SetLineColor(0);
  legendmass4l->SetLineWidth(1);
  legendmass4l->SetFillColor(kWhite);
  legendmass4l->SetBorderSize(0);
  
  legendmass4l->AddEntry(bkg_ZZ,"gg#rightarrowZZ#rightarrow4l, qq#rightarrowZZ#rightarrow4l","f");
  legendmass4l->AddEntry(bkg_H,"gg#rightarrowH, qq#rightarrowH m_{H}= 125 GeV","f");
  legendmass4l->AddEntry(bkg_ttV,"ttV where V = W, Z","f");
  legendmass4l->AddEntry(bkg_others,"HV, VVV, VV, where V = Z, W","f");
  legendmass4l->AddEntry(bkg_DY,"DY","f");
  legendmass4l->AddEntry(total_data,"Data","lep");
  legendmass4l->Draw("HISTO");
  
  ll->Draw("HISTO");

  ciao->SaveAs("plots/name_canvas.png");

}


