#!/bin/bash

#Run it from root ibof directory
#eg bash ./script/wbtTestScript.sh
#cd $(dirname $0)
scriptPath=$(pwd)
echo "In Directory " $scriptPath

ROOT_DIR=$(readlink -f $(dirname $0))/../..

#Relative_Path_root="../.."
BIN_DIR=${ROOT_DIR}/bin

VM_IP_RANGE_1="10.1.11."
VM_IP_RANGE_2="10.100.11."

ARRAYNAME=POSArray

#####################################################################
fileSize1=4096
fileSize2=4096
fileName1="testFile1"
fileName2="testFile2"
fileOffset1=0
fileOffset2=0
#Dont put special chars in data Cause its going though Json parser.
#TODO: Find another way to send data to API
fileData1="sdfsfsdfsdfsdfsdfsdfsdfsdfsdfsfsdfsd.....ABCDEFGHIJKLMNOPQRSTUVWXYZ09876543211234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
fileData2="sdslkhspweoncmspwenajhdfvglqelkhsdsfisdkfjasdkfsdjghwgjwdsfsalghsgsoisligpiuoiuysalgytity53493534538r937085q34850802949wfhgjwl19035820r82wjhwejrhwkhfksdfhksdfsdfsadf"

dataLength1=$(expr ${#fileData1} + ${#fileData2})
dataLength2=$(expr ${#fileData1} + ${#fileData2})

#:TODO Need to get fileDesc from MFS util, but for now using this HARDCODED
fileDesc1=3
fileDesc2=4
inputFile="${scriptPath}/../wbtWriteBufferFile"
cliOutput="${scriptPath}/../wbtCliOutput.txt"
InodeOutput="${scriptPath}/../inodeInfo.json"
FilesInfoOutput="${scriptPath}/../filesInfo.json"
InodeInput="${scriptPath}/../inodeInfo.json"

echo "Input Buffer File " $inputFile
echo "Cli Output file " $cliOutput
#####################################################################
#read -p "Want to MountArray?. Enter y or n:" runMountArray
echo "Input in file.\n"
touch $inputFile
echo -n "$fileData1" >> $inputFile
echo -n "$fileData2" >> $inputFile

cwd="/home/ibof/ibofos/"
exec_mode=0
touch $cliOutput

nss="nqn.2019-04.ibof:subsystem1"

transport=tcp
target_ip=10.1.11.254
target_fabric_ip=10.100.11.254
target_port=1158

ip="10.100.11.25"
test_iteration=2000
totalsize=100 #pm : 12500
volcnt=4
test_time=3600
cpusallowed="10-11"

# array mode [normal, degraded]
arraymode="normal"
# shutdown type [none, spor, npor]
shutdowntype="none"
# rebuild mode [none, rebuild_before_gc, rebuild_after_gc]
rebuild="none"

exit_result=0

print_result()
{
    local result=$1
    local expectedResult=$2

    if [ $expectedResult -eq 0 ];then
        echo -e "\033[1;34m${date} [result] ${result} \033[0m" 1>&2;
    else
        echo -e "\033[1;41m${date} [TC failed] ${result} \033[0m" 1>&2;

        exit_result=1
    fi
}

check_result()
{
    #local result=$1
    #local expectedResult=$2

    cat ${cliOutput} | jq ".Response.result.status.code" > result.txt
    result=$(<result.txt)
    
    if [ ${result} -ne 0 ];then        
        print_result "there is a problem" 1        
    else
        print_result "CMD is working" 0
    fi
}


check_result_expected_fail()
{
    #local result=$1
    #local expectedResult=$2

    cat ${cliOutput} | jq ".Response.result.status.code" > result.txt
    result=$(<result.txt)
    
    if [ ${result} -ne 0 ];then        
        print_result "CMD is working" 0 
    else
        print_result "there is a problem" 1
    fi
}


pause()
{
    echo "Press any key to continue.."
    read -rsn1
}


while getopts "f:t:i:s:c:p:a:r:" opt
do
    case "$opt" in
        f) ip="$OPTARG"
            ;;
        t) test_time="$OPTARG"
            ;;
        i) test_iteration="$OPTARG"
            ;;
        s) totalsize="$OPTARG"
            ;;
        c) cpusallowed="$OPTARG"
            ;;
        p) shutdowntype="$OPTARG"
            ;;
        a) arraymode="$OPTARG"
            ;;
        r) rebuild="$OPTARG"
    esac
done


echo "------------[Kill & Start ibofos]----------------------------------"

