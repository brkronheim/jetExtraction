import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')
options.parseArguments()


process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        options.inputFiles
    )
)

process.demo = cms.EDAnalyzer('extractJets',
  genParticles = cms.InputTag("prunedGenParticles"),
  genJets = cms.InputTag("slimmedGenJetsAK8"),
  patJetsAK8 = cms.InputTag("slimmedJetsAK8"),
  patJetsPuppi = cms.InputTag("slimmedJetsPuppi"),
  

)

process.options = cms.untracked.PSet(
     SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile)
)

process.p = cms.Path(process.demo)
