

// system include files
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/Utilities/interface/InputTag.h"
 #include "DataFormats/TrackReco/interface/Track.h"
 #include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
//#include "DataFormats/HepMCCandidate/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"


#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/PatCandidates/interface/VIDCutFlowResult.h"


#include "TLorentzVector.h"
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TVectorD.h>
#include <cmath>

#include "TH1.h"
#include "TTree.h"
#include "TFile.h"


#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"



//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using reco::TrackCollection;

class extractJets : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
	public:
		explicit extractJets(const edm::ParameterSet&);
		~extractJets();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


	private:
		virtual void beginJob() override;
		virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
		virtual void endJob() override;

		// ----------member data ---------------------------

		edm::EDGetToken m_genParticleToken;
		edm::EDGetToken genJetToken;
		edm::EDGetToken patJetsPuppiToken;
		edm::EDGetToken patJetsAK8Token;





    // gen particle variables
    std::vector<Int_t> genpdgid;
    std::vector<Int_t> motherpdgid;

    // event variables
    ULong64_t event;
    UInt_t run;
    UInt_t lumi;

    // Output
    TTree* jetTree;
    TFile* outputFile;



};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
extractJets::extractJets(const edm::ParameterSet& iConfig)
	:
	m_genParticleToken(consumes<std::vector<reco::GenParticle>> (iConfig.getParameter<edm::InputTag>("genParticles"))),
	genJetToken(consumes<std::vector<reco::GenJet>> (iConfig.getParameter<edm::InputTag>("genJets"))),
	patJetsPuppiToken(consumes<std::vector<pat::Jet>> (iConfig.getParameter<edm::InputTag>("patJetsAK8"))),
	patJetsAK8Token(consumes<std::vector<pat::Jet>> (iConfig.getParameter<edm::InputTag>("patJetsPuppi")))

{
   //now do what ever initialization is needed
}


extractJets::~extractJets()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
extractJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	genpdgid.clear();
	motherpdgid.clear();
	edm::Handle<std::vector<reco::GenParticle>> genParticles;
	iEvent.getByToken(m_genParticleToken, genParticles);
	
	edm::Handle<std::vector<reco::GenJet>> genJets;
	iEvent.getByToken(genJetToken, genJets);
	
	edm::Handle<std::vector<pat::Jet>> patJetsAK8;
	iEvent.getByToken(patJetsPuppiToken, patJetsAK8);
	
	edm::Handle<std::vector<pat::Jet>> patJetsPuppi;
	iEvent.getByToken(patJetsAK8Token, patJetsPuppi);

	// add event info to tree
	event = iEvent.id().event();
	run = iEvent.id().run();
	lumi = iEvent.id().luminosityBlock();


	std::cout << "Gen particles" << std::endl;
	for (std::vector<reco::GenParticle>::const_iterator iParticle = genParticles->begin(); iParticle != genParticles->end(); iParticle++) {
		std::cout << "STATUS: " << iParticle->status() << " PDGID: " << iParticle->pdgId() << " MOTHER: " << iParticle->mother()->pdgId() << std::endl;
	}
	
	std::cout << "Gen jets" << std::endl;
	for (std::vector<reco::GenJet>::const_iterator iParticle = genJets->begin(); iParticle != genJets->end(); iParticle++) {
		std::cout << iParticle << std::endl;
		std::cout << "Number of Daughters:" << iParticle->numberOfDaughters() << std::endl;
	}
	
	std::cout << "Pat jets AK8" << std::endl;
	for (std::vector<pat::Jet>::const_iterator iParticle = patJetsAK8->begin(); iParticle != patJetsAK8->end(); iParticle++) {
		std::cout << "Number of Daughters:" << iParticle->numberOfDaughters() << std::endl;
	}
	
	std::cout << "Pat jets Puppi" << std::endl;
	for (std::vector<pat::Jet>::const_iterator iParticle = patJetsPuppi->begin(); iParticle != patJetsPuppi->end(); iParticle++) {
		std::cout << "Number of Daughters:" << iParticle->numberOfDaughters() << std::endl;
	}


	// Save data in tree
	jetTree->Fill();

}
// ------------ method called once each job just before starting event loop  ------------
void
extractJets::beginJob() {
	edm::Service<TFileService> fs;
	// create tree and add branches
	jetTree = fs->make<TTree>("jetTree", "jetTree");
	jetTree->Branch("genpdgid", &genpdgid);
	jetTree->Branch("motherpdgid", &motherpdgid);
	jetTree->Branch("event", &event);
	jetTree->Branch("run", &run);
	jetTree->Branch("lumi", &lumi);
}

// ------------ method called once each job just after ending the event loop  ------------
void
extractJets::endJob() {

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
extractJets::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(extractJets);