sudo ${ROOT_DIR}/test/script/kill_ibofos.sh
sudo ${ROOT_DIR}/script/start_ibofos.sh
sleep 10

echo ------------[setup ibofos]-------------------------------------------

#sudo ${ROOT_DIR}/test/system/longterm/setup_ibofos.sh create ${arraymode} ${totalsize} ${volcnt} ${ip}

${ROOT_DIR}/test/system/io_path/setup_ibofos_nvmf_volume.sh -a ${ip}

echo ------------[setup Done]-------------------------------------------
echo -------------------------------------------------------------------


echo ------------[Map WBT CMDs]-------------------------------------------
volname="vol1"
volsize=21474836480

echo -[Map : get_map_layout ]------------------------------------------
${BIN_DIR}/cli wbt get_map_layout --json > ${cliOutput}
check_result

echo -[Map : read_vsamap ]------------------------------------------
${BIN_DIR}/cli wbt read_vsamap --name vol1 --output VSAMap_vol1.bin --json > ${cliOutput}
check_result

echo -[Map : write_vsamap ]------------------------------------------
${BIN_DIR}/cli wbt write_vsamap --name vol1 --input VSAMap_vol1.bin --json > ${cliOutput}
check_result

echo -[Map : read_vsamap_entry ]------------------------------------------
${BIN_DIR}/cli wbt read_vsamap_entry --name $volname --rba 0 --json > ${cliOutput}
check_result

echo -[Map : write_vsamap_entry ]------------------------------------------
${BIN_DIR}/cli wbt write_vsamap_entry --name $volname --rba 0 --vsid 1 --offset 1 --json > ${cliOutput}
check_result

echo -[Map : read_stripemap ]------------------------------------------
${BIN_DIR}/cli wbt read_stripemap --output StripeMap.bin --json > ${cliOutput}
check_result

echo -[Map : write_stripemap ]------------------------------------------
${BIN_DIR}/cli wbt write_stripemap --input StripeMap.bin --json > ${cliOutput}
check_result

echo -[Map : read_stripemap_entry ]------------------------------------------
${BIN_DIR}/cli wbt read_stripemap_entry --vsid 0 --json > ${cliOutput}
check_result

echo -[Map : write_stripemap_entry ]------------------------------------------
${BIN_DIR}/cli wbt write_stripemap_entry --vsid 0 --loc 1 --lsid 123 --json > ${cliOutput}
check_result

<<'END'
echo -[Map : read_whole_reverse_map ]------------------------------------------
${BIN_DIR}/cli wbt read_whole_reverse_map --output ReverseMapWhole.bin --json > ${cliOutput}
check_result

echo -[Map : write_whole_reverse_map ]------------------------------------------
${BIN_DIR}/cli wbt write_whole_reverse_map --input ReverseMapWhole.bin --json > ${cliOutput}
check_result
END

echo -[Map : read_reverse_map ]------------------------------------------
${BIN_DIR}/cli wbt read_reverse_map --vsid 0 --output ReverseMap_vsid0.bin --json > ${cliOutput}
check_result

echo -[Map : write_reverse_map ]------------------------------------------
${BIN_DIR}/cli wbt write_reverse_map --vsid 0 --input ReverseMap_vsid0.bin --json > ${cliOutput}
check_result


echo -[Map : read_reverse_map_entry ]------------------------------------------
${BIN_DIR}/cli wbt read_reverse_map_entry --vsid 0 --offset 0 --json > ${cliOutput}
check_result

echo -[Map : write_reverse_map_entry ]------------------------------------------
${BIN_DIR}/cli wbt write_reverse_map_entry --vsid 0 --offset 0 --rba 0 --name vol1 --json > ${cliOutput}
check_result

echo -[Map : get_bitmap_layout ]------------------------------------------
${BIN_DIR}/cli wbt get_bitmap_layout --json > ${cliOutput}
check_result

echo -[Map : get_instant_meta_info ]------------------------------------------
${BIN_DIR}/cli wbt get_instant_meta_info --json > ${cliOutput}
check_result

echo -[Map : get_wb_lsid_bitmap ]------------------------------------------
${BIN_DIR}/cli wbt get_wb_lsid_bitmap --output wbLsidBitmap.bin --json > ${cliOutput}
check_result

echo -[Map : set_wb_lsid_bitmap ]------------------------------------------
${BIN_DIR}/cli wbt set_wb_lsid_bitmap --input wbLsidBitmap.bin --json > ${cliOutput}
check_result

