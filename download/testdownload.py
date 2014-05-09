import sys
import os
import ftpbyname
from adb import *

#myadb = ADB('C:\\Android\\android-sdk\\platform-tools\\adb')
myadb = ADB()
#myadb = ADB('C:\Android\android-sdk\platform-tools\adb')


print(myadb.wait_for_device())
print(myadb.sync_directory())
print(myadb.set_adb_root())
print(myadb.set_system_rw())

partitions=myadb.shell_command('cat /proc/partitions')
part=partitions.split('\n')
i=0;
blockname=[];
for line in part:
	i+=1
	if (i<=2):
		continue
	array=line.split()
	if (len(array)>1):
		blockname.append(array[3])




#print(blockname)

partitions=myadb.shell_command('ls -l /dev/block/platform/sdhci-tegra.3/by-name')
#print(partitions)
partmap={}

part = partitions.split('\n')
for line in part:
	array=line.split()
	
	if (len(array)>1):
		partmap[array[5]]=array[7]
partname='LNX'
parttowrite=''
#print(partmap.keys())
if partname in partmap.keys():
	parttowrite=partmap[partname]
	
if parttowrite=='':
	print("Error:Can not find the partititon")
	exit()
localfile='boot.img'
remotefile='/system/'+localfile


if ( not os.path.exists(localfile)):
	print("Error: can not find file %s" % localfile)
	remotepath='/home/songhao/tf4/out/target/product/dogwood/'
	ftpbyname.download_ftp_files([remotepath+localfile])
	#exit()


myadb.push_local_file(localfile,remotefile)
if 'No such file or directory' in myadb.get_error():
	print("Error: can not find file %s" % localfile)
	exit()


#partitions=myadb.shell_command('ls -l /dev/block/platform/sdhci-tegra.3/by-name')
if parttowrite != '':
	parttowrite='dd if=%s of=%s '% (remotefile ,parttowrite)
#print(parttowrite)
myadb.shell_command(parttowrite)


print(myadb.get_output())

	
	

