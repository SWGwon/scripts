#! /usr/bin/env bash
##------------------------------------------------------------------------------

SFGD_FRAMEWORK=$1
DATAPATH=$2
MCR=$3

#echo "ls -1  ${DATAPATH}/*.daq | grep -v Slot > data_file.list"
ls -1  ${DATAPATH}/*.daq | grep -v Slot > data_file.list
for i in $(cat data_file.list); do
    echo "${SFGD_FRAMEWORK}/data_preprocessing/bin/TDMunpack -f $i"
    ${SFGD_FRAMEWORK}/data_preprocessing/bin/TDMunpack -f $i
done

#echo "ls -1  ${DATAPATH}/*Slot* > febs_files_list.list"
ls -1  ${DATAPATH}/*Slot* > febs_files_list.list
echo "${SFGD_FRAMEWORK}/data_preprocessing/bin/unpack –f d"
${SFGD_FRAMEWORK}/data_preprocessing/bin/unpack –f d

#echo "ls -1 ${DATAPATH}/${MCR}*_raw.root > data_raw_file.list"
ls -1 ${DATAPATH}/${MCR}*_raw.root > data_raw_file.list
echo "55" > RUN_CALIBRATION
echo "50" >> RUN_CALIBRATION
echo "0" >> RUN_CALIBRATION
for i in $(cat data_raw_file.list); do
    #echo "${SFGD_FRAMEWORK}/data_preprocessing/bin/Calibration $i < RUN_CALIBRATION"
    #${SFGD_FRAMEWORK}/data_preprocessing/bin/Calibration $i < RUN_CALIBRATION
    echo "${SFGD_FRAMEWORK}/data_preprocessing/bin/Calibration $i 55 50"
    ${SFGD_FRAMEWORK}/data_preprocessing/bin/Calibration $i 55 50
done
${SFGD_FRAMEWORK}/data_preprocessing/bin/EventStructure ${DATAPATH}/*calib.root USJ

#rm data_file.list
#rm febs_files_list.list
rm RUN_CALIBRATION
#rm data_raw_file.list
