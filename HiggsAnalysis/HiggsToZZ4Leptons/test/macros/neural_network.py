#!/usr/bin/env python

import numpy
import os
#from ROOT import *
import ROOT

ROOT.TMVA.Tools.Instance()
os.environ["KERAS_BACKEND"] = "tensorflow"

#import the dictionary
from sig_list import *
from bkg_list import *

#TMVA::Tools::Instance();


#gSystem.Load("libblas.so")
#ROOT.TMVA.Tools.Instance()
ROOT.TMVA.PyMethodBase.PyInitialize()


#create the outputfile
outputFile = ROOT.TFile.Open("Higgs_ClassificationOutput_try.root", "RECREATE")

factory = ROOT.TMVA.Factory("TMVA_Higgs_Classification", outputFile,
                            "!V:ROC:!Silent:Color:!DrawProgressBar:AnalysisType=Classification" )

#loading input variable and name of the input tree

tree_name = 'HZZ4LeptonsAnalysisReduced'
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
    print("signal tree: ", name_dataset)
    print("signal weight: ", weight_dataset)

#bkg section
for dataset, infos in bkg_input.items():
    name_dataset = infos[0]
    weight_dataset = infos[1]

    #opening the dataset and loading the tree
    input_file = ROOT.TFile(name_dataset)
    input_tree = input_file.Get(tree_name)
    loader.AddBackgroundTree(input_tree, weight_dataset)
    print("signal tree: ", name_dataset)
    print("signal weight: ", weight_dataset)

#let's start the training!

#put some cut if you want
sig_cut = ROOT.TCut("")#"f_deltaphi_norm > 0 && f_deltar_norm > 0 && f_MET_norm > 0 && f_bdiscjet1 >= 0 && f_bdiscjet2 >=0")
bkg_cut = ROOT.TCut("")#"f_deltaphi_norm > 0 && f_deltar_norm > 0 && f_MET_norm > 0 && f_bdiscjet1 >= 0 && f_bdiscjet2 >=0")

loader.PrepareTrainingAndTestTree( sig_cut, bkg_cut,
                                    "nTrain_Signal=3000:nTrain_Background=3000:nTest_Signal=1000:nTest_Background=1000:SplitMode=Random:NormMode=NumEvents:!V" )

#please select the MLP/BDT/NN

BDT = True
MLP = True
NN = False

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
    #factory.BookMethod(loader,ROOT.TMVA.Types.kMLP,"MLP","H:!V:NeuronType=sigmoid:VarTransform=Norm:NCycles=500;HiddenLayers=N:TestRate=5")
    factory.BookMethod(loader,ROOT.TMVA.Types.kMLP,"MLP_500_N-7","H:!V:NeuronType=sigmoid:VarTransform=Norm:NCycles=100;HiddenLayers=N-7:TestRate=5")

if(NN is True):
    from keras.models import Sequential
    from keras.optimizers import Adam, SGD
    from keras.layers import Input, Dense, Dropout, Flatten, Conv2D, MaxPooling2D, Reshape

    model = Sequential()
    model.add(Dense(64, kernel_initializer='glorot_normal', activation='tanh', input_dim=8))
    #model.add(Dense(64, kernel_initializer='glorot_normal', activation='tanh'))
    model.add(Dense(64, kernel_initializer='glorot_normal', activation='tanh'))
    model.add(Dense(2, kernel_initializer='glorot_uniform', activation='softmax'))
    model.compile(loss='categorical_crossentropy', optimizer=Adam(), metrics=['categorical_accuracy',])
    model.save('model_dense.h5')
    model.summary()
    
    factory.BookMethod(loader, ROOT.TMVA.Types.kPyKeras, 'Keras_Dense',
                   'H:!V:VarTransform=G:FilenameModel=./model_dense.h5:'+'NumEpochs=10:BatchSize=32:TriesEarlyStopping=10')

factory.TrainAllMethods()
factory.TestAllMethods()
factory.EvaluateAllMethods()

#plot the ROC curve
#%jsroot on
c1 = factory.GetROCCurve(loader)
ROOT.TMVA.TMVAGui("Higgs_ClassificationOutput_try.root")
c1.Draw()
