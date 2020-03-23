#!/bin/bash


mkdir -p /lustre/cms/store/user/atalierc/HH_2017_mc_data_signal/jobdir
mkdir -p /lustre/cms/store/user/atalierc/HH_2017_mc_data_signal/histodir

echo "Running HtoZZto4Leptons Analysis with executables RunHZZ4LeptonsAnalysis"


if [ -d "$_CONDOR_SCRATCH_DIR" ]; then
    workdir=`echo $_CONDOR_SCRATCH_DIR`;
    cd ${workdir};
else 
    workdir=`echo $PWD`;
    cd ${workdir};
fi

source /cvmfs/cms.cern.ch/cmsset_default.sh

scramv1 project CMSSW TARfile
cd TARfile     
tar -zxvf ../TARfile.tgz
rm ../TARfile.tgz
cd src
rm ../.SCRAM/arch/ProjectCache.db.gz
scramv1 b ProjectRename
eval `scramv1 runtime -sh`
cd ${workdir}

export LD_LIBRARY_PATH=${CMSSW_BASE}/src/ZZMatrixElement/MELA/data/$SCRAM_ARCH:$LD_LIBRARY_PATH

savedir=`echo /lustre/cms/store/user/atalierc/HH_2017_mc_data_signal/histodir`

echo "Working dir is $workdir"
#echo "Executable dir is $exedir"
echo "Saving dir is $savedir"

echo "Compiling the macros"
bash compilereference.sh 4mu


./RunReferenceAnalysis ./sig_input_h150.txt 1 ./bkg_input.txt 1 ./data_input.txt 1 site year mc >& ${workdir}/HZZ4LeptonsAnalysis_log
cp -f ${workdir}/HZZ4LeptonsAnalysis_log /lustre/cms/store/user/atalierc/HH_2017_mc_data_signal/jobdir/HZZ4LeptonsAnalysis_log

mv ${workdir}/output.root    ${savedir}/.
mv ${workdir}/output_bnn.txt ${savedir}/.
mv ${workdir}/output_bnn.root ${savedir}/.
mv ${workdir}/output_txt.txt ${savedir}/.
mv ${workdir}/output_txt_vbf.txt ${savedir}/.

if [ -d "$_CONDOR_SCRATCH_DIR" ]; then
 rm -f $_CONDOR_SCRATCH_DIR/*
fi
