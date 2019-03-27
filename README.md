# Labsensing

## Directories:

### Components:
  Just the bare minimum.
  
### Nodes:
  Actual implementations being used in our network.
  
### Scripts:
  Installation and useful commands.

### Data:
  Just being used to fetch data from the server (which is positioned behind a firewall) without having to use multiple SCP commands.

### Flows:
  Every new flow added to the current implementation will be put in this directory. 


## Main flow used:
![Main Flow Used](images/flow.png?raw=true "Main flow used.")


## Dependencies and Configuration Files:
  The global functions written for this system are kept in a separate repository to facilitate development on Windows and Linux. This is because the Arduino software is very picky about where you keep your libraries. This way you can clone this repository to your Arduino directory and clone the dependencies into your Arduino libraries' directory. The repository is https://github.com/kaylani2/labsensingDependencies.
  The system also uses third party libraries, all of these can be installed with the library manager in the Arduino software. Every other dependency will be listed in this section with the associated download link and date.
