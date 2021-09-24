#! /usr/bin/env bash
##------------------------------------------------------------------------------

HORN=$1
GEO=$2
PROD=$3
TEST=$4
if [ "${HORN}" != "FHC" ] && [ "${HORN}" != "RHC" ]; then
echo "Invalid beam mode ${HORN}"
echo "Must be FHC or RHC"
kill -INT $$
fi

CP="ifdh cp"
MKDIR="ifdh mkdir"
MODE="neutrino"
if [ "${HORN}" = "RHC" ]; then
MODE="antineutrino"
fi

echo "Running cubeAnalysis for ${HORN} mode, geo ${GEO} and prod ${PROD}"

if [ "${TEST}" = "test" ]; then
PROCESS=4
fi

RNDSEED=${PROCESS}
GEOMETRY="full3DST"
OUTFLAG="full3DST"

NEUTRINO="neutrino"
RHC=""
if [ "${HORN}" = "RHC" ]; then
echo "Using RHC beam mode"
NEUTRINO="antineutrino"
RHC=" --rhc"
fi

##------------------------------------------------------------------------------
## Setup UPS and required products
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup dk2nu        v01_05_01b   -q e15:prof
setup genie        v2_12_10c    -q e15:prof
setup genie_xsec   v2_12_10     -q DefaultPlusValenciaMEC
setup genie_phyopt v2_12_10     -q dkcharmtau
setup geant4       v4_10_3_p01b -q e15:prof
setup ifdhc
##------------------------------------------------------------------------------

#INPUTTOP="/pnfs/dune/persistent/users/gyang/3DST"
INPUTTOP="/pnfs/dune/persistent/users/sgwon"
USERDIR="/pnfs/dune/persistent/users/sgwon"
TARBALLDIR="/pnfs/dune/resilient/users/sgwon"
OUTDIR="/pnfs/dune/persistent/users/sgwon/output/standard${GEO}/${PROD}"
#OUTDIR="/pnfs/dune/persistent/users/sgwon/output/T2Kstandard${GEO}/${PROD}"
${MKDIR} /pnfs/dune/persistent/users/sgwon/output/T2Kstandard${GEO}
${MKDIR} /pnfs/dune/persistent/users/sgwon/output/T2Kstandard${GEO}/${PROD}

## Fetch the cuberecon and genie output files
echo "ifdh ls ${INPUTTOP}/cuberecon/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root > ls1.txt"
ifdh ls ${INPUTTOP}/cuberecon/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root > ls1.txt
echo "ifdh ls ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).rootracker.root > ls2.txt"
ifdh ls ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).rootracker.root > ls2.txt
echo "ifdh ls ${INPUTTOP}/genie/standard${GEO}/${PROD}/reweight_merged.$(($RNDSEED+1)).root > ls3.txt"
ifdh ls ${INPUTTOP}/genie/standard${GEO}/${PROD}/reweight_merged.$(($RNDSEED+1)).root > ls3.txt
#echo "ifdh ls ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).reweight.root > ls3.txt"
#ifdh ls ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).reweight.root > ls3.txt
if [ -s ls1.txt ] && [ -s ls2.txt ] && [ -s ls3.txt ]; then
    echo "${CP} ${INPUTTOP}/cuberecon/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root input_cuberecon.root"
    ${CP} ${INPUTTOP}/cuberecon/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root input_cuberecon.root
    echo "${CP} ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).rootracker.root input_genie.root"
    ${CP} ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).rootracker.root input_genie.root
    echo "${CP} ${INPUTTOP}/genie/standard${GEO}/${PROD}/reweight_merged.$(($RNDSEED+1)).root input_genie_reweight.root"
    ${CP} ${INPUTTOP}/genie/standard${GEO}/${PROD}/reweight_merged.$(($RNDSEED+1)).root input_genie_reweight.root
    #echo "${CP} ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).reweight.root input_genie_reweight.root"
    #${CP} ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).reweight.root input_genie_reweight.root
else
    if [ ! -s ls1.txt ]
    then
       echo "OOPS! Could not find: ${INPUTTOP}/cuberecon/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root"
    fi
    if [ ! -s ls2.txt ]
    then
        echo "OOPS! Could not find: ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).rootracker.root"
    fi
    if [ ! -s ls3.txt ]
    then
        echo "OOPS! Could not find: ${INPUTTOP}/genie/standard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED+1)).reweight.root"
    fi
fi

