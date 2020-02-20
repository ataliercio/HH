

#!/bin/bash

if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ] || [ "$4" == "" ]; then
    echo "Please provide arguments to the script: site configuration, data type and MC type"
    echo "Usage bash loopcheck_bkg_<finalstate>.sh <arg1> <arg2> <arg3> <arg4>"
    exit      
fi

echo "$1 configuration";
echo "$2 data"
echo "$3 simulation"
echo "$4 site"

SCERN="CERN";
SFNAL="FNAL";
SDESY="DESY";
SBARI="BARI";

# Tar CMSSW enviroment
if  [ $1 = ${SBARI} ]; then
 macrosdir=`echo ${PWD}`
 tarname=`basename ${CMSSW_BASE}`
 cd ${CMSSW_BASE}; tar --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/macros/jobs4mu --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/macros/jobs4e --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/macros/jobs2e2mu --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/Signal_HH --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/Fall17 --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/mc_test --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/macros/For_trigeff/hlt_eff --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/macros/plot_HH --exclude=.git --exclude=tmp --exclude=src/hlt_eff --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/macros/angela --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/macros/plots --exclude=src/HiggsAnalysis/HiggsToZZ4Leptons/test/macros/CMSSW_9_4_9.tgz --exclude=src/HiggsAnalysis/HiggsToZZ4Leptonstest/macros/CMSSW_9_4_13_patch4.tgz -zcvf ${macrosdir}/${tarname}.tgz . #tar --exclude=.git --exclude=tmp -zcvf ${macrosdir}/${tarname}.tgz .
 cd ${macrosdir};
else
 tarname="dummy"
fi

###### Data
n=0;
m=0;

mkdir -p jobs$4;

echo "Reading data_input_$4_$2_AN_miniaod.txt file"

cp data_input_$4_$2_AN_miniaod.txt data_input.txt
nlines=`wc -l data_input_$4_$2_AN_miniaod.txt | awk '{print $1}'`;
echo "Number of files is " $nlines

