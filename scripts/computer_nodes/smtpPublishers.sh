#!/bin/bash

### This will be used to send workload information from each computer to the central node.
### It uses smtp to send the information, which will be parsed and written to the centralized database.
### It uses netcat, which should be present in all Debian 9 machines in the network.

from="kaylani@gta.ufrj.br"
to="kaylani@gta.ufrj.br"
server="gta.ufrj.br"
port="25"


########### Get information
free -h | grep Mem: > mem.txt
mpstat | grep all > cpu.txt ## may not be available in all machines
## /\ it is not available on raspbian or mint
cat /proc/cpuinfo > cpu_info.txt
lshw  > machine_info.txt ## this gives most of the info, parsing it should almost be enough
lscpu > cpu_info_lscpu.txt ## may not be available in all machines
## /\ it is not available on mint
lspci > pci.txt
## /\ it is not available on raspbian
top -b -n 1 > top.txt
## \/ Get bios information
#sudo dmidecode -t bios
paste <(cat /sys/class/thermal/thermal_zone*/type) <(cat /sys/class/thermal/thermal_zone*/temp) | column -s $'\t' -t | sed 's/\(.\)..$/.\1C/' > temp.txt


########## Process information
#sed -i ':a;N;$!ba;s/\n/ /g' mem.txt # remove line feed
#sed -i 's/ \{1,\}/;/g' mem.txt
#sed -i ':a;N;$!ba;s/\n/ /g' cpu.txt # remove line feed
#sed -i 's/ \{1,\}/;/g' cpu.txt
#sed -i ':a;N;$!ba;s/\n/ /g' temp.txt # remove line feed
#sed -i 's/ \{1,\}/;/g' temp.txt



########## Send information
myHostname=$(echo $HOSTNAME)
myHostname=(${myHostname//;/ })
freeMemory=$(cat mem.txt)
cpuUsage=$(cat cpu.txt)
temperature=$(cat temp.txt)

# create message
function mail_input {
echo "ehlo $(hostname -f)"
echo "MAIL FROM: <$from>"
echo "RCPT TO: <$to>"
echo "DATA"
echo "From: <$from>"
echo "To: <$to>"
echo "Subject: Testing SMTP Mail"
echo "This is only a test. Please do not panic. If this works, then all is well, else all is not well."
echo "<$freeMemory>"
echo "<$cpuUsage>"
echo "<$temperature>"
echo "<$myHostname>"
echo "."
echo "quit"
}
mail_input | netcat $server $port || err_exit

## CRONTAB:
## * * * * *  bash /home/kaylani/thisFile.sh
## /\ runs the script every minute using bash
