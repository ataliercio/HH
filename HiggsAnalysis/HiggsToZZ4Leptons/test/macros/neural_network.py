#!/usr/bin/env python

import numpy
import os
from ROOT import *

os.environ["KERAS_BACKEND"] = "tensorflow"

#import the dictionary
from sig_list import *
from bkg_list import *

#gSystem.Load("libblas.so")
TMVA.Tools.Instance()
TMVA.PyMethodBase.PyInitialize()


#create the outputfile
outputFile = TFile.Open("Higgs_ClassificationOutput.root", "RECREATE")

factory = TMVA.Factory("TMVA_Higgs_Classification", outputFile,
                            "!V:ROC:!Silent:Color:!DrawProgressBar:AnalysisType=Classification" )

#loading input variable and name of the input tree

tree_name = 'HZZ4LeptonsAnalysisReduced;1'
loader = TMVA.DataLoader("dataset")

loader.AddVariabile('f_lept1_pt')
loader.AddVariabile('f_lept2_pt', 'F')
loader.AddVariabile('f_lept3_pt', 'F')
loader.AddVariabile('f_lept4_pt', 'F')


#loading input trees and weight (signal and bkg)
#signal section
for dataset, infos in sig_files.items(): 
    name_dataset = infos[0]
    weight_dataset = infos[1]

    #opening the dataset and loading the tree
    input_file = TFile(name_dataset)
    input_tree = input_file.Get(tree_name)
    loader.AddSignalTree(input_tree, weight_dataset)

#bkg section
for dataset, infos in bkg_files.items():
    name_dataset = infos[0]
    weight_dataset = infos[1]

    #opening the dataset and loading the tree
    input_file = TFile(name_dataset)
    input_tree = input_file.Get(tree_name)
    loader.AddBackgroundTree(input_tree, weight_dataset)

#let's start the training!

#put some cut if you want
sig_cut = TCut()
bkg_cut = TCut()

loader.PrepareTrainingAndTestTree( mycuts, mycutb,
                                    "nTrain_Signal=3000:nTrain_Background=20000:nTest_Signal=3000:nTest_Background=3000:SplitMode=Random:NormMode=NumEvents:!V" )

#please select the NN/BDT

BDT = True
MLP = True

if(BDT is True):
    factory.BookMethod(loader,TMVA.Types.kBDT, "BDT","!H:!V:NTrees=800:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" )

if(MLP is True):
    factory.BookMethod(loader,TMVA.Types.kMLP,"MLP","H:!V:NeuronType=sigmoid:VarTransform=Norm:NCycles=500;HiddenLayers=N:TestRate=5")

factory.TrainAllMethods()
factory.TestAllMethods()
factory.EvaluateAllMethods()

#plot the ROC curve
#%jsroot on
c1 = factory.GetROCCurve(loader);
c1.Draw();

#close the output file
outputFile.close()
