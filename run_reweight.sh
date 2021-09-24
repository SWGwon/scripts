#! /usr/bin/env bash
##------------------------------------------------------------------------------
HORN=$1
GEO=$2
PROD=$3
LOOP=$4
EVENT=$5
TEST=$6
CP="ifdh cp"

NEUTRINO="neutrino"
if [ "${HORN}" = "RHC" ]; then
echo "Using RHC beam mode"
NEUTRINO="antineutrino"
fi

if [ "${TEST}" = "test" ]; then
PROCESS=0
fi
RNDSEED=${PROCESS}
##------------------------------------------------------------------------------
## Setup UPS and required products
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
source /cvmfs/larsoft.opensciencegrid.org/products/root/v6_12_06a/Linux64bit+2.6-2.12-e15-prof/bin/thisroot.sh
setup dk2nu        v01_05_01b   -q e15:prof
setup genie        v2_12_10c    -q e15:prof
setup genie_xsec   v2_12_10     -q DefaultPlusValenciaMEC
setup genie_phyopt v2_12_10     -q dkcharmtau
setup geant4       v4_10_3_p01b -q e15:prof
setup dk2nugenie  v01_08_00f -q e17:prof
setup ifdhc
setup gcc v7_3_0
##------------------------------------------------------------------------------
TARBALLDIR="/pnfs/dune/resilient/users/sgwon"
INPUTTOP="/pnfs/dune/persistent/users/sgwon/genie/standard${GEO}/${PROD}"
OUTDIR="/pnfs/dune/persistent/users/sgwon/genie/standard${GEO}/${PROD}"
#INPUTTOP="/pnfs/dune/persistent/users/sgwon/genie/T2Kstandard${GEO}/${PROD}"
#OUTDIR="/pnfs/dune/persistent/users/sgwon/genie/T2Kstandard${GEO}/${PROD}"
##------------------------------------------------------------------------------
echo "ifdh ls ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).ghep.root > ls1.txt"
ifdh ls ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).ghep.root > ls1.txt
if [ -s ls1.txt ]; then
    echo "${CP} ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).ghep.root input_ghep.root"
    ${CP} ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).ghep.root input_ghep.root
else
    echo "OOPS! Could not find: ${INPUTTOP}/full3DST.${NEUTRINO}.$(($RNDSEED)).ghep.root"
fi

#echo "${CP} ${TARBALLDIR}/Generator.tar.gz ${PWD}/Generator.tar.gz"
#${CP} ${TARBALLDIR}/Generator.tar.gz ${PWD}/Generator.tar.gz
#chmod +x Generator.tar.gz
#tar -xzf Generator.tar.gz
#chmod +x Generator

echo "${CP} ${TARBALLDIR}/grwght1p ${PWD}/grwght1p"
${CP} ${TARBALLDIR}/grwght1p ${PWD}/grwght1p
chmod +x grwght1p

#./Generator/bin/grwght1p -f input_ghep.root -l ${LOOP} -n ${EVENT}
./grwght1p -f input_ghep.root -l ${LOOP} -n ${EVENT}
if [ "${TEST}" = "test" ]; then
    echo "test"
else
    ${CP} reweight.root ${OUTDIR}/full3DST.${NEUTRINO}.$(($RNDSEED)).reweight4.root
fi
