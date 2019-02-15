## Author: Kaylani Bochie
## kaylani2 AT hotmail DOT com
## Written based on the official documentation on raspberrypi.org
## Remove the access point created by the ap script


###################### RASPBERRY PI UNDO AP CONNECTION ###################### 
echo "This script will reboot the system. Press ENTER to continue."
read uselessBuffer
## Remove created files
## Delete previous modifications

## Remove static IP address
sudo sed -i '/^interface wlan0/d' /etc/dhcpcd.conf
sudo sed -i '/^static ip_address=192.168.4.1\/24/d' /etc/dhcpcd.conf
sudo sed -i '/^nohook wpa_supplicant/d' /etc/dhcpcd.conf

sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig$(date +%Y%m%d%H%M%S) 

sudo rm /etc/hostapd/hostapd.conf

## Flush iptables
sudo iptables -F
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"
sudo sed -i '/^iptables-restore < \/etc\/iptables.ipv4.nat/d' /etc/rc.local

sudo ifconfig wlan0 down
sudo ifconfig wlan0 up

## Stop them running on startup
sudo systemctl stop hostapd
sudo systemctl stop dnsmasq
sudo systemctl disable hostapd
sudo systemctl disable dnsmasq

sudo shutdown -r now
