## Author: Kaylani Bochie
## kaylani2 AT hotmail DOT com
## Credits to Stuart Childs and Richard Wenner
## TODO: configure SSL connection

##################### MOSQUITTO SCRIPT ##################### 
## Install the broker and the clients
## We can get the latest version using:
sudo wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
sudo apt-key add mosquitto-repo.gpg.key
# cd /etc/apt/sources.list.d/
# sudo wget http://repo.mosquitto.org/debian/mosquitto-stretch.list
sudo apt-get update
sudo apt install -y mosquitto mosquitto-clients
## Check mosquitto instalation
# mosquitto -v
## If you get "Error: Address already in use", it's because mosquitto is already running. Everything is fine.## The MQTT broker is now operating on TCP port 1883

## Secure mosquitto with password
## Stop mosquitto first
sudo /etc/init.d/mosquitto stop
## Create a config file
cd /etc/mosquitto/conf.d/
sudo touch mosquitto.conf
## Stop anonymous clients connecting
echo "allow_anonymous false" | sudo tee -a mosquitto.conf
echo "password_file /etc/mosquitto/conf.d/passwd" | sudo tee -a mosquitto.conf
## This must be changed later to use SSL
## Passwords are currently being sent in plain text
echo "require_certificate false" | sudo tee -a mosquitto.conf
## Create a password file
sudo touch passwd
## This line will prompt you to input and confirm a new password
sudo mosquitto_passwd -c /etc/mosquitto/conf.d/passwd $USER
## The broker will request the password after it has been restarted
## Testing mosquitto
## Open three terminal windows. In one, make sure the Mosquitto broker is running:
# sudo mosquitto
## In the next terminal, run the command line subscriber:
# mosquitto_sub -v -t 'topic/test'
## You should see the first terminal window echo that a new client is connected.
## In the next terminal, run the command line publisher:
# sudo mosquitto_pub -t 'topic/test' -m 'helloWorld'
## You should see another message in the first terminal window saying another client is connected. You should also see this message in the subscriber terminal:
## topic/test helloWorld
## We have shown that Mosquitto is configured correctly and we can both publish and subscribe to a topic.
## Restart mosquitto to enforce new security measures
sudo /etc/init.d/mosquitto stop
sudo /etc/init.d/mosquitto start
##################### END MOSQUITTO SCRIPT ##################### 
