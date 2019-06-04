#!/bin/bash

### This script stablishes an ssh connection with each of the hosts in the network, clone the repository and install the crontab corresponding with the desired scripts.


## TODO: script must clone the repo and install the new cron job
SCRIPT="cd ~/; git clone https://github.com/kaylani2/labsensing; cd labsensing/scripts/computer_nodes"
HOSTS=("ponte" "sepetiba")
USERNAME="gta"

for HOSTNAME in ${HOSTS} ; do
    ssh -l ${USERNAME} ${HOSTNAME} "${SCRIPT}"
done