echo -[Map : get_active_stripe_tail ]------------------------------------------
${BIN_DIR}/cli wbt get_active_stripe_tail --output activeStripeTail.bin --json > ${cliOutput}
check_result

echo -[Map : set_active_stripe_tail ]------------------------------------------
${BIN_DIR}/cli wbt set_active_stripe_tail --input activeStripeTail.bin --json > ${cliOutput}
check_result

echo -[Map : get_current_ssd_lsid ]------------------------------------------
${BIN_DIR}/cli wbt get_current_ssd_lsid --output currentSsdLsid.bin --json > ${cliOutput}
check_result

echo -[Map : set_current_ssd_lsid ]------------------------------------------
${BIN_DIR}/cli wbt set_current_ssd_lsid --input currentSsdLsid.bin --json > ${cliOutput}
check_result

echo -[Map : get_user_segment_bitmap]------------------------------------------
${BIN_DIR}/cli wbt get_user_segment_bitmap --output segmentBitmap.bin --json > ${cliOutput}
check_result

echo -[Map : set_user_segment_bitmap]------------------------------------------
${BIN_DIR}/cli wbt set_user_segment_bitmap --input segmentBitmap.bin --json > ${cliOutput}
check_result

echo -[Map : get_segment_info]------------------------------------------
${BIN_DIR}/cli wbt get_segment_info --output segmentInfo.bin --json > ${cliOutput}
check_result

echo -[Map : set_segment_info]------------------------------------------
${BIN_DIR}/cli wbt set_segment_info --input segmentInfo.bin --json > ${cliOutput}
check_result

echo -[Map : get_segment_valid_count]------------------------------------------
${BIN_DIR}/cli wbt get_segment_valid_count --output segValidCount.bin --json > ${cliOutput}
check_result


echo -------------[User Data IO WBT CMDs]------------------------------------

MAXCOUNT=3
count=0
lbaIdx=0
lbaCnt=0

while [ "$count" -le $MAXCOUNT ] # ($MAXCOUNT) 개의 랜덤 정수 발생.
do    

    lbaIdx=$RANDOM 
    let "lbaIdx %= 1024*8"
    lbaCnt=$RANDOM 
    let "lbaCnt %= 1024"

    echo -[IO Path : unvme-ns-${count} : wbt write_raw]------------------------------------------

    ${BIN_DIR}/cli wbt write_raw --dev unvme-ns-${count} --lba ${lbaIdx} --count ${lbaCnt} --pattern 0xdeadbeef --output segValidCount.bin --json > ${cliOutput}
    check_result 

    if [[ "$ip" =~ "$VM_IP_RANGE_1" ]] || [[ "$ip" =~ "$VM_IP_RANGE_2" ]]; then
        
        echo -[IO Path : unvme-ns-${count} : wbt write_uncorrectable_lba ]------------------------------------------
        echo -[ wbt write_uncorrectable_lba is not supported at VM Test  ]------------------------------------------        

    else 

        echo -[IO Path : unvme-ns-${count} : wbt write_uncorrectable_lba ]------------------------------------------
        ${BIN_DIR}/cli wbt write_uncorrectable_lba --dev unvme-ns-${count} --lba ${lbaIdx} --output segValidCount.bin --json > ${cliOutput}
        check_result
    fi

    echo -[IO Path : unvme-ns-${count} : wbt flush]------------------------------------------
    ${BIN_DIR}/cli wbt flush --output segValidCount.bin --json > ${cliOutput}
 
    check_result

    

    echo -[IO Path : unvme-ns-${count} : wbt read_raw]------------------------------------------
    ${BIN_DIR}/cli wbt read_raw --dev unvme-ns-${count} --lba ${lbaIdx} --count ${lbaCnt} --output dump.bin --output segValidCount.bin --json > ${cliOutput}
 
    
    if [[ "$ip" =~ "$VM_IP_RANGE_1" ]] || [[ "$ip" =~ "$VM_IP_RANGE_2" ]]; then
        check_result
    else 
        check_result_expected_fail
    fi
  
    let "count += 1"  # 카운터 증가.
done  

echo --------------------------------------------------------------------

echo ------------[MFS WBT CMDs]------------------------------------------
echo --------------------------------------------------------------------

# In normal case, if the script executed mount array sequence, metafs creation/mount will get failed after 'mount_ibofos' execution
${BIN_DIR}/cli wbt mfs_create_filesystem
${BIN_DIR}/cli wbt mfs_mount_filesystem

