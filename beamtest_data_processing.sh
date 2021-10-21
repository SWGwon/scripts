#! /usr/bin/env bash
##------------------------------------------------------------------------------

SFGD_FRAMEWORK=$1
DATAPATH=$2
MCR=$3

SCRIPTPWD=$(pwd)

cd ${SFGD_FRAMEWORK}/data_preprocessing/bin

echo "ls -1  ${DATAPATH}/*.daq | grep -v Slot > data_file.list"
ls -1  ${DATAPATH}/*.daq | grep -v Slot > data_file.list
for i in $(cat data_file.list); do
    echo "./TDMunpack -f $i"
    ./TDMunpack -f $i
done

echo "ls ${DATAPATH}/*Slot* > febs_files_list.list"
ls  ${DATAPATH}/*Slot* > febs_files_list.list
echo "./unpack –f d"
./unpack # –f d

#echo "ls -1 ${DATAPATH}/${MCR}*_raw.root > data_raw_file.list"
    #echo "${SFGD_FRAMEWORK}/data_preprocessing/bin/Calibration $i < RUN_CALIBRATION"
    #${SFGD_FRAMEWORK}/data_preprocessing/bin/Calibration $i < RUN_CALIBRATION
echo "./Calibration ${DATAPATH}/*__raw.root 55 50"
./Calibration ${DATAPATH}/*__raw.root 55 50
echo "./EventStructure ${DATAPATH}/*calib.root USJ"
./EventStructure ${DATAPATH}/*calib.root USJ

cd ${SCRIPTPWD}

#rm data_file.list
#rm febs_files_list.list
#rm data_raw_file.list
