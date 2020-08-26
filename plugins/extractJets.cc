

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
	m_genParticleToken(consumes<std::vector<reco::GenParticle>> (iConfig.getParameter<edm::InputTag>("genParticles")))

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
	
	edm::Handle<std::vector<reco::GenParticle>> genParticles;
	iEvent.getByToken(m_genParticleToken, genParticles);

	
	std::cout << "Gen particles" << std::endl;
	for (std::vector<reco::GenParticle>::const_iterator iParticle = genParticles->begin(); iParticle != genParticles->end(); iParticle++) {
		//std::cout << "Particle: " << iParticle << std::endl;
		std::cout << "STATUS: " << iParticle->status() << " PDGID: " << iParticle->pdgId() << std::endl;
		//std::cout << "STATUS: " << iParticle->status() << " PDGID: " << iParticle->pdgId() << " MOTHER: " << iParticle->mother()->pdgId() << std::endl;
	}

}
// ------------ method called once each job just before starting event loop  ------------
void
extractJets::beginJob() {
	edm::Service<TFileService> fs; 
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