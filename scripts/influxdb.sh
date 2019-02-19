## Author: Kaylani Bochie
## kaylani2 AT hotmail DOT com

## https://gist.github.com/boseji/bb71910d43283a1b84ab200bcce43c26
sudo curl -sL https://repos.influxdata.com/influxdb.key | sudo apt-key add -
lsb_release -a # stretch in my case
echo "deb https://repos.influxdata.com/debian stretch stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
sudo apt-get update
sudo apt-get install -y influxdb

## Assuming that node-red and npm are correctly installed
cd ~/.node-red
npm install node-red-contrib-influxdb
## The new nodes will appear on node-red after rebooting

# https://docs.influxdata.com/influxdb/v1.7/query_language/database_management/
# https://docs.influxdata.com/influxdb/v1.7/query_language/data_exploration/
# https://docs.influxdata.com/influxdb/v1.7/concepts/crosswalk/
## Useful commands
# CREATE USER <"user"> WITH PASSWORD <"password"> WITH ALL PRIVILEGES
# SHOW DATABASES
# CREATE DATABASE <database>
# DROP DATABASE <database>
# USE <database>
#   SHOW MEASUREMENTS
#   SHOW FIELD KEYS
#   SELECT * FROM <measurement>
#   SELECT * FROM <measurement> LIMIT <limit>
#  DROP MEASUREMENT <measurement>