while [ $n -lt ${nlines} ]; do
  (( n = n + 1 ))
  (( m = ${nlines} - n ))
  echo $n $m
  mkdir -p  DataCards$4$2
  rm -f DataCards$4$2/data_input_${n}.txt
  cat data_input.txt | head -1 > DataCards$4$2/data_input_${n}.txt
  samplename=`cat DataCards$4$2/data_input_${n}.txt | awk '{print $1}'`
  echo $samplename
  cat data_input.txt | tail -n $m >  data_input_tmp.txt
  mv  data_input_tmp.txt data_input.txt
  rm -f jobs4mu/submit_ReferenceAnalysis_${samplename}_4mu.sh
  if [ $1 = ${SCERN} ]; then
     cat submit_HZZ4LeptonsAnalysis_CERN.sh  | sed "s?site?$1?g" | sed "s?mc?$3?g" |sed "s?year?$2?g" | sed "s?HZZ4LeptonsAnalysis?RunReferenceAnalysis?g" | sed "s?jobdir?jobs4mu?g" | sed "s?histodir?histos4mu?g" | sed "s?output?output_${samplename}?g" | sed "s?jobs/?jobs4mu/?g" | sed "s?RunReferenceAnalysis?RunReference4mu_data?g" | sed "s?data_input.txt?DataCards4mu$2/data_input_${n}.txt?g" | sed "s?_log?_${samplename}_4mu.log?g" > jobs4mu/submit_ReferenceAnalysis_${samplename}_4mu.sh
  elif  [ $1 = ${SFNAL} ]; then 
      mkdir -p /eos/uscms/store/user/`whoami`/80X/jobs$4_25ns
      mkdir -p /eos/uscms/store/user/`whoami`/80X/histos$4_25ns
      cat submit_HZZ4LeptonsAnalysis_FNAL.sh | sed "s?site?$1?g" | sed "s?CMSSW_SEARCH_PATH_DIR?${CMSSW_SEARCH_PATH}?g" | sed "s?CMSSW_BASE_DIR?${CMSSW_BASE}?g" | sed "s?path?$PATH?g"  | sed "s?lib:?$LD_LIBRARY_PATH:?g" | sed "s?4mu?$4?g" | sed "s?mc?$3?g" |sed "s?year?$2?g" | sed "s?HZZ4LeptonsAnalysis?RunReferenceAnalysis_data?g" | sed "s?jobdir?jobs$4_25ns?g" | sed "s?histodir?histos$4_25ns?g" | sed "s?output?output_${samplename}?g" | sed "s?RunReferenceAnalysis?RunReference$4_data?g" | sed "s?data_input.txt?data_input_${n}.txt?g" | sed "s?_log?_${samplename}_$4.log?g" > jobs$4/submit_ReferenceAnalysis_data_${samplename}_$4.sh
      cat condor_template.cfg  | sed "s?4mu?$4?g" | sed "s?submit_HZZ4LeptonsAnalysis_BARI?submit_ReferenceAnalysis_data_${samplename}_$4?g" | sed "s?RunReferenceAnalysis?RunReference$4_data?g" | sed "s?sig_input_h150.txt?DataCards$4$2/data_input_${n}.txt?g" | sed "s?mail?`whoami`?g" > jobs$4/condor_ReferenceAnalysis_data_${samplename}_$4.cfg      
  elif  [ $1 = ${SDESY} ]; then 
      cat submit_HZZ4LeptonsAnalysis_DESY.sh | sed "s?site?$1?g" | sed "s?mc?$3?g" |sed "s?year?$2?g" | sed "s?HZZ4LeptonsAnalysis?RunReferenceAnalysis?g" | sed "s?jobdir?jobs4mu?g" | sed "s?histodir?histos4mu?g" | sed "s?output?output_${samplename}?g" | sed "s?jobs/?jobs4mu/?g" | sed "s?RunReferenceAnalysis?RunReference4mu_data?g" | sed "s?data_input.txt?DataCards4mu$2/data_input_${n}.txt?g" | sed "s?_log?_${samplename}_4mu.log?g" > jobs4mu/submit_ReferenceAnalysis_${samplename}_4mu.sh
  elif  [ $1 = ${SBARI} ]; then
     cat submit_HZZ4LeptonsAnalysis_BARI.sh | sed "s?site?$1?g" | sed "s?TARfile?${tarname}?g" | sed "s?CMSSW_BASE_DIR?${CMSSW_BASE}?g" | sed "s?path?$PATH?g"  | sed "s?lib:?$LD_LIBRARY_PATH:?g" | sed "s?4mu?$4?g" | sed "s?mc?$3?g" |sed "s?year?$2?g" | sed "s?HZZ4LeptonsAnalysis?RunReferenceAnalysis_data?g" | sed "s?jobdir?jobs$4_25ns_data_FR_step1_2017_questions_PU_new_new?g" | sed "s?histodir?histos$4_25ns_data_FR_step1_2017_questions_PU_new_new?g" | sed "s?output?output_${samplename}?g" | sed "s?RunReferenceAnalysis?RunReference$4_data?g" | sed "s?data_input.txt?data_input_${n}.txt?g" | sed "s?_log?_${samplename}_$4.log?g" > jobs$4/submit_ReferenceAnalysis_data_${samplename}_$4.sh
      cat condor_template.cfg  | sed "s?nameSample?${samplename}_$4?g"| sed "s?TARfile?${tarname}.tgz?g" | sed "s?4mu?$4?g" | sed "s?submit_HZZ4LeptonsAnalysis_BARI?submit_ReferenceAnalysis_data_${samplename}_$4?g" | sed "s?RunReferenceAnalysis?RunReference$4_data?g" | sed "s?sig_input_h150.txt?DataCards$4$2/data_input_${n}.txt?g" | sed "s?mail?`whoami`?g" > jobs$4/condor_ReferenceAnalysis_data_${samplename}_$4.cfg       
  else
      cat submit_HZZ4LeptonsAnalysis.sh | sed "s?mc?$3?g" |sed "s?year?$2?g" | sed "s?HZZ4LeptonsAnalysis?RunReferenceAnalysis?g" | sed "s?jobdir?jobs4mu_25ns?g" | sed "s?histodir?histos4mu_25ns?g" | sed "s?output?output_${samplename}?g" | sed "s?RunReferenceAnalysis?RunReference4mu_data?g" | sed "s?data_input.txt?DataCards4mu$2/data_input_${n}.txt?g" | sed "s?_log?_${samplename}_4mu.log?g" > jobs4mu/submit_ReferenceAnalysis_${samplename}_4mu.sh
  fi

  chmod u+xr jobs$4/submit_ReferenceAnalysis_data_${samplename}_$4.sh

  cd jobs$4

  if [ $1 = ${SCERN} ]; then
      echo "Submitting jobs4mu via LSF at CERN"
      bsub -q 8nh submit_ReferenceAnalysis_${samplename}_4mu.sh
  elif  [ $1 = ${SFNAL} ]; then
      echo "Submitting jobs4mu via CONDOR at FNAL"
      condor_submit condor_ReferenceAnalysis_data_${samplename}_$4.cfg
  elif  [ $1 = ${SDESY} ]; then
      echo "Submitting jobs via SGE"
      qsub submit_ReferenceAnalysis_${samplename}_4mu.sh   
  elif  [ $1 = ${SBARI} ]; then
      echo "Submitting jobs via CONDOR at BARI"
      condor_submit -name ettore  condor_ReferenceAnalysis_data_${samplename}_$4.cfg
      # condor_submit -name tmp condor_ReferenceAnalysis_data_${samplename}_$4.cfg	
  else
      echo "Submitting jobs4mu via PBS"    
      qsub -q local submit_ReferenceAnalysis_${samplename}_4mu.sh
  fi
  cd ..
done 

