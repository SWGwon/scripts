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
if [ "${TEST}" = "test" ]; then
echo "Test mode"
#CP="cp"
PROCESS=0
fi

echo "Running edepsim for ${HORN} mode, ${NPER} events"

RNDSEED=$((${PROCESS}))

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

GEOMETRY="full3DST"
OUTFLAG="full3DST"

USERDIR="/pnfs/dune/persistent/users/gyang/CAF"
#INDIR="/pnfs/dune/persistent/users/sgwon/genie/standard${GEO}/${PROD}"
#OUTDIR="/pnfs/dune/persistent/users/sgwon/edep/standard${GEO}/${PROD}"

MKDIR="ifdh mkdir"

INDIR="/pnfs/dune/persistent/users/sgwon/genie/T2Kstandard${GEO}/${PROD}"
OUTDIR="/pnfs/dune/persistent/users/sgwon/edep/T2Kstandard${GEO}/${PROD}"

${MKDIR} /pnfs/dune/persistent/users/sgwon/edep/T2Kstandard${GEO}
${MKDIR} /pnfs/dune/persistent/users/sgwon/edep/T2Kstandard${GEO}/${PROD}

## Copy a tarball of all the code we need
ifdh ls ${INDIR}/${OUTFLAG}.${MODE}.$(($RNDSEED)).ghep.root > ls1.txt
if [ -s ls1.txt ]; then
    break
else
    echo " the file is not here "
fi

echo "${CP} ${INDIR}/${OUTFLAG}.${MODE}.$(($RNDSEED)).ghep.root input_file.ghep.root"
      ${CP} ${INDIR}/${OUTFLAG}.${MODE}.$(($RNDSEED)).ghep.root input_file.ghep.root

echo "${CP} ${USERDIR}/edep.tar.gz ${PWD}/edep.tar.gz"
      ${CP} ${USERDIR}/edep.tar.gz ${PWD}/edep.tar.gz
tar -xzf edep.tar.gz
mv edep/* .
${CP} ${USERDIR}/edep-sim4.tar.gz ${PWD}/edep-sim.tar.gz
tar -xzf edep-sim.tar.gz

rm full3DST.gdml
rm dune-nd.mac

#${CP} ${USERDIR}/standard${GEO}.gdml ${PWD}/full3DST.gdml
${CP} /pnfs/dune/persistent/users/sgwon/standardGeo15_fakeSFGD.gdml full3DST.gdml
${CP} ${USERDIR}/dune-nd.mac ${PWD}/dune-nd.mac

export LD_LIBRARY_PATH=${PWD}/edep-sim/build/lib:${LD_LIBRARY_PATH}
export PATH=${PWD}/edep-sim/build/bin:${PATH}

echo "About to run gntpc, ls:"
ls
echo "LD_LIBRARY_PATH:"
echo ${LD_LIBRARY_PATH}
gntpc -i input_file.ghep.root -f rootracker \
      --event-record-print-level 0 \
      --message-thresholds Messenger_production.xml

##################################################

# run all the events in a file
if [ "${NPER}" = -1 ]; then
echo "Specified all events, determining how many events there are"
NPER=$(echo "std::cout << gtree->GetEntries() << std::endl;" | genie -l -b input_file.ghep.root 2>/dev/null  | tail -1)
echo "There are ${NPER} events"
fi

## Run edep-sim
ls
echo "edep-sim -C -g ${GEOMETRY}.gdml -o ${PWD}/output_file.root -u -e ${NPER} dune-nd.mac"
edep-sim \
    -C \
    -g ${GEOMETRY}.gdml \
    -o ${PWD}/output_file.root \
    -e ${NPER} \
    -u \
    dune-nd.mac

## Copy output file to dCache persistent
echo "${CP} output_file.root ${OUTDIR}/${OUTFLAG}.${MODE}.$(($RNDSEED)).edepsim.root"
      ${CP} output_file.root ${OUTDIR}/${OUTFLAG}.${MODE}.$(($RNDSEED)).edepsim.root

##################################################


