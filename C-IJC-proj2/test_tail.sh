#!/bin/bash

ARRAY=()

#find /etc -exec ARRAY+='{}' \; > /dev/null

filename='files.txt'

uspesne=0;
neuspesne=0;
celkem=0

wc_uspesne=0;
wc_neuspesne=0;


nl=15

while read i; do
	tail -n $nl < "$i" > unix_tail_output.txt
	./tail -n $nl < "$i" > my_tail_output.txt
  #echo "ahoj" >> my_tail_output.txt
	diff unix_tail_output.txt my_tail_output.txt > /dev/null
	if [[ $? -ne 0 ]]; then
		echo $i se neshoduje;
		(( neuspesne += 1 ))
	else
		(( uspesne += 1 ))
	fi
	(( celkem += 1 ))

	prom=$(cat my_tail_output.txt | wc -l)
	prom1=$(cat unix_tail_output.txt | wc -l)

	if [[ $prom -eq $prom1 ]]; then
		(( wc_uspesne += 1))
	else
		(( wc_neuspesne += 1))
		echo wc error $i
	fi

done < $filename
echo uspesne $uspesne
echo neuspesne $neuspesne
echo celkem $celkem
echo wc_neuspesne $wc_neuspesne
echo wc_uspesne $wc_uspesne
