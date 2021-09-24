#! /usr/bin/env bash

##################################################

HORN=$1
GEO=$2
PROD=$3
NPER=$4
TEST=$5
if [ "${HORN}" != "FHC" ] && [ "${HORN}" != "RHC" ]; then
echo "Invalid beam mode ${HORN}"
echo "Must be FHC or RHC"
kill -INT $$
fi

MODE="neutrino"
if [ "${HORN}" = "RHC" ]; then
MODE="antineutrino"
fi

if [ "${NPER}" = "" ]; then
echo "Number of events per job not specified, using 1000"
NPER=1000
fi

CP="ifdh cp"
MKDIR="ifdh mkdir"
if [ "${TEST}" = "test" ]; then
echo "Test mode"
#CP="cp"
PROCESS=0
fi

echo "Running edepsim for ${HORN} mode, ${NPER} events"

RNDSEED=$((${PROCESS}))

GEOMETRY="full3DST"
OUTFLAG="full3DST"

#INPUTTOP="/pnfs/dune/persistent/users/sgwon/edep/standard${GEO}/${PROD}"
INPUTTOP="/pnfs/dune/persistent/users/sgwon/edep/T2Kstandard${GEO}/${PROD}"
USERDIR="/pnfs/dune/persistent/users/gyang/CAF"
#OUTDIR="/pnfs/dune/persistent/users/sgwon/cuberecon/standard${GEO}/${PROD}"
#OUTDIR="/pnfs/dune/persistent/users/sgwon/cuberecon/standard${GEO}/${PROD}"
OUTDIR="/pnfs/dune/persistent/users/sgwon/cuberecon/T2Kstandard${GEO}/${PROD}"

NEUTRINO="neutrino"
RHC=""
if [ "${HORN}" = "RHC" ]; then
echo "Using RHC beam mode"
NEUTRINO="antineutrino"
RHC=" --rhc"
fi

##################################################

## Setup UPS and required products

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

setup dk2nu        v01_05_01b   -q e15:prof
setup genie        v2_12_10c    -q e15:prof
setup genie_xsec   v2_12_10     -q DefaultPlusValenciaMEC
setup genie_phyopt v2_12_10     -q dkcharmtau
setup geant4       v4_10_3_p01b -q e15:prof
setup ifdhc

##################################################
${MKDIR} /pnfs/dune/persistent/users/sgwon/cuberecon/T2Kstandard${GEO}
${MKDIR} /pnfs/dune/persistent/users/sgwon/cuberecon/T2Kstandard${GEO}/${PROD}
## Fetch the erep-sim and edep-sim output files
echo "Copying input files for runs ${FIRSTRUN} to ${LASTRUN}..."
ifdh ls ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).edepsim.root > ls1.txt
if [ -s ls1.txt ]; then
    echo "${CP} ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).edepsim.root edep.$(($RNDSEED)).root"
          ${CP} ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).edepsim.root edep.$(($RNDSEED)).root
    break
else
    echo "OOPS! Could not find: ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).edepsim.root"
fi

##################################################

echo "${CP} ${USERDIR}/cubeRecon.tar.gz ${PWD}/cubeRecon.tar.gz"
${CP} ${USERDIR}/CubeRecon_Dec2020.tar.gz ${PWD}/cubeRecon.tar.gz
tar -xzf cubeRecon.tar.gz
mv CubeRecon_Dec2020 CubeRecon
export PATH=${PWD}/CubeRecon/build/app:${PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon/build/src:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon/build/display:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${PWD}/CubeRecon/build/io:${LD_LIBRARY_PATH}
export ROOT_INCLUDE_PATH=${PWD}/CubeRecon/build/include/CubeRecon:${ROOT_INCLUDE_PATH}
export ROOT_INCLUDE_PATH=${PWD}/CubeRecon/build/include:${ROOT_INCLUDE_PATH}


echo "${CP} ${USERDIR}/erep-sim.tar.gz ${PWD}/erep-sim.tar.gz"
${CP} ${USERDIR}/erep-sim.tar.gz ${PWD}/erep-sim.tar.gz
tar -xzf erep-sim.tar.gz
export PATH=${PWD}/erep-sim/build/app:${PATH}
export LD_LIBRARY_PATH=${PWD}/erep-sim/build/src:${LD_LIBRARY_PATH}

echo "${CP} ${USERDIR}/edep-sim2.tar.gz ${PWD}/edep-sim.tar.gz"
${CP} ${USERDIR}/edep-sim2.tar.gz ${PWD}/edep-sim.tar.gz
tar -xzf edep-sim.tar.gz
export LD_LIBRARY_PATH=${PWD}/edep-sim/build/lib:${LD_LIBRARY_PATH}
export PATH=${PWD}/edep-sim/build/bin:${PATH}

##################################################

./erep-sim/build/app/erep-sim -o ${PWD}/erep.$(($RNDSEED)).root -n ${NPER} ${PWD}/edep.$(($RNDSEED)).root
./CubeRecon/build/app/cubeERepTranslate erep.$(($RNDSEED)).root out.root 
./CubeRecon/build/app/cubeRecon out.root output_cuberecon.root -n ${NPER}

## Copy output file to dCache persistent
${CP} output_cuberecon.root ${OUTDIR}/${OUTFLAG}.${MODE}.$(($RNDSEED)).cuberecon_newSize.root

##################################################
