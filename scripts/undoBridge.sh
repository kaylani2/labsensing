## Author: Kaylani Bochie
## kaylani2 AT hotmail DOT com
## Written based on the official documentation on raspberrypi.org
## The Pi will reboot and you can connect again after a few seconds

###################### RASPBERRY PI UNDO BRIDGE CONNECTION ###################### 
echo "This script will reboot the system. Press ENTER to continue."
read uselessBuffer

## Reverse all modifications made to all files
## Remove all files created
sudo sed -i '/^denyinterfaces wlan0/d' /etc/dhcpcd.conf
sudo sed -i '/^denyinterfaces eth0/d' /etc/dhcpcd.conf
sudo sed -i '/^interface wlan0/d' /etc/dhcpcd.conf
sudo sed -i '/^static ip_address=192.168.4.1\/24/d' /etc/dhcpcd.conf

sudo sed -i '/^# Bridge setup/d' /etc/network/interfaces
sudo sed -i '/^auto br0/d' /etc/network/interfaces
sudo sed -i '/^iface br0 inet manual/d' /etc/network/interfaces
sudo sed -i '/^bridge_ports eth0 wlan0/d' /etc/network/interfaces

sudo rm /etc/hostapd/hostapd.conf

sudo sed -i 's/^DAEMON_CONF="\/etc\/hostapd\/hostapd.conf"/#DAEMON_CONF=""/g' /etc/default/hostapd


## Stop remaining services
sudo systemctl stop hostapd
#sudo systemctl stop dnsmasq

sudo sed -i 's/^net.ipv4.ip_forward=1/#net.ipv4.ip_forward=1/g' /etc/sysctl.conf

## Flush iptables
sudo iptables -F
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"
sudo sed -i '/^iptables-restore < \/etc\/iptables.ipv4.nat/d' /etc/rc.local


sudo sed -i '/^exit 0/d' /etc/rc.local
sudo sed -i '/^iptables-restore < \/etc\/iptables.ipv4.nat/d' /etc/rc.local
echo "exit 0" | sudo tee -a /etc/rc.local


## Remove bridge and reboot wireless interface
sudo brctl delif br0 eth0
sudo ifconfig br0 down
sudo brctl delbr br0
sudo ifconfig wlan0 down
sudo ifconfig wlan0 up
sudo ifconfig eth0 down
sudo ifconfig eth0 up

## Stop hostapd running on boot
sudo systemctl disable hostapd

sudo shutdown -r now
