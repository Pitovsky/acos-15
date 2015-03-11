#!/bin/bash

complex=0

rm answer.txt

for file in "$@"; do
if [ "$file" = "-i" -o "$file" = "--complex" ] 
then
    complex=1
else
    if [ -a "$file" ]
    then
	if [ "$complex" = 1 ] 
        then
            cat $file | ./sol -i | tr '.' ',' >> answer.txt
        else 
            cat $file | ./sol | tr '.' ',' >> answer.txt
        fi
    else 
        echo "File $file not found."
    fi
fi

done

echo "Answers are:"
cat answer.txt

echo -e "\nSort by biggest root:"
sort -k7 -g -k9 -g answer.txt > sorted.txt
cat sorted.txt

echo -e "\nOnly complex roots:"
awk '($9!~/([+-]0,000000)/) || ($4!~/[+-]0,000000/)  {print}' sorted.txt > complexrootsonly.txt
cat complexrootsonly.txt

echo -e "\nNumber of unique strings are:"
cat complexrootsonly.txt | uniq | wc -l

rm answer.txt sorted.txt complexrootsonly.txt
