## Author: Kaylani Bochie
## kaylani2 AT hotmail DOT com
## Written based on the official documentation on raspberrypi.org
## Configures the PI to serve a wireless network while connected
## through an ethernet connection to the internet
## Network SSID: WIFI_SSID
## Network password: NETWORK_PASSWORD
## Change accordingly

## NOTE: dnsmasq wouldn't start on boot following the instructions
## from raspberrypi.org. The solution provided on
## https://bugs.launchpad.net/ubuntu/+source/dnsmasq/+bug/1531184
## did not work.
## The solution used was to, instead of configuring the IP on the dhcpcd.conf
## file, we use the /etc/network/interfaces as seen here:
## https://unix.stackexchange.com/questions/410833/i-am-not-able-to-start-dnsmasq-on-boot


###################### RASPBERRY PI BRIDGE CONNECTION ###################### 
echo "This script will reboot the system. Press ENTER to continue."
read uselessBuffer
sudo apt-get -y install hostapd dnsmasq
sudo systemctl stop hostapd
sudo systemctl stop dnsmasq
## You should reboot after installing, but it works...


## Configure static IP address
## This was removed, see note
#sudo sed -i '/^interface wlan0/d' /etc/dhcpcd.conf
#sudo sed -i '/^static ip_address=192.168.4.1\/24/d' /etc/dhcpcd.conf
#sudo sed -i '/^nohook wpa_supplicant/d' /etc/dhcpcd.conf
#echo "interface wlan0" | sudo tee -a /etc/dhcpcd.conf
#echo "static ip_address=192.168.4.1/24" | sudo tee -a /etc/dhcpcd.conf
#echo "nohook wpa_supplicant" | sudo tee -a /etc/dhcpcd.conf
## Instead:
sudo sed -i '/^auto wlan0/d' /etc/network/interfaces
sudo sed -i '/^iface wlan0 inet static/d' /etc/network/interfaces
sudo sed -i '/^address 192.168.4.1/d' /etc/network/interfaces
sudo sed -i '/^netmask 255.255.255.0/d' /etc/network/interfaces
sudo sed -i '/^auto eth0/d' /etc/network/interfaces
sudo sed -i '/^iface eth0 inet dhcp/d' /etc/network/interfaces
echo "auto wlan0" | sudo tee -a /etc/network/interfaces
echo "iface wlan0 inet static" | sudo tee -a /etc/network/interfaces
echo "address 192.168.4.1" | sudo tee -a /etc/network/interfaces
echo "netmask 255.255.255.0" | sudo tee -a /etc/network/interfaces
echo "auto eth0" | sudo tee -a /etc/network/interfaces
echo "iface eth0 inet dhcp" | sudo tee -a /etc/network/interfaces



## Now restart the dhcpcd daemon and set up the new wlan0 configuration:
#sudo service dhcpcd restart

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
#sudo systemctl start hostapd
#sudo systemctl start dnsmasq

## Add routing and masquerade
sudo sed -i 's/^#net.ipv4.ip_forward=1/net.ipv4.ip_forward=1/g' /etc/sysctl.conf
## Add a masquerade for outbound traffic on eth0:
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
## Save the iptables rule
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"

## To load the rule on boot, we need to edit the file /etc/rc.local
## and add the following:
## We'll also need to make dnsmasq run on boot since
## it does not work with sudo systemctl enable dnsmasq because
## it tries to access the interface before it (interface) gets up.
## We'll add to rc.local instead
sudo sed -i '/^exit 0/d' /etc/rc.local
sudo sed -i '/^iptables-restore < \/etc\/iptables.ipv4.nat/d' /etc/rc.local
#sudo sed -i '/^sudo systemctl start dnsmasq/d' /etc/rc.local
echo "iptables-restore < /etc/iptables.ipv4.nat" | sudo tee -a /etc/rc.local
#echo "sudo systemctl start dnsmasq" | sudo tee -a /etc/rc.local
echo "exit 0" | sudo tee -a /etc/rc.local

## Make them run on startup
sudo systemctl enable hostapd
## dnsmasq will not run on startup
## it tries to access the interface before it (interface) gets up
## TODO: add line to bashrc to start dnsmasq after the interfaces are initialized
sudo systemctl enable dnsmasq

sudo shutdown -r now
