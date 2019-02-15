## Author: Kaylani Bochie
## kaylani2 AT hotmail DOT com
## Written based on the official documentation on raspberrypi.org
## Configures the PI to serve a wireless network while connected
## through an ethernet connection to the internet
## Network SSID: WIFI_SSID
## Network password: NETWORK_PASSWORD
## Change accordingly


###################### RASPBERRY PI BRIDGE CONNECTION ###################### 
echo "This script will reboot the system. Press ENTER to continue."
read uselessBuffer
sudo apt-get -y install hostapd dnsmasq
sudo systemctl stop hostapd
sudo systemctl stop dnsmasq
## You should reboot after installing, but it works...


## Configure static IP address
sudo sed -i '/^interface wlan0/d' /etc/dhcpcd.conf
sudo sed -i '/^static ip_address=192.168.4.1\/24/d' /etc/dhcpcd.conf
sudo sed -i '/^nohook wpa_supplicant/d' /etc/dhcpcd.conf
echo "interface wlan0" | sudo tee -a /etc/dhcpcd.conf
echo "static ip_address=192.168.4.1/24" | sudo tee -a /etc/dhcpcd.conf
echo "nohook wpa_supplicant" | sudo tee -a /etc/dhcpcd.conf

## Now restart the dhcpcd daemon and set up the new wlan0 configuration:
sudo service dhcpcd restart

## Setup the DHCP server 
sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig$(date +%Y%m%d%H%M%S) 
sudo touch /etc/dnsmasq.conf 
echo "interface=wlan0" | sudo tee -a /etc/dnsmasq.conf 
echo "dhcp-range=192.168.4.2,192.168.4.20,255.255.255.0,24h" | sudo tee -a /etc/dnsmasq.conf 


## Configuring the access point host software (hostapd):
sudo rm /etc/hostapd/hostapd.conf
sudo touch /etc/hostapd/hostapd.conf
echo "interface=wlan0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "driver=nl80211" | sudo tee -a /etc/hostapd/hostapd.conf
echo "ssid=NETWORK_SSID" | sudo tee -a /etc/hostapd/hostapd.conf
echo "hw_mode=g" | sudo tee -a /etc/hostapd/hostapd.conf
echo "channel=7" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wmm_enabled=0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "macaddr_acl=0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "auth_algs=1" | sudo tee -a /etc/hostapd/hostapd.conf
echo "ignore_broadcast_ssid=0" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wpa=2" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wpa_passphrase=NETWORK_PASSWORD" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wpa_key_mgmt=WPA-PSK" | sudo tee -a /etc/hostapd/hostapd.conf
echo "wpa_pairwise=TKIP" | sudo tee -a /etc/hostapd/hostapd.conf
echo "rsn_pairwise=CCMP" | sudo tee -a /etc/hostapd/hostapd.conf

## We still have to show the system the location of the configuration file:
sudo sed -i 's/^#DAEMON_CONF=""/DAEMON_CONF="\/etc\/hostapd\/hostapd.conf"/g' /etc/default/hostapd

## Start remaining services
sudo systemctl start hostapd
sudo systemctl start dnsmasq

## Add routing and masquerade
sudo sed -i 's/^#net.ipv4.ip_forward=1/net.ipv4.ip_forward=1/g' /etc/sysctl.conf
## Add a masquerade for outbound traffic on eth0:
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
## Save the iptables rule
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"

## To load the rule on boot, we need to edit the file /etc/rc.local
## and add the following:
sudo sed -i '/^exit 0/d' /etc/rc.local
sudo sed -i '/^iptables-restore < \/etc\/iptables.ipv4.nat/d' /etc/rc.local
echo "iptables-restore < /etc/iptables.ipv4.nat" | sudo tee -a /etc/rc.local
echo "exit 0" | sudo tee -a /etc/rc.local

## Make them run on startup
sudo systemctl enable hostapd
## dnsmasq will not run on startup
## TODO: add line to bashrc to start dnsmasq after the interfaces are initialized
#sudo systemctl enable dnsmasq

## I think you always have to reboot for some reason
sudo shutdown -r now
