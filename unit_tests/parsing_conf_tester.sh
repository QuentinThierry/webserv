#!/bin/bash

GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

good_message="OK"
tmp_file_name=".tmp_res"

rm -rf unit_tests/logs/*

launch_test()
{
	./webserv unit_tests/$1/$2 2>&1 > $tmp_file_name
	if [ "$3" == "TRUE" ]
	then
		if [ "`cat $tmp_file_name`" != "$good_message" ]
		then
			echo -e "$RED""NOT OK $2$RESET"
			cp $tmp_file_name unit_tests/logs/$2
			echo -e "\n\n----\nShould be OK" >> unit_tests/logs/$2
		else
			echo -e "$GREEN""OK $2$RESET"
		fi
	elif [ "$3" == "FALSE" ]
	then
		if [ "`cat $tmp_file_name`" == "$good_message" ]
		then
			echo -e "$RED""NOT OK $2$RESET"
			cp $tmp_file_name unit_tests/logs/$2
			echo -e "\n\n----\nShould not be OK" >> unit_tests/logs/$2
		else
			echo -e "$GREEN""OK $2$RESET"
		fi
	else
		echo Test argument error for \"$1$2 $3\"
	fi
}






launch_test conf_tester server1.conf FALSE
launch_test conf_tester server2.conf FALSE
launch_test conf_tester server3.conf FALSE
launch_test conf_tester server4.conf FALSE
launch_test conf_tester server5.conf FALSE
launch_test conf_tester server6.conf FALSE
launch_test conf_tester server7.conf FALSE
launch_test conf_tester server8.conf FALSE
launch_test conf_tester server9.conf FALSE
launch_test conf_tester server10.conf FALSE
launch_test conf_tester server11.conf FALSE
launch_test conf_tester server12.conf TRUE
launch_test conf_tester server13.conf FALSE
launch_test conf_tester server14.conf TRUE
launch_test conf_tester server15.conf FALSE
launch_test conf_tester server16.conf FALSE
launch_test conf_tester server17.conf FALSE
launch_test conf_tester server18.conf FALSE
launch_test conf_tester server19.conf FALSE
launch_test conf_tester server20.conf FALSE
launch_test conf_tester server21.conf TRUE






rm -rf $tmp_file_name