#!/usr/bin/env python

import numpy
import os
#from ROOT import *
import ROOT

os.environ["KERAS_BACKEND"] = "tensorflow"

#import the dictionary
from sig_list import *
from bkg_list import *

#gSystem.Load("libblas.so")
ROOT.TMVA.Tools.Instance()
ROOT.TMVA.PyMethodBase.PyInitialize()


#create the outputfile
outputFile = ROOT.TFile.Open("Higgs_ClassificationOutput.root", "RECREATE")

factory = ROOT.TMVA.Factory("TMVA_Higgs_Classification", outputFile,
                            "!V:ROC:!Silent:Color:!DrawProgressBar:AnalysisType=Classification" )

#loading input variable and name of the input tree

tree_name = 'HZZ4LeptonsAnalysisReduced;1'
loader = ROOT.TMVA.DataLoader("dataset")

loader.AddVariable('f_lept1_pt', 'F')
loader.AddVariable('f_lept2_pt', 'F')
loader.AddVariable('f_lept3_pt', 'F')
loader.AddVariable('f_lept4_pt', 'F')
loader.AddVariable("f_deltar_norm",'F')
loader.AddVariable("f_MET_norm",'F')
loader.AddVariable("f_bdiscjet1",'F')
loader.AddVariable("f_bdiscjet2",'F')

#loading input trees and weight (signal and bkg)
#signal section
for dataset, infos in sig_input.items(): 
    name_dataset = infos[0]
    weight_dataset = infos[1]

    #opening the dataset and loading the tree
    input_file = ROOT.TFile(name_dataset)
    input_tree = input_file.Get(tree_name)
    loader.AddSignalTree(input_tree, weight_dataset)
    print("signal tree: %s", name_dataset)
    print("signal weight: ", weight_dataset)
#bkg section
for dataset, infos in bkg_input.items():
    name_dataset = infos[0]
    weight_dataset = infos[1]

    #opening the dataset and loading the tree
    input_file = ROOT.TFile(name_dataset)
    input_tree = input_file.Get(tree_name)
    loader.AddBackgroundTree(input_tree, weight_dataset)
    print("signal tree: %s", name_dataset)
    print("signal weight: ", weight_dataset)

#let's start the training!

#put some cut if you want
sig_cut = ROOT.TCut("")
bkg_cut = ROOT.TCut("")

loader.PrepareTrainingAndTestTree( sig_cut, bkg_cut,
                                    "nTrain_Signal=3000:nTrain_Background=3000:nTest_Signal=1000:nTest_Background=1000:SplitMode=Random:NormMode=NumEvents:!V" )

#please select the NN/BDT

BDT = True
MLP = True

#model = Sequential()

if(BDT is True):
    #factory.BookMethod(loader,ROOT.TMVA.Types.kBDT, "BDT","!H:!V:NTrees=800:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" )
#    factory.BookMethod(loader,ROOT.TMVA.Types.kBDT, "BDT",
#                   "!V:NTrees=200:MinNodeSize=2.5%:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:"
#                   "BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" )

 #   factory.BookMethod(loader, ROOT.TMVA.Types.kPyGTB, "PyGTB","H:!V:VarTransform=G:NEstimators=400:LearningRate=0.1:"
#                                                  "MaxDepth=3")

#    factory.BookMethod(loader, ROOT.TMVA.Types.kPyAdaBoost, "PyAdaBoost","!V:VarTransform=G:NEstimators=400" )
    factory.BookMethod(loader,ROOT.TMVA.Types.kBDT, "BDT_new", "!H:!V:NTrees=400:MinNodeSize=2.5%:BoostType=AdaBoost:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2")

if(MLP is True):
    factory.BookMethod(loader,ROOT.TMVA.Types.kMLP,"MLP","H:!V:NeuronType=sigmoid:VarTransform=Norm:NCycles=500;HiddenLayers=N:TestRate=5")

factory.TrainAllMethods()
factory.TestAllMethods()
factory.EvaluateAllMethods()

#plot the ROC curve
#%jsroot on
c1 = factory.GetROCCurve(loader)
ROOT.TMVA.TMVAGui()#"Higgs_ClassificationOutput.root")
c1.Draw()

#close the output file
#outputFile.close()
