## Author: Kaylani Bochie
## kaylani2 AT hotmail DOT com
## Written based on the official documentation on raspberrypi.org
## Configures the PI to serve a wireless network while connected
## through an ethernet connection to the internet
## Network SSID: WIFI_SSID
## Network password: NETWORK_PASSWORD
## Change accordingly


###################### RASPBERRY PI BRIDGE CONNECTION ###################### 
sudo apt-get -y install hostapd bridge-utils dnsmasq
sudo systemctl stop hostapd
## Stop eth0 and wlan0 ports being allocated IP addresses by the DHCP client
## on the Raspberry Pi
sudo sed -i '/^denyinterfaces wlan0/d' /etc/dhcpcd.conf
sudo sed -i '/^denyinterfaces eth0/d' /etc/dhcpcd.conf
echo "denyinterfaces wlan0" | sudo tee -a /etc/dhcpcd.conf
echo "denyinterfaces eth0" | sudo tee -a /etc/dhcpcd.conf
## Add a new bridge called br0
sudo brctl addbr br0
## Connect the network ports (eth0 to br0)
sudo brctl addif br0 eth0

sudo sed -i '/^# Bridge setup/d' /etc/network/interfaces
sudo sed -i '/^auto br0/d' /etc/network/interfaces
sudo sed -i '/^iface br0 inet manual/d' /etc/network/interfaces
sudo sed -i '/^bridge_ports eth0 wlan0/d' /etc/network/interfaces
echo "# Bridge setup" | sudo tee -a /etc/network/interfaces
echo "auto br0" | sudo tee -a /etc/network/interfaces
echo "iface br0 inet manual" | sudo tee -a /etc/network/interfaces
echo "bridge_ports eth0 wlan0" | sudo tee -a /etc/network/interfaces

sudo rm /etc/hostapd/hostapd.conf
sudo touch /etc/hostapd/hostapd.conf
echo "interface=wlan0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "bridge=br0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "#driver=nl80211" | sudo tee -a /etc/hostapd/hostapd.conf
echo "ssid=WIFI_SSID" | sudo tee -a /etc/hostapd/hostapd.conf
echo "hw_mode=g" | sudo tee -a /etc/hostapd/hostapd.conf
echo "channel=7" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wmm_enabled=0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "macaddr_acl=0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "auth_algs=1" | sudo tee -a /etc/hostapd/hostapd.conf
echo "ignore_broadcast_ssid=0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wpa=2" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wpa_passphrase=WIFI_PASSWORD" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wpa_key_mgmt=WPA-PSK" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wpa_pairwise=TKIP" | sudo tee -a /etc/hostapd/hostapd.conf
echo "rsn_pairwise=CCMP" | sudo tee -a /etc/hostapd/hostapd.conf

sudo sed -i 's/^#DAEMON_CONF=""/DAEMON_CONF="\/etc\/hostapd\/hostapd.conf"/g' /etc/default/hostapd

## Start remaining services
sudo systemctl start hostapd
sudo systemctl start dnsmasq

## Enable ipv4 forwarding
sudo sed -i 's/^#net.ipv4.ip_forward=1/net.ipv4.ip_forward=1/g' /etc/sysctl.conf

sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"

sudo sed -i '/^exit 0/d' /etc/rc.local
sudo sed -i '/^iptables-restore < \/etc\/iptables.ipv4.nat/d' /etc/rc.local
echo "iptables-restore < /etc/iptables.ipv4.nat" | sudo tee -a /etc/rc.local
echo "exit 0" | sudo tee -a /etc/rc.local
