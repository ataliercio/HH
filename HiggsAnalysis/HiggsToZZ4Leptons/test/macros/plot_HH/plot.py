#!/usr/bin/env python
import yaml
import os, re

input_data='data_2017_4mu.txt'
input_bkg='bkg_4mu_2017_new.txt'

with open('plotter.yml','r') as f:
    doc = yaml.load(f)
    histos = doc['histos'].keys()
       
for histo in histos:

    input_histo = doc['histos'][histo]['name_histo']
    input_Nbins = doc['histos'][histo]['Nbins']
    input_Xmin = doc['histos'][histo]['Xmin']
    input_Xmax = doc['histos'][histo]['Xmax']
    input_name_canvas = histo
    input_labelX = doc['histos'][histo]['labelX']
    input_labelY = doc['histos'][histo]['labelY']
    input_frame_Nbins = doc['histos'][histo]['frame_Nbins']
    input_frame_Xmin = doc['histos'][histo]['frame_Xmin']
    input_frame_Xmax = doc['histos'][histo]['frame_Xmax']
    input_rebin = doc['histos'][histo]['rebin']
#    print input_histo
#    print input_Nbins
#    print input_Xmin
#    print input_Xmax
    os.system("cat Plot_macro_new.C | sed 's?histo_name?"+input_histo+"?g' | sed 's?input_data.txt?"+input_data+"?g'| sed 's?input_bkg.txt?"+input_bkg+"?g'  | sed 's?Nbins?"+str(input_Nbins)+"?g' | sed 's?Xmin?"+str(input_Xmin)+"?g' | sed 's?Xmax?"+str(input_Xmax)+"?g' | sed 's?frame_bins?"+str(input_frame_Nbins)+"?g' | sed 's?frame_min?"+str(input_frame_Xmin)+"?g' | sed 's?frame_max?"+str(input_frame_Xmax)+"?g' | sed 's?name_canvas?"+input_name_canvas+"?g' | sed 's?labelX?"+input_labelX+"?g' | sed 's?labelY?"+input_labelY+"?g' | sed 's?num_rebin?"+str(input_rebin)+"?g' > Plot_macro_plot.C")
    os.system("root -l Plot_macro_plot.C")
    #os.system("root -l .q") 

