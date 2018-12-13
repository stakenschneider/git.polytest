#!/bin/bash
trap "echo Log start at: `date '+%Y-%m-%d %H:%M:%S'` >> read.txt" SIGINT
echo Log start at: `date '+%Y-%m-%d %H:%M:%S'` > read.txt
var1=5

while [ $var1 -gt 0 ]
do

	host = $(hostname -s)
	read in
	echo $host >> read.txt
	basename `pwd`: >> read.txt
	echo -n $in >> read.txt 
	$in | tee -a read.txt
	# echo -n $host
	# echo -n basename `pwd`
done



