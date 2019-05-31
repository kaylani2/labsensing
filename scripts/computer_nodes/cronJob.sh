free -h | grep Mem: > mem.txt
mpstat | grep all > cpu.txt
paste <(cat /sys/class/thermal/thermal_zone*/type) <(cat /sys/class/thermal/thermal_zone*/temp) | column -s $'\t' -t | sed 's/\(.\)..$/.\1°C/' > temp.txt

sed -i ':a;N;$!ba;s/\n/ /g' mem.txt # remove line feed
sed -i 's/ \{1,\}/;/g' mem.txt
sed -i ':a;N;$!ba;s/\n/ /g' cpu.txt # remove line feed
sed -i 's/ \{1,\}/;/g' cpu.txt
sed -i ':a;N;$!ba;s/\n/ /g' temp.txt # remove line feed
sed -i 's/ \{1,\}/;/g' temp.txt

myHostname=$(hostname)
#echo $myHostname
myHostName=(${myHostName//;/ })
#echo $myHostname

brokerIP=$(ping marechal -c 1 | grep PING)
#echo $brokerIP
brokerIP=$(echo $brokerIP | cut -d "(" -f2 | cut -d ")" -f1)
#echo $brokerIP

mosquitto_pub -t "gta/aquario/$myHostname/memoria" -u pi -P gta -h "$brokerIP" -f mem.txt
mosquitto_pub -t "gta/aquario/$myHostname/cpu" -u pi -P gta -h "$brokerIP" -f cpu.txt
mosquitto_pub -t "gta/aquario/$myHostname/temperatura" -u pi -P gta -h "$brokerIP" -f temp.txt

## CRONTAB:
## * * * * *  bash /home/kaylani/thisFile.sh
## /\ runs the script every minute using bash
