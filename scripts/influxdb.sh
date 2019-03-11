## Author: Kaylani Bochie
## kaylani2 AT hotmail DOT com
## Written based on the official documentation
## Target: Raspberry Pi running Stretch
##
## Known issue when dropping measurements:
## https://github.com/influxdata/influxdb/issues/10052

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

## Useful commands:
# CREATE USER <"user"> WITH PASSWORD <"password"> WITH ALL PRIVILEGES
# precision rfc3339 
# /\ Shows time in human readable format
# SHOW DATABASES
# CREATE DATABASE <database>
# DROP DATABASE <database>
# USE <database>
#   SHOW MEASUREMENTS
#   SHOW FIELD KEYS
#   INSERT <measurement> <field>=<value>, <otherField>=<otherValue>
#   SELECT * FROM <measurement>
#   SELECT * FROM <measurement> LIMIT <limit>
#   DROP MEASUREMENT <measurement>
#   SHOW SERIES
#   DELETE FROM <series>
# du -sh /var/lib/influxdb/data/<db name> (get disk usage of influxdb)

## From docs.influxdata.com:
# Database names, measurements, tag keys, field keys, and tag values are stored only once and always as strings.
# Only field values and timestamps are stored per-point.
# Non-string values require approximately three bytes.
# String values require variable space as determined by string compression.

## For Debian installation: (24/02/19)
## https://portal.influxdata.com/downloads/
## The official version on the docs page is outdated
# wget https://dl.influxdata.com/influxdb/releases/influxdb_1.7.4_amd64.deb
# sudo dpkg -i influxdb_1.7.4_amd64.deb