${BIN_DIR}/cli wbt mfs_create_file --name $fileName1 --size $fileSize1 --integrity 0 --access 2 --operation 2
${BIN_DIR}/cli wbt mfs_open_file --name ${fileName1}  --json > ${cliOutput}
cat ${cliOutput} | jq ".Response.result.data.returnCode" > result.txt
fileDesc1=$(<result.txt)

${BIN_DIR}/cli wbt mfs_create_file --name $fileName2 --size $fileSize2 --integrity 0 --access 2 --operation 2
${BIN_DIR}/cli wbt mfs_open_file  --name $fileName2 --json >  ${cliOutput}
cat ${cliOutput} | jq ".Response.result.data.returnCode" > result.txt
fileDesc2=$(<result.txt)

 #make sure you are providing path also to inputFile.
${BIN_DIR}/cli wbt mfs_write_file --fd $fileDesc1 --offset $fileOffset1 --count $dataLength1 --input $inputFile
${BIN_DIR}/cli wbt mfs_read_file --fd $fileDesc1 --offset $fileOffset1 --count $dataLength1 --output mfs_read_one.bin

${BIN_DIR}/cli wbt mfs_write_file --fd $fileDesc2 --offset $fileOffset2 --count $dataLength2 --input $inputFile
${BIN_DIR}/cli wbt mfs_read_file --fd $fileDesc2 --offset $fileOffset2 --count $dataLength2 --output mfs_read_two.bin

${BIN_DIR}/cli wbt mfs_get_file_size --fd $fileDesc1 --json > ${cliOutput}
cat ${cliOutput} | jq ".Response.result.data.returnCode" > result.txt
fileSize=$(<result.txt)

${BIN_DIR}/cli wbt mfs_get_aligned_file_io_size --fd $fileDesc1 --json > ${cliOutput}
cat ${cliOutput} | jq ".Response.result.data.returnCode" > result.txt
AlignedFileIoSize=$(<result.txt)

${BIN_DIR}/cli wbt mfs_get_max_file_size --json > ${cliOutput}
cat ${cliOutput} | jq ".Response.result.data.returnCode" > result.txt
MaxFileSize=$(<result.txt)

echo -------------------------------------------------------
echo fileDesc1 = ${fileDesc1} fileDesc2 = ${fileDesc2}
echo fileSize = ${fileSize}   AlignedFileIOSize = ${AlignedFileIoSize}   MaxFileSize = ${MaxFileSize}
echo -------------------------------------------------------

${BIN_DIR}/cli wbt mfs_dump_files_list --output $FilesInfoOutput
echo ------- [opend files] ---------------------------------
sed 's/},{/\n /g' ../filesInfo.json > result.txt
cat ${scriptPath}/result.txt
echo -------------------------------------------------------

${BIN_DIR}/cli wbt mfs_dump_inode_info --name $fileName1 --output $InodeOutput
${BIN_DIR}/cli wbt mfs_dump_inode_info --name $fileName2 --output $InodeOutput

# make sure you are providing path also to inputFile.
${BIN_DIR}/cli wbt mfs_write_file --fd $fileDesc1 --offset $fileOffset1 --count $dataLength1 --input $inputFile
${BIN_DIR}/cli wbt mfs_read_file --fd $fileDesc1 --offset $fileOffset1 --count $dataLength1 --output mfs_read_one.bin

${BIN_DIR}/cli wbt mfs_write_file --fd $fileDesc2 --offset $fileOffset2 --count $dataLength2 --input $inputFile
${BIN_DIR}/cli wbt mfs_read_file --fd $fileDesc2 --offset $fileOffset2 --count $dataLength2 --output mfs_read_two.bin


${BIN_DIR}/cli volume unmount --name ${volname} --array $ARRAYNAME

for fidx in `seq 0 ${fileDesc2}`
do           
${BIN_DIR}/cli wbt mfs_close_file --fd $fidx

done

${BIN_DIR}/cli wbt mfs_unmount_filesystem
echo ------- [Created files] ------------
echo fileDesc1 = ${fileDesc1} fileDesc2 = ${fileDesc2} have closed

sed 's/},{/\n /g' $FilesInfoOutput > result.txt
cat ${scriptPath}/result.txt

echo ----------------------------------

rm -rf result.txt
rm -rf ${inputFile}
rm -rf ${cliOutput}

echo "------------[WBT Test End, Kill ibofos]----------------------------------"
sudo ${ROOT_DIR}/test/script/kill_ibofos.sh

if [ $exit_result -eq 0 ];then
    echo -[ Test Success ] -
else
    echo -[ Test Fail ] -
fi

exit $exit_result