##################################################
#echo "ifdh ls ${INPUTTOP}/cuberecon/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root > ls1.txt"
#ifdh ls ${INPUTTOP}/cuberecon/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root > ls1.txt
#echo "ifdh ls ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).rootracker.root > ls2.txt"
#ifdh ls ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).rootracker.root > ls2.txt
##echo "ifdh ls ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/mergedReweight_$(($RNDSEED+1)).root > ls3.txt"
##ifdh ls ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/mergedReweight_$(($RNDSEED+1)).root > ls3.txt
#echo "ifdh ls ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).reweight.root > ls3.txt"
#ifdh ls ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).reweight.root > ls3.txt
#if [ -s ls1.txt ] && [ -s ls2.txt ] && [ -s ls3.txt ]; then
#    echo "${CP} ${INPUTTOP}/cuberecon/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root input_cuberecon.root"
#    ${CP} ${INPUTTOP}/cuberecon/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root input_cuberecon.root
#    echo "${CP} ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).rootracker.root input_genie.root"
#    ${CP} ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).rootracker.root input_genie.root
#    #echo "${CP} ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/mergedReweight_$(($RNDSEED+1)).root input_genie_reweight.root"
#    #${CP} ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/mergedReweight_$(($RNDSEED+1)).root input_genie_reweight.root
#    echo "${CP} ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).reweight.root input_genie_reweight.root"
#    ${CP} ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).reweight.root input_genie_reweight.root
#else
#    if [ ! -s ls1.txt ]
#    then
#        echo "OOPS! Could not find: ${INPUTTOP}/cuberecon/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).cuberecon_newSize.root"
#    fi
#    if [ ! -s ls2.txt ]
#    then
#        echo "OOPS! Could not find: ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).rootracker.root"
#    fi
#    if [ ! -s ls3.txt ]
#    then
#        echo "OOPS! Could not find: ${INPUTTOP}/genie/T2Kstandard${GEO}/${PROD}/full3DST.${NEUTRINO}.$(($RNDSEED)).reweight.root"
#    fi
#fi

echo "${CP} ${TARBALLDIR}/CubeRecon_Dec2020.tar.gz ${PWD}/CubeRecon_Dec2020.tar.gz"
${CP} ${TARBALLDIR}/CubeRecon_Dec2020.tar.gz ${PWD}/CubeRecon_Dec2020.tar.gz
chmod +x CubeRecon_Dec2020.tar.gz
tar -xzf CubeRecon_Dec2020.tar.gz

chmod +x CubeRecon_Dec2020

export PATH=${PWD}/CubeRecon_Dec2020/build/app:${PATH}
export PATH=${PWD}/CubeRecon_Dec2020/build/analysis:${PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon_Dec2020/build/src:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon_Dec2020/build/display:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon_Dec2020/build/io:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon_Dec2020/build/analysis:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon_Dec2020/build/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon_Dec2020/build/tools:${LD_LIBRARY_PATH}
#export LD_LIBRARY_PATH=${TBB_LIB}:${LD_LIBRARY_PATH}
export ROOT_INCLUDE_PATH=${PWD}/CubeRecon_Dec2020/build/include/CubeRecon:${ROOT_INCLUDE_PATH}
export ROOT_INCLUDE_PATH=${PWD}/CubeRecon_Dec2020/build/include:${ROOT_INCLUDE_PATH}

##################################################

echo "${CP} ${USERDIR}/edep-sim.tar.gz ${PWD}/edep-sim.tar.gz"
${CP} ${USERDIR}/edep-sim.tar.gz ${PWD}/edep-sim.tar.gz
tar -xzf edep-sim.tar.gz

chmod +x edep-sim.tar.gz
chmod +x edep-sim

export LD_LIBRARY_PATH=${PWD}/edep-sim/build/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PWD}/edep-sim/build/io:${LD_LIBRARY_PATH}
export PATH=${PWD}/edep-sim/build/bin:${PATH}

echo "./CubeRecon_Dec2020/build/analysis/EventSelection"
##################################################
rm -f variableOutput.root
./CubeRecon_Dec2020/build/CubeReconAnalysis/EventSelection --input-cuberecon=input_cuberecon.root --input-genie=input_genie.root --input-genie-reweight=input_genie_reweight.root
if [ "${TEST}" = "test"]; then
    echo "test"
else
    ${CP} variableOutput.root ${OUTDIR}/${OUTFLAG}.${MODE}.$(($RNDSEED)).cutfile.root
fi
