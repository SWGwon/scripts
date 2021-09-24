#! /usr/bin/env bash

####################

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

if [ "${NPER}" = "" ]; then
echo "Number of events per job not specified, using 1000"
NPER=1000
fi

CP="ifdh cp"
if [ "${TEST}" = "test" ]; then
echo "In TEST mode, assuming interactive running"
#CP="cp"
PROCESS=0
fi

MODE="neutrino"
if [ "${HORN}" = "RHC" ]; then
MODE="antineutrino"
fi

echo "Running gevgen for ${NPER} events in ${HORN} mode"

SPOT=10
SEEDING=$(($RANDOM%$SPOT+2944*$PROCESS ))
OFFPOS=1

RNDSEED=${PROCESS}
NEVENTS="-n ${NPER}"      # No. of events, -e XE16 for POT
#NEVENTS="-e 7.5E13"

GEOMETRY="full3DST"
#TOPVOL="volDetEnclosure"
TOPVOL="vol3DST"
#TOPVOL="volWorld"

#USERDIR="/pnfs/dune/persistent/users/marshalc/CAF"
USERDIR="/pnfs/dune/persistent/users/gyang/CAF"
#USERDIR="/nashome/g/gyang/DUNE_ND_CAF"
#OUTDIR="/dune/data/users/gyang/offAxis"
#OUTDIR="/pnfs/dune/persistent/users/sgwon/genie/standard${GEO}/${PROD}"

OUTDIR="/pnfs/dune/persistent/users/sgwon/genie/T2Kstandard${GEO}/${PROD}"
MKDIR="ifdh mkdir"
${MKDIR} /pnfs/dune/persistent/users/sgwon/genie/T2Kstandard${GEO}
${MKDIR} ${OUTDIR}

OUTFLAG="full3DST"

#CMC="G18_01a_00_000"
#cmc="G1801a00000"

#CMC="G18_02a_00_000"
#cmc="G1802a00000"

#CMC="G18_10a_00_000"
#cmc="G1810a00000"

CMC="G18_10i_00_000"
cmc="G1810i00000"

####################

## Setup UPS and required products

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

setup genie_xsec   v3_00_04a   -q ${cmc}:e1000:k250
setup genie_phyopt v3_00_04   -q dkcharmtau
setup dk2nugenie   v01_08_00f -q e17:prof
setup ifdhc

#setup genie_xsec   v3_00_04a   -q G1810b00000:e1000:k250
####################

## Copy stuff to the local node
${CP} ${USERDIR}/gevgen.tar.gz gevgen.tar.gz
tar -xzf gevgen.tar.gz

# tarball has contents in a folder to avoid tarbombing while testing
mv gevgen/* .

#${CP} ${USERDIR}/standard${GEO}.gdml full3DST.gdml
${CP} /pnfs/dune/persistent/users/sgwon/standardGeo15_fakeSFGD.gdml full3DST.gdml

#rm GNuMIFlux.xml
${CP} ${USERDIR}/GNuMIFlux.xml ./GNuMIFlux.xml
#${CP} ${USERDIR}/GNuMIFlux_$(($OFFPOS * 10000)).xml ./GNuMIFlux.xml

# Get flux files to local node
#${CP} ${USERDIR}/copy_dune_ndtf_flux_dk2nu copy_dune_ndtf_flux_dk2nu
chmod +x copy_dune_ndtf_flux
./copy_dune_ndtf_flux --top /pnfs/dune/persistent/users/dbrailsf/flux/nd/gsimple/v2_8_6d --outpath local_flux_files --flavor ${MODE} --base OptimizedEngineeredNov2017 --maxmb=200

####################

## Add the location of the GNuMIFlux.xml to the GENIE xml path

export GXMLPATH=${PWD}:${GXMLPATH}
export GNUMIXML="GNuMIFlux.xml"

## Run GENIE and copy output file to dCache persistent

gevgen_fnal \
    -f local_flux_files/gsimple*.root,DUNEND \
    -g ${GEOMETRY}.gdml \
    -t ${TOPVOL} \
    -m ${GEOMETRY}.${TOPVOL}.maxpl.xml \
    -L cm -D g_cm3 \
    ${NEVENTS} \
    --seed ${SEEDING} \
    -r ${RNDSEED} \
    -o ${MODE} \
    --message-thresholds Messenger_production.xml \
    --cross-sections ${GENIEXSECPATH}/gxspl-FNALsmall.xml \
    --tune ${CMC} \
	--event-record-print-level 0 \
    	##--event-generator-list Default+MEC
    --event-generator-list CC
##    --event-generator-list NuEElastic


echo "${CP} ${MODE}.${RNDSEED}.ghep.root ${OUTDIR}/${OUTFLAG}.${MODE}.${RNDSEED}.ghep.root"
gntpc -i ${MODE}.${RNDSEED}.ghep.root -o ${MODE}.${RNDSEED}.rootracker.root -f rootracker --tune ${CMC}
if [ "${TEST}" = "test" ]; then
    echo "test"
else
    ${CP} ${MODE}.${RNDSEED}.ghep.root ${OUTDIR}/${OUTFLAG}.${MODE}.${RNDSEED}.ghep.root
    ${CP} ${MODE}.${RNDSEED}.rootracker.root ${OUTDIR}/${OUTFLAG}.${MODE}.${RNDSEED}.rootracker.root
fi
