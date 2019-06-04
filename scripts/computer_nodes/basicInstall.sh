#!/bin/bash

### This script stablishes an ssh connection with each of the hosts in the network, clone the repository and install the crontab corresponding with the desired scripts.
### Needs sshpass


## TODO: script must clone the repo and install the new cronjob
## TODO: test to see if the cronjob is being installed properly
INSTALL_CRONJOB='(crontab -l 2>/dev/null; echo "0,10,20,30,40,50 * * * * cd ~/labsensing/scripts/computer_nodes/ && bash smtpPublishers.sh >> ~/labsensing/scripts/computer_nodes/logs.txt 2>&1") | crontab -'
TOUCH="touch it_werks"
SCRIPT="wall running scripts; cd ~/; rm -rf labsensing; git clone https://github.com/kaylani2/labsensing; cd labsensing/scripts/computer_nodes; $TOUCH;"
CLEAN="cd ~/; rm -rf labsensing"
HOSTS="ponte sepetiba"
USERNAME="gta"

if [ $# -eq 0 ]; then
    echo "Usage: bash $0 [password]"
    exit 1
fi

## Trying not to leave the password in a public script
PASSWORD=$1 

for HOSTNAME in ${HOSTS} ; do
    sshpass -p ${PASSWORD} ssh -o StrictHostKeyChecking=no -l ${USERNAME} ${HOSTNAME} "${SCRIPT}"
done
