#!/bin/bash

E_BADARGS=65

if [ ! -n "$1" ]
then 
	echo "usage: `basename $0` argument1 argument2 etc."
	exit $E_BADARGS
fi


echo "\$0=$0"
echo "\$1=$1"
echo "\$2=$2"

index=1
echo "listing args with \"\$*\":"

for arg in "$*"
do
	echo "Arg #$index=$arg"
	let "index+=1"
done

index=1
echo "listing args with \"\$@\":"

for arg in "$@"
do
	echo "Arg #$index=$arg"
	let "index+=1"
done

index=1
echo "listing args with \$*:"

for arg in $*
do
	echo "Arg #$index=$arg"
	let "index+=1"
done
