# Labsensing

## BibTeX:

If you find this code useful in your research, please consider citing:

Se este código foi útil para sua pesquisa, considere citar:

```
@article{bochie2019_labsensing,
    author = {Bochie, K. and Campista, M. E. M},
    journal = {II Workshop de Trabalhos de Iniciação Científica e de Graduação (WTG 2019)},
    month = {May},
    title = {{LabSensing}: Um Sistema de Sensoriamento para Laboratórios Científicos com Computação Inteligente nas Bordas},
    year = {2019}
}
```

## Directories:

### Components:
  Just the bare minimum.

### Nodes:
  Actual implementations being used in our network.

### Scripts:
  Installation and useful commands.
## ap.sh
  Setup the Raspberry Pi as an access point.
## bridge.sh
  Setup the Raspberry Pi as a bridge.
## undoAp.sh
  Undo what ap.sh does.
## undoBridge.sh
  Undo what bridge.sh does.
## mosquitto.sh
  Install the mosquitto broker and client.
## influxdb.sh
  Install influx db
## temperatura.gnu
  A simple example plotter.


#### Computer Nodes:
  Scripts to install and configure regular transmissions on regular machines (tested on Debian 9).

### Data:
  Just being used to fetch data from the server (which is positioned behind a firewall) without having to use multiple SCP commands.

### Flows:
  Every new flow added to the current implementation will be put in this directory.


#### Main flow used:
![Main Flow Used](images/flow.png?raw=true "Main flow used.")


### Dependencies and Configuration Files:
  The global functions written for this system are kept in a separate repository to facilitate development on Windows and Linux. This is because the Arduino software is very picky about where you keep your libraries. This way you can clone this repository to your Arduino directory and clone the dependencies into your Arduino libraries' directory. The repository is https://github.com/kaylani2/labsensingDependencies.
  The system also uses third party libraries, all of these can be installed with the library manager in the Arduino software. Every other dependency will be listed in this section with the associated download link and date.

### Note on Security:
  None of the installation scripts are configuring secure connections (SSL/TLS). These options are available for Mosquitto, Node-RED and InfluxDB, but the ESP8266 has a hard time with that. That means that the credentials sent from the ESP8266 modules are in plain text, be aware of this when positioning your network.
  Secure connections are being developed for other microcontrollers and will be added when they are tested and implemented on Labsensing.
