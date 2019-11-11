#!/bin/bash

### This will be used to send workload information from each computer to the central node.
### It uses smtp to send the information, which will be parsed and written to the centralized database.
### It uses netcat, which should be present in all Debian 9 machines in the network.

from="kaylani@gta.ufrj.br"
to="kaylani@gta.ufrj.br"
server="gta.ufrj.br"
port="25"


############################## Get information ###############################

############################## Memory information ###############################
free -h > mem.txt
#free -h | grep Mem: > mem.txt ## grep will remove the headers



############################## CPU information ###############################
#mpstat | grep all > cpu.txt ## may not be available in all machines
## /\ it is not available on raspbian or mint, it's also not very descriptive
#lscpu > cpu_info_lscpu.txt ## may not be available in all machines
## cat /proc/cpuinfo > ... provides a more complete log
cat /proc/cpuinfo > cpu_info.txt
## /\ it is not available on mint

############################## General information ###############################
lshw  > machine_info.txt ## this gives most of the info, parsing it should almost be enough

############################## PCI information ###############################
lspci > pci.txt
## /\ it is not available on raspbian

############################## Processes information ###############################
top -b -n 1 > top.txt

############################## BIOS information ###############################
## \/ Get bios information
#sudo dmidecode -t bios

############################## Temperature information ###############################
paste <(cat /sys/class/thermal/thermal_zone*/type) <(cat /sys/class/thermal/thermal_zone*/temp) | column -s $'\t' -t | sed 's/\(.\)..$/.\1C/' > temp.txt

############################## Disk information ###############################
## \/ Get disk usage
df > disk.txt


########### Preprocess information before sending TODO
#sed -i ':a;N;$!ba;s/\n/ /g' mem.txt # remove line feed
#sed -i 's/ \{1,\}/;/g' mem.txt
#sed -i ':a;N;$!ba;s/\n/ /g' cpu.txt # remove line feed
#sed -i 's/ \{1,\}/;/g' cpu.txt
#sed -i ':a;N;$!ba;s/\n/ /g' temp.txt # remove line feed
#sed -i 's/ \{1,\}/;/g' temp.txt



########## Send information
#myHostname=$(echo $HOSTNAME)
#myHostname=(${myHostname//;/ })
#freeMemory=$(cat mem.txt)
#cpuUsage=$(cat cpu.txt)
#temperature=$(cat temp.txt)
#
## create message
#function mail_input {
#echo "ehlo $(hostname -f)"
#echo "MAIL FROM: <$from>"
#echo "RCPT TO: <$to>"
#echo "DATA"
#echo "From: <$from>"
#echo "To: <$to>"
#echo "Subject: Testing SMTP Mail"
#echo "This is only a test. Please do not panic. If this works, then all is well, else all is not well."
#echo "<$freeMemory>"
#echo "<$cpuUsage>"
#echo "<$temperature>"
#echo "<$myHostname>"
#echo "."
#echo "quit"
#}
#mail_input | netcat $server $port || err_exit

## CRONTAB:
## * * * * *  bash /home/kaylani/thisFile.sh
## /\ runs the script every minute using bash
