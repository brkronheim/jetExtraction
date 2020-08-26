# Jet extraction
Extracting jet information out of Root files for machine learning work.

## Setup
First, follow the instruction [here](http://opendata.cern.ch/docs/cms-guide-docker) to set up docker and install CMSSW. Make sure to get cmssw_10_6_9_patch1, not the version given in the tutorial.

Next, make a directory jets and enter it.
```
mkdir jets
cd jets
```
Clone this repo by doing:
```
git clone https://github.com/brkronheim/jetExtraction.git
```
Next, make a directory data and enter it
```
mkdir data
cd data
```
Here we get a dataset from CERN Open Data and save it as dataset1.root
```
wget -O dataset1.root http://opendata.cern.ch/record/12021/files/assets/cms/MonteCarlo2016/RunIISummer16MiniAODv2/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/MINIAODSIM/PUMoriond17_magnetOn_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/100000/08278E4E-E4EF-E611-8BD7-FA163E3ABA64.root
```

To compile, return to the jets directory and execute
```
scram b -j32
```

## Run
To run the code execute
```
cmsRun jetExctraction/python/cfg.py inputFiles=file:data/dataset1.root outputFile=out.root
```
At the moment, the code should loop over all the events in the file and display the status, particle ID, and particle ID of the particle's mother for all status 1 (resultant) particles.
