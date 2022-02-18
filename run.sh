#!/bin/bash

# Global script constants
binaries_dir=binaries
tests_dir=test
long_results_dir=0
positional_args=()

# Parsing all arguments
while [[ $# -gt 0 ]]; do
	case $1 in
		-l | --long )
			long_results_dir=1
			shift
			;;
		-*|--*)
			echo "Unknown option $1"
			exit 1
			;;
		*)
			positional_args+=("$1") # save positional arg
			shift # past argument
			;;
	esac
done

# Positional arguments:
set -- ${positional_args[@]}
binary_name=$1
test_name=$2

if [[ -z $binary_name ]]; then
	echo "Please specify executable $1"
	exit 1
fi

# Create results directory
if [ $long_results_dir -eq 1 ]
then
	timestamp=$(date +"%I-%M")
	results_dir="results/${1}_${timestamp}_${RANDOM}"
else
	results_dir="results/${binary_name}"
fi
mkdir -p $results_dir
echo "Storing results to ${results_dir}"

# Copy binary to the results
cp ${binaries_dir}/${binary_name} ${results_dir}

# Save code archive to results
mkdir tmp
cp ./src/$1.cpp tmp
cp ./include/common.h tmp
cd tmp
tar -cvf "../${results_dir}/code.tar" $1.cpp common.h
cd ..
rm -r tmp

# Use executable from the results dir to run on tests
execulable=./${results_dir}/${binary_name}
if [[ -z $test_name ]]; then
	for filename in ./${tests_dir}/*.in; do
        echo "===================="
		echo "Running on $filename"
		$execulable < $filename > ./${results_dir}/$(basename "$filename" .in).ans
	done
else
	$execulable < ./${tests_dir}/${test_name}.in > ./${results_dir}/${test_name}.ans
fi
