// -*- C++ -*-
// Class:      llp_ntupler
/*
  Description: Base class for miniAOD analysis with CRAB
*/
//         Author:  Cristián Peña and Si Xie.
//         Created:  Thu, 26 March 2019 15:00:06 GMT

#include "llp_ntupler.h"
#include "cms_lpc_llp/llp_ntupler/interface/DBSCAN.h"

//------ Constructors and destructor ------//
llp_ntupler::llp_ntupler(const edm::ParameterSet& iConfig):
  //get inputs from config file
  isData_(iConfig.getParameter<bool> ("isData")),
  //isFourJet_(iConfig.getParameter<bool> ("isFourJet")),
  useGen_(iConfig.getParameter<bool> ("useGen")),
  isFastsim_(iConfig.getParameter<bool> ("isFastsim")),
  //isQCD_(iConfig.getParameter<bool> ("isQCD")),
  enableTriggerInfo_(iConfig.getParameter<bool> ("enableTriggerInfo")),
  enableCaloJet_(iConfig.getParameter<bool> ("enableCaloJet")),
  enableGenLLPInfo_(iConfig.getParameter<bool> ("enableGenLLPInfo")),
  enableEcalRechits_(iConfig.getParameter<bool> ("enableEcalRechits")),
  readGenVertexTime_(iConfig.getParameter<bool> ("readGenVertexTime")),
  llpId_(iConfig.getParameter<int> ("llpId")),
  triggerPathNamesFile_(iConfig.getParameter<string> ("triggerPathNamesFile")),
  eleHLTFilterNamesFile_(iConfig.getParameter<string> ("eleHLTFilterNamesFile")),
  muonHLTFilterNamesFile_(iConfig.getParameter<string> ("muonHLTFilterNamesFile")),
  photonHLTFilterNamesFile_(iConfig.getParameter<string> ("photonHLTFilterNamesFile")),
  verticesToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  tracksTag_(consumes<edm::View<reco::Track> >(iConfig.getParameter<edm::InputTag>("tracks"))),
  cscSegmentInputToken_(consumes<CSCSegmentCollection>(edm::InputTag("cscSegments"))),
  dtSegmentInputToken_(consumes<DTRecSegment4DCollection>(edm::InputTag("dt4DCosmicSegments"))),
  rpcRecHitInputToken_(consumes<RPCRecHitCollection>(edm::InputTag("rpcRecHits"))),
  muonsToken_(consumes<reco::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
  electronsToken_(consumes<reco::GsfElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
  tausToken_(consumes<reco::PFTauCollection>(iConfig.getParameter<edm::InputTag>("taus"))),
  photonsToken_(consumes<reco::PhotonCollection>(iConfig.getParameter<edm::InputTag>("photons"))),
  jetsCaloToken_(consumes<reco::CaloJetCollection>(iConfig.getParameter<edm::InputTag>("jetsCalo"))),
  // jetsPFToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jetsPF"))),
  jetsToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
  jetsPuppiToken_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jetsPuppi"))),
  jetsAK8Token_(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jetsAK8"))),
  PFCandsToken_(consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCands"))),
  PFClustersToken_(consumes<reco::PFClusterCollection>(iConfig.getParameter<edm::InputTag>("pfClusters"))),
  //genParticlesToken_(consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("genParticles"))),
  //genParticlesToken_(consumes<edm::View<pat::PackedGenParticle> >(iConfig.getParameter<edm::InputTag>("genParticles"))),
  genParticlesToken_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticles"))),
  genJetsToken_(consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genJets"))),
  triggerBitsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerBits"))),
  hepMCToken_(consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("hepMC"))),
  //triggerObjectsToken_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("triggerObjects"))),
  //triggerPrescalesToken_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("triggerPrescales"))),
  genMetCaloToken_(consumes<reco::GenMETCollection>(iConfig.getParameter<edm::InputTag>("genMetsCalo"))),
  genMetTrueToken_(consumes<reco::GenMETCollection>(iConfig.getParameter<edm::InputTag>("genMetsTrue"))),
  metToken_(consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("mets"))),
//  metNoHFToken_(consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("metsNoHF"))),
  metPuppiToken_(consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("metsPuppi"))),
  metFilterBitsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("metFilterBits"))),
  //hbheNoiseFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("hbheNoiseFilter"))),
  //hbheTightNoiseFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("hbheTightNoiseFilter"))),
  //hbheIsoNoiseFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("hbheIsoNoiseFilter"))),
  //badChargedCandidateFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("BadChargedCandidateFilter"))),
  //badMuonFilterToken_(consumes<bool>(iConfig.getParameter<edm::InputTag>("BadMuonFilter"))),
//  lheRunInfoTag_(iConfig.getParameter<edm::InputTag>("lheInfo")),
//  lheRunInfoToken_(consumes<LHERunInfoProduct,edm::InRun>(lheRunInfoTag_)),
//  lheInfoToken_(consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("lheInfo"))),
  genInfoToken_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genInfo"))),
  genLumiHeaderToken_(consumes<GenLumiInfoHeader,edm::InLumi>(edm::InputTag("generator",""))),
  puInfoToken_(consumes<std::vector<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("puInfo"))),
  //hcalNoiseInfoToken_(consumes<HcalNoiseSummary>(iConfig.getParameter<edm::InputTag>("hcalNoiseInfo"))),
  secondaryVerticesToken_(consumes<vector<reco::VertexCompositePtrCandidate> >(iConfig.getParameter<edm::InputTag>("secondaryVertices"))),
  rhoAllToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoAll"))),
  rhoFastjetAllToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetAll"))),
  rhoFastjetAllCaloToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetAllCalo"))),
  rhoFastjetCentralCaloToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetCentralCalo"))),
  rhoFastjetCentralChargedPileUpToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetCentralChargedPileUp"))),
  rhoFastjetCentralNeutralToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoFastjetCentralNeutral"))),
  beamSpotToken_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamSpot"))),
  ebRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("ebRecHits"))),
  eeRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("eeRecHits"))),
  esRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("esRecHits"))),
  ebeeClustersToken_(consumes<vector<reco::CaloCluster> >(iConfig.getParameter<edm::InputTag>("ebeeClusters"))),
  esClustersToken_(consumes<vector<reco::CaloCluster> >(iConfig.getParameter<edm::InputTag>("esClusters"))),
  conversionsToken_(consumes<vector<reco::Conversion> >(iConfig.getParameter<edm::InputTag>("conversions"))),
  singleLegConversionsToken_(consumes<vector<reco::Conversion> >(iConfig.getParameter<edm::InputTag>("singleLegConversions"))),
  gedGsfElectronCoresToken_(consumes<vector<reco::GsfElectronCore> >(iConfig.getParameter<edm::InputTag>("gedGsfElectronCores"))),
  gedPhotonCoresToken_(consumes<vector<reco::PhotonCore> >(iConfig.getParameter<edm::InputTag>("gedPhotonCores"))),
  generalTrackToken_(consumes<std::vector<reco::Track>>(edm::InputTag("generalTracks")))
  //superClustersToken_(consumes<vector<reco::SuperCluster> >(iConfig.getParameter<edm::InputTag>("superClusters"))),
  //  lostTracksToken_(consumes<vector<reco::PFCandidate> >(iConfig.getParameter<edm::InputTag>("lostTracks")))
  // mvaGeneralPurposeValuesMapToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaGeneralPurposeValuesMap"))),
  // mvaGeneralPurposeCategoriesMapToken_(consumes<edm::ValueMap<int> >(iConfig.getParameter<edm::InputTag>("mvaGeneralPurposeCategoriesMap"))),
  // mvaHZZValuesMapToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaHZZValuesMap"))),
  // mvaHZZCategoriesMapToken_(consumes<edm::ValueMap<int> >(iConfig.getParameter<edm::InputTag>("mvaHZZCategoriesMap")))
{
  //declare the TFileService for output
  edm::Service<TFileService> fs;

  //set up output tree
  llpTree = fs->make<TTree>("llp", "selected AOD information for llp analyses");
  //llpTree = new TTree("Jets", "selected AOD information");
  NEvents = fs->make<TH1F>("NEvents",";;NEvents;",1,-0.5,0.5);


  //------------------------------------------------------------------
  //Read in HLT Trigger Path List from config file
  //------------------------------------------------------------------
  for (int i = 0; i<NTriggersMAX; ++i) triggerPathNames[i] = "";
  ifstream myfile (edm::FileInPath(triggerPathNamesFile_.c_str()).fullPath().c_str()) ;
  if (myfile.is_open())
  {
    std::string line;
    int index;
    std::string hltpathname;

    while(myfile>>index>>hltpathname)
    {
      if (index < NTriggersMAX)
      {
        triggerPathNames[index] = hltpathname;
      }
    }
    myfile.close();
  }
  else
  {
    std::cout << "ERROR!!! Could not open trigger path name file : " << edm::FileInPath(triggerPathNamesFile_.c_str()).fullPath().c_str() << "\n";
  }

  if(enableTriggerInfo_)
  {
    std::cout << "\n";
    std::cout << "****************** Trigger Paths Defined For Razor Ntuple ******************\n";
    for (int i = 0; i<NTriggersMAX; ++i)
    {
      if (triggerPathNames[i] != "") std::cout << "Trigger " << i << " " << triggerPathNames[i] << "\n";
    }
    std::cout << "****************************************************************************\n";
    std::cout << "\n";
  }
  if(readGenVertexTime_) genParticles_t0_Token_ = consumes<float>(iConfig.getParameter<edm::InputTag>("genParticles_t0"));

}

llp_ntupler::~llp_ntupler()
{
};

//***********************************************
//Enable output ntuple branches
//***********************************************

void llp_ntupler::setBranches()
{
  enableEventInfoBranches();
  //enablePVAllBranches();
  enablePileUpBranches();
  enableMuonBranches();
  enableElectronBranches();
  enablePhotonBranches();
  enableMuonSystemBranches();
  enableJetBranches();
  enableMetBranches();
  enableTriggerBranches();
  enableMCBranches();
  enableGenParticleBranches();
};

void llp_ntupler::enableEventInfoBranches()
{
  llpTree->Branch("isData", &isData, "isData/O");
  llpTree->Branch("nPV", &nPV, "nPV/I");
  llpTree->Branch("runNum", &runNum, "runNum/i");
  llpTree->Branch("nSlimmedSecondV", &nSlimmedSecondV, "nSlimmedSecondV/i");
  llpTree->Branch("lumiNum", &lumiNum, "lumiNum/i");
  llpTree->Branch("eventNum", &eventNum, "eventNum/i");
  llpTree->Branch("eventTime", &eventTime, "eventTime/i");
  llpTree->Branch("pvX", &pvX, "pvX/F");
  llpTree->Branch("pvY", &pvY, "pvY/F");
  llpTree->Branch("pvZ", &pvZ, "pvZ/F");
  llpTree->Branch("fixedGridRhoAll", &fixedGridRhoAll, "fixedGridRhoAll/F");
  llpTree->Branch("fixedGridRhoFastjetAll", &fixedGridRhoFastjetAll, "fixedGridRhoFastjetAll/F");
  //llpTree->Branch("fixedGridRhoFastjetAllCalo", &fixedGridRhoFastjetAllCalo, "fixedGridRhoFastjetAllCalo/F");
  // llpTree->Branch("fixedGridRhoFastjetCentralCalo", &fixedGridRhoFastjetCentralCalo, "fixedGridRhoFastjetCentralCalo/F");
  // llpTree->Branch("fixedGridRhoFastjetCentralChargedPileUp", &fixedGridRhoFastjetCentralChargedPileUp, "fixedGridRhoFastjetCentralChargedPileUp/F");
  // llpTree->Branch("fixedGridRhoFastjetCentralNeutral", &fixedGridRhoFastjetCentralNeutral, "fixedGridRhoFastjetCentralNeutral/F");
}

void llp_ntupler::enablePVAllBranches()
{
  llpTree->Branch("nPVAll", &nPVAll,"nPVAll/I");
  llpTree->Branch("pvAllX", pvAllX,"pvAllX[nPVAll]/F");
  llpTree->Branch("pvAllY", pvAllY,"pvAllY[nPVAll]/F");
  llpTree->Branch("pvAllZ", pvAllZ,"pvAllZ[nPVAll]/F");
  llpTree->Branch("pvAllLogSumPtSq", pvAllLogSumPtSq,"pvAllLogSumPtSq[nPVAll]/F");
  llpTree->Branch("pvAllSumPx", pvAllSumPx,"pvAllSumPx[nPVAll]/F");
  llpTree->Branch("pvAllSumPy", pvAllSumPy,"pvAllSumPy[nPVAll]/F");
};

void llp_ntupler::enablePVTracksBranches()
{
  llpTree->Branch("nPVTracks", &nPVTracks,"nPVTracks/I");
  llpTree->Branch("pvTrackPt", pvTrackPt,"pvTrackPt[nPVTracks]/F");
  llpTree->Branch("pvTrackEta", pvTrackEta,"pvTrackEta[nPVTracks]/F");
  llpTree->Branch("pvTrackPhi", pvTrackPhi,"pvTrackPhi[nPVTracks]/F");
};

void llp_ntupler::enablePileUpBranches()
{
  llpTree->Branch("nBunchXing", &nBunchXing, "nBunchXing/I");
  llpTree->Branch("BunchXing", BunchXing, "BunchXing[nBunchXing]/I");
  llpTree->Branch("nPU", nPU, "nPU[nBunchXing]/I");
  llpTree->Branch("nPUmean", nPUmean, "nPUmean[nBunchXing]/F");
};

void llp_ntupler::enableMuonBranches()
{
  llpTree->Branch("nMuons", &nMuons,"nMuons/I");
  llpTree->Branch("muonE", muonE,"muonE[nMuons]/F");
  llpTree->Branch("muonPt", muonPt,"muonPt[nMuons]/F");
  llpTree->Branch("muonEta", muonEta,"muonEta[nMuons]/F");
  llpTree->Branch("muonPhi", muonPhi,"muonPhi[nMuons]/F");
  llpTree->Branch("muonCharge", muonCharge, "muonCharge[nMuons]/I");
  llpTree->Branch("muonIsLoose", muonIsLoose,"muonIsLoose[nMuons]/O");
  llpTree->Branch("muonIsMedium", muonIsMedium,"muonIsMedium[nMuons]/O");
  llpTree->Branch("muonIsTight", muonIsTight,"muonIsTight[nMuons]/O");
  llpTree->Branch("muon_d0", muon_d0, "muon_d0[nMuons]/F");
  llpTree->Branch("muon_dZ", muon_dZ, "muon_dZ[nMuons]/F");
  llpTree->Branch("muon_ip3d", muon_ip3d, "muon_ip3d[nMuons]/F");
  llpTree->Branch("muon_ip3dSignificance", muon_ip3dSignificance, "muon_ip3dSignificance[nMuons]/F");
  llpTree->Branch("muonType", muonType, "muonType[nMuons]/i");
  llpTree->Branch("muonQuality", muonQuality, "muonQuality[nMuons]/i");
  llpTree->Branch("muon_pileupIso", muon_pileupIso, "muon_pileupIso[nMuons]/F");
  llpTree->Branch("muon_chargedIso", muon_chargedIso, "muon_chargedIso[nMuons]/F");
  llpTree->Branch("muon_photonIso", muon_photonIso, "muon_photonIso[nMuons]/F");
  llpTree->Branch("muon_neutralHadIso", muon_neutralHadIso, "muon_neutralHadIso[nMuons]/F");
  llpTree->Branch("muon_ptrel", muon_ptrel, "muon_ptrel[nMuons]/F");
  llpTree->Branch("muon_passSingleMuTagFilter", muon_passSingleMuTagFilter, "muon_passSingleMuTagFilter[nMuons]/O");
  llpTree->Branch("muon_passHLTFilter", &muon_passHLTFilter, Form("muon_passHLTFilter[nMuons][%d]/O",MAX_MuonHLTFilters));
  llpTree->Branch("muon_validFractionTrackerHits", muon_validFractionTrackerHits, "muon_validFractionTrackerHits[nMuons]/F");
  llpTree->Branch("muon_isGlobal", muon_isGlobal,"muon_isGlobal[nMuons]/O");
  llpTree->Branch("muon_normChi2", muon_normChi2,"muon_normChi2[nMuons]/F");
  llpTree->Branch("muon_chi2LocalPosition", muon_chi2LocalPosition,"muon_chi2LocalPosition[nMuons]/F");
  llpTree->Branch("muon_kinkFinder", muon_kinkFinder,"muon_kinkFinder[nMuons]/F");
  llpTree->Branch("muon_segmentCompatability", muon_segmentCompatability,"muon_segmentCompatability[nMuons]/F");
  llpTree->Branch("muonIsICHEPMedium", muonIsICHEPMedium,"muonIsICHEPMedium[nMuons]/O");
};

void llp_ntupler::enableElectronBranches()
{
  llpTree->Branch("nElectrons", &nElectrons,"nElectrons/I");
  llpTree->Branch("eleE", eleE,"eleE[nElectrons]/F");
  llpTree->Branch("elePt", elePt,"elePt[nElectrons]/F");
  llpTree->Branch("eleEta", eleEta,"eleEta[nElectrons]/F");
  llpTree->Branch("elePhi", elePhi,"elePhi[nElectrons]/F");
  llpTree->Branch("eleCharge", eleCharge, "eleCharge[nElectrons]/F");
  llpTree->Branch("eleEta_SC", eleEta_SC,"eleEta_SC[nElectrons]/F");
  llpTree->Branch("eleSigmaIetaIeta", eleSigmaIetaIeta, "eleSigmaIetaIeta[nElectrons]/F");
  llpTree->Branch("eleFull5x5SigmaIetaIeta", eleFull5x5SigmaIetaIeta, "eleFull5x5SigmaIetaIeta[nElectrons]/F");
  llpTree->Branch("eleR9", eleR9, "eleR9[nElectrons]/F");
  llpTree->Branch("ele_dEta", ele_dEta, "ele_dEta[nElectrons]/F");
  llpTree->Branch("ele_dPhi", ele_dPhi, "ele_dPhi[nElectrons]/F");
  llpTree->Branch("ele_HoverE", ele_HoverE, "ele_HoverE[nElectrons]/F");
  llpTree->Branch("ele_d0", ele_d0, "ele_d0[nElectrons]/F");
  llpTree->Branch("ele_dZ", ele_dZ, "ele_dZ[nElectrons]/F");
  llpTree->Branch("ele_ip3d", ele_ip3d, "ele_ip3d[nElectrons]/F");
  llpTree->Branch("ele_ip3dSignificance", ele_ip3dSignificance, "ele_ip3dSignificance[nElectrons]/F");
  llpTree->Branch("ele_pileupIso", ele_pileupIso, "ele_pileupIso[nElectrons]/F");
  llpTree->Branch("ele_chargedIso", ele_chargedIso, "ele_chargedIso[nElectrons]/F");
  llpTree->Branch("ele_photonIso", ele_photonIso, "ele_photonIso[nElectrons]/F");
  llpTree->Branch("ele_neutralHadIso", ele_neutralHadIso, "ele_neutralHadIso[nElectrons]/F");
  llpTree->Branch("ele_MissHits", ele_MissHits, "ele_MissHits[nElectrons]/I");
  llpTree->Branch("ele_PassConvVeto", ele_PassConvVeto, "ele_PassConvVeto[nElectrons]/O");
  llpTree->Branch("ele_OneOverEminusOneOverP", ele_OneOverEminusOneOverP, "ele_OneOverEminusOneOverP[nElectrons]/F");
  llpTree->Branch("ele_IDMVAGeneralPurpose", ele_IDMVAGeneralPurpose, "ele_IDMVAGeneralPurpose[nElectrons]/F");
  llpTree->Branch("ele_IDMVACategoryGeneralPurpose", ele_IDMVACategoryGeneralPurpose, "ele_IDMVACategoryGeneralPurpose[nElectrons]/I");
  llpTree->Branch("ele_IDMVAHZZ", ele_IDMVAHZZ, "ele_IDMVAHZZ[nElectrons]/F");
  llpTree->Branch("ele_IDMVACategoryHZZ", ele_IDMVACategoryHZZ, "ele_IDMVACategoryHZZ[nElectrons]/I");
  llpTree->Branch("ele_passSingleEleTagFilter", ele_passSingleEleTagFilter, "ele_passSingleEleTagFilter[nElectrons]/O");
  llpTree->Branch("ele_passTPOneTagFilter", ele_passTPOneTagFilter, "ele_passTPOneTagFilter[nElectrons]/O");
  llpTree->Branch("ele_passTPTwoTagFilter", ele_passTPTwoTagFilter, "ele_passTPTwoTagFilter[nElectrons]/O");
  llpTree->Branch("ele_passTPOneProbeFilter", ele_passTPOneProbeFilter, "ele_passTPOneProbeFilter[nElectrons]/O");
  llpTree->Branch("ele_passTPTwoProbeFilter", ele_passTPTwoProbeFilter, "ele_passTPTwoProbeFilter[nElectrons]/O");
  llpTree->Branch("ele_passHLTFilter", &ele_passHLTFilter, Form("ele_passHLTFilter[nElectrons][%d]/O",MAX_ElectronHLTFilters));

};

void llp_ntupler::enableTauBranches()
{
  llpTree->Branch("nTaus", &nTaus,"nTaus/I");
  llpTree->Branch("tauE", tauE,"tauE[nTaus]/F");
  llpTree->Branch("tauPt", tauPt,"tauPt[nTaus]/F");
  llpTree->Branch("tauEta", tauEta,"tauEta[nTaus]/F");
  llpTree->Branch("tauPhi", tauPhi,"tauPhi[nTaus]/F");
  llpTree->Branch("tau_IsLoose", tau_IsLoose, "tau_IsLoose[nTaus]/O");
  llpTree->Branch("tau_IsMedium", tau_IsMedium, "tau_IsMedium[nTaus]/O");
  llpTree->Branch("tau_IsTight", tau_IsTight, "tau_IsTight[nTaus]/O");
  llpTree->Branch("tau_passEleVetoLoose", tau_passEleVetoLoose, "tau_passEleVetoLoose[nTaus]/O");
  llpTree->Branch("tau_passEleVetoMedium", tau_passEleVetoMedium, "tau_passEleVetoMedium[nTaus]/O");
  llpTree->Branch("tau_passEleVetoTight", tau_passEleVetoTight, "tau_passEleVetoTight[nTaus]/O");
  llpTree->Branch("tau_passMuVetoLoose", tau_passMuVetoLoose, "tau_passMuVetoLoose[nTaus]/O");
  llpTree->Branch("tau_passMuVetoMedium", tau_passMuVetoMedium, "tau_passMuVetoMedium[nTaus]/O");
  llpTree->Branch("tau_passMuVetoTight", tau_passMuVetoTight, "tau_passMuVetoTight[nTaus]/O");
  llpTree->Branch("tau_ID", tau_ID, "tau_ID[nTaus]/i");
  llpTree->Branch("tau_combinedIsoDeltaBetaCorr3Hits", tau_combinedIsoDeltaBetaCorr3Hits, "tau_combinedIsoDeltaBetaCorr3Hits[nTaus]/F");
  llpTree->Branch("tau_chargedIsoPtSum", tau_chargedIsoPtSum, "tau_chargedIsoPtSum[nTaus]/F");
  llpTree->Branch("tau_neutralIsoPtSum", tau_neutralIsoPtSum, "tau_neutralIsoPtSum[nTaus]/F");
  llpTree->Branch("tau_puCorrPtSum", tau_puCorrPtSum, "tau_puCorrPtSum[nTaus]/F");
  llpTree->Branch("tau_eleVetoMVA", tau_eleVetoMVA, "tau_eleVetoMVA[nTaus]/F");
  llpTree->Branch("tau_eleVetoCategory", tau_eleVetoCategory, "tau_eleVetoCategory[nTaus]/I");
  llpTree->Branch("tau_muonVetoMVA", tau_muonVetoMVA, "tau_muonVetoMVA[nTaus]/F");
  llpTree->Branch("tau_isoMVAnewDMwLT", tau_isoMVAnewDMwLT, "tau_isoMVAnewDMwLT[nTaus]/F");
  llpTree->Branch("tau_isoMVAnewDMwoLT", tau_isoMVAnewDMwoLT, "tau_isoMVAnewDMwoLT[nTaus]/F");
  llpTree->Branch("tau_leadCandPt", tau_leadCandPt, "tau_leadCandPt[nTaus]/F");
  llpTree->Branch("tau_leadCandID", tau_leadCandID, "tau_leadCandID[nTaus]/I");
  llpTree->Branch("tau_leadChargedHadrCandPt", tau_leadChargedHadrCandPt, "tau_leadChargedHadrCandPt[nTaus]/F");
  llpTree->Branch("tau_leadChargedHadrCandID", tau_leadChargedHadrCandID, "tau_leadChargedHadrCandID[nTaus]/I");
};

void llp_ntupler::enableIsoPFCandidateBranches()
{
  llpTree->Branch("nIsoPFCandidates", &nIsoPFCandidates, "nIsoPFCandidates/i");
  llpTree->Branch("isoPFCandidatePt", isoPFCandidatePt, "isoPFCandidatePt[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidateEta", isoPFCandidateEta, "isoPFCandidateEta[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidatePhi", isoPFCandidatePhi, "isoPFCandidatePhi[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidateIso04", isoPFCandidateIso04, "isoPFCandidateIso04[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidateD0", isoPFCandidateD0, "isoPFCandidateD0[nIsoPFCandidates]/F");
  llpTree->Branch("isoPFCandidatePdgId", isoPFCandidatePdgId, "isoPFCandidatePdgId[nIsoPFCandidates]/I");
};

void llp_ntupler::enablePhotonBranches()
{
  llpTree->Branch("nPhotons", &nPhotons,"nPhotons/I");
  llpTree->Branch("nPhotons_overlap", &nPhotons_overlap,"nPhotons_overlap/I");
  llpTree->Branch("phoE", phoE,"phoE[nPhotons]/F");
  llpTree->Branch("phoPt", phoPt,"phoPt[nPhotons]/F");
  llpTree->Branch("phoEta", phoEta,"phoEta[nPhotons]/F");
  llpTree->Branch("phoPhi", phoPhi,"phoPhi[nPhotons]/F");
  llpTree->Branch("phoSigmaIetaIeta", phoSigmaIetaIeta, "phoSigmaIetaIeta[nPhotons]/F");
  llpTree->Branch("phoFull5x5SigmaIetaIeta", phoFull5x5SigmaIetaIeta, "phoFull5x5SigmaIetaIeta[nPhotons]/F");
  llpTree->Branch("phoR9", phoR9, "phoR9[nPhotons]/F");
  llpTree->Branch("pho_sminor", pho_sminor, "pho_sminor[nPhotons]/F");
  llpTree->Branch("pho_smajor", pho_smajor, "pho_smajor[nPhotons]/F");
  llpTree->Branch("pho_HoverE", pho_HoverE, "pho_HoverE[nPhotons]/F");
  llpTree->Branch("pho_sumChargedHadronPtAllVertices", &pho_sumChargedHadronPtAllVertices,Form("pho_sumChargedHadronPtAllVertices[nPhotons][%d]/F",MAX_NPV));
  llpTree->Branch("pho_sumChargedHadronPt", &pho_sumChargedHadronPt, "pho_sumChargedHadronPt[nPhotons]/F");
  llpTree->Branch("pho_sumNeutralHadronEt", pho_sumNeutralHadronEt, "pho_sumNeutralHadronEt[nPhotons]/F");
  llpTree->Branch("pho_sumPhotonEt", pho_sumPhotonEt, "pho_sumPhotonEt[nPhotons]/F");
  llpTree->Branch("pho_ecalPFClusterIso", pho_ecalPFClusterIso, "pho_ecalPFClusterIso[nPhotons]/F");
  llpTree->Branch("pho_hcalPFClusterIso", pho_hcalPFClusterIso, "pho_hcalPFClusterIso[nPhotons]/F");
  llpTree->Branch("pho_trkSumPtHollowConeDR03", pho_trkSumPtHollowConeDR03, "pho_trkSumPtHollowConeDR03[nPhotons]/F");
  llpTree->Branch("pho_sumWorstVertexChargedHadronPt", pho_sumWorstVertexChargedHadronPt, "pho_sumWorstVertexChargedHadronPt[nPhotons]/F");
  llpTree->Branch("pho_pfIsoChargedHadronIso", pho_pfIsoChargedHadronIso, "pho_pfIsoChargedHadronIso[nPhotons]/F");
  llpTree->Branch("pho_pfIsoChargedHadronIsoWrongVtx", pho_pfIsoChargedHadronIsoWrongVtx, "pho_pfIsoChargedHadronIsoWrongVtx[nPhotons]/F");
  llpTree->Branch("pho_pfIsoNeutralHadronIso", pho_pfIsoNeutralHadronIso, "pho_pfIsoNeutralHadronIso[nPhotons]/F");
  llpTree->Branch("pho_pfIsoPhotonIso", pho_pfIsoPhotonIso, "pho_pfIsoPhotonIso[nPhotons]/F");
  llpTree->Branch("pho_pfIsoModFrixione", pho_pfIsoModFrixione, "pho_pfIsoModFrixione[nPhotons]/F");
  llpTree->Branch("pho_pfIsoSumPUPt", pho_pfIsoSumPUPt, "pho_pfIsoSumPUPt[nPhotons]/F");
  llpTree->Branch("pho_isConversion", pho_isConversion, "pho_isConversion[nPhotons]/O");
  llpTree->Branch("pho_passEleVeto", pho_passEleVeto, "pho_passEleVeto[nPhotons]/O");
  llpTree->Branch("pho_RegressionE", pho_RegressionE, "pho_RegressionE[nPhotons]/F");
  llpTree->Branch("pho_RegressionEUncertainty", pho_RegressionEUncertainty, "pho_RegressionEUncertainty[nPhotons]/F");
  llpTree->Branch("pho_IDMVA", pho_IDMVA, "pho_IDMVA[nPhotons]/F");
  llpTree->Branch("pho_superClusterEnergy", pho_superClusterEnergy, "pho_superClusterEnergy[nPhotons]/F");
  llpTree->Branch("pho_superClusterRawEnergy", pho_superClusterRawEnergy, "pho_superClusterRawEnergy[nPhotons]/F");
  llpTree->Branch("pho_superClusterEta", pho_superClusterEta, "pho_superClusterEta[nPhotons]/F");
  llpTree->Branch("pho_superClusterPhi", pho_superClusterPhi, "pho_superClusterPhi[nPhotons]/F");
  llpTree->Branch("pho_superClusterX", pho_superClusterX, "pho_superClusterX[nPhotons]/F");
  llpTree->Branch("pho_superClusterY", pho_superClusterY, "pho_superClusterY[nPhotons]/F");
  llpTree->Branch("pho_superClusterZ", pho_superClusterZ, "pho_superClusterZ[nPhotons]/F");
  llpTree->Branch("pho_hasPixelSeed", pho_hasPixelSeed, "pho_hasPixelSeed[nPhotons]/O");
  llpTree->Branch("pho_passHLTFilter", &pho_passHLTFilter, Form("pho_passHLTFilter[nPhotons][%d]/O",MAX_PhotonHLTFilters));
  llpTree->Branch("pho_convType", pho_convType, "pho_convType[nPhotons]/I");
  llpTree->Branch("pho_convTrkZ", pho_convTrkZ, "pho_convTrkZ[nPhotons]/F");
  llpTree->Branch("pho_convTrkClusZ", pho_convTrkClusZ, "pho_convTrkClusZ[nPhotons]/F");
  llpTree->Branch("pho_vtxSumPx", &pho_vtxSumPx,Form("pho_vtxSumPx[nPhotons][%d]/F",MAX_NPV));
  llpTree->Branch("pho_vtxSumPy", &pho_vtxSumPy,Form("pho_vtxSumPy[nPhotons][%d]/F",MAX_NPV));
  llpTree->Branch("pho_isStandardPhoton", pho_isStandardPhoton, "pho_isStandardPhoton[nPhotons]/O");
  llpTree->Branch("pho_seedRecHitSwitchToGain6", pho_seedRecHitSwitchToGain6, "pho_seedRecHitSwitchToGain6[nPhotons]/F");
  llpTree->Branch("pho_seedRecHitSwitchToGain1", pho_seedRecHitSwitchToGain1, "pho_seedRecHitSwitchToGain1[nPhotons]/F");
  llpTree->Branch("pho_anyRecHitSwitchToGain6", pho_anyRecHitSwitchToGain6, "pho_anyRecHitSwitchToGain6[nPhotons]/F");
  llpTree->Branch("pho_anyRecHitSwitchToGain1", pho_anyRecHitSwitchToGain1, "pho_anyRecHitSwitchToGain1[nPhotons]/F");


};

void llp_ntupler::enableMuonSystemBranches()
{

    // llpTree->Branch("nCsc",&nCsc,"nCsc/I");
    // llpTree->Branch("cscPhi",cscPhi,"cscPhi[nCsc]");
    // llpTree->Branch("cscEta",cscEta,"cscEta[nCsc]");
    // llpTree->Branch("cscX",cscX,"cscX[nCsc]");
    // llpTree->Branch("cscY",cscY,"cscY[nCsc]");
    // llpTree->Branch("cscZ",cscZ,"cscZ[nCsc]");
    // llpTree->Branch("cscDirectionX",cscDirectionX,"cscDirectionX[nCsc]");
    // llpTree->Branch("cscDirectionY",cscDirectionY,"cscDirectionY[nCsc]");
    // llpTree->Branch("cscDirectionZ",cscDirectionZ,"cscDirectionZ[nCsc]");
    // llpTree->Branch("cscNRecHits",cscNRecHits,"cscNRecHits[nCsc]");
    // llpTree->Branch("cscNRecHits_flag",cscNRecHits_flag,"cscNRecHits_flag[nCsc]");
    // llpTree->Branch("cscT",cscT,"cscT[nCsc]");
    // llpTree->Branch("cscChi2",cscChi2,"cscChi2[nCsc]");

  // all csc clusters
  llpTree->Branch("nCscClusters",             &nCscClusters, "nCscClusters/I");
  llpTree->Branch("cscClusterX",             cscClusterX,             "cscClusterX[nCscClusters]/F");
  llpTree->Branch("cscClusterY",             cscClusterY,             "cscClusterY[nCscClusters]/F");
  llpTree->Branch("cscClusterZ",             cscClusterZ,             "cscClusterZ[nCscClusters]/F");
  llpTree->Branch("cscClusterTime",             cscClusterTime,             "cscClusterTime[nCscClusters]/F");
  llpTree->Branch("cscClusterTimeSpread",             cscClusterTimeSpread,             "cscClusterTimeSpread[nCscClusters]/F");
  llpTree->Branch("cscClusterGenMuonDeltaR",             cscClusterGenMuonDeltaR,             "cscClusterGenMuonDeltaR[nCscClusters]/F");
  llpTree->Branch("cscClusterMajorAxis",             cscClusterMajorAxis,             "cscClusterMajorAxis[nCscClusters]/F");
  llpTree->Branch("cscClusterMinorAxis",             cscClusterMinorAxis,             "cscClusterMinorAxis[nCscClusters]/F");
  llpTree->Branch("cscClusterEtaPhiSpread",             cscClusterEtaPhiSpread,             "cscClusterEtaPhiSpread[nCscClusters]/F");
  llpTree->Branch("cscClusterPhiSpread",             cscClusterPhiSpread,             "cscClusterPhiSpread[nCscClusters]/F");
  llpTree->Branch("cscClusterEtaSpread",             cscClusterEtaSpread,             "cscClusterEtaSpread[nCscClusters]/F");
  llpTree->Branch("cscClusterXSpread",             cscClusterXSpread,             "cscClusterXSpread[nCscClusters]/F");
  llpTree->Branch("cscClusterYSpread",             cscClusterYSpread,             "cscClusterYSpread[nCscClusters]/F");
  llpTree->Branch("cscClusterZSpread",             cscClusterZSpread,             "cscClusterZSpread[nCscClusters]/F");
  llpTree->Branch("cscClusterPhi",             cscClusterPhi,             "cscClusterPhi[nCscClusters]/F");
  llpTree->Branch("cscClusterEta",             cscClusterEta,             "cscClusterEta[nCscClusters]/F");
  llpTree->Branch("cscClusterJetVetoPt",       cscClusterJetVetoPt,       "cscClusterJetVetoPt[nCscClusters]/F");
  llpTree->Branch("cscClusterJetVetoE",        cscClusterJetVetoE,        "cscClusterJetVetoE[nCscClusters]/F");
  llpTree->Branch("cscClusterMuonVetoPt",      cscClusterMuonVetoPt,      "cscClusterMuonVetoPt[nCscClusters]/F");
  llpTree->Branch("cscClusterMuonVetoE",       cscClusterMuonVetoE,       "cscClusterMuonVetoE[nCscClusters]/F");
  llpTree->Branch("cscClusterCaloJetVeto",     cscClusterCaloJetVeto,     "cscClusterCaloJetVeto[nCscClusters]/F");
  llpTree->Branch("cscClusterSize",            cscClusterSize,            "cscClusterSize[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus11",             cscClusterNSegmentChamberPlus11,             "cscClusterNSegmentChamberPlus11[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus12",             cscClusterNSegmentChamberPlus12,             "cscClusterNSegmentChamberPlus12[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus13",             cscClusterNSegmentChamberPlus13,             "cscClusterNSegmentChamberPlus13[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus21",             cscClusterNSegmentChamberPlus21,             "cscClusterNSegmentChamberPlus21[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus22",             cscClusterNSegmentChamberPlus22,             "cscClusterNSegmentChamberPlus22[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus31",             cscClusterNSegmentChamberPlus31,             "cscClusterNSegmentChamberPlus31[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus32",             cscClusterNSegmentChamberPlus32,             "cscClusterNSegmentChamberPlus32[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus41",             cscClusterNSegmentChamberPlus41,             "cscClusterNSegmentChamberPlus41[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberPlus42",             cscClusterNSegmentChamberPlus42,             "cscClusterNSegmentChamberPlus42[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus11",            cscClusterNSegmentChamberMinus11,            "cscClusterNSegmentChamberMinus11[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus12",            cscClusterNSegmentChamberMinus12,            "cscClusterNSegmentChamberMinus12[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus13",            cscClusterNSegmentChamberMinus13,            "cscClusterNSegmentChamberMinus13[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus21",            cscClusterNSegmentChamberMinus21,            "cscClusterNSegmentChamberMinus21[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus22",            cscClusterNSegmentChamberMinus22,            "cscClusterNSegmentChamberMinus22[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus31",            cscClusterNSegmentChamberMinus31,            "cscClusterNSegmentChamberMinus31[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus32",            cscClusterNSegmentChamberMinus32,            "cscClusterNSegmentChamberMinus32[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus41",            cscClusterNSegmentChamberMinus41,            "cscClusterNSegmentChamberMinus41[nCscClusters]/I");
  llpTree->Branch("cscClusterNSegmentChamberMinus42",            cscClusterNSegmentChamberMinus42,            "cscClusterNSegmentChamberMinus42[nCscClusters]/I");
  llpTree->Branch("cscClusterMe11Ratio",             cscClusterMe11Ratio,             "cscClusterMe11Ratio[nCscClusters]/F");
  llpTree->Branch("cscClusterMe12Ratio",             cscClusterMe12Ratio,             "cscClusterMe12Ratio[nCscClusters]/F");
  llpTree->Branch("cscClusterNStation",             cscClusterNStation,             "cscClusterNStation[nCscClusters]/I");
  llpTree->Branch("cscClusterMaxStation",             cscClusterMaxStation,             "cscClusterMaxStation[nCscClusters]/I");
  llpTree->Branch("cscClusterMaxStationRatio",             cscClusterMaxStationRatio,             "cscClusterMaxStationRatio[nCscClusters]/F");
  llpTree->Branch("cscClusterNChamber",             cscClusterNChamber,             "cscClusterNChamber[nCscClusters]/I");
  llpTree->Branch("cscClusterMaxChamber",             cscClusterMaxChamber,             "cscClusterMaxChamber[nCscClusters]/I");
  llpTree->Branch("cscClusterMaxChamberRatio",             cscClusterMaxChamberRatio,             "cscClusterMaxChamberRatio[nCscClusters]/F");
  llpTree->Branch("cscClusterVertexR",             cscClusterVertexR,             "cscClusterVertexR[nCscClusters]/F");
  llpTree->Branch("cscClusterVertexZ",             cscClusterVertexZ,             "cscClusterVertexZ[nCscClusters]/F");
  llpTree->Branch("cscClusterVertexDis",             cscClusterVertexDis,             "cscClusterVertexDis[nCscClusters]/F");
  llpTree->Branch("cscClusterVertexChi2",             cscClusterVertexChi2,             "cscClusterVertexChi2[nCscClusters]/F");
  llpTree->Branch("cscClusterVertexN1",             cscClusterVertexN1,             "cscClusterVertexN1[nCscClusters]/I");
  llpTree->Branch("cscClusterVertexN5",             cscClusterVertexN5,             "cscClusterVertexN5[nCscClusters]/I");
  llpTree->Branch("cscClusterVertexN10",             cscClusterVertexN10,             "cscClusterVertexN10[nCscClusters]/I");
  llpTree->Branch("cscClusterVertexN15",             cscClusterVertexN15,             "cscClusterVertexN15[nCscClusters]/I");
  llpTree->Branch("cscClusterVertexN20",             cscClusterVertexN20,             "cscClusterVertexN20[nCscClusters]/I");
  llpTree->Branch("cscClusterVertexN",             cscClusterVertexN,             "cscClusterVertexN[nCscClusters]/I");

  // llpTree->Branch("nRpc",&nRpc,"nRpc/I");
  // llpTree->Branch("rpcPhi",rpcPhi,"rpcPhi[nRpc]");
  // llpTree->Branch("rpcEta",rpcEta,"rpcEta[nRpc]");
  // llpTree->Branch("rpcX",rpcX,"rpcX[nRpc]");
  // llpTree->Branch("rpcY",rpcY,"rpcY[nRpc]");
  // llpTree->Branch("rpcZ",rpcZ,"rpcZ[nRpc]");
  // llpTree->Branch("rpcT",rpcT,"rpcT[nRpc]");
  // llpTree->Branch("rpcTError",rpcTError,"rpcTError[nRpc]");

  // llpTree->Branch("nDt",&nDt,"nDt/I");
  // llpTree->Branch("dtPhi",dtPhi,"dtPhi[nDt]");
  // llpTree->Branch("dtEta",dtEta,"dtEta[nDt]");
  // llpTree->Branch("dtX",dtX,"dtX[nDt]");
  // llpTree->Branch("dtY",dtY,"dtY[nDt]");
  // llpTree->Branch("dtZ",dtZ,"dtZ[nDt]");
  // llpTree->Branch("dtDirX",dtDirX,"dtDirX[nDt]");
  // llpTree->Branch("dtDirY",dtDirY,"dtDirY[nDt]");
  // llpTree->Branch("dtDirZ",dtDirZ,"dtDirZ[nDt]");
  // llpTree->Branch("dtT",dtT,"dtT[nDt]");
  // llpTree->Branch("dtTError",dtTError,"dtTError[nDt]");
};

void llp_ntupler::enableEcalRechitBranches()
{
  llpTree->Branch("nRechits", &nRechits,"nRechits/I");
  llpTree->Branch("ecalRechit_Eta", ecalRechit_Eta,"ecalRechit_Eta[nRechits]/F");
  llpTree->Branch("ecalRechit_Phi",ecalRechit_Phi, "ecalRechit_Phi[nRechits]/F");
  llpTree->Branch("ecalRechit_E", ecalRechit_E, "ecalRechit_E[nRechits]/F");
  llpTree->Branch("ecalRechit_T", ecalRechit_T, "ecalRechit_T[nRechits]/F");
  llpTree->Branch("ecalRechit_E_Error", ecalRechit_E_Error, "ecalRechit_E_Error[nRechits]/F");
  llpTree->Branch("ecalRechit_T_Error", ecalRechit_T_Error, "ecalRechit_T_Error[nRechits]/F");
  llpTree->Branch("ecalRechit_kSaturatedflag", ecalRechit_kSaturatedflag, "ecalRechit_kSaturatedflag[nRechits]/O");
  llpTree->Branch("ecalRechit_kLeadingEdgeRecoveredflag", ecalRechit_kLeadingEdgeRecoveredflag, "ecalRechit_kLeadingEdgeRecoveredflag[nRechits]/O");
  llpTree->Branch("ecalRechit_kPoorRecoflag", ecalRechit_kPoorRecoflag, "ecalRechit_kPoorRecoflag[nRechits]/O");
  llpTree->Branch("ecalRechit_kWeirdflag", ecalRechit_kWeirdflag, "ecalRechit_kWeirdflag[nRechits]/O");
  llpTree->Branch("ecalRechit_kDiWeirdflag", ecalRechit_kDiWeirdflag, "ecalRechit_kDiWeirdflag[nRechits]/O");

};

void llp_ntupler::enableJetBranches()
{
  llpTree->Branch("nJets", &nJets,"nJets/I");
  llpTree->Branch("jetE", jetE,"jetE[nJets]/F");
  llpTree->Branch("jetPt", jetPt,"jetPt[nJets]/F");
  llpTree->Branch("jetEta", jetEta,"jetEta[nJets]/F");
  llpTree->Branch("jetEt", jetEt,"jetEt[nJets]/F");

  llpTree->Branch("jetPhi", jetPhi,"jetPhi[nJets]/F");
  llpTree->Branch("jetCSV", jetCSV,"jetCSV[nJets]/F");
  llpTree->Branch("jetCISV", jetCISV,"jetCISV[nJets]/F");
  llpTree->Branch("jetProbb", jetProbb,"jetProbb[nJets]/F");
  llpTree->Branch("jetProbc", jetProbc,"jetProbc[nJets]/F");
  llpTree->Branch("jetProbudsg", jetProbudsg,"jetProbudsg[nJets]/F");
  llpTree->Branch("jetProbbb", jetProbbb,"jetProbbb[nJets]/F");
  llpTree->Branch("jetMass", jetMass, "jetMass[nJets]/F");
  llpTree->Branch("jetJetArea", jetJetArea, "jetJetArea[nJets]/F");
  llpTree->Branch("jetPileupE", jetPileupE, "jetPileupE[nJets]/F");
  llpTree->Branch("jetPileupId", jetPileupId, "jetPileupId[nJets]/F");
  llpTree->Branch("jetPileupIdFlag", jetPileupIdFlag, "jetPileupIdFlag[nJets]/I");
  llpTree->Branch("jetPassIDLoose", jetPassIDLoose, "jetPassIDLoose[nJets]/O");
  llpTree->Branch("jetPassIDTight", jetPassIDTight, "jetPassIDTight[nJets]/O");
  llpTree->Branch("jetPassMuFrac", jetPassMuFrac, "jetPassMuFrac[nJets]/O");
  llpTree->Branch("jetPassEleFrac", jetPassEleFrac, "jetPassEleFrac[nJets]/O");
  llpTree->Branch("jetPartonFlavor", jetPartonFlavor, "jetPartonFlavor[nJets]/I");
  llpTree->Branch("jetHadronFlavor", jetHadronFlavor, "jetHadronFlavor[nJets]/I");
  llpTree->Branch("jetChargedEMEnergyFraction", jetChargedEMEnergyFraction, "jetChargedEMEnergyFraction[nJets]/F");
  llpTree->Branch("jetNeutralEMEnergyFraction", jetNeutralEMEnergyFraction, "jetNeutralEMEnergyFraction[nJets]/F");
  llpTree->Branch("jetChargedHadronEnergyFraction", jetChargedHadronEnergyFraction, "jetChargedHadronEnergyFraction[nJets]/F");
  llpTree->Branch("jetNeutralHadronEnergyFraction", jetNeutralHadronEnergyFraction, "jetNeutralHadronEnergyFraction[nJets]/F");
  llpTree->Branch("jetMuonEnergyFraction", jetMuonEnergyFraction, "jetMuonEnergyFraction[nJets]/F");
  llpTree->Branch("jetHOEnergyFraction", jetHOEnergyFraction, "jetHOEnergyFraction[nJets]/F");
  llpTree->Branch("jetHFHadronEnergyFraction", jetHFHadronEnergyFraction, "jetHFHadronEnergyFraction[nJets]/F");
  llpTree->Branch("jetHFEMEnergyFraction",jetHFEMEnergyFraction, "jetHFEMEnergyFraction[nJets]/F");
  llpTree->Branch("jetAllMuonPt", jetAllMuonPt,"jetAllMuonPt[nJets]/F");
  llpTree->Branch("jetAllMuonEta", jetAllMuonEta,"jetAllMuonEta[nJets]/F");
  llpTree->Branch("jetAllMuonPhi", jetAllMuonPhi,"jetAllMuonPhi[nJets]/F");
  llpTree->Branch("jetAllMuonM", jetAllMuonM,"jetAllMuonM[nJets]/F");
  llpTree->Branch("jetPtWeightedDZ", jetPtWeightedDZ,"jetPtWeightedDZ[nJets]/F");
  llpTree->Branch("jetNRechits", jetNRechits,"jetNRechits[nJets]/I");
  llpTree->Branch("jetRechitE", jetRechitE,"jetRechitE[nJets]/F");
  llpTree->Branch("jetRechitT", jetRechitT,"jetRechitT[nJets]/F");
  llpTree->Branch("jetRechitT_rms", jetRechitT_rms,"jetRechitT_rms[nJets]/F");

  llpTree->Branch("jetRechitE_Error", jetRechitE_Error,"jetRechitE_Error[nJets]/F");
  llpTree->Branch("jetRechitT_Error", jetRechitT_Error,"jetRechitT_Error[nJets]/F");
  llpTree->Branch("jetAlphaMax",jetAlphaMax,"jetAlphaMax[nJets]/F");
  llpTree->Branch("jetBetaMax",jetBetaMax,"jetBetaMax[nJets]/F");
  llpTree->Branch("jetGammaMax_ET",jetGammaMax_ET,"jetGammaMax_ET[nJets]/F");
  llpTree->Branch("jetGammaMax_EM",jetGammaMax_EM,"jetGammaMax_EM[nJets]/F");
  llpTree->Branch("jetGammaMax_Hadronic",jetGammaMax_Hadronic,"jetGammaMax_Hadronic[nJets]/F");
  llpTree->Branch("jetGammaMax",jetGammaMax,"jetGammaMax[nJets]/F");
  llpTree->Branch("jetPtAllTracks",jetPtAllTracks,"jetPtAllTracks[nJets]/F");
  llpTree->Branch("jetPtAllPVTracks",jetPtAllPVTracks,"jetPtAllPVTracks[nJets]/F");
  llpTree->Branch("jetMedianTheta2D",jetMedianTheta2D,"jetMedianTheta2D[nJets]/F");
  llpTree->Branch("jetMedianIP",jetMedianIP,"jetMedianIP[nJets]/F");
  llpTree->Branch("jetMinDeltaRAllTracks",jetMinDeltaRAllTracks,"jetMinDeltaRAllTracks[nJets]/F");
  llpTree->Branch("jetMinDeltaRPVTracks",jetMinDeltaRPVTracks,"jetMinDeltaRPVTracks[nJets]/F");
  llpTree->Branch("jet_sig_et1",jet_sig_et1,"jet_sig_et1[nJets]/F");
  llpTree->Branch("jet_sig_et2",jet_sig_et2,"jet_sig_et2[nJets]/F");
  llpTree->Branch("jet_energy_frac",jet_energy_frac,"jet_energy_frac[nJets]/F");
  llpTree->Branch("jet_matched",jet_matched,"jet_matched[nJets]/O");//matched to gen particles



};

void llp_ntupler::enableCaloJetBranches()
{
  llpTree->Branch("nCaloJets", &nCaloJets,"nCaloJets/I");
  llpTree->Branch("calojetE", calojetE,"calojetE[nCaloJets]/F");
  llpTree->Branch("calojetEt", calojetEt,"calojetEt[nCaloJets]/F");
  llpTree->Branch("calojetPt", calojetPt,"calojetPt[nCaloJets]/F");
  llpTree->Branch("calojetEta", calojetEta,"calojetEta[nCaloJets]/F");
  llpTree->Branch("calojetPhi", calojetPhi,"calojetPhi[nCaloJets]/F");
  llpTree->Branch("calojetMass", calojetMass, "calojetMass[nCaloJets]/F");
  llpTree->Branch("calojetJetArea", calojetJetArea, "calojetJetArea[nCaloJets]/F");
  llpTree->Branch("calojetPileupE", calojetPileupE, "calojetPileupE[nCaloJets]/F");
  llpTree->Branch("calojetPileupId", calojetPileupId, "calojetPileupId[nCaloJets]/F");
  llpTree->Branch("calojetPileupIdFlag", calojetPileupIdFlag, "calojetPileupIdFlag[nCaloJets]/I");
  llpTree->Branch("calojetPassIDLoose", calojetPassIDLoose, "calojetPassIDLoose[nCaloJets]/O");
  llpTree->Branch("calojetPassIDTight", calojetPassIDTight, "calojetPassIDTight[nCaloJets]/O");
  llpTree->Branch("calojetNRechits", calojetNRechits,"calojetNRechits[nCaloJets]/I");
  llpTree->Branch("calojetRechitE", calojetRechitE,"calojetRechitE[nCaloJets]/F");
  llpTree->Branch("calojetRechitT", calojetRechitT,"calojetRechitT[nCaloJets]/F");
  llpTree->Branch("calojetRechitT_rms", calojetRechitT_rms,"calojetRechitT_rms[nCaloJets]/F");
  llpTree->Branch("calojetAlphaMax",calojetAlphaMax,"calojetAlphaMax[nCaloJets]/F");
  llpTree->Branch("calojetBetaMax",calojetBetaMax,"calojetBetaMax[nCaloJets]/F");
  llpTree->Branch("calojetGammaMax_ET",calojetGammaMax_ET,"calojetGammaMax_ET[nCaloJets]/F");
  llpTree->Branch("calojetGammaMax_EM",calojetGammaMax_EM,"calojetGammaMax_EM[nCaloJets]/F");
  llpTree->Branch("calojetGammaMax_Hadronic",calojetGammaMax_Hadronic,"calojetGammaMax_Hadronic[nCaloJets]/F");
  llpTree->Branch("calojetGammaMax",calojetGammaMax,"calojetGammaMax[nCaloJets]/F");
  llpTree->Branch("calojet_HadronicEnergyFraction", calojet_HadronicEnergyFraction,"calojet_HadronicEnergyFraction[nCaloJets]/F");
  llpTree->Branch("calojet_EMEnergyFraction", calojet_EMEnergyFraction,"calojet_EMEnergyFraction[nCaloJets]/F");
  llpTree->Branch("calojetPtAllTracks",calojetPtAllTracks,"calojetPtAllTracks[nCaloJets]/F");
  llpTree->Branch("calojetPtAllPVTracks",calojetPtAllPVTracks,"calojetPtAllPVTracks[nCaloJets]/F");
  llpTree->Branch("calojetMedianTheta2D",calojetMedianTheta2D,"calojetMedianTheta2D[nCaloJets]/F");
  llpTree->Branch("calojetMedianIP",calojetMedianIP,"calojetMedianIP[nCaloJets]/F");
  llpTree->Branch("calojetMinDeltaRAllTracks",calojetMinDeltaRAllTracks,"calojetMinDeltaRAllTracks[nCaloJets]/F");
  llpTree->Branch("calojetMinDeltaRPVTracks",calojetMinDeltaRPVTracks,"calojetMinDeltaRPVTracks[nCaloJets]/F");
};


void llp_ntupler::enableJetAK8Branches()
{
  llpTree->Branch("nFatJets", &nFatJets,"nFatJets/i");
  llpTree->Branch("fatJetE", fatJetE,"fatJetE[nFatJets]/F");
  llpTree->Branch("fatJetPt", fatJetPt,"fatJetPt[nFatJets]/F");
  llpTree->Branch("fatJetEta", fatJetEta,"fatJetEta[nFatJets]/F");
  llpTree->Branch("fatJetPhi", fatJetPhi,"fatJetPhi[nFatJets]/F");
  llpTree->Branch("fatJetCorrectedPt", fatJetCorrectedPt,"fatJetCorrectedPt[nFatJets]/F");
  // llpTree->Branch("fatJetCorrectedEta", fatJetCorrectedEta,"fatJetCorrectedEta[nFatJets]/F");
  // llpTree->Branch("fatJetCorrectedPhi", fatJetCorrectedPhi,"fatJetCorrectedPhi[nFatJets]/F");
  llpTree->Branch("fatJetPrunedM", fatJetPrunedM,"fatJetPrunedM[nFatJets]/F");
  llpTree->Branch("fatJetTrimmedM", fatJetTrimmedM,"fatJetTrimmedM[nFatJets]/F");
  llpTree->Branch("fatJetFilteredM", fatJetFilteredM,"fatJetFilteredM[nFatJets]/F");
  llpTree->Branch("fatJetSoftDropM", fatJetSoftDropM,"fatJetSoftDropM[nFatJets]/F");
  llpTree->Branch("fatJetCorrectedSoftDropM", fatJetCorrectedSoftDropM,"fatJetCorrectedSoftDropM[nFatJets]/F");
  llpTree->Branch("fatJetUncorrectedSoftDropM", fatJetUncorrectedSoftDropM,"fatJetUncorrectedSoftDropM[nFatJets]/F");
  llpTree->Branch("fatJetTau1", fatJetTau1,"fatJetTau1[nFatJets]/F");
  llpTree->Branch("fatJetTau2", fatJetTau2,"fatJetTau2[nFatJets]/F");
  llpTree->Branch("fatJetTau3", fatJetTau3,"fatJetTau3[nFatJets]/F");
  llpTree->Branch("fatJetMaxSubjetCSV", fatJetMaxSubjetCSV, "fatJetMaxSubjetCSV[nFatJets]/F");
  llpTree->Branch("fatJetPassIDLoose", fatJetPassIDLoose,"fatJetPassIDLoose[nFatJets]/O");
  llpTree->Branch("fatJetPassIDTight", fatJetPassIDTight,"fatJetPassIDTight[nFatJets]/O");
};

void llp_ntupler::enableMetBranches()
{
  llpTree->Branch("metPt", &metPt, "metPt/F");
  llpTree->Branch("metPhi", &metPhi, "metPhi/F");
  llpTree->Branch("sumMET", &sumMET, "sumMET/F");
  llpTree->Branch("metType0Pt", &metType0Pt, "metType0Pt/F");
  llpTree->Branch("metType0Phi", &metType0Phi, "metType0Phi/F");
  llpTree->Branch("metType1Pt_raw", &metType1Pt_raw, "metType1Pt_raw/F");
  llpTree->Branch("metType1Pt", &metType1Pt, "metType1Pt/F");
  llpTree->Branch("metType1Px", &metType1Px, "metType1Px/F");
  llpTree->Branch("metType1Py", &metType1Py, "metType1Py/F");
  llpTree->Branch("metType1Eta", &metType1Eta, "metType1Eta/F");
  llpTree->Branch("metType1Phi", &metType1Phi, "metType1Phi/F");
  llpTree->Branch("metType1Phi_raw", &metType1Phi_raw, "metType1Phi_raw/F");
  llpTree->Branch("metType0Plus1Pt", &metType0Plus1Pt, "metType0Plus1Pt/F");
  llpTree->Branch("metType0Plus1Phi", &metType0Plus1Phi, "metType0Plus1Phi/F");
  llpTree->Branch("metNoHFPt", &metNoHFPt, "metNoHFPt/F");
  llpTree->Branch("metNoHFPhi", &metNoHFPhi, "metNoHFPhi/F");
  llpTree->Branch("metPuppiPt", &metPuppiPt, "metPuppiPt/F");
  llpTree->Branch("metPuppiPhi", &metPuppiPhi, "metPuppiPhi/F");
  llpTree->Branch("metCaloPt", &metCaloPt, "metCaloPt/F");
  llpTree->Branch("metCaloPhi", &metCaloPhi, "metCaloPhi/F");

  llpTree->Branch("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter, "Flag_HBHENoiseFilter/O");
  llpTree->Branch("Flag_HBHETightNoiseFilter", &Flag_HBHETightNoiseFilter, "Flag_HBHETightNoiseFilter/O");
  llpTree->Branch("Flag_HBHEIsoNoiseFilter", &Flag_HBHEIsoNoiseFilter, "Flag_HBHEIsoNoiseFilter/O");
  llpTree->Branch("Flag_badChargedCandidateFilter", &Flag_badChargedCandidateFilter, "Flag_badChargedCandidateFilter/O");
  llpTree->Branch("Flag_badMuonFilter", &Flag_badMuonFilter, "Flag_badMuonFilter/O");
  llpTree->Branch("Flag_badGlobalMuonFilter", &Flag_badGlobalMuonFilter, "Flag_badGlobalMuonFilter/O");
  llpTree->Branch("Flag_duplicateMuonFilter", &Flag_duplicateMuonFilter, "Flag_duplicateMuonFilter/O");
  llpTree->Branch("Flag_CSCTightHaloFilter", &Flag_CSCTightHaloFilter, "Flag_CSCTightHaloFilter/O");
  llpTree->Branch("Flag_hcalLaserEventFilter", &Flag_hcalLaserEventFilter, "Flag_hcalLaserEventFilter/O");
  llpTree->Branch("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter, "Flag_EcalDeadCellTriggerPrimitiveFilter/O");
  llpTree->Branch("Flag_EcalDeadCellBoundaryEnergyFilter", &Flag_EcalDeadCellBoundaryEnergyFilter, "Flag_EcalDeadCellBoundaryEnergyFilter/O");
  llpTree->Branch("Flag_goodVertices", &Flag_goodVertices, "Flag_goodVertices/O");
  llpTree->Branch("Flag_trackingFailureFilter", &Flag_trackingFailureFilter, "Flag_trackingFailureFilter/O");
  llpTree->Branch("Flag_eeBadScFilter", &Flag_eeBadScFilter, "Flag_eeBadScFilter/O");
  llpTree->Branch("Flag_ecalLaserCorrFilter", &Flag_ecalLaserCorrFilter, "Flag_ecalLaserCorrFilter/O");
  llpTree->Branch("Flag_trkPOGFilters", &Flag_trkPOGFilters, "Flag_trkPOGFilters/O");
  llpTree->Branch("Flag_trkPOG_manystripclus53X", &Flag_trkPOG_manystripclus53X, "Flag_trkPOG_manystripclus53X/O");
  llpTree->Branch("Flag_trkPOG_toomanystripclus53X", &Flag_trkPOG_toomanystripclus53X, "Flag_trkPOG_toomanystripclus53X/O");
  llpTree->Branch("Flag_trkPOG_logErrorTooManyClusters", &Flag_trkPOG_logErrorTooManyClusters, "Flag_trkPOG_logErrorTooManyClusters/O");
  llpTree->Branch("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter, "Flag_BadPFMuonFilter/O");
  llpTree->Branch("Flag_BadChargedCandidateFilter", &Flag_BadChargedCandidateFilter, "Flag_BadChargedCandidateFilter/O");
  llpTree->Branch("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter, "Flag_ecalBadCalibFilter/O");
  llpTree->Branch("Flag_METFilters", &Flag_METFilters, "Flag_METFilters/O");
};

void llp_ntupler::enableTriggerBranches()
{
  nameHLT = new std::vector<std::string>; nameHLT->clear();
  llpTree->Branch("HLTDecision", &triggerDecision, ("HLTDecision[" + std::to_string(NTriggersMAX) +  "]/O").c_str());
  llpTree->Branch("HLTPrescale", &triggerHLTPrescale, ("HLTPrescale[" + std::to_string(NTriggersMAX) +  "]/I").c_str());
  //llpTree->Branch("HLTMR", &HLTMR, "HLTMR/F");
  //llpTree->Branch("HLTRSQ", &HLTRSQ, "HLTRSQ/F");
};

void llp_ntupler::enableMCBranches()
{
  llpTree->Branch("nGenJets", &nGenJets, "nGenJets/I");
  llpTree->Branch("genJetE", genJetE, "genJetE[nGenJets]/F");
  llpTree->Branch("genJetPt", genJetPt, "genJetPt[nGenJets]/F");
  llpTree->Branch("genJetEta", genJetEta, "genJetEta[nGenJets]/F");
  llpTree->Branch("genJetPhi", genJetPhi, "genJetPhi[nGenJets]/F");
  llpTree->Branch("genMetPtCalo", &genMetPtCalo, "genMetPtCalo/F");
  llpTree->Branch("genMetPhiCalo", &genMetPhiCalo, "genMetPhiCalo/F");
  llpTree->Branch("genMetPtTrue", &genMetPtTrue, "genMetPtTrue/F");
  llpTree->Branch("genMetPhiTrue", &genMetPhiTrue, "genMetPhiTrue/F");
  llpTree->Branch("genVertexX", &genVertexX, "genVertexX/F");
  llpTree->Branch("genVertexY", &genVertexY, "genVertexY/F");
  llpTree->Branch("genVertexZ", &genVertexZ, "genVertexZ/F");
  llpTree->Branch("genVertexT", &genVertexT, "genVertexT/F");
  llpTree->Branch("genWeight", &genWeight, "genWeight/F");
  llpTree->Branch("genSignalProcessID", &genSignalProcessID, "genSignalProcessID/i");
  llpTree->Branch("genQScale", &genQScale, "genQScale/F");
  llpTree->Branch("genAlphaQCD", &genAlphaQCD, "genAlphaQCD/F");
  llpTree->Branch("genAlphaQED", &genAlphaQED, "genAlphaQED/F");
  scaleWeights = new std::vector<float>; scaleWeights->clear();
  pdfWeights = new std::vector<float>; pdfWeights->clear();
  alphasWeights = new std::vector<float>; alphasWeights->clear();
  if (isFastsim_) {
    llpTree->Branch("lheComments", "std::string",&lheComments);
  }
  llpTree->Branch("scaleWeights", "std::vector<float>",&scaleWeights);
  llpTree->Branch("pdfWeights", "std::vector<float>",&pdfWeights);
  llpTree->Branch("alphasWeights", "std::vector<float>",&alphasWeights);
};

void llp_ntupler::enableGenParticleBranches()
{
  llpTree->Branch("nGenParticle", &nGenParticle, "nGenParticle/I");
  llpTree->Branch("gParticleMotherId", gParticleMotherId, "gParticleMotherId[nGenParticle]/I");
  llpTree->Branch("gParticleMotherIndex", gParticleMotherIndex, "gParticleMotherIndex[nGenParticle]/I");
  llpTree->Branch("gParticleId", gParticleId, "gParticleId[nGenParticle]/I");
  llpTree->Branch("gParticleStatus", gParticleStatus, "gParticleStatus[nGenParticle]/I");
  llpTree->Branch("gParticleE", gParticleE, "gParticleE[nGenParticle]/F");
  llpTree->Branch("gParticlePt", gParticlePt, "gParticlePt[nGenParticle]/F");
  llpTree->Branch("gParticlePx", gParticlePx, "gParticlePx[nGenParticle]/F");
  llpTree->Branch("gParticlePy", gParticlePy, "gParticlePy[nGenParticle]/F");
  llpTree->Branch("gParticlePz", gParticlePz, "gParticlePz[nGenParticle]/F");
  llpTree->Branch("gParticleEta", gParticleEta, "gParticleEta[nGenParticle]/F");
  llpTree->Branch("gParticlePhi", gParticlePhi, "gParticlePhi[nGenParticle]/F");
  llpTree->Branch("gParticleProdVertexX", gParticleProdVertexX, "gParticleProdVertexX[nGenParticle]/F");
  llpTree->Branch("gParticleProdVertexY", gParticleProdVertexY, "gParticleProdVertexY[nGenParticle]/F");
  llpTree->Branch("gParticleProdVertexZ", gParticleProdVertexZ, "gParticleProdVertexZ[nGenParticle]/F");

   llpTree->Branch("gLLP_prod_vertex_x", gLLP_prod_vertex_x, "gLLP_prod_vertex_x[2]/F");
   llpTree->Branch("gLLP_prod_vertex_y", gLLP_prod_vertex_y, "gLLP_prod_vertex_y[2]/F");
   llpTree->Branch("gLLP_prod_vertex_z", gLLP_prod_vertex_z, "gLLP_prod_vertex_z[2]/F");
   llpTree->Branch("gLLP_decay_vertex_x", gLLP_decay_vertex_x, "gLLP_decay_vertex_x[2]/F");
   llpTree->Branch("gLLP_decay_vertex_y", gLLP_decay_vertex_y, "gLLP_decay_vertex_y[2]/F");
   llpTree->Branch("gLLP_decay_vertex_z", gLLP_decay_vertex_z, "gLLP_decay_vertex_z[2]/F");
   llpTree->Branch("gLLP_beta", gLLP_beta, "gLLP_beta[2]/F");
   llpTree->Branch("gLLP_e", gLLP_e, "gLLP_e[2]/F");
   llpTree->Branch("gLLP_pt", gLLP_pt, "gLLP_pt[2]/F");
   llpTree->Branch("gLLP_eta", gLLP_eta, "gLLP_eta[2]/F");
   llpTree->Branch("gLLP_phi", gLLP_phi, "gLLP_phi[2]/F");

   llpTree->Branch("gLLP_travel_time", gLLP_travel_time, "gLLP_travel_time[2]/F");

   llpTree->Branch("gLLP_daughter_travel_time", gLLP_daughter_travel_time, "gLLP_daughter_travel_time[4]/F");
   llpTree->Branch("gLLP_daughter_id", gLLP_daughter_id, "gLLP_daughter_id[4]/I");
   llpTree->Branch("gLLP_daughter_pt", gLLP_daughter_pt, "gLLP_daughter_pt[4]/F");
   llpTree->Branch("gLLP_daughter_eta", gLLP_daughter_eta, "gLLP_daughter_eta[4]/F");
   llpTree->Branch("gLLP_daughter_phi", gLLP_daughter_phi, "gLLP_daughter_phi[4]/F");
   llpTree->Branch("gLLP_daughter_eta_ecalcorr", gLLP_daughter_eta_ecalcorr, "gLLP_daughter_eta_ecalcorr[4]/F");
   llpTree->Branch("gLLP_daughter_phi_ecalcorr", gLLP_daughter_phi_ecalcorr, "gLLP_daughter_phi_ecalcorr[4]/F");

   llpTree->Branch("gLLP_daughter_e", gLLP_daughter_e, "gLLP_daughter_e[4]/F");
   llpTree->Branch("photon_travel_time", photon_travel_time, "photon_travel_time[4]/F");
   llpTree->Branch("gen_time", gen_time, "gen_time[4]/F");
   llpTree->Branch("gen_time_pv", gen_time_pv, "gen_time_pv[4]/F");

   llpTree->Branch("gLLP_min_delta_r_match_calojet", gLLP_min_delta_r_match_calojet, "gLLP_min_delta_r_match_calojet[4]/F");
   llpTree->Branch("gLLP_daughter_match_calojet_index", gLLP_daughter_match_calojet_index, "gLLP_daughter_match_calojet_index[4]/i");
   llpTree->Branch("gLLP_daughter_match_jet_index", gLLP_daughter_match_jet_index, "gLLP_daughter_match_jet_index[4]/i");
   llpTree->Branch("gLLP_min_delta_r_match_jet", gLLP_min_delta_r_match_jet, "gLLP_min_delta_r_match_jet[4]/F");

};

//------ Load the miniAOD objects and reset tree variables for each event ------//
void llp_ntupler::loadEvent(const edm::Event& iEvent)//load all miniAOD objects for the current event
{
  iEvent.getByToken(triggerBitsToken_, triggerBits);
  iEvent.getByToken(hepMCToken_, hepMC);
  iEvent.getByToken(triggerBitsToken_, triggerBits);
  iEvent.getByToken(metFilterBitsToken_, metFilterBits);
  iEvent.getByToken(verticesToken_, vertices);
  iEvent.getByToken(cscSegmentInputToken_,cscSegments);
  iEvent.getByToken(dtSegmentInputToken_,dtSegments);
  iEvent.getByToken(rpcRecHitInputToken_,rpcRecHits);
  iEvent.getByToken(tracksTag_,tracks);
  iEvent.getByToken(PFCandsToken_, pfCands);
  iEvent.getByToken(PFClustersToken_, pfClusters);
  iEvent.getByToken(muonsToken_, muons);
  iEvent.getByToken(electronsToken_, electrons);
  iEvent.getByToken(photonsToken_, photons);
  iEvent.getByToken(tausToken_, taus);
  iEvent.getByToken(jetsCaloToken_, jetsCalo);
  // iEvent.getByToken(jetsPFToken_, jetsPF);
  iEvent.getByToken(jetsToken_, jets);
  iEvent.getByToken(jetsPuppiToken_, jetsPuppi);
  iEvent.getByToken(jetsAK8Token_, jetsAK8);
  iEvent.getByToken(genMetCaloToken_, genMetsCalo);
  iEvent.getByToken(genMetTrueToken_, genMetsTrue);
  iEvent.getByToken(metToken_, mets);
  //iEvent.getByToken(metNoHFToken_, metsNoHF);
  iEvent.getByToken(metPuppiToken_, metsPuppi);
//  iEvent.getByToken(hcalNoiseInfoToken_,hcalNoiseInfo);
  iEvent.getByToken(secondaryVerticesToken_,secondaryVertices);
  iEvent.getByToken(rhoAllToken_,rhoAll);
  iEvent.getByToken(rhoFastjetAllToken_,rhoFastjetAll);
  iEvent.getByToken(rhoFastjetAllCaloToken_,rhoFastjetAllCalo);
  iEvent.getByToken(rhoFastjetCentralCaloToken_,rhoFastjetCentralCalo);
  iEvent.getByToken(rhoFastjetCentralChargedPileUpToken_,rhoFastjetCentralChargedPileUp);
  iEvent.getByToken(rhoFastjetCentralNeutralToken_,rhoFastjetCentralNeutral);
  iEvent.getByToken(beamSpotToken_,beamSpot);
  iEvent.getByToken(ebRecHitsToken_,ebRecHits);
  iEvent.getByToken(eeRecHitsToken_,eeRecHits);
  iEvent.getByToken(esRecHitsToken_,esRecHits);
  iEvent.getByToken(ebeeClustersToken_,ebeeClusters);
  iEvent.getByToken(esClustersToken_,esClusters);
  iEvent.getByToken(conversionsToken_,conversions);
  iEvent.getByToken(singleLegConversionsToken_,singleLegConversions);
  iEvent.getByToken(gedGsfElectronCoresToken_,gedGsfElectronCores);
  iEvent.getByToken(gedPhotonCoresToken_, gedPhotonCores);
  iEvent.getByToken(generalTrackToken_,generalTracks);
//  iEvent.getByToken(superClustersToken_,superClusters);
//  iEvent.getByToken(lostTracksToken_,lostTracks);
//  iEvent.getByToken(hbheNoiseFilterToken_, hbheNoiseFilter);
//  iEvent.getByToken(hbheTightNoiseFilterToken_, hbheTightNoiseFilter);
//  iEvent.getByToken(hbheIsoNoiseFilterToken_, hbheIsoNoiseFilter);
  //iEvent.getByToken(badChargedCandidateFilterToken_, badChargedCandidateFilter);
  //iEvent.getByToken(badMuonFilterToken_, badMuonFilter);
  if(readGenVertexTime_) iEvent.getByToken(genParticles_t0_Token_,genParticles_t0);
  if (useGen_) {
//    iEvent.getByToken(genParticlesToken_,genParticles);
    iEvent.getByToken(genParticlesToken_,genParticles);
    iEvent.getByToken(genJetsToken_,genJets);

    //for Spring16 fastsim, this has been changed and removed
//    if (!isFastsim_) iEvent.getByToken(lheInfoToken_, lheInfo);

    iEvent.getByToken(genInfoToken_,genInfo);
    iEvent.getByToken(puInfoToken_,puInfo);
  }


}

//called by the loadEvent() method
void llp_ntupler::resetBranches()
{
    //reset tree variables
    resetEventInfoBranches();
    resetPVAllBranches();
    resetPVTracksBranches();
    resetPileUpBranches();
    resetMuonBranches();
    resetElectronBranches();
    resetTauBranches();
    resetPhotonBranches();
    resetJetBranches();
    resetCaloJetBranches();
    resetMuonSystemBranches();
    resetMetBranches();
    resetGenParticleBranches();
    resetMCBranches();
    resetTriggerBranches();
    resetEcalRechitBranches();

};

void llp_ntupler::resetEventInfoBranches()
{
  //Event
  nPV = -1;
  eventNum = 0;
  eventTime = 0;
  lumiNum = 0;
  runNum = 0;
  nSlimmedSecondV = 0;
  pvX = -999.0;
  pvY = -999.0;
  pvZ = -999.0;
  fixedGridRhoAll = -999.0;
  fixedGridRhoFastjetAll = -999.0;
  fixedGridRhoFastjetAllCalo = -999.0;
  fixedGridRhoFastjetCentralCalo = -999.0;
  fixedGridRhoFastjetCentralChargedPileUp = -999.0;
  fixedGridRhoFastjetCentralNeutral = -999.0;
  return;
};

void llp_ntupler::resetPVAllBranches()
{
  nPVAll = 0;
  for(int i = 0; i < MAX_NPV; i++)
  {
    pvAllX[i] = -999.;
    pvAllY[i] = -999.;
    pvAllZ[i] = -999.;
    pvAllLogSumPtSq[i] = -999.;
    pvAllSumPx[i] = -999.;
    pvAllSumPy[i] = -999.;
  }
};

void llp_ntupler::resetPVTracksBranches()
{
  nPVTracks = 0;
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    pvTrackPt[i]  = -999.;
    pvTrackEta[i] = -999.;
    pvTrackPhi[i] = -999.;
  }
};

void llp_ntupler::resetPileUpBranches()
{
  nBunchXing = 0;
  for(int i = 0; i < MAX_NBX; i++)
  {
    BunchXing[i] = -999;
    nPU[i]       = -999;
    nPUmean[i]   = -999.;
  }
};

void llp_ntupler::resetMuonBranches()
{
  nMuons = 0;
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    muonE[i] = 0.0;
    muonPt[i] = 0.0;
    muonEta[i] = 0.0;
    muonPhi[i] = 0.0;
    muonCharge[i] = -99;
    muonIsLoose[i] = false;
    muonIsMedium[i] = false;
    muonIsTight[i] = false;
    muon_d0[i] = -99.0;
    muon_dZ[i] = -99.0;
    muon_ip3d[i] = -99.0;
    muon_ip3dSignificance[i] = -99.0;
    muonType[i] = 0;
    muonQuality[i] = 0;
    muon_pileupIso[i] = -99.0;
    muon_chargedIso[i] = -99.0;
    muon_photonIso[i] = -99.0;
    muon_neutralHadIso[i] = -99.0;
    muon_ptrel[i] = -99.0;
    muon_chargedMiniIso[i] = -99.0;
    muon_photonAndNeutralHadronMiniIso[i] = -99.0;
    muon_chargedPileupMiniIso[i] = -99.0;
    muon_activityMiniIsoAnnulus[i] = -99.0;
    muon_passSingleMuTagFilter[i] = false;
    for (int q=0;q<MAX_MuonHLTFilters;q++) muon_passHLTFilter[i][q] = false;
    muon_validFractionTrackerHits[i] = -99.0;
    muon_isGlobal[i] = false;
    muon_normChi2[i] = -99.0;
    muon_chi2LocalPosition[i] = -99.0;
    muon_kinkFinder[i] = -99.0;
    muon_segmentCompatability[i] = -99.0;
    muonIsICHEPMedium[i] = false;
  }
};

void llp_ntupler::resetElectronBranches()
{
  nElectrons = 0;
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    eleE[i] = 0.0;
    elePt[i] = 0.0;
    eleEta[i] = 0.0;
    elePhi[i] = 0.0;
    eleE_SC[i] = -99.0;
    eleEta_SC[i] = -99.0;
    elePhi_SC[i] = -99.0;
    eleSigmaIetaIeta[i] = -99.0;
    eleFull5x5SigmaIetaIeta[i] = -99.0;
    eleR9[i] = -99;
    ele_dEta[i] = -99;
    ele_dPhi[i] = -99;
    ele_HoverE[i] = -99;
    ele_d0[i] = -99;
    ele_dZ[i] = -99;
    ele_ip3d[i] = -99;
    ele_ip3dSignificance[i] = -99;
    ele_pileupIso[i] = -99.0;
    ele_chargedIso[i] = -99.0;
    ele_photonIso[i] = -99.0;
    ele_neutralHadIso[i] = -99.0;
    ele_MissHits[i] = -99;
    ele_PassConvVeto[i] = false;
    ele_OneOverEminusOneOverP[i] = -99.0;
    ele_IDMVAGeneralPurpose[i] = -99.0;
    ele_IDMVACategoryGeneralPurpose[i] = -1;
    ele_IDMVAHZZ[i] = -99.0;
    ele_IDMVACategoryHZZ[i] = -1;
    ele_RegressionE[i] = -99.0;
    ele_CombineP4[i] = -99.0;
    ele_ptrel[i] = -99.0;
    ele_chargedMiniIso[i] = -99.0;
    ele_photonAndNeutralHadronMiniIso[i] = -99.0;
    ele_chargedPileupMiniIso[i] = -99.0;
    ele_activityMiniIsoAnnulus[i] = -99.0;
    ele_passSingleEleTagFilter[i] = false;
    ele_passTPOneTagFilter[i] = false;
    ele_passTPTwoTagFilter[i] = false;
    ele_passTPOneProbeFilter[i] = false;
    ele_passTPTwoProbeFilter[i] = false;
    for (int q=0;q<MAX_ElectronHLTFilters;q++) ele_passHLTFilter[i][q] = false;
  }

};

void llp_ntupler::resetTauBranches()
{
  nTaus = 0;
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    tauE[i] = -999.;
    tauPt[i] = -999.;
    tauEta[i] = -999.;
    tauPhi[i] = -999.;
    tau_IsLoose[i] = false;
    tau_IsMedium[i] = false;
    tau_IsTight[i] = false;
    tau_passEleVetoLoose[i] = false;
    tau_passEleVetoMedium[i] = false;
    tau_passEleVetoTight[i] = false;
    tau_passMuVetoLoose[i] = false;
    tau_passMuVetoMedium[i] = false;
    tau_passMuVetoTight[i] = false;
    tau_ID[i] = 0;//tauID Bits
    tau_combinedIsoDeltaBetaCorr3Hits[i] = -999.;;
    tau_chargedIsoPtSum[i] = -999.;;
    tau_neutralIsoPtSum[i] = -999.;;
    tau_puCorrPtSum[i] = -999.;;
    tau_eleVetoMVA[i] = -999.;;
    tau_eleVetoCategory[i] = -999.;;
    tau_muonVetoMVA[i] = -999.;;
    tau_isoMVAnewDMwLT[i] = -999.;;
    tau_isoMVAnewDMwoLT[i] = -999.;;
    tau_leadCandPt[i] = -999.;;
    tau_leadCandID[i] = -999.;;
    tau_leadChargedHadrCandPt[i] = -999.;;
    tau_leadChargedHadrCandID[i] = -999.;;
  }
};

void llp_ntupler::resetPhotonBranches()
{
  nPhotons = 0;
  nPhotons_overlap = 0;
  //Photon
  for(int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    phoE[i] = 0.0;
    phoPt[i] = 0.0;
    phoEta[i] = 0.0;
    phoPhi[i] = 0.0;
    phoSigmaIetaIeta[i] = -99.0;
    phoFull5x5SigmaIetaIeta[i] = -99.0;
    phoR9[i] = -99.0;
    pho_sminor[i] = -99.0;
    pho_smajor[i] = -99.0;
    pho_HoverE[i] = -99.0;
    pho_sumChargedHadronPt[i] = -99.0;
    pho_sumNeutralHadronEt[i] = -99.0;
    pho_sumPhotonEt[i] = -99.0;
    pho_ecalPFClusterIso[i] = -99.0;
    pho_hcalPFClusterIso[i] = -99.0;
    pho_trkSumPtHollowConeDR03[i] = -99.0;
    pho_sumWorstVertexChargedHadronPt[i] = -99.0;
    pho_pfIsoChargedHadronIso[i] = -99.0;
    pho_pfIsoChargedHadronIsoWrongVtx[i] = -99.0;
    pho_pfIsoNeutralHadronIso[i] = -99.0;
    pho_pfIsoPhotonIso[i] = -99.0;
    pho_pfIsoModFrixione[i] = -99.0;
    pho_pfIsoSumPUPt[i] = -99.0;
    pho_isConversion[i] = false;
    pho_passEleVeto[i] = false;
    pho_RegressionE[i] = -99.0;
    pho_RegressionEUncertainty[i] = -99.0;
    pho_IDMVA[i] = -99.0;
    pho_superClusterEnergy[i] = -99.0;
    pho_superClusterRawEnergy[i] = -99.0;
    pho_superClusterEta[i]    = -99.0;
    pho_superClusterPhi[i]    = -99.0;
    pho_superClusterX[i]      = -99.0;
    pho_superClusterY[i]      = -99.0;
    pho_superClusterZ[i]      = -99.0;
    pho_hasPixelSeed[i] = false;
    for (int q=0;q<MAX_PhotonHLTFilters;q++) pho_passHLTFilter[i][q] = false;
    pho_convType[i] = -99;
    pho_convTrkZ[i] = -99.;
    pho_convTrkClusZ[i] = -99.;

    for (int ipv=0; ipv < MAX_NPV; ++ipv) {
      pho_sumChargedHadronPtAllVertices[i][ipv] = -99.0;
      pho_vtxSumPx[i][ipv] = 0.;
      pho_vtxSumPy[i][ipv] = 0.;
    }
    pho_isStandardPhoton[i] = true;
    pho_seedRecHitSwitchToGain6[i] = false;
    pho_seedRecHitSwitchToGain1[i] = false;
    pho_anyRecHitSwitchToGain6[i] = false;
    pho_anyRecHitSwitchToGain1[i] = false;
  }
  return;
};

void llp_ntupler::resetMuonSystemBranches()
{
    nCsc = 0;
    for ( int i = 0; i < OBJECTARRAYSIZE; i++)
    {
      cscPhi[i] = 0.0;
      cscEta[i] = 0.0;
      cscX[i] = 0.0;
      cscY[i] = 0.0;
      cscZ[i] = 0.0;
      cscDirectionX[i] = 0.0;
      cscDirectionY[i] = 0.0;
      cscDirectionZ[i] = 0.0;
      cscNRecHits[i] = 0.0;
      cscNRecHits_flag[i] = 0.0;
      cscT[i] = 0.0;
      cscChi2[i] = 0.0;
    }
    nCscClusters = 0;
    for ( int i = 0; i < OBJECTARRAYSIZE; i++)
    {
      cscClusterSize[i] = -999;
      cscClusterX[i] = -999.;
      cscClusterY[i] = -999.;
      cscClusterZ[i] = -999.;
      cscClusterTime[i] = -999.;
      cscClusterGenMuonDeltaR[i] = 999.;
      cscClusterTimeSpread[i] = -999.;
      cscClusterMajorAxis[i] = -999.;
      cscClusterMinorAxis[i] = -999.;
      cscClusterXSpread[i] = -999.;
      cscClusterYSpread[i] = -999.;
      cscClusterZSpread[i] = -999.;
      cscClusterEtaPhiSpread[i] = -999.;
      cscClusterEtaSpread[i] = -999.;
      cscClusterPhiSpread[i] = -999.;
      cscClusterEta[i] = -999.;
      cscClusterPhi[i] = -999.;
      cscClusterJetVetoPt[i] = 0.0;
      cscClusterJetVetoE[i] = 0.0;
      cscClusterCaloJetVeto[i] = 0.0;
      cscClusterMuonVetoPt[i] = 0.0;
      cscClusterMuonVetoE[i] = 0.0;
      cscClusterNChamber[i] = -999;
      cscClusterMaxChamberRatio[i] = -999.;
      cscClusterMaxChamber[i] = -999;
      cscClusterNStation[i] = -999;
      cscClusterMaxStationRatio[i] = -999.;
      cscClusterMaxStation[i] = -999;
      cscClusterMe11Ratio[i] = -999.;
      cscClusterMe12Ratio[i] = -999.;
      cscClusterNSegmentChamberPlus11[i] = -999;
      cscClusterNSegmentChamberPlus12[i] = -999;
      cscClusterNSegmentChamberPlus13[i] = -999;
      cscClusterNSegmentChamberPlus21[i] = -999;
      cscClusterNSegmentChamberPlus22[i] = -999;
      cscClusterNSegmentChamberPlus31[i] = -999;
      cscClusterNSegmentChamberPlus32[i] = -999;
      cscClusterNSegmentChamberPlus41[i] = -999;
      cscClusterNSegmentChamberPlus42[i] = -999;
      cscClusterNSegmentChamberMinus11[i] = -999;
      cscClusterNSegmentChamberMinus12[i] = -999;
      cscClusterNSegmentChamberMinus13[i] = -999;
      cscClusterNSegmentChamberMinus21[i] = -999;
      cscClusterNSegmentChamberMinus22[i] = -999;
      cscClusterNSegmentChamberMinus31[i] = -999;
      cscClusterNSegmentChamberMinus32[i] = -999;
      cscClusterNSegmentChamberMinus41[i] = -999;
      cscClusterNSegmentChamberMinus42[i] = -999;
      cscClusterVertexR[i] = 0.0;
      cscClusterVertexZ[i] = 0.0;
      cscClusterVertexDis[i] = 0.0;
      cscClusterVertexChi2[i] = 0.0;
      cscClusterVertexN[i] = 0;
      cscClusterVertexN1[i] = 0;
      cscClusterVertexN5[i] = 0;
      cscClusterVertexN10[i] = 0;
      cscClusterVertexN15[i] = 0;
      cscClusterVertexN20[i] = 0;
    }
    nRpc = 0;
    for ( int i = 0; i < OBJECTARRAYSIZE; i++)
    {
      rpcPhi[i] = 0.0;
      rpcEta[i] = 0.0;
      rpcX[i] = 0.0;
      rpcY[i] = 0.0;
      rpcZ[i] = 0.0;
      rpcT[i] = 0.0;
      rpcTError[i] = 0.0;
    }
    nDt = 0;
    for ( int i = 0; i < OBJECTARRAYSIZE; i++)
    {
      dtPhi[i] = 0.0;
      dtEta[i] = 0.0;
      dtX[i] = 0.0;
      dtY[i] = 0.0;
      dtZ[i] = 0.0;
      dtDirX[i] = 0.0;
      dtDirY[i] = 0.0;
      dtDirZ[i] = 0.0;
      dtT[i] = 0.0;
      dtTError[i] = 0.0;
    }
    return;
};
void llp_ntupler::resetJetBranches()
{
  nJets = 0;
  for ( int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    jetE[i] = 0.0;
    jetEt[i] = 0.0;
    jetPt[i] = 0.0;
    jetEta[i] = 0.0;
    jetPhi[i] = 0.0;
    jetCSV[i] = 0.0;
    jetCISV[i] = 0.0;
    jetMass[i] =  -99.0;
    jetJetArea[i] = -99.0;
    jetPileupE[i] = -99.0;
    jetPileupId[i] = -99.0;
    jetPileupIdFlag[i] = -1;
    jetPassIDLoose[i] = false;
    jetPassIDTight[i] = false;
    jetPassMuFrac[i] = false;
    jetPassEleFrac[i] = false;
    jetPartonFlavor[i] = 0;
    jetHadronFlavor[i] = 0;
    jetChargedEMEnergyFraction[i] = -99.0;
    jetNeutralEMEnergyFraction[i] = -99.0;
    jetChargedHadronEnergyFraction[i] = -99.0;
    jetNeutralHadronEnergyFraction[i] = -99.0;
    jetMuonEnergyFraction[i] = -99.0;
    jetHOEnergyFraction[i] = -99.0;
    jetHFHadronEnergyFraction[i] = -99.0;
    jetHFEMEnergyFraction[i] = -99.0;
    jetAllMuonPt[i] = 0.0;
    jetAllMuonEta[i] = 0.0;
    jetAllMuonPhi[i] = 0.0;
    jetAllMuonM[i] = 0.0;
    jetPtWeightedDZ[i] = 0.0;
    jetNRechits[i] = 0;
    jetRechitE[i] = 0.0;
    jetRechitT[i] = 0.0;
    jetRechitT_rms[i] = 0.0;

    jetRechitE_Error[i] = 0.0;
    jetRechitT_Error[i] = 0.0;
    jetGammaMax[i] = -99.0;
    jetGammaMax_EM[i] = -99.0;
    jetGammaMax_Hadronic[i] = -99.0;
    jetGammaMax_ET[i] = -99.0;
    jetAlphaMax[i] = -99.0;
    jetBetaMax[i] = -99.0;
    jetPtAllTracks[i] = -99.0;
    jetPtAllPVTracks[i] = -99.0;
    jetMedianTheta2D[i] = -99.0;
    jetMedianIP[i] = -99.0;
    jetMinDeltaRAllTracks[i] =-99.0;
    jetMinDeltaRPVTracks[i] = -99.0;
    jet_sig_et1[i] = -99.0;
    jet_sig_et2[i] = -99.0;
    jet_energy_frac[i] = 0.0;
    jet_matched[i] = false;

  }
  return;
};
void llp_ntupler::resetEcalRechitBranches()
{
  nRechits = 0;
  for ( int i = 0; i < RECHITARRAYSIZE; i++)
  {

    ecalRechit_Eta[i] = -999.;
    ecalRechit_Phi[i] = -999.;
    ecalRechit_E[i] = -999.;
    ecalRechit_T[i] = -999.;
    ecalRechit_E_Error[i] = -999.;
    ecalRechit_T_Error[i] = -999.;
    ecalRechit_kSaturatedflag[i] = false;
    ecalRechit_kLeadingEdgeRecoveredflag[i] = false;
    ecalRechit_kPoorRecoflag[i] = false;
    ecalRechit_kWeirdflag[i] = false;
    ecalRechit_kDiWeirdflag[i] = false;

  }
  return;
};
void llp_ntupler::resetCaloJetBranches()
{
  nCaloJets = 0;
  for ( int i = 0; i < OBJECTARRAYSIZE; i++)
  {
    calojetE[i] = 0.0;
    calojetEt[i] = 0.0;
    calojetPt[i] = 0.0;
    calojetEta[i] = 0.0;
    calojetPhi[i] = 0.0;
    calojetMass[i] =  -99.0;
    calojetJetArea[i] = -99.0;
    calojetPileupE[i] = -99.0;
    calojetPileupId[i] = -99.0;
    calojetPileupIdFlag[i] = -1;
    calojetPassIDLoose[i] = false;
    calojetPassIDTight[i] = false;

    calojetNRechits[i] = 0;
    calojetRechitE[i] = 0.0;
    calojetRechitT[i] = 0.0;
    calojetRechitT_rms[i] = 0.0;

    calojetAlphaMax[i] = -99.0;
    calojetBetaMax[i] = -99.0;
    calojetGammaMax[i] = -99.0;
    calojetGammaMax_EM[i] = -99.0;
    calojetGammaMax_Hadronic[i] = -99.0;
    calojetGammaMax_ET[i] = -99.0;
    calojetPtAllTracks[i] = -99.0;
    calojetPtAllPVTracks[i] = -99.0;
    calojetMedianTheta2D[i] = -99.0;
    calojetMedianIP[i] = -99.0;
    calojetMinDeltaRAllTracks[i] =-99.0;
    calojetMinDeltaRPVTracks[i] = -99.0;

    calojet_HadronicEnergyFraction[i] = -666.;
    calojet_EMEnergyFraction[i] = -666.;
  }
  return;
};


void llp_ntupler::resetMetBranches()
{
  metPt = -999;
  metPhi = -999;
  sumMET = -99.0;
  UncMETdpx = -99.0;
  UncMETdpy = -99.0;
  UncMETdSumEt = -99.0;
  metType0Pt = -99.0;
  metType0Phi = -99.0;
  metType1Pt_raw = -99.0;
  metType1Pt = -99.0;
  metType1Px = -99.0;
  metType1Py = -99.0;
  metType1Eta = -99.0;
  metType1Phi = -99.0;
  metType1Phi_raw = -99.0;
  metType0Plus1Pt = -99.0;
  metType0Plus1Phi = -99.0;
  metPtRecomputed = -99.0;
  metPhiRecomputed = -99.0;
  metNoHFPt = -99.0;
  metNoHFPhi = -99.0;
  metPuppiPt = -99.0;
  metPuppiPhi = -99.0;
  metCaloPt = -999;
  metCaloPhi = -999;
  Flag_HBHENoiseFilter = false;
  Flag_HBHETightNoiseFilter = false;
  Flag_HBHEIsoNoiseFilter = false;
  Flag_badChargedCandidateFilter = false;
  Flag_badMuonFilter = false;
  Flag_badGlobalMuonFilter = false;
  Flag_duplicateMuonFilter = false;
  Flag_CSCTightHaloFilter = false;
  Flag_hcalLaserEventFilter = false;
  Flag_EcalDeadCellTriggerPrimitiveFilter = false;
  Flag_EcalDeadCellBoundaryEnergyFilter = false;
  Flag_goodVertices = false;
  Flag_trackingFailureFilter = false;
  Flag_eeBadScFilter = false;
  Flag_ecalLaserCorrFilter = false;
  Flag_trkPOGFilters = false;
  Flag_trkPOG_manystripclus53X = false;
  Flag_trkPOG_toomanystripclus53X = false;
  Flag_trkPOG_logErrorTooManyClusters = false;
  Flag_BadPFMuonFilter = false;
  Flag_BadChargedCandidateFilter = false;
  Flag_ecalBadCalibFilter = false;
  Flag_METFilters = false;

  metType1PtJetResUp=-999.;
  metType1PtJetResDown=-999.;
  metType1PtJetEnUp=-999.;
  metType1PtJetEnDown=-999.;
  metType1PtMuonEnUp=-999.;
  metType1PtMuonEnDown=-999.;
  metType1PtElectronEnUp=-999.;
  metType1PtElectronEnDown=-999.;
  metType1PtTauEnUp=-999.;
  metType1PtTauEnDown=-999.;
  metType1PtUnclusteredEnUp=-999.;
  metType1PtUnclusteredEnDown=-999.;
  metType1PtPhotonEnUp=-999.;
  metType1PtPhotonEnDown=-999.;
  metType1PtMETUncertaintySize=-999.;
  metType1PtJetResUpSmear=-999.;
  metType1PtJetResDownSmear=-999.;
  metType1PtMETFullUncertaintySize=-999.;

  metType1PhiJetResUp=-999.;
  metType1PhiJetResDown=-999.;
  metType1PhiJetEnUp=-999.;
  metType1PhiJetEnDown=-999.;
  metType1PhiMuonEnUp=-999.;
  metType1PhiMuonEnDown=-999.;
  metType1PhiElectronEnUp=-999.;
  metType1PhiElectronEnDown=-999.;
  metType1PhiTauEnUp=-999.;
  metType1PhiTauEnDown=-999.;
  metType1PhiUnclusteredEnUp=-999.;
  metType1PhiUnclusteredEnDown=-999.;
  metType1PhiPhotonEnUp=-999.;
  metType1PhiPhotonEnDown=-999.;
  metType1PhiMETUncertaintySize=-999.;
  metType1PhiJetResUpSmear=-999.;
  metType1PhiJetResDownSmear=-999.;
  metType1PhiMETFullUncertaintySize=-999.;
};
void llp_ntupler::resetGenParticleBranches()
{
  for(int i = 0; i < GENPARTICLEARRAYSIZE; i++)
  {
    gParticleMotherId[i] = -99999;
    gParticleMotherIndex[i] = -99999;
    gParticleId[i] = -99999;
    gParticleStatus[i] = -99999;
    gParticleE[i] = -99999.0;
    gParticlePt[i] = -99999.0;
    gParticlePx[i] = -99999.0;
    gParticlePy[i] = -99999.0;
    gParticlePz[i] = -99999.0;
    gParticleEta[i] = -99999.0;
    gParticlePhi[i] = -99999.0;

    gParticleProdVertexX[i] = -99999.0;
    gParticleProdVertexY[i] = -99999.0;
    gParticleProdVertexZ[i] = -99999.0;

  }
  for ( int i = 0; i < LLP_ARRAY_SIZE; i++ )
  {
    gLLP_prod_vertex_x[i] = -666.;
    gLLP_prod_vertex_y[i] = -666.;
    gLLP_prod_vertex_z[i] = -666.;
    gLLP_decay_vertex_x[i] = -666.;
    gLLP_decay_vertex_y[i] = -666.;
    gLLP_decay_vertex_z[i] = -666.;
    gLLP_beta[i] = -666.;
    gLLP_pt[i] = -666.;
    gLLP_eta[i] = -666.;
    gLLP_e[i] = -666.;
    gLLP_phi[i] = -666.;
    gLLP_travel_time[i] = -666.;
  }

  for ( int i = 0; i < LLP_DAUGHTER_ARRAY_SIZE; i++ )
  {
    gLLP_daughter_id[i] = 0;
    gLLP_daughter_pt[i] = -666.;
    gLLP_daughter_eta[i] = -666.;
    gLLP_daughter_phi[i] = -666.;
    gLLP_daughter_eta_ecalcorr[i] = -666.;
    gLLP_daughter_phi_ecalcorr[i] = -666.;
    gLLP_daughter_e[i] = -666.;
    gLLP_daughter_travel_time[i] = -666.;
    gen_time[i] = -666.;
    gen_time_pv[i] = -666.;
    photon_travel_time[i] = -666.;
    photon_travel_time_pv[i] = -666.;
    gLLP_daughter_match_calojet_index[i] = 666;
    gLLP_daughter_match_jet_index[i] = 666;
    gLLP_min_delta_r_match_calojet[i] = -666.;
    gLLP_min_delta_r_match_jet[i] = -666.;



}
  return;
};

void llp_ntupler::resetMCBranches()
{
  nGenJets = 0;
  for ( int i = 0; i < OBJECTARRAYSIZE; i++ )
  {
    genJetE[i] = -999.;
    genJetPt[i] = -999.;
    genJetEta[i] = -999.;
    genJetPhi[i] = -999.;
    genJetMET[i] = -999.;
    //genJet_match_jet_index[i] = 666;
    //genJet_min_delta_r_match_jet[i] = -666.;
  }

  genMetPtCalo  = -999.;
  genMetPhiCalo = -999.;
  genMetPtTrue  = -999.;
  genMetPhiTrue = -999.;

  genVertexX = -999.;
  genVertexY = -999.;
  genVertexZ = -999.;
  genVertexT = -999.;

  genWeight = -999.;
  genSignalProcessID = 0;
  genQScale = -999.;
  genAlphaQCD = -999.;
  genAlphaQED = -999.;

  return;
};

void llp_ntupler::resetTriggerBranches()
{
  for( int i = 0; i < NTriggersMAX; i++ )
  {
    triggerDecision[i] = false;
    triggerHLTPrescale[i] = 0;
  }
};
//------ Method called for each run ------//

void llp_ntupler::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {


}


//------ Method called for each lumi block ------//
void llp_ntupler::beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const&) {

}


//------ Method called for each event ------//

void llp_ntupler::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;

  //initialize
  loadEvent(iEvent); //loads objects and resets tree branches
  NEvents->Fill(0); //increment event count

  //resetting output tree branches

  resetBranches();
  fillEventInfo(iEvent);
  // fillPVAll();
  // fillPVTracks();
  fillMuons(iEvent);
  fillMuonSystem(iEvent, iSetup);
  fillElectrons(iEvent);
  fillPhotons(iEvent, iSetup);
  fillJets(iSetup);
  fillMet(iEvent);
  if ( enableTriggerInfo_ ) fillTrigger( iEvent );
  if ( enableCaloJet_ ) fillCaloJets( iSetup );
  if (!isData) {
    fillPileUp();
    fillMC();
    fillGenParticles();
  }

  llpTree->Fill();


};

//------ Method called once each job just before starting event loop ------//
void llp_ntupler::beginJob()
{
  setBranches();
}

//------ Method called once each job just after ending the event loop ------//
void llp_ntupler::endJob(){};



//------ Methods to fill tree variables ------//
bool llp_ntupler::fillEventInfo(const edm::Event& iEvent)
{
  isData = isData_;
  runNum = iEvent.id().run();
  lumiNum = iEvent.luminosityBlock();
  eventNum = iEvent.id().event();
  eventTime = iEvent.eventAuxiliary().time().unixTime();

  //number of slimmedSecondaryVertices
  nSlimmedSecondV = secondaryVertices->size();

  //select the primary vertex, if any
  nPV = 0;
  myPV = &(vertices->front());
  bool foundPV = false;
  for(unsigned int i = 0; i < vertices->size(); i++)
  {
    if(vertices->at(i).isValid() && !vertices->at(i).isFake())
    {
      if (!foundPV)
      {
        myPV = &(vertices->at(i));
        foundPV = true;
      }
      nPV++;
    }
  }

  pvX = myPV->x();
  pvY = myPV->y();
  pvZ = myPV->z();

  //get rho
  fixedGridRhoAll = *rhoAll;
  fixedGridRhoFastjetAll = *rhoFastjetAll;
  fixedGridRhoFastjetAllCalo = *rhoFastjetAllCalo;
  fixedGridRhoFastjetCentralCalo = *rhoFastjetCentralCalo;
  fixedGridRhoFastjetCentralChargedPileUp = *rhoFastjetCentralChargedPileUp;
  fixedGridRhoFastjetCentralNeutral = *rhoFastjetCentralNeutral;

  return true;
};

bool llp_ntupler::fillPVAll()
{
  nPVAll = std::min(int(vertices->size()),int(MAX_NPV));
  for (int ipv = 0; ipv < nPVAll; ++ipv)
  {
    const reco::Vertex &vtx = vertices->at(ipv);
    pvAllX[ipv] = vtx.x();
    pvAllY[ipv] = vtx.y();
    pvAllZ[ipv] = vtx.z();
  }

  double pvAllSumPtSqD[MAX_NPV];
  double pvAllSumPxD[MAX_NPV];
  double pvAllSumPyD[MAX_NPV];

  for (int ipv=0; ipv<nPVAll; ++ipv)
  {
    pvAllSumPtSqD[ipv] = 0.;
    pvAllSumPxD[ipv]   = 0.;
    pvAllSumPyD[ipv]   = 0.;
  }

/*
  int counter = 0;
  for (const pat::PackedCandidate &pfcand : *packedPFCands)
  {
    counter++;
    if (pfcand.charge()==0) continue;
    double mindz = std::numeric_limits<double>::max();
    int ipvmin = -1;
    for (int ipv = 0; ipv < nPVAll; ++ipv) {
      const reco::Vertex &vtx = vertices->at(ipv);
      double dz = std::abs(pfcand.dz(vtx.position()));
      if (dz<mindz) {
        mindz = dz;
        ipvmin = ipv;
      }
    }

    if (mindz<0.2 && ipvmin>=0 && ipvmin<MAX_NPV) {
      pvAllSumPtSqD[ipvmin] += pfcand.pt()*pfcand.pt();
      pvAllSumPxD[ipvmin] += pfcand.px();
      pvAllSumPyD[ipvmin] += pfcand.py();
    }
  }
  */
  for (int ipv=0; ipv<nPVAll; ++ipv) {
    pvAllLogSumPtSq[ipv] = log(pvAllSumPtSqD[ipv]);
    pvAllSumPx[ipv] = pvAllSumPxD[ipv];
    pvAllSumPy[ipv] = pvAllSumPyD[ipv];
  }

  return true;
};

bool llp_ntupler::fillPVTracks()
{
  //
  // for(unsigned int i = 0; i < vertices->size(); i++)
  // {
  //   if(vertices->at(i).isValid() && !vertices->at(i).isFake())
  //   {
  //     myPV = &(vertices->at(i));
  //     for(auto pvTrack = myPV->tracks_begin(); pvTrack != myPV->tracks_end(); pvTrack++)
  //     {
  //       if( (*pvTrack)->pt() > pvTrack_pt_cut )
  //       {
  //         pvTrackPt[nPVTracks]  = (*pvTrack)->pt();
  //         pvTrackEta[nPVTracks] = (*pvTrack)->eta();
  //         pvTrackPhi[nPVTracks] = (*pvTrack)->phi();
  //         nPVTracks++;
  //       }
  //     }
  //   }
  // }
  //
  for (auto vertex = vertices->begin(); vertex != vertices->end(); vertex++){
    if(vertex->isValid() && !vertex->isFake())
    {
      for(auto pvTrack = vertex->tracks_begin(); pvTrack != vertex->tracks_end(); pvTrack++)
      {
        if( (*pvTrack)->pt() > pvTrack_pt_cut )
        {
          pvTrackPt[nPVTracks]  = (*pvTrack)->pt();
          pvTrackEta[nPVTracks] = (*pvTrack)->eta();
          pvTrackPhi[nPVTracks] = (*pvTrack)->phi();
          nPVTracks++;
        }
      }
    }
  }
  return true;
};

bool llp_ntupler::fillPileUp()
{
  for(const PileupSummaryInfo &pu : *puInfo)
  {
    BunchXing[nBunchXing] = pu.getBunchCrossing();
    nPU[nBunchXing] = pu.getPU_NumInteractions();
    nPUmean[nBunchXing] = pu.getTrueNumInteractions();
    nBunchXing++;
  }
  return true;
};

bool llp_ntupler::fillMuonSystem(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::ESHandle<CSCGeometry> cscG;
    edm::ESHandle<DTGeometry> dtG;
    edm::ESHandle<RPCGeometry> rpcG;

    iSetup.get<MuonGeometryRecord>().get(cscG);
    iSetup.get<MuonGeometryRecord>().get(dtG);
    iSetup.get<MuonGeometryRecord>().get(rpcG);

    vector<Point> points; //vector defined for DBScan Clustering algorithm
    vector<Point> pointsInTimeOnly; //vector defined for DBScan Clustering algorithm
    for (const CSCSegment cscSegment : *cscSegments) {
	float globPhi   = 0.;
	float globX = 0.;
	float globY = 0.;
	float globZ = 0.;
	float globEta = 0.;


	CSCDetId id  = (CSCDetId)(cscSegment).cscDetId();
	LocalPoint segPos = (cscSegment).localPosition();
	LocalVector segDirection = (cscSegment).localDirection();
	const CSCChamber* cscchamber = cscG->chamber(id);
	if (cscchamber) {
	  GlobalPoint globalPosition = cscchamber->toGlobal(segPos);
	  GlobalVector globalDirection = cscchamber->toGlobal(segDirection);
	  globPhi   = globalPosition.phi();
	  globEta   = globalPosition.eta();
	  globX = globalPosition.x();
	  globY = globalPosition.y();
	  globZ = globalPosition.z();
	  // globR = pow(globX*globX+globY*globY,0.5);
	  cscNRecHits[nCsc] = cscSegment.nRecHits();
	  cscX[nCsc] = globX;
	  cscY[nCsc] = globY;
	  cscZ[nCsc] = globZ;
	  cscPhi[nCsc] = globPhi;
	  cscEta[nCsc] = globEta;
	  cscDirectionX[nCsc] = globalDirection.x();
	  cscDirectionY[nCsc] = globalDirection.y();
	  cscDirectionZ[nCsc] = globalDirection.z();
	  cscT[nCsc] = cscSegment.time();
	  cscChi2[nCsc] = cscSegment.chi2();

	  // look at flags of the rechits
	  const std::vector<CSCRecHit2D> cscrechits2d = cscSegment.specificRecHits();
	  int cscNRecHits_flagged = 0;
	  for (const CSCRecHit2D recHit2d : cscrechits2d) {
	    if (!(recHit2d.quality()==1)) continue;
	    if(recHit2d.badStrip()) continue;
	    if (recHit2d.badWireGroup()) continue;

	    // std::cout<<cscSegment.nRecHits()<<", " << recHit2d.quality()<<", "<<recHit2d.badStrip()<<", "<<recHit2d.badWireGroup()<<", "<<recHit2d.errorWithinStrip()<<", "<<recHit2d.energyDepositedInLayer()<<std::endl;
	    cscNRecHits_flagged++;
	  }
	  cscNRecHits_flag[nCsc] = cscNRecHits_flagged;

	  // for dbscan
	    Point p;
	    p.phi = globPhi;
	    p.eta = globEta;
	    p.x = globX;
	    p.y = globY;
	    p.z = globZ;
	    p.t = cscSegment.time();
	    p.dirX = globalDirection.x();
	    p.dirY = globalDirection.y();
	    p.dirZ = globalDirection.z();
	    p.station = cscStation(globX,globY,globZ);
	    p.chamber = cscChamber(globX,globY,globZ);
	    p.clusterID = UNCLASSIFIED;
	    points.push_back(p);
	    nCsc++;
	}
    }


    //Do DBSCAN Clustering
    int min_point = 10;  //minimum number of segments to call it a cluster
    float epsilon = 0.2; //cluster radius parameter
    DBSCAN ds(min_point, epsilon, points);
    ds.run();
    ds.result();
    ds.clusterMoments();
    ds.vertexing();
    ds.sort_clusters();

    //Save cluster information
    for ( auto &tmp : ds.CscCluster ) {
      cscClusterX[nCscClusters] =tmp.x;
      cscClusterY[nCscClusters] =tmp.y;
      cscClusterZ[nCscClusters] =tmp.z;
      cscClusterTime[nCscClusters] = tmp.t;
      cscClusterEta[nCscClusters] =tmp.eta;
      cscClusterPhi[nCscClusters] = tmp.phi;
      cscClusterMajorAxis[nCscClusters] =tmp.MajorAxis;
      cscClusterMinorAxis[nCscClusters] =tmp.MinorAxis;
      cscClusterXSpread[nCscClusters] =tmp.XSpread;
      cscClusterYSpread[nCscClusters] =tmp.YSpread;
      cscClusterZSpread[nCscClusters] =tmp.ZSpread;
      cscClusterEtaPhiSpread[nCscClusters] =tmp.EtaPhiSpread;
      cscClusterEtaSpread[nCscClusters] =tmp.EtaSpread;
      cscClusterPhiSpread[nCscClusters] = tmp.PhiSpread;
      cscClusterTimeSpread[nCscClusters] = tmp.TSpread;
      cscClusterSize[nCscClusters] = tmp.nCscSegments;

      cscClusterNSegmentChamberPlus11[nCscClusters] = tmp.nCscSegmentChamberPlus11;
      cscClusterNSegmentChamberPlus12[nCscClusters] = tmp.nCscSegmentChamberPlus12;
      cscClusterNSegmentChamberPlus13[nCscClusters] = tmp.nCscSegmentChamberPlus13;
      cscClusterNSegmentChamberPlus21[nCscClusters] = tmp.nCscSegmentChamberPlus21;
      cscClusterNSegmentChamberPlus22[nCscClusters] = tmp.nCscSegmentChamberPlus22;
      cscClusterNSegmentChamberPlus31[nCscClusters] = tmp.nCscSegmentChamberPlus31;
      cscClusterNSegmentChamberPlus32[nCscClusters] = tmp.nCscSegmentChamberPlus32;
      cscClusterNSegmentChamberPlus41[nCscClusters] = tmp.nCscSegmentChamberPlus41;
      cscClusterNSegmentChamberPlus42[nCscClusters] = tmp.nCscSegmentChamberPlus42;
      cscClusterNSegmentChamberMinus11[nCscClusters] = tmp.nCscSegmentChamberMinus11;
      cscClusterNSegmentChamberMinus12[nCscClusters] = tmp.nCscSegmentChamberMinus12;
      cscClusterNSegmentChamberMinus13[nCscClusters] = tmp.nCscSegmentChamberMinus13;
      cscClusterNSegmentChamberMinus21[nCscClusters] = tmp.nCscSegmentChamberMinus21;
      cscClusterNSegmentChamberMinus22[nCscClusters] = tmp.nCscSegmentChamberMinus22;
      cscClusterNSegmentChamberMinus31[nCscClusters] = tmp.nCscSegmentChamberMinus31;
      cscClusterNSegmentChamberMinus32[nCscClusters] = tmp.nCscSegmentChamberMinus32;
      cscClusterNSegmentChamberMinus41[nCscClusters] = tmp.nCscSegmentChamberMinus41;
      cscClusterNSegmentChamberMinus42[nCscClusters] = tmp.nCscSegmentChamberMinus42;
      cscClusterMaxChamber[nCscClusters] = tmp.maxChamber;
      cscClusterMaxChamberRatio[nCscClusters] = 1.0*tmp.maxChamberSegment/tmp.nCscSegments;
      cscClusterNChamber[nCscClusters] = tmp.nChamber;
      cscClusterMaxStation[nCscClusters] = tmp.maxStation;
      cscClusterMaxStationRatio[nCscClusters] = 1.0*tmp.maxStationSegment/tmp.nCscSegments;
      cscClusterNStation[nCscClusters] = tmp.nStation;

      cscClusterMe11Ratio[nCscClusters] = tmp.Me11Ratio;
      cscClusterMe12Ratio[nCscClusters] = tmp.Me12Ratio;
      cscClusterVertexR[nCscClusters] = tmp.vertex_r;
      cscClusterVertexZ[nCscClusters] = tmp.vertex_z;
      cscClusterVertexChi2[nCscClusters] = tmp.vertex_chi2;
      cscClusterVertexDis[nCscClusters] = tmp.vertex_dis;
      cscClusterVertexN[nCscClusters] = tmp.vertex_n;
      cscClusterVertexN1[nCscClusters] = tmp.vertex_n1;
      cscClusterVertexN5[nCscClusters] = tmp.vertex_n5;
      cscClusterVertexN15[nCscClusters] = tmp.vertex_n15;
      cscClusterVertexN20[nCscClusters] = tmp.vertex_n20;
      cscClusterVertexN10[nCscClusters] = tmp.vertex_n10;

      //Jet veto/ muon veto
      cscClusterJetVetoPt[nCscClusters] = 0.0;
      cscClusterJetVetoE[nCscClusters] = 0.0;
      cscClusterCaloJetVeto[nCscClusters] = 0.0;
      cscClusterMuonVetoPt[nCscClusters] = 0.0;
      cscClusterMuonVetoE[nCscClusters] = 0.0;

      for (const reco::PFJet &j : *jets) {
        //if (j.pt() < 10) continue;
        if (fabs(j.eta())>3.0) continue;
        if (deltaR(tmp.eta, tmp.phi, j.eta(),j.phi()) < 0.4 && j.pt() > cscClusterJetVetoPt[nCscClusters] ) {
          cscClusterJetVetoPt[nCscClusters]  = j.pt();
        }
        if (deltaR(tmp.eta, tmp.phi, j.eta(),j.phi()) < 0.4 && j.energy() > cscClusterJetVetoE[nCscClusters] ) {
          cscClusterJetVetoE[nCscClusters]  = j.energy();
        }
      }
      for(const pat::Muon &mu : *muons) {
	//if (mu.pt() < 20.0) continue;
        if (fabs(mu.eta()) > 3.0) continue;
        if (deltaR(tmp.eta, tmp.phi, mu.eta(), mu.phi()) < 0.4 && mu.pt() > cscClusterMuonVetoPt[nCscClusters]) {
          cscClusterMuonVetoPt[nCscClusters] = mu.pt();
        }
        if (deltaR(tmp.eta, tmp.phi, mu.eta(), mu.phi()) < 0.4 && mu.energy() > cscClusterMuonVetoE[nCscClusters]) {
          cscClusterMuonVetoE[nCscClusters] = mu.energy();
        }
      }

      nCscClusters++;
    }





    for (const RPCRecHit rpcRecHit : *rpcRecHits){
	LocalPoint  rpcRecHitLocalPosition       = rpcRecHit.localPosition();
	// LocalError  segmentLocalDirectionError = iDT->localDirectionError();
	DetId geoid = rpcRecHit.geographicalId();
	RPCDetId rpcdetid = RPCDetId(geoid);
	const RPCChamber * rpcchamber = rpcG->chamber(rpcdetid);
	if (rpcchamber) {
	    GlobalPoint globalPosition = rpcchamber->toGlobal(rpcRecHitLocalPosition);
	    rpcX[nRpc] = globalPosition.x();
	    rpcY[nRpc] = globalPosition.y();
	    rpcZ[nRpc] = globalPosition.z();
	    rpcPhi[nRpc] = globalPosition.phi();
	    rpcEta[nRpc] = globalPosition.eta();
	    rpcT[nRpc] = rpcRecHit.time();
	    rpcTError[nRpc] = rpcRecHit.timeError();
	    nRpc++;
	}
    }
    for(DTRecSegment4D dtSegment : *dtSegments){
	LocalPoint  segmentLocalPosition       = dtSegment.localPosition();
	LocalVector segmentLocalDirection      = dtSegment.localDirection();
	// LocalError  segmentLocalPositionError  = iDT->localPositionError();
	// LocalError  segmentLocalDirectionError = iDT->localDirectionError();
	DetId geoid = dtSegment.geographicalId();
	DTChamberId dtdetid = DTChamberId(geoid);
	const DTChamber * dtchamber = dtG->chamber(dtdetid);
	if (dtchamber) {
	    GlobalPoint globalPosition = dtchamber->toGlobal(segmentLocalPosition);
	    GlobalVector globalDirection = dtchamber->toGlobal(segmentLocalDirection);

	    dtPhi[nDt] = globalPosition.phi();
	    dtEta[nDt] = globalPosition.eta();
	    dtX[nDt] = globalPosition.x();
	    dtY[nDt] = globalPosition.y();
	    dtZ[nDt] = globalPosition.z();
	    dtDirX[nDt] = globalDirection.x();
	    dtDirY[nDt] = globalDirection.y();
	    dtDirZ[nDt] = globalDirection.z();
	    dtT[nDt] = 0;//dtSegment.time();
	    dtTError[nDt] = -1;//dtSegment.timeError();
	    nDt++;
	}

    }


    return true;
}

bool llp_ntupler::fillMuons(const edm::Event& iEvent)
{
  for(const pat::Muon &mu : *muons)
  {
    if(mu.pt() < 5) continue;
    muonE[nMuons] = mu.energy();
    muonPt[nMuons] = mu.pt();
    muonEta[nMuons] = mu.eta();
    muonPhi[nMuons] = mu.phi();
    muonCharge[nMuons] = mu.charge();
    muonIsLoose[nMuons] = mu.isLooseMuon();
    muonIsMedium[nMuons] = mu.isMediumMuon();
    muonIsTight[nMuons] = mu.isTightMuon(*myPV);
    muon_d0[nMuons] = -mu.muonBestTrack()->dxy(myPV->position());
    muon_dZ[nMuons] = mu.muonBestTrack()->dz(myPV->position());
    muon_ip3d[nMuons] = mu.dB(pat::Muon::PV3D);
    muon_ip3dSignificance[nMuons] = mu.dB(pat::Muon::PV3D)/mu.edB(pat::Muon::PV3D);
    muonType[nMuons] = mu.isMuon() + mu.isGlobalMuon() + mu.isTrackerMuon() + mu.isStandAloneMuon()
      + mu.isCaloMuon() + mu.isPFMuon() + mu.isRPCMuon();
    muonQuality[nMuons] =
      muon::isGoodMuon(mu,muon::All)
    + muon::isGoodMuon(mu,muon::AllGlobalMuons)
    + muon::isGoodMuon(mu,muon::AllStandAloneMuons)
    + muon::isGoodMuon(mu,muon::AllTrackerMuons)
    + muon::isGoodMuon(mu,muon::TrackerMuonArbitrated)
    + muon::isGoodMuon(mu,muon::AllArbitrated)
    + muon::isGoodMuon(mu,muon::GlobalMuonPromptTight)
    + muon::isGoodMuon(mu,muon::TMLastStationLoose)
    + muon::isGoodMuon(mu,muon::TMLastStationTight)
    + muon::isGoodMuon(mu,muon::TM2DCompatibilityLoose)
    + muon::isGoodMuon(mu,muon::TM2DCompatibilityTight)
    + muon::isGoodMuon(mu,muon::TMOneStationLoose)
    + muon::isGoodMuon(mu,muon::TMOneStationTight)
    + muon::isGoodMuon(mu,muon::TMLastStationOptimizedLowPtLoose)
    + muon::isGoodMuon(mu,muon::TMLastStationOptimizedLowPtTight)
    + muon::isGoodMuon(mu,muon::GMTkChiCompatibility)
    + muon::isGoodMuon(mu,muon::GMStaChiCompatibility)
    + muon::isGoodMuon(mu,muon::GMTkKinkTight)
    + muon::isGoodMuon(mu,muon::TMLastStationAngLoose)
    + muon::isGoodMuon(mu,muon::TMLastStationAngTight)
    + muon::isGoodMuon(mu,muon::TMOneStationAngLoose)
    + muon::isGoodMuon(mu,muon::TMOneStationAngTight)
    + muon::isGoodMuon(mu,muon::TMLastStationOptimizedBarrelLowPtLoose)
    + muon::isGoodMuon(mu,muon::TMLastStationOptimizedBarrelLowPtTight)
    + muon::isGoodMuon(mu,muon::RPCMuLoose);
    muon_pileupIso[nMuons] = mu.pfIsolationR04().sumPUPt;
    muon_chargedIso[nMuons] = mu.pfIsolationR04().sumChargedHadronPt;
    muon_photonIso[nMuons] = mu.pfIsolationR04().sumPhotonEt;
    muon_neutralHadIso[nMuons] = mu.pfIsolationR04().sumNeutralHadronEt;
    muon_validFractionTrackerHits[nMuons] = (mu.innerTrack().isNonnull() ? mu.track()->validFraction() : -99.0);
    muon_isGlobal[nMuons] = muon::isGoodMuon(mu,muon::AllGlobalMuons);
    muon_normChi2[nMuons] = ( muon::isGoodMuon(mu,muon::AllGlobalMuons) ? mu.globalTrack()->normalizedChi2() : -99.0);
    muon_chi2LocalPosition[nMuons] = mu.combinedQuality().chi2LocalPosition;
    muon_kinkFinder[nMuons] = mu.combinedQuality().trkKink;
    muon_segmentCompatability[nMuons] = muon::segmentCompatibility(mu);
    bool isGoodGlobal = mu.isGlobalMuon() && mu.globalTrack()->normalizedChi2() < 3 && mu.combinedQuality().chi2LocalPosition < 12 && mu.combinedQuality().trkKink < 20;
    muonIsICHEPMedium[nMuons] = muon::isLooseMuon(mu) && muon_validFractionTrackerHits[nMuons] > 0.49 && muon::segmentCompatibility(mu) > (isGoodGlobal ? 0.303 : 0.451);
    //-----------------------
    //Trigger Object Matching
    //-----------------------
    bool passTagMuonFilter = false;

    muon_passSingleMuTagFilter[nMuons] = passTagMuonFilter;
    nMuons++;
  }

  return true;
};


bool llp_ntupler::fillElectrons(const edm::Event& iEvent)
{



  for(const pat::Electron &ele : *electrons) {
    if(ele.pt() < 5) continue;
    eleE[nElectrons] = ele.energy();
    elePt[nElectrons] = ele.pt();
    eleEta[nElectrons] = ele.eta();
    elePhi[nElectrons] = ele.phi();
    eleCharge[nElectrons] = ele.charge();

    eleE_SC[nElectrons] = ele.superCluster()->energy();
    eleEta_SC[nElectrons] = ele.superCluster()->eta();
    elePhi_SC[nElectrons] = ele.superCluster()->phi();

    eleSigmaIetaIeta[nElectrons] = ele.sigmaIetaIeta();
    eleFull5x5SigmaIetaIeta[nElectrons] = ele.full5x5_sigmaIetaIeta();
    eleR9[nElectrons] = ele.r9();
    ele_dEta[nElectrons] = ele.deltaEtaSuperClusterTrackAtVtx() - ele.superCluster()->eta() + ele.superCluster()->seed()->eta();

    ele_dPhi[nElectrons] = ele.deltaPhiSuperClusterTrackAtVtx();
    ele_HoverE[nElectrons] = ele.hcalOverEcal();
    ele_d0[nElectrons] = -ele.gsfTrack().get()->dxy(myPV->position());
    ele_dZ[nElectrons] = ele.gsfTrack().get()->dz(myPV->position());

    //ele_ip3d[nElectrons] = ((edm::Ptr<pat::Electron>)(ele))->dB(pat::Electron::PV3D);
    //ele_ip3dSignificance[nElectrons] = ((edm::Ptr<pat::Electron>)(ele))->dB(pat::Electron::PV3D)/((edm::Ptr<pat::Electron>)(ele))->edB(pat::Electron::PV3D);
    ele_pileupIso[nElectrons] = ele.pfIsolationVariables().sumPUPt;
    ele_chargedIso[nElectrons] = ele.pfIsolationVariables().sumChargedHadronPt;
    ele_photonIso[nElectrons] = ele.pfIsolationVariables().sumPhotonEt;
    ele_neutralHadIso[nElectrons] = ele.pfIsolationVariables().sumNeutralHadronEt;
    ele_MissHits[nElectrons] = ele.gsfTrack()->hitPattern().numberOfAllHits(reco::HitPattern::MISSING_INNER_HITS);


    //---------------
    //Conversion Veto
    //---------------

    ele_PassConvVeto[nElectrons] = false;
    if( beamSpot.isValid() && conversions.isValid() )
    {
      ele_PassConvVeto[nElectrons] = !ConversionTools::hasMatchedConversion(ele,*conversions,beamSpot->position());
    } else {
      cout << "\n\nERROR!!! conversions not found!!!\n";
    }
    // 1/E - 1/P
    if( ele.ecalEnergy() == 0 ){
      ele_OneOverEminusOneOverP[nElectrons] = 1e30;
    } else if( !std::isfinite(ele.ecalEnergy())){
      ele_OneOverEminusOneOverP[nElectrons] = 1e30;
    } else {
    ele_OneOverEminusOneOverP[nElectrons] = 1./ele.ecalEnergy()  -  ele.eSuperClusterOverP()/ele.ecalEnergy();
    }

    //-----------------------
    //Trigger Object Matching
    //-----------------------
    bool passSingleEleTagFilter = false;
    bool passTPOneTagFilter= false;
    bool passTPTwoTagFilter= false;
    bool passTPOneProbeFilter= false;
    bool passTPTwoProbeFilter= false;

    //std::cout << "debug ele 9 " << nElectrons << std::endl;
    ele_passSingleEleTagFilter[nElectrons] = passSingleEleTagFilter;
    ele_passTPOneTagFilter[nElectrons] = passTPOneTagFilter;
    ele_passTPTwoTagFilter[nElectrons] = passTPTwoTagFilter;
    ele_passTPOneProbeFilter[nElectrons] = passTPOneProbeFilter;
    ele_passTPTwoProbeFilter[nElectrons] = passTPTwoProbeFilter;


    //std::cout << "debug ele 10 " << nElectrons << std::endl;
    nElectrons++;
  }

  return true;
};

bool llp_ntupler::fillTaus(){
  for (const reco::PFTau &tau : *taus) {
    if (tau.pt() < 20) continue;
    tauE[nTaus] = tau.energy();
    tauPt[nTaus] = tau.pt();
    tauEta[nTaus] = tau.eta();
    tauPhi[nTaus] = tau.phi();
    tau_leadCandPt[nTaus] = 0;
    tau_leadCandID[nTaus] = 0;
    tau_leadChargedHadrCandPt[nTaus] = 0;
    tau_leadChargedHadrCandID[nTaus] = 0;

  if (tau.leadPFCand().isNonnull()) {
      tau_leadCandPt[nTaus] = tau.leadPFCand()->pt();
      tau_leadCandID[nTaus] = tau.leadPFCand()->pdgId();
    }


    if (tau.leadPFChargedHadrCand().isNonnull()) {
      tau_leadChargedHadrCandPt[nTaus] = tau.leadPFChargedHadrCand()->pt();
      tau_leadChargedHadrCandID[nTaus] = tau.leadPFChargedHadrCand()->pdgId();
    }


    nTaus++;
  }

  return true;
};


bool llp_ntupler::fillPhotons(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  noZS::EcalClusterLazyTools *lazyToolnoZS = new noZS::EcalClusterLazyTools(iEvent, iSetup, ebRecHitsToken_, eeRecHitsToken_);
  for (const reco::Photon &pho : *photons) {
    if (pho.pt() < 20) continue;
    std::vector<float> vCov = lazyToolnoZS->localCovariances( *(pho.superCluster()->seed()) );
    //-------------------------------------------------
    //default photon 4-mometum already vertex corrected
    //-------------------------------------------------
    //phoE[nPhotons] = pho.getCorrectedEnergy(reco::Photon::P4type::ecal_standard);
    phoE[nPhotons]   = pho.energy();
    phoPt[nPhotons]  = pho.pt();
    phoEta[nPhotons] = pho.eta(); //correct this for the vertex
    phoPhi[nPhotons] = pho.phi(); //correct this for the vertex
    /*std::cout << "phoE: " << pho.energy() << " phoCorr En:" << pho.getCorrectedEnergy(reco::Photon::P4type::regression2) << " un: "
    << pho.getCorrectedEnergyError(reco::Photon::P4type::regression2) << " "
    << pho.getCorrectedEnergyError( pho.getCandidateP4type() ) << std::endl;*/

    phoSigmaIetaIeta[nPhotons] = pho.sigmaIetaIeta();
    phoFull5x5SigmaIetaIeta[nPhotons] = pho.full5x5_sigmaIetaIeta();

    //phoR9[nPhotons] = pho.r9();
    //Use the noZS version of this according to Emanuele
    phoR9[nPhotons] = pho.full5x5_r9();
    pho_HoverE[nPhotons] = pho.hadTowOverEm();
    pho_isConversion[nPhotons] = pho.hasConversionTracks();
    //pho_passEleVeto[nPhotons] = !hasMatchedPromptElectron(pho.superCluster(),electrons,conversions, beamSpot->position());

    //------------------------------------------
    // Fill default miniAOD isolation quantities
    //------------------------------------------
    pho_pfIsoChargedHadronIso[nPhotons] = pho.chargedHadronIso();
    pho_pfIsoChargedHadronIsoWrongVtx[nPhotons] = pho.chargedHadronWorstVtxIso();
    pho_pfIsoNeutralHadronIso[nPhotons] = pho.neutralHadronIso();
    pho_pfIsoPhotonIso[nPhotons] = pho.photonIso();
    //pho_pfIsoModFrixione[nPhotons] = pho.getPflowIsolationVariables().modFrixione;
    //pho_pfIsoSumPUPt[nPhotons] = pho.sumPUPt();
    //-----------------------
    // super cluster position and time
    //-----------------------
    pho_superClusterEnergy[nPhotons] = pho.superCluster()->energy();
    pho_superClusterRawEnergy[nPhotons] = pho.superCluster()->rawEnergy();
    pho_superClusterEta[nPhotons]    = pho.superCluster()->eta();
    pho_superClusterPhi[nPhotons]    = pho.superCluster()->phi();
    pho_superClusterX[nPhotons]      = pho.superCluster()->x();
    pho_superClusterY[nPhotons]      = pho.superCluster()->y();
    pho_superClusterZ[nPhotons]      = pho.superCluster()->z();
    pho_hasPixelSeed[nPhotons]       = pho.hasPixelSeed();

    pho_superClusterSeedX[nPhotons]      = pho.superCluster()->seed()->x();
    pho_superClusterSeedY[nPhotons]      = pho.superCluster()->seed()->y();
    pho_superClusterSeedZ[nPhotons]      = pho.superCluster()->seed()->z();

    pho_superClusterSeedE[nPhotons]      = pho.superCluster()->seed()->energy();



    //------------------------------------------------------
    //Compute PF isolation
    //absolute uncorrected isolations with footprint removal
    //------------------------------------------------------
    const float coneSizeDR = 0.3;
    const float dxyMax = 0.1;
    const float dzMax = 0.2;
    float chargedIsoSumAllVertices[MAX_NPV];
    for (int q=0;q<MAX_NPV;++q) chargedIsoSumAllVertices[q] = 0.0;
    float chargedIsoSum = 0;
    float chargedIsoSum_NewPV_NoTiming = 0;
    float chargedIsoSum_NewPV_Timing50_TrkVtx = 0;
    float chargedIsoSum_NewPV_Timing80_TrkVtx = 0;
    float chargedIsoSum_NewPV_Timing100_TrkVtx = 0;
    float chargedIsoSum_NewPV_Timing120_TrkVtx = 0;
    float chargedIsoSum_NewPV_Timing50_TrkPho = 0;
    float chargedIsoSum_NewPV_Timing80_TrkPho = 0;
    float chargedIsoSum_NewPV_Timing100_TrkPho = 0;
    float chargedIsoSum_NewPV_Timing120_TrkPho = 0;

    float neutralHadronIsoSum = 0;
    float photonIsoSum = 0;
    // First, find photon direction with respect to the good PV
    math::XYZVector photon_directionWrtVtx(pho.superCluster()->x() - myPV->x(),pho.superCluster()->y() - myPV->y(),pho.superCluster()->z() - myPV->z());
    //math::XYZVector photon_directionWrtVtx_GenMatch(pho.superCluster()->x() - myPV_GenMatch->x(),pho.superCluster()->y() - myPV_GenMatch->y(),pho.superCluster()->z() - myPV_GenMatch->z());

    // old PV, Loop over all PF candidates
    for (const reco::PFCandidate &candidate : *pfCands)
    {
      // Check if this candidate is within the isolation cone
      float dR=deltaR(photon_directionWrtVtx.Eta(),photon_directionWrtVtx.Phi(),
      candidate.eta(), candidate.phi());
      if( dR > coneSizeDR ) continue;

      // Check if this candidate is not in the footprint

      // Find candidate type
      reco::PFCandidate::ParticleType thisCandidateType = reco::PFCandidate::X;

      // the neutral hadrons and charged hadrons can be of pdgId types
      // only 130 (K0L) and +-211 (pi+-) in packed candidates
      const int pdgId = candidate.pdgId();
      if( pdgId == 22 )
      thisCandidateType = reco::PFCandidate::gamma;
      else if( abs(pdgId) == 130) // PDG ID for K0L
      thisCandidateType = reco::PFCandidate::h0;
      else if( abs(pdgId) == 211) // PDG ID for pi+-
      thisCandidateType = reco::PFCandidate::h;


      // Increment the appropriate isolation sum
      if( thisCandidateType == reco::PFCandidate::h ){
        // for charged hadrons, additionally check consistency
        // with the PV
        float dxy = -999, dz = -999;

        //For the primary vertex
        dz = candidate.trackRef()->dz(myPV->position());
        dxy =candidate.trackRef()->dxy(myPV->position());
        if (fabs(dz) <= dzMax && fabs(dxy) <= dxyMax) {
          chargedIsoSum += candidate.pt();
        }

        //loop over all vertices
        for(int q = 0; q < nPVAll; q++){
          if(!(vertices->at(q).isValid() && !vertices->at(q).isFake())) continue;

          dz = candidate.trackRef()->dz(vertices->at(q).position());
          dxy =candidate.trackRef()->dxy(vertices->at(q).position());
          if (fabs(dz) > dzMax) continue;
          if(fabs(dxy) > dxyMax) continue;
          // The candidate is eligible, increment the isolation
          chargedIsoSumAllVertices[q] += candidate.pt();
        }
      }
      if( thisCandidateType == reco::PFCandidate::h0 )
      neutralHadronIsoSum += candidate.pt();
      if( thisCandidateType == reco::PFCandidate::gamma )
      photonIsoSum += candidate.pt();
    }


    //fill the proper variables
    for(int q = 0; q < nPVAll; q++) {
      pho_sumChargedHadronPtAllVertices[nPhotons][q] = chargedIsoSumAllVertices[q];
    }
    pho_sumChargedHadronPt[nPhotons] = chargedIsoSum;

    pho_sumChargedHadronPt_NewPV_NoTiming[nPhotons] = chargedIsoSum_NewPV_NoTiming;

    pho_sumChargedHadronPt_NewPV_Timing50_TrkVtx[nPhotons] = chargedIsoSum_NewPV_Timing50_TrkVtx;
    pho_sumChargedHadronPt_NewPV_Timing80_TrkVtx[nPhotons] = chargedIsoSum_NewPV_Timing80_TrkVtx;
    pho_sumChargedHadronPt_NewPV_Timing100_TrkVtx[nPhotons] = chargedIsoSum_NewPV_Timing100_TrkVtx;
    pho_sumChargedHadronPt_NewPV_Timing120_TrkVtx[nPhotons] = chargedIsoSum_NewPV_Timing120_TrkVtx;

    pho_sumChargedHadronPt_NewPV_Timing50_TrkPho[nPhotons] = chargedIsoSum_NewPV_Timing50_TrkPho;
    pho_sumChargedHadronPt_NewPV_Timing80_TrkPho[nPhotons] = chargedIsoSum_NewPV_Timing80_TrkPho;
    pho_sumChargedHadronPt_NewPV_Timing100_TrkPho[nPhotons] = chargedIsoSum_NewPV_Timing100_TrkPho;
    pho_sumChargedHadronPt_NewPV_Timing120_TrkPho[nPhotons] = chargedIsoSum_NewPV_Timing120_TrkPho;

    pho_sumNeutralHadronEt[nPhotons] = neutralHadronIsoSum;
    pho_sumPhotonEt[nPhotons] = photonIsoSum;


    //-------------------------------------------------
    //Compute Worst Isolation Looping over all vertices
    //-------------------------------------------------
    const double ptMin = 0.0;
    const float dRvetoBarrel = 0.0;
    const float dRvetoEndcap = 0.0;
    float dRveto = 0;
    if (pho.isEB()) dRveto = dRvetoBarrel;
    else dRveto = dRvetoEndcap;

    float worstIsolation = 999;
    std::vector<float> allIsolations;
    for(unsigned int ivtx=0; ivtx<vertices->size(); ++ivtx) {

      // Shift the photon according to the vertex
      reco::VertexRef vtx(vertices, ivtx);
      math::XYZVector photon_directionWrtVtx(pho.superCluster()->x() - vtx->x(),
      pho.superCluster()->y() - vtx->y(),
      pho.superCluster()->z() - vtx->z());

      float sum = 0;
      // Loop over all PF candidates
      for (const reco::PFCandidate &candidate : *pfCands) {

        //require that PFCandidate is a charged hadron
        const int pdgId = candidate.pdgId();
        if( abs(pdgId) != 211) continue;

        if (candidate.pt() < ptMin)
        continue;

        float dxy = -999, dz = -999;
        dz = candidate.trackRef()->dz(myPV->position());
        dxy =candidate.trackRef()->dxy(myPV->position());
        if( fabs(dxy) > dxyMax) continue;
        if ( fabs(dz) > dzMax) continue;

        float dR = deltaR(photon_directionWrtVtx.Eta(), photon_directionWrtVtx.Phi(),
        candidate.eta(),      candidate.phi());
        if(dR > coneSizeDR || dR < dRveto) continue;

        sum += candidate.pt();
      }

      allIsolations.push_back(sum);
    }

    if( allIsolations.size()>0 )
    worstIsolation = * std::max_element( allIsolations.begin(), allIsolations.end() );

    pho_sumWorstVertexChargedHadronPt[nPhotons] = worstIsolation;

    //-----------------------
    //Photon ID MVA variable
    //-----------------------
    //pho_IDMVA[nPhotons] = myPhotonMVA->mvaValue( pho,  *rhoAll, photonIsoSum, chargedIsoSum, worstIsolation,lazyToolnoZS, false);

    //pho_RegressionE[nPhotons] = pho.getCorrectedEnergy(reco::Photon::P4type::regression1);
    //pho_RegressionEUncertainty[nPhotons] = pho.getCorrectedEnergyError(reco::Photon::P4type::regression1);

    //---------------------
    //Use Latest Regression
    //---------------------
    pho_RegressionE[nPhotons]            = pho.getCorrectedEnergy( pho.getCandidateP4type() );
    pho_RegressionEUncertainty[nPhotons] = pho.getCorrectedEnergyError( pho.getCandidateP4type() );


    //conversion matching for beamspot pointing
    const reco::Conversion *convmatch = 0;
    double drmin = std::numeric_limits<double>::max();
    //double leg conversions
    for (const reco::Conversion &conv : *conversions) {
      if (conv.refittedPairMomentum().rho()<10.) continue;
      if (!conv.conversionVertex().isValid()) continue;
      if (TMath::Prob(conv.conversionVertex().chi2(),  conv.conversionVertex().ndof())<1e-6) continue;

      math::XYZVector mom(conv.refittedPairMomentum());
      math::XYZPoint scpos(pho.superCluster()->position());
      math::XYZPoint cvtx(conv.conversionVertex().position());
      math::XYZVector cscvector = scpos - cvtx;

      double dr = reco::deltaR(mom,cscvector);

      if (dr<drmin && dr<0.1) {
        drmin = dr;
        convmatch = &conv;
      }
    }
    if (!convmatch) {
      drmin = std::numeric_limits<double>::max();
      //single leg conversions
      for (const reco::Conversion &conv : *singleLegConversions) {
        math::XYZVector mom(conv.tracksPin()[0]);
        math::XYZPoint scpos(pho.superCluster()->position());
        math::XYZPoint cvtx(conv.conversionVertex().position());
        math::XYZVector cscvector = scpos - cvtx;

        double dr = reco::deltaR(mom,cscvector);

        if (dr<drmin && dr<0.1) {
          drmin = dr;
          convmatch = &conv;
        }
      }
    }

    //matched conversion, compute conversion type
    //and extrapolation to beamline
    //FIXME Both of these additional two requirements are inconsistent and make the conversion
    //selection depend on poorly defined criteria, but we keep them for sync purposes
    //if (convmatch && pho.hasConversionTracks() && conversions->size()>0) {
    if (convmatch){// && pho.hasConversionTracks() && conversions->size()>0) {
      int ntracks = convmatch->nTracks();

      math::XYZVector mom(ntracks==2 ? convmatch->refittedPairMomentum() : convmatch->tracksPin()[0]);
      math::XYZPoint scpos(pho.superCluster()->position());
      math::XYZPoint cvtx(convmatch->conversionVertex().position());
      math::XYZVector cscvector = scpos - cvtx;

      double z = cvtx.z();
      double rho = cvtx.rho();

      int legtype = ntracks==2 ? 0 : 1;
      int dettype = pho.isEB() ? 0 : 1;
      int postype =0;

      if (pho.isEB()) {
        if (rho<15.) {
          postype = 0;
        }
        else if (rho>=15. && rho<60.) {
          postype = 1;
        }
        else {
          postype = 2;
        }
      }
      else {
        if (std::abs(z) < 50.) {
          postype = 0;
        }
        else if (std::abs(z) >= 50. && std::abs(z) < 100.) {
          postype = 1;
        }
        else {
          postype = 2;
        }
      }

      pho_convType[nPhotons] = legtype + 2*dettype + 4*postype;
      pho_convTrkZ[nPhotons] = cvtx.z() - ((cvtx.x()-beamSpot->x0())*mom.x()+(cvtx.y()-beamSpot->y0())*mom.y())/mom.rho() * mom.z()/mom.rho();
      pho_convTrkClusZ[nPhotons] = cvtx.z() - ((cvtx.x()-beamSpot->x0())*cscvector.x()+(cvtx.y()-beamSpot->y0())*cscvector.y())/cscvector.rho() * cscvector.z()/cscvector.rho();
    }

    nPhotons++;
  }


  delete lazyToolnoZS;
  return true;

};


bool llp_ntupler::fillJets(const edm::EventSetup& iSetup)
{

  for (const reco::PFJet &j : *jets)
  {
    if (j.pt() < 20) continue;
    if (fabs(j.eta()) > 5.0) continue;
    //-------------------
    //Fill Jet-Level Info
    //-------------------
    jetE[nJets] = j.energy();
    jetPt[nJets] = j.pt();
    jetEta[nJets] = j.eta();
    jetPhi[nJets] = j.phi();
    jetMass[nJets] = j.mass();
    jetEt[nJets] = j.et();

    TLorentzVector thisJet;
    thisJet.SetPtEtaPhiE(jetPt[nJets], jetEta[nJets], jetPhi[nJets], jetE[nJets]);
    //jetCISV = j.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");

    jetJetArea[nJets] = j.jetArea();
    jetPileupE[nJets] = j.pileup();

    jetPileupIdFlag[nJets] = 0;
    jetPassIDLoose[nJets] = passJetID(&j, 0);
    jetPassIDTight[nJets] = passJetID(&j, 1);
    jetPassMuFrac[nJets]  = ( j.muonEnergyFraction() < 0.80 );
    jetPassEleFrac[nJets]  = ( j.electronEnergyFraction() < 0.90 );


    // if (useGen_) {
    //   jetPartonFlavor = j.partonFlavour();
    //   jetHadronFlavor = j.hadronFlavour();
    // }

    jetChargedEMEnergyFraction[nJets] = j.chargedEmEnergyFraction();
    jetNeutralEMEnergyFraction[nJets] = j.neutralEmEnergyFraction();
    jetChargedHadronEnergyFraction[nJets] = j.chargedHadronEnergyFraction();
    jetNeutralHadronEnergyFraction[nJets] = j.neutralHadronEnergyFraction();
    // jet_charged_hadron_multiplicity[nJets] = j.chargedHadronMultiplicity();
    // jet_neutral_hadron_multiplicity[nJets] = j.neutralHadronMultiplicity();
    //jet_photon_multiplicity[nJets] = j.photonMultiplicity();
    //jet_electron_multiplicity[nJets] = j.electronMultiplicity();
    //jet_muon_multiplicity[nJets] = j.muonMultiplicity();
    //jet_HF_hadron_multiplicity[nJets] = j.HFHadronMultiplicity();
    //jet_HF_em_multiplicity[nJets] = j.HFEMMultiplicity();
    // jet_charged_multiplicity[nJets] = j.chargedMultiplicity();
    // jet_neutral_multiplicity[nJets] = j.neutralMultiplicity();

    //---------------------------
    //Trackless variables
    //---------------------------
    float alphaMax(0.0),medianTheta2D(0.0),medianIP(0.0),minDeltaRAllTracks(0.0),minDeltaRPVTracks(0.0),ptAllTracks(0.0), ptAllPVTracks(0.0);
    int nTracksPV(0);
    findTrackingVariables(thisJet,iSetup,alphaMax,medianTheta2D,medianIP,nTracksPV,ptAllPVTracks,ptAllTracks, minDeltaRAllTracks, minDeltaRPVTracks);
    //jetCISV = j.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    jetAlphaMax[nJets] = alphaMax;
    jetBetaMax[nJets] = alphaMax * ptAllTracks/(j.pt());
    jetGammaMax[nJets] = alphaMax * ptAllTracks/(j.energy());
    jetGammaMax_EM[nJets] = alphaMax * ptAllTracks/(j.energy()*(j.chargedEmEnergyFraction()+j.neutralEmEnergyFraction()));
    jetGammaMax_Hadronic[nJets] = alphaMax * ptAllTracks/(j.energy()*(j.chargedHadronEnergyFraction()+j.neutralHadronEnergyFraction()));
    jetGammaMax_ET[nJets] = alphaMax * ptAllTracks/j.et();
    jetMedianTheta2D[nJets] = medianTheta2D;
    jetMedianIP[nJets] = medianIP;
    jetPtAllPVTracks[nJets] = ptAllPVTracks;
    jetPtAllTracks[nJets] = ptAllTracks;
    jetMinDeltaRAllTracks[nJets] = minDeltaRAllTracks;
    jetMinDeltaRPVTracks[nJets] = minDeltaRPVTracks;

    //---------------------------
    //find photons inside the jet
    //---------------------------
    /*
    for (const reco::Photon &pho : *photons) {
      //cout << "Nphoton: " << fJetNPhotons << "\n";

      if (!(deltaR(pho.eta(), pho.phi() , j.eta(), j.phi()) < 0.5)) continue;


      fJetPhotonPt[fJetNPhotons]  = pho.pt();
      fJetPhotonEta[fJetNPhotons] = pho.eta(); //correct this for the vertex
      fJetPhotonPhi[fJetNPhotons] = pho.phi(); //correct this for the vertex

      fJetPhotonSeedRecHitE[fJetNPhotons]      = pho.superCluster()->seed()->x();
      fJetPhotonSeedRecHitEta[fJetNPhotons]      = pho.superCluster()->seed()->y();
      fJetPhotonSeedRecHitPhi[fJetNPhotons]      = pho.superCluster()->seed()->z();
      fJetPhotonSeedRecHitTime[fJetNPhotons]      = pho.superCluster()->seed()->energy();

      // //get time coordinate for the seed
      // for (const reco::PFCluster &pfcluster : *pfClusters) {
      // 	if(pfcluster.seed() == pho.superCluster()->seed()->seed())
      // 	  {
      // 	    pho_superClusterSeedT[fJetNPhotons] = pfcluster.time();
      // 	    pho_pfClusterSeedE[fJetNPhotons]      = pfcluster.energy();
      // 	  }
      // }

      //-------------------------------
      //fill all rechits inside photons
      //-------------------------------

      fJetNPhotons++;

    }
    */

    //---------------------------
    //Find RecHits Inside the Jet
    //---------------------------
    // geometry (from ECAL ELF)

    edm::ESHandle<CaloGeometry> geoHandle;
    iSetup.get<CaloGeometryRecord>().get(geoHandle);
    const CaloSubdetectorGeometry *barrelGeometry = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
    //const CaloSubdetectorGeometry *endcapGeometry = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
    //double ecal_radius = 129.0;
    int n_matched_rechits = 0;
    std::vector<double> rechitphi;
    std::vector<double> rechiteta;
    std::vector<double> rechitet;
    std::vector<double> rechitt;
    for (EcalRecHitCollection::const_iterator recHit = ebRecHits->begin(); recHit != ebRecHits->end(); ++recHit)
    {
      const DetId recHitId = recHit->detid();
      const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();

      if (enableEcalRechits_ && recHit->energy() > 0.5 && recHit->timeError() > 0.0 && recHit->timeError() < 100.0)
      {
        ecalRechit_Eta[nRechits] = recHitPos.eta();
        ecalRechit_Phi[nRechits] = recHitPos.phi();
        ecalRechit_E[nRechits] = recHit->energy();
        ecalRechit_T[nRechits] = recHit->time();
        ecalRechit_E_Error[nRechits] = recHit->energyError();
        ecalRechit_T_Error[nRechits] = recHit->timeError();
        ecalRechit_kSaturatedflag[nRechits] = recHit->checkFlag(EcalRecHit::kSaturated);
        ecalRechit_kLeadingEdgeRecoveredflag[nRechits] = recHit->checkFlag(EcalRecHit::kLeadingEdgeRecovered);
        ecalRechit_kPoorRecoflag[nRechits] = recHit->checkFlag(EcalRecHit::kPoorReco);
        ecalRechit_kWeirdflag[nRechits]= recHit->checkFlag(EcalRecHit::kWeird);
        ecalRechit_kDiWeirdflag[nRechits] = recHit->checkFlag(EcalRecHit::kDiWeird);
        nRechits ++;

      }

      if (recHit->checkFlag(EcalRecHit::kSaturated) || recHit->checkFlag(EcalRecHit::kLeadingEdgeRecovered) || recHit->checkFlag(EcalRecHit::kPoorReco) || recHit->checkFlag(EcalRecHit::kWeird) || recHit->checkFlag(EcalRecHit::kDiWeird)) continue;
      if (recHit->timeError() < 0 || recHit->timeError() > 100) continue;
      if (abs(recHit->time()) > 12.5) continue;

      if ( deltaR(jetEta[nJets], jetPhi[nJets], recHitPos.eta(), recHitPos.phi())  < 0.4)
      {
        //double rechit_x = ecal_radius * cos(recHitPos.phi());
        //double rechit_y = ecal_radius * sin(recHitPos.phi());
        //double rechit_z = ecal_radius * sinh(recHitPos.eta());
        //double photon_pv_travel_time = (1./30) * sqrt(pow(pvX-rechit_x,2)+pow(pvY-rechit_y,2)+pow(pvZ-rechit_z,2));
        if ( deltaR(jetEta[nJets], jetPhi[nJets], recHitPos.eta(), recHitPos.phi())  < 0.15)
        {
          if (recHit->energy() > Rechit_cut) jet_energy_frac[nJets] += recHit->energy();
        }

        if (recHit->energy() > Rechit_cut)
        {
    	    // jetRechitT_Error[nJets] += 0.0;
          jetRechitE_Error[nJets] += recHit->energyError() * recHit->energyError();
          jetRechitE[nJets] += recHit->energy();
          jetRechitT[nJets] += recHit->time()*recHit->energy();
          jetRechitT_rms[nJets] += recHit->time()*recHit->time();
          rechitphi.push_back(recHitPos.phi());
          rechiteta.push_back(recHitPos.eta());
          rechitet.push_back(recHit->energy()/cosh(recHitPos.eta()));
          rechitt.push_back(recHit->time());
          n_matched_rechits++;
        }

      }
    }

    jetRechitT[nJets] = jetRechitT[nJets]/jetRechitE[nJets];
    jetNRechits[nJets] = n_matched_rechits;
    jetRechitE_Error[nJets] = sqrt(jetRechitE_Error[nJets]);
    jetRechitT_rms[nJets] = sqrt(jetRechitT_rms[nJets]);
    double sig1(0.0),sig2(0.0);
    jet_second_moments(rechitet,rechiteta,rechitphi,sig1,sig2);
    jet_sig_et1[nJets] = sig1;
    jet_sig_et2[nJets] = sig2;
    jet_energy_frac[nJets] = jet_energy_frac[nJets]/jetRechitE[nJets];

    nJets++;
  } //loop over jets

  return true;
};

bool llp_ntupler::fillCaloJets(const edm::EventSetup& iSetup)
{
  for (const reco::CaloJet &j : *jetsCalo)
  {
    if (j.pt() < 20) continue;
    if (fabs(j.eta()) > 2.4) continue;
    //-------------------
    //Fill Jet-Level Info
    //-------------------
    calojetE[nCaloJets] = j.energy();
    calojetEt[nCaloJets] = j.et();
    calojetPt[nCaloJets] = j.pt();
    calojetEta[nCaloJets] = j.eta();
    calojetPhi[nCaloJets] = j.phi();
    calojetMass[nCaloJets] = j.mass();
    calojet_HadronicEnergyFraction[nCaloJets] = j.energyFractionHadronic();
    calojet_EMEnergyFraction[nCaloJets] = j.emEnergyFraction();

    TLorentzVector thisJet;
    thisJet.SetPtEtaPhiE(calojetPt[nCaloJets], calojetEta[nCaloJets], calojetPhi[nCaloJets], calojetE[nCaloJets]);
    //calojetCISV = j.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    float alphaMax(0.0),medianTheta2D(0.0),medianIP(0.0),minDeltaRAllTracks(0.0),minDeltaRPVTracks(0.0),ptAllTracks(0.0), ptAllPVTracks(0.0);
    int nTracksPV(0);
    findTrackingVariables(thisJet,iSetup,alphaMax,medianTheta2D,medianIP,nTracksPV,ptAllPVTracks,ptAllTracks, minDeltaRAllTracks, minDeltaRPVTracks);
    //jetCISV = j.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    calojetAlphaMax[nCaloJets] = alphaMax;
    calojetBetaMax[nCaloJets] = alphaMax * ptAllTracks/j.pt();
    calojetGammaMax[nCaloJets] = alphaMax * ptAllTracks/(j.energy());
    calojetGammaMax_EM[nCaloJets] = alphaMax * ptAllTracks/(j.energy()*j.emEnergyFraction());
    calojetGammaMax_Hadronic[nCaloJets] =  alphaMax * ptAllTracks/(j.energy()*j.energyFractionHadronic());
    calojetGammaMax_ET[nCaloJets] = alphaMax * ptAllTracks/j.et();

    calojetMedianTheta2D[nCaloJets] = medianTheta2D;
    calojetMedianIP[nCaloJets] = medianIP;
    calojetPtAllPVTracks[nCaloJets] = ptAllPVTracks;
    calojetPtAllTracks[nCaloJets] = ptAllTracks;
    calojetMinDeltaRAllTracks[nCaloJets] = minDeltaRAllTracks;
    calojetMinDeltaRPVTracks[nCaloJets] = minDeltaRPVTracks;

    calojetJetArea[nCaloJets] = j.jetArea();
    calojetPileupE[nCaloJets] = j.pileup();

    calojetPileupIdFlag[nCaloJets] = 0;
    calojetPassIDLoose[nCaloJets] = passCaloJetID(&j, 0);
    calojetPassIDTight[nCaloJets] = passCaloJetID(&j, 1);


    //---------------------------
    //Find RecHits Inside the Jet
    //---------------------------
    // geometry (from ECAL ELF)

    edm::ESHandle<CaloGeometry> geoHandle;
    iSetup.get<CaloGeometryRecord>().get(geoHandle);
    const CaloSubdetectorGeometry *barrelGeometry = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
    //const CaloSubdetectorGeometry *endcapGeometry = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
    //double ecal_radius = 129.0;
    int n_matched_rechits = 0;
    for (EcalRecHitCollection::const_iterator recHit = ebRecHits->begin(); recHit != ebRecHits->end(); ++recHit)
    {
      if (recHit->checkFlag(EcalRecHit::kSaturated) || recHit->checkFlag(EcalRecHit::kLeadingEdgeRecovered) || recHit->checkFlag(EcalRecHit::kPoorReco) || recHit->checkFlag(EcalRecHit::kWeird) || recHit->checkFlag(EcalRecHit::kDiWeird)) continue;
      if (recHit->timeError() < 0 || recHit->timeError() > 100) continue;
      if ( recHit->checkFlag(0) )
      {
        const DetId recHitId = recHit->detid();
        const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
        if ( deltaR(calojetEta[nCaloJets], calojetPhi[nCaloJets], recHitPos.eta(), recHitPos.phi())  < 0.4)
        {
          //double rechit_x = ecal_radius * cos(recHitPos.phi());
          //double rechit_y = ecal_radius * sin(recHitPos.phi());
          //double rechit_z = ecal_radius * sinh(recHitPos.eta());
          //double photon_pv_travel_time = (1./30) * sqrt(pow(pvX-rechit_x,2)+pow(pvY-rechit_y,2)+pow(pvZ-rechit_z,2));

          if (recHit->energy() > Rechit_cut)
          {
            calojetRechitE[nCaloJets] += recHit->energy();
            calojetRechitT[nCaloJets] += recHit->time()*recHit->energy();
            calojetRechitT_rms[nCaloJets] += recHit->time()*recHit->time();

            n_matched_rechits++;
          }

        }
      }
    }
    //cout << "Last Nphoton: " << fJetNPhotons << "\n";
    //std::cout << "n: " << n_matched_rechits << std::endl;
    if (calojetRechitE[nCaloJets] > 0.0 ){
      calojetNRechits[nCaloJets] = n_matched_rechits;
      calojetRechitT[nCaloJets] = calojetRechitT[nCaloJets]/calojetRechitE[nCaloJets];
      calojetRechitT_rms[nCaloJets] = sqrt(calojetRechitT_rms[nCaloJets]);
      nCaloJets++;
    }
  } //loop over calojets

  return true;
};


bool llp_ntupler::fillMet(const edm::Event& iEvent)
{
  const reco::PFMET &Met = mets->front();

  sumMET = Met.sumEt();
  metType0Pt = 0;
  metType0Phi = 0;
  metType1Pt_raw = Met.pt();
  metType1Pt = Met.pt();
  metType1Px = Met.px();
  metType1Py = Met.py();
  metType1Eta = Met.eta();
  metType1Phi_raw = Met.phi();
  metType1Phi = Met.phi();
  metType0Plus1Pt = 0;
  metType0Plus1Phi = 0;

  const reco::PFMET &MetPuppi = metsPuppi->front();
  metPuppiPt = MetPuppi.pt();
  metPuppiPhi = MetPuppi.phi();

  //MET filters
  if (!isFastsim_) {
    const edm::TriggerNames &metNames = iEvent.triggerNames(*metFilterBits);

    //----------------------------
    //For Debug printout
    //----------------------------
    // for (unsigned int i = 0, n = metFilterBits->size(); i < n; ++i) {
    // 	std::cout << "MET Filter " << metNames.triggerName(i).c_str() << "\n";
    // }

    for(unsigned int i = 0, n = metFilterBits->size(); i < n; ++i)
    {
      if(strcmp(metNames.triggerName(i).c_str(), "Flag_trackingFailureFilter") == 0)
      Flag_trackingFailureFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_goodVertices") == 0)
      Flag_goodVertices = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_globalTightHalo2016Filter") == 0)
      Flag_CSCTightHaloFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_trkPOGFilters") == 0)
      Flag_trkPOGFilters = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_trkPOG_logErrorTooManyClusters") == 0)
      Flag_trkPOG_logErrorTooManyClusters = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_EcalDeadCellTriggerPrimitiveFilter") == 0)
      Flag_EcalDeadCellTriggerPrimitiveFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_EcalDeadCellBoundaryEnergyFilter") == 0)
      Flag_EcalDeadCellBoundaryEnergyFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_ecalLaserCorrFilter") == 0)
      Flag_ecalLaserCorrFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_trkPOG_manystripclus53X") == 0)
      Flag_trkPOG_manystripclus53X = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_eeBadScFilter") == 0)
      Flag_eeBadScFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_METFilters") == 0)
      Flag_METFilters = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_HBHENoiseFilter") == 0)
      Flag_HBHENoiseFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_HBHENoiseIsoFilter") == 0)
      Flag_HBHEIsoNoiseFilter = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_trkPOG_toomanystripclus53X") == 0)
      Flag_trkPOG_toomanystripclus53X = metFilterBits->accept(i);
      else if(strcmp(metNames.triggerName(i).c_str(), "Flag_hcalLaserEventFilter") == 0)
      Flag_hcalLaserEventFilter = metFilterBits->accept(i);
    } //loop over met filters

  }

  return true;
};

bool llp_ntupler::passCaloJetID( const reco::CaloJet *jetCalo, int cutLevel) {
  bool result = false;

  return result;
}//passJetID CaloJet


bool llp_ntupler::passJetID( const reco::PFJet *jet, int cutLevel) {
  bool result = false;

  double NHF = jet->neutralHadronEnergyFraction();
  double NEMF = jet->neutralEmEnergyFraction();
  int NumConst = jet->chargedMultiplicity() + jet->neutralMultiplicity() ;
  double CHF = jet->chargedHadronEnergyFraction();
  double MUF = jet->muonEnergyFraction();
  double CEMF = jet->chargedEmEnergyFraction();
  int NumNeutralParticles =jet->neutralMultiplicity();
  int CHM = jet->chargedMultiplicity();

  //Loose
  if (cutLevel == 0) {
    if ( fabs(jet->eta()) <= 2.4) {
      if ( NHF  < 0.99 && NEMF < 0.99 && NumConst > 1
	   && CHF > 0 && CHM > 0 && CEMF < 0.99 ) result = true;
    } else if( fabs(jet->eta()) <= 3.0)  {
      if ( NHF  < 0.99 && NEMF < 0.99 && NumConst > 1 ) result = true;
    } else {
      if ( NEMF < 0.90 && NumNeutralParticles > 10 ) result = true;
    }
  }

  //Tight
  else if (cutLevel == 1) {
    if ( fabs(jet->eta()) <= 2.4) {
      if ( NHF  < 0.90 && NEMF < 0.90 && NumConst > 1
	   && CHF > 0 && CHM > 0 && CEMF < 0.99 ) result = true;
    } else if( fabs(jet->eta()) <= 3.0)  {
      if ( NHF  < 0.90 && NEMF < 0.90 && NumConst > 1 ) result = true;
    } else {
      if ( NEMF < 0.90 && NumNeutralParticles > 10 ) result = true;
    }
  }

  //Tight Lep Veto
  else if (cutLevel == 2) {
    if ( fabs(jet->eta()) <= 2.4) {
      if ( NHF  < 0.90 && NEMF < 0.90 && NumConst > 1
	   && CHF > 0 && CHM > 0 && CEMF < 0.99 && MUF < 0.8 ) result = true;
    } else if( fabs(jet->eta()) <= 3.0)  {
      if ( NHF  < 0.90 && NEMF < 0.90 && NumConst > 1 ) result = true;
    } else {
      if ( NEMF < 0.90 && NumNeutralParticles > 10 ) result = true;
    }
  }

  return result;
}//passJetID PFJet

double llp_ntupler::deltaPhi(double phi1, double phi2)
{
  double dphi = phi1-phi2;
  while (dphi > TMath::Pi())
  {
    dphi -= TMath::TwoPi();
  }
  while (dphi <= -TMath::Pi())
  {
    dphi += TMath::TwoPi();
  }
  return dphi;
};

double llp_ntupler::deltaR(double eta1, double phi1, double eta2, double phi2)
{
  double dphi = deltaPhi(phi1,phi2);
  double deta = eta1 - eta2;
  return sqrt( dphi*dphi + deta*deta);
};
void llp_ntupler::findTrackingVariables(const TLorentzVector &jetVec,const edm::EventSetup& iSetup,float &alphaMax,float &medianTheta2D,float &medianIP, int &nTracksPV,float &ptAllPVTracks,float &ptAllTracks,float &minDeltaRAllTracks, float &minDeltaRPVTracks)
{
  int nTracksAll = 0;
  //Displaced jet stuff
  double ptPVTracksMax = 0.;
  minDeltaRAllTracks = 15;
  minDeltaRPVTracks = 15;
  reco::Vertex primaryVertex = vertices->at(0);
  std::vector<double> theta2Ds;
  std::vector<double> IP2Ds;
  for (unsigned int iTrack = 0; iTrack < generalTracks->size(); iTrack ++){
  	reco::Track generalTrack = generalTracks->at(iTrack);
  	TLorentzVector generalTrackVecTemp;
  	generalTrackVecTemp.SetPtEtaPhiM(generalTrack.pt(),generalTrack.eta(),generalTrack.phi(),0);

  	if (generalTrack.pt() > 1) {
	    if (minDeltaRAllTracks > generalTrackVecTemp.DeltaR(jetVec))
	    {
		    minDeltaRAllTracks =  generalTrackVecTemp.DeltaR(jetVec);
	    }
	    if (generalTrackVecTemp.DeltaR(jetVec) < 0.4){
    		nTracksAll ++;
    		//tot pt for alpha
    		ptAllTracks += generalTrack.pt();

    		// theta 2d
    		// ROOT::Math::XYZPoint innerPos = generalTrack.innerPosition();
    		// ROOT::Math::XYZPoint vertexPos = primaryVertex.position();
    		// ROOT::Math::XYZVector deltaPos = innerPos - vertexPos;
    		// ROOT::Math::XYZVector momentum = generalTrack.innerMomentum();
    		// double mag2DeltaPos = TMath::Sqrt((deltaPos.x()*deltaPos.x()) + (deltaPos.y()*deltaPos.y()));
    		// double mag2Mom = TMath::Sqrt((momentum.x()*momentum.x()) + (momentum.y()*momentum.y()));
    		// double theta2D = TMath::ACos((deltaPos.x()*momentum.x()+deltaPos.y()*momentum.y())/(mag2Mom*mag2DeltaPos));
    		// theta2Ds.push_back(theta2D);

    		//IP sig
    		edm::ESHandle<TransientTrackBuilder> theB;
    		iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
    		reco::TransientTrack transTrack = theB->build(generalTrack);
    		TrajectoryStateClosestToBeamLine traj = transTrack.stateAtBeamLine();
    		Measurement1D meas = traj.transverseImpactParameter();
    		std::pair<bool, Measurement1D> ip2d = IPTools::absoluteTransverseImpactParameter(transTrack,primaryVertex);
    		IP2Ds.push_back(ip2d.second.value()/ip2d.second.error());
      }
    }
  }
  if (ptAllTracks > 0.9){
	//No matched jets
  	for (auto vertex = vertices->begin(); vertex != vertices->end(); vertex++){
      double ptPVTracks = 0.;
      int nTracksPVTemp = 0;
      if(!vertex->isValid())continue;
      if (vertex->isFake())continue;
      for(auto pvTrack=vertex->tracks_begin(); pvTrack!=vertex->tracks_end(); pvTrack++){
    		TLorentzVector pvTrackVecTemp;
    		pvTrackVecTemp.SetPtEtaPhiM((*pvTrack)->pt(),(*pvTrack)->eta(),(*pvTrack)->phi(),0);
  		//If pv track associated with jet add pt to ptPVTracks
    		if ((*pvTrack)->pt() > 1) {
    	    if (minDeltaRPVTracks > pvTrackVecTemp.DeltaR(jetVec))
    	    {
    		     minDeltaRPVTracks =  pvTrackVecTemp.DeltaR(jetVec);
    	    }
    	    if (pvTrackVecTemp.DeltaR(jetVec) < 0.4){
        		ptPVTracks += (*pvTrack)->pt();
        		ptAllPVTracks += (*pvTrack)->pt();
        		nTracksPVTemp++;
    	    }
    		}
      }
      if (ptPVTracks > ptPVTracksMax) {
      	ptPVTracksMax = ptPVTracks;
      	nTracksPV = nTracksPVTemp;
      }
      alphaMax = ptPVTracksMax/ptAllTracks;
  	}
  }
  std::sort(IP2Ds.begin(),IP2Ds.end());
  if (IP2Ds.size() > 0){
	   medianIP = IP2Ds[IP2Ds.size()/2];
  }
  std::sort(theta2Ds.begin(),theta2Ds.end());
  if (theta2Ds.size() > 0){
    medianTheta2D = theta2Ds[theta2Ds.size()/2];
  }
};
void llp_ntupler::jet_second_moments(std::vector<double> &et,std::vector<double> &eta,std::vector<double> &phi,double &sig1,double &sig2)
{
  double mean_eta = 0.0;
  double mean_phi = 0.0;
  double et_squared = 0.0;
  for(unsigned int i = 0;i < eta.size();i++)
  {
    mean_eta += et[i]*et[i]*eta[i];
    mean_phi += et[i]*et[i]*phi[i];
    et_squared += et[i]*et[i];
  }
  mean_eta = mean_eta/et_squared;
  mean_phi = mean_phi/et_squared;

  double m11(0.0),m22(0.0),m12(0.0);
  for(unsigned int i = 0;i < eta.size();i++)
  {
    m11 += et[i]*et[i]*(eta[i]-mean_eta)*(eta[i]-mean_eta);
    m22 += et[i]*et[i]*(phi[i]-mean_phi)*(phi[i]-mean_phi);
    m12 += et[i]*et[i]*(phi[i]-mean_phi)*(eta[i]-mean_eta);
  }
  double a = (m11+m22)/2;
  double b = 0.5*sqrt(pow(m11+m22,2)-4*(m11*m22-m12*m12));
  sig1 = sqrt(abs(a+b)/et_squared);
  sig2 = sqrt(abs(a-b)/et_squared);

};
bool llp_ntupler::fillMC()
{
  for(const reco::GenJet &j : *genJets)
  {
    //std::cout << nGenJets << std::endl;
    genJetE[nGenJets] = j.energy();
    genJetPt[nGenJets] = j.pt();
    genJetEta[nGenJets] = j.eta();
    genJetPhi[nGenJets] = j.phi();
    genJetMET[nGenJets] = j.invisibleEnergy();
    nGenJets++;
  }

  const reco::GenMET &GenMetCalo = genMetsCalo->front();
  genMetPtCalo  = GenMetCalo.pt();
  genMetPhiCalo = GenMetCalo.phi();

  const reco::GenMET &GenMetTrue = genMetsTrue->front();
  genMetPtTrue  = GenMetTrue.pt();
  genMetPhiTrue = GenMetTrue.phi();

  bool foundGenVertex = false;
  for(size_t i=0; i<genParticles->size();i++)
  {
    if (!foundGenVertex)
    {
      for (unsigned int j=0; j<(*genParticles)[i].numberOfDaughters(); ++j)
      {
        const reco::Candidate *dau = (*genParticles)[i].daughter(j);
        if (dau)
        {
          genVertexX = dau->vx();
          genVertexY = dau->vy();
          genVertexZ = dau->vz();
          if(readGenVertexTime_) genVertexT = *genParticles_t0;
	  else genVertexT = 0.;
          foundGenVertex = true;
          break;
        }
      }
    }
  }

  genWeight = genInfo->weight();
  genSignalProcessID = genInfo->signalProcessID();
  genQScale = genInfo->qScale();
  genAlphaQCD = genInfo->alphaQCD();
  genAlphaQED = genInfo->alphaQED();


  return true;
};


bool llp_ntupler::fillGenParticles(){
  std::vector<const reco::Candidate*> prunedV;//Allows easier comparison for mother finding
  //Fills selected gen particles
  //double pt_cut = isFourJet ? 20.:20.;//this needs to be done downstream
  const double pt_cut = 0.0;

  //define the list of particle IDs that we save as LLPs
  vector<int> llpIDs;
  llpIDs.push_back(9000006);
  llpIDs.push_back(9000007);
  llpIDs.push_back(1023);
  llpIDs.push_back(1000023);
  llpIDs.push_back(1000025);

  for(size_t i=0; i<genParticles->size();i++)
  {
    if( (abs((*genParticles)[i].pdgId()) >= 1 && abs((*genParticles)[i].pdgId()) <= 6 && ( (*genParticles)[i].status() < 30 ))
       || (abs((*genParticles)[i].pdgId()) >= 11 && abs((*genParticles)[i].pdgId()) <= 16)
       || (abs((*genParticles)[i].pdgId()) == 21 && (*genParticles)[i].status() < 30)
	|| (abs((*genParticles)[i].pdgId()) == 22 && (*genParticles)[i].pt() > 10.0)
       || (abs((*genParticles)[i].pdgId()) >= 23 && abs((*genParticles)[i].pdgId()) <= 25)
       || (abs((*genParticles)[i].pdgId()) >= 32 && abs((*genParticles)[i].pdgId()) <= 42)
       || (abs((*genParticles)[i].pdgId()) == 1023)
       || (abs((*genParticles)[i].pdgId()) >= 1000001 && abs((*genParticles)[i].pdgId()) <= 1000039)
       || (abs((*genParticles)[i].pdgId()) == 9000006 || abs((*genParticles)[i].pdgId()) == 9000007))
       {
         if ((*genParticles)[i].pt()>pt_cut){
           prunedV.push_back(&(*genParticles)[i]);
         }
       }

  }

  //Total number of gen particles
  nGenParticle = prunedV.size();
  bool _found_first_llp = false;
  //Look for mother particle and Fill gen variables
  for(unsigned int i = 0; i < prunedV.size(); i++)
  {
    gParticleId[i] = prunedV[i]->pdgId();
    gParticleStatus[i] = prunedV[i]->status();
    gParticleE[i] = prunedV[i]->energy();
    gParticlePt[i] = prunedV[i]->pt();
    gParticlePx[i] = prunedV[i]->px();
    gParticlePy[i] = prunedV[i]->py();
    gParticlePz[i] = prunedV[i]->pz();
    gParticleEta[i] = prunedV[i]->eta();
    gParticlePhi[i] = prunedV[i]->phi();
    gParticleProdVertexX[i] = prunedV[i]->vx();
    gParticleProdVertexY[i] = prunedV[i]->vy();
    gParticleProdVertexZ[i] = prunedV[i]->vz();
    gParticleMotherId[i] = 0;
    gParticleMotherIndex[i] = -1;


    if(prunedV[i]->numberOfMothers() > 0)
    {
      //find the ID of the first mother that has a different ID than the particle itself
      const reco::Candidate* firstMotherWithDifferentID = findFirstMotherWithDifferentID(prunedV[i]);
      if (firstMotherWithDifferentID)
      {
        gParticleMotherId[i] = firstMotherWithDifferentID->pdgId();
      }

      //find the mother and keep going up the mother chain if the ID's are the same
      const reco::Candidate* originalMotherWithSameID = findOriginalMotherWithSameID(prunedV[i]);
      for(unsigned int j = 0; j < prunedV.size(); j++)
      {
        if(prunedV[j] == originalMotherWithSameID)
        {
          gParticleMotherIndex[i] = j;
          break;
        }
      }
    }
    else
    {
      gParticleMotherIndex[i] = -1;
    }

    //---------------------------------------
    //Find LLPs production and decay vertices
    //---------------------------------------
    if (enableGenLLPInfo_)
    {

      //match with one of the entries in the llpIDs List
      bool matchedLLP = false;
      int matchedLLPID = 0;
      if (gParticleStatus[i] == 22) {
	for (uint d=0 ; d < llpIDs.size() ; d++) {
	  if ( abs(gParticleId[i]) == llpIDs[d] ) {
	    matchedLLPID = gParticleId[i];
	    matchedLLP = true;
	  }
	}
      }



      if ( matchedLLP ) {
	if (!_found_first_llp) {
	  gLLP_prod_vertex_x[0] = prunedV[i]->vx();
	  gLLP_prod_vertex_y[0] = prunedV[i]->vy();
	  gLLP_prod_vertex_z[0] = prunedV[i]->vz();
	} else {
	  gLLP_prod_vertex_x[1] = prunedV[i]->vx();
	  gLLP_prod_vertex_y[1] = prunedV[i]->vy();
	  gLLP_prod_vertex_z[1] = prunedV[i]->vz();
	}

        const reco::Candidate *dau = 0;
        bool foundDaughter = false;
        bool noDaughter = false;
        const reco::Candidate *tmpParticle = prunedV[i];

        while (!foundDaughter && !noDaughter) {
          if (tmpParticle->numberOfDaughters() > 0) {
            dau = tmpParticle->daughter(0);
            if (dau && (dau->pdgId() != matchedLLPID)) {
	      foundDaughter = true;
            } else {
	      tmpParticle = dau;
            }
          } else {
            noDaughter = true;
          }
        }

	if (foundDaughter) {
          if (!_found_first_llp) {
	    _found_first_llp = true;
            gLLP_decay_vertex_x[0] = dau->vx();
            gLLP_decay_vertex_y[0] = dau->vy();
            gLLP_decay_vertex_z[0] = dau->vz();
            gLLP_pt[0] = sqrt(gParticlePx[i]*gParticlePx[i]+gParticlePy[i]*gParticlePy[i]);
            gLLP_e[0] = gParticleE[i];
            gLLP_eta[0] = gParticleEta[i];
            gLLP_phi[0] = gParticlePhi[i];
            gLLP_beta[0] = sqrt(gParticlePx[i]*gParticlePx[i]+gParticlePy[i]*gParticlePy[i]+gParticlePz[i]*gParticlePz[i])/gParticleE[i];
            gLLP_travel_time[0] = sqrt(pow(gLLP_decay_vertex_x[0]-gLLP_prod_vertex_x[0],2)
                                    +pow(gLLP_decay_vertex_y[0]-gLLP_prod_vertex_y[0],2)
                                    +pow(gLLP_decay_vertex_z[0]-gLLP_prod_vertex_z[0],2))/(30. * gLLP_beta[0]);//1/30 is to convert cm to ns
            double radius = sqrt( pow(gLLP_decay_vertex_x[0],2) + pow(gLLP_decay_vertex_y[0],2) );
            double ecal_radius = 129.0;
            // double hcal_radius = 179.0;
            double EB_z = 268.36447217; // 129*sinh(1.479)
            double EE_z = 298.5; //where Ecal Endcap starts in z direction

            for (unsigned int id = 0; id < tmpParticle->numberOfDaughters(); id++ ) {
            //std::cout << "====================" << std::endl;
            //std::cout << " -> "<< tmpParticle->daughter(id)->pdgId() << std::endl;
              if( id > 1 ) break;
              TLorentzVector tmp;
              tmp.SetPxPyPzE(tmpParticle->daughter(id)->px(), tmpParticle->daughter(id)->py(), tmpParticle->daughter(id)->pz(), tmpParticle->daughter(id)->energy());
              if(tmp.Pt()<pt_cut) continue;
	      gLLP_daughter_id[id] = tmpParticle->daughter(id)->pdgId();
              gLLP_daughter_pt[id] = tmp.Pt();
              gLLP_daughter_eta[id] = tmp.Eta();
              gLLP_daughter_phi[id] = tmp.Phi();
              gLLP_daughter_e[id]  = tmp.E();

              // double gLLP_daughter_travel_time_hcal= (1./30.)*(hcal_radius-radius)/(tmp.Pt()/tmp.E());// - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns

              gLLP_daughter_travel_time[id] = (1./30.)*(ecal_radius-radius)/(tmp.Pt()/tmp.E());// - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns
            //Calculate dt from generation point to ECAL face
              double x_ecal = gLLP_decay_vertex_x[0] + 30. * (tmp.Px()/tmp.E())*gLLP_daughter_travel_time[id];
              double y_ecal = gLLP_decay_vertex_y[0] + 30. * (tmp.Py()/tmp.E())*gLLP_daughter_travel_time[id];
              double z_ecal = gLLP_decay_vertex_z[0] + 30. * (tmp.Pz()/tmp.E())*gLLP_daughter_travel_time[id];
              // double x_hcal = gLLP_decay_vertex_x[0] + 30. * (tmp.Px()/tmp.E())*gLLP_daughter_travel_time_hcal;
              // double y_hcal = gLLP_decay_vertex_y[0] + 30. * (tmp.Py()/tmp.E())*gLLP_daughter_travel_time_hcal;
              // double z_hcal = gLLP_decay_vertex_z[0] + 30. * (tmp.Pz()/tmp.E())*gLLP_daughter_travel_time_hcal;

              if( fabs(z_ecal) < EB_z && radius <= ecal_radius &&  fabs(gLLP_decay_vertex_z[0]) < EE_z) {
                photon_travel_time[id] = (1./30) * sqrt(pow(ecal_radius,2)+pow(z_ecal,2));
                photon_travel_time_pv[id] = (1./30) * sqrt(pow(x_ecal-genVertexX,2) + pow(y_ecal-genVertexY,2) + pow(z_ecal-genVertexZ,2));
                gen_time_pv[id] =  gLLP_travel_time[0] + gLLP_daughter_travel_time[id] - photon_travel_time_pv[id] + genVertexT;
                gen_time[id] = gLLP_travel_time[0] + gLLP_daughter_travel_time[id] - photon_travel_time[id] + genVertexT;

              } else {
                gLLP_daughter_travel_time[id] = -666;
                gen_time_pv[id] = -666.;
                gen_time[id] = -666.;
                photon_travel_time[id] = -666.;
                photon_travel_time_pv[id] = -666.;
              }
              double min_delta_r = 666.;
              double min_delta_r_calo = 666.;
              unsigned int match_jet_index = 666;
              unsigned int match_calojet_index = 666;


          // Correction of eta and phi based on ecal points
              double phi = atan((y_ecal-genVertexY)/(x_ecal-genVertexX));
              if  (x_ecal < 0.0){
                phi = TMath::Pi() + phi;
              }
              phi = deltaPhi(phi,0.0);
              double theta = atan(sqrt(pow(x_ecal-genVertexX,2)+pow(y_ecal-genVertexY,2))/abs(z_ecal-genVertexZ));
              double eta = -1.0*TMath::Sign(1.0, z_ecal-genVertexZ)*log(tan(theta/2));
              gLLP_daughter_eta_ecalcorr[id] = eta;
              gLLP_daughter_phi_ecalcorr[id] = phi;
              for ( int i_jet = 0; i_jet < nCaloJets; i_jet++ ) {
                double current_delta_r = deltaR(gLLP_daughter_eta_ecalcorr[id], gLLP_daughter_phi_ecalcorr[id], calojetEta[i_jet], calojetPhi[i_jet]);
                if ( current_delta_r < min_delta_r_calo ) {
                  // min_delta_r_nocorr = deltaR(gLLP_daughter_eta[id], gLLP_daughter_phi[id], jetEta[i_jet], jetPhi[i_jet]);
                  min_delta_r_calo = current_delta_r;
                  match_calojet_index = i_jet;
                }
                  // std::cout << i_jet << " min dR = " << min_delta_r_calo << std::endl;
              }
              for ( int i_jet = 0; i_jet < nJets; i_jet++ )
              {
                double current_delta_r = deltaR(gLLP_daughter_eta_ecalcorr[id], gLLP_daughter_phi_ecalcorr[id], jetEta[i_jet], jetPhi[i_jet]);
                if ( current_delta_r < min_delta_r )
                {
                  min_delta_r = current_delta_r;
                  match_jet_index = i_jet;
            //std::cout << i_jet << " min dR = " << min_delta_r << std::endl;
                }
              }//end matching to jets using ECAL radius
              // std::cout<<"min delta r "<<min_delta_r<<std::endl;

              if ( min_delta_r < 0.3 ) {
                gLLP_daughter_match_jet_index[id] = match_jet_index;
                gLLP_min_delta_r_match_jet[id] = min_delta_r;
                jet_matched[match_jet_index] = true;
                // std::cout<<"gllp daughted match jet index "<<gLLP_daughter_match_jet_index[id]<<std::endl;

                // std::cout<<"matched "<<jet_matched[match_jet_index]<<std::endl;
              }

              if (min_delta_r_calo < 0.3) {
                gLLP_daughter_match_calojet_index[id] = match_calojet_index;
                gLLP_min_delta_r_match_calojet[id] = min_delta_r_calo;
              }
            }
          } else {
            gLLP_decay_vertex_x[1] = dau->vx();
            gLLP_decay_vertex_y[1] = dau->vy();
            gLLP_decay_vertex_z[1] = dau->vz();
            gLLP_pt[1] = sqrt(gParticlePx[i]*gParticlePx[i]+gParticlePy[i]*gParticlePy[i]);
            gLLP_e[1] = gParticleE[i];
            gLLP_eta[1] = gParticleEta[i];
            gLLP_phi[1] = gParticlePhi[i];
            gLLP_beta[1] = sqrt(gParticlePx[i]*gParticlePx[i]+gParticlePy[i]*gParticlePy[i]+gParticlePz[i]*gParticlePz[i])/gParticleE[i];
            gLLP_travel_time[1] = sqrt(pow(gLLP_decay_vertex_x[1]-gLLP_prod_vertex_x[1],2)
                    +pow(gLLP_decay_vertex_y[1]-gLLP_prod_vertex_y[1],2)
                    +pow(gLLP_decay_vertex_z[1]-gLLP_prod_vertex_z[1],2))/(30. * gLLP_beta[1]);//1/30 is to convert cm to ns
            double radius = sqrt( pow(gLLP_decay_vertex_x[1],2) + pow(gLLP_decay_vertex_y[1],2) );
            double ecal_radius = 129.0;
            // double hcal_radius = 179.0;
            double EB_z = 268.36447217; // 129*sinh(1.479)
            double EE_z = 298.5; //where Ecal Endcap starts in z direction
            //--------------------------------------------------
            //Second two LLP daughters belong to LLP->pdgID()=36
            //--------------------------------------------------
            for (unsigned int id = 0; id < tmpParticle->numberOfDaughters(); id++ ) {
              //std::cout << " -> "<< tmpParticle->daughter(id)->pdgId() << std::endl;
              if( id > 1 ) break;
              TLorentzVector tmp;
              tmp.SetPxPyPzE(tmpParticle->daughter(id)->px(), tmpParticle->daughter(id)->py(), tmpParticle->daughter(id)->pz(), tmpParticle->daughter(id)->energy());
              if(tmp.Pt()<pt_cut) continue;
	      gLLP_daughter_id[id+2] = tmpParticle->daughter(id)->pdgId();
              gLLP_daughter_pt[id+2] = tmp.Pt();
              gLLP_daughter_eta[id+2] = tmp.Eta();
              gLLP_daughter_phi[id+2] = tmp.Phi();
              gLLP_daughter_e[id+2]  = tmp.E();
              //gLLP_daughter_travel_time[id+2] = (1./30.)*(ecal_radius-radius)/(tmp.Pt()/tmp.E()) - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns
              // double gLLP_daughter_travel_time_hcal = (1./30.)*(hcal_radius-radius)/(tmp.Pt()/tmp.E());// - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns
              gLLP_daughter_travel_time[id+2] = (1./30.)*(ecal_radius-radius)/(tmp.Pt()/tmp.E());// - (1./30.) * ecal_radius * cosh(tmp.Eta());//1/30 is to convert cm to ns

              //Calculate dt from generation point to ECAL face
              double x_ecal = gLLP_decay_vertex_x[1] + 30. * (tmp.Px()/tmp.E())*gLLP_daughter_travel_time[id+2];
              double y_ecal = gLLP_decay_vertex_y[1] + 30. * (tmp.Py()/tmp.E())*gLLP_daughter_travel_time[id+2];
              double z_ecal = gLLP_decay_vertex_z[1] + 30. * (tmp.Pz()/tmp.E())*gLLP_daughter_travel_time[id+2];
              //
              // double x_hcal = gLLP_decay_vertex_x[1] + 30. * (tmp.Px()/tmp.E())*gLLP_daughter_travel_time_hcal;
              // double y_hcal = gLLP_decay_vertex_y[1] + 30. * (tmp.Py()/tmp.E())*gLLP_daughter_travel_time_hcal;
              // double z_hcal = gLLP_decay_vertex_z[1] + 30. * (tmp.Pz()/tmp.E())*gLLP_daughter_travel_time_hcal;
              if( fabs(z_ecal) < EB_z && radius <= ecal_radius && fabs(gLLP_decay_vertex_z[1]) < EE_z)  {
                photon_travel_time[id+2] = (1./30) * sqrt(pow(ecal_radius,2)+pow(z_ecal,2));
                photon_travel_time_pv[id+2] = (1./30) * sqrt(pow(x_ecal-genVertexX,2) + pow(y_ecal-genVertexY,2) + pow(z_ecal-genVertexZ,2));
                gen_time_pv[id+2] =  gLLP_travel_time[1] + gLLP_daughter_travel_time[id+2] - photon_travel_time_pv[id+2] + genVertexT;
                gen_time[id+2] = gLLP_travel_time[1] + gLLP_daughter_travel_time[id+2] - photon_travel_time[id+2] + genVertexT;

              } else {
                gLLP_daughter_travel_time[id+2] = -666;
                gen_time_pv[id+2] = -666.;
                gen_time[id+2] = -666.;
                photon_travel_time[id+2] = -666.;
                photon_travel_time_pv[id+2] = -666.;
              }

              double min_delta_r = 666.;
              double min_delta_r_calo = 666.;
              unsigned int match_jet_index = 666;
              unsigned int match_calojet_index = 666;

              //Corrections for angles based on ECAL radius
              double phi = atan((y_ecal-genVertexY)/(x_ecal-genVertexX));
              if  (x_ecal < 0.0){
                phi = TMath::Pi() + phi;
              }
              phi = deltaPhi(phi,0.0);
              double theta = atan(sqrt(pow(x_ecal-genVertexX,2)+pow(y_ecal-genVertexY,2))/abs(z_ecal-genVertexZ));
              double eta = -1.0*TMath::Sign(1.0,z_ecal-genVertexZ)*log(tan(theta/2));
              gLLP_daughter_eta_ecalcorr[id+2] = eta;
              gLLP_daughter_phi_ecalcorr[id+2] = phi;
              for ( int i_jet = 0; i_jet < nCaloJets; i_jet++ ) {
                double current_delta_r = deltaR(gLLP_daughter_eta_ecalcorr[id+2], gLLP_daughter_phi_ecalcorr[id+2], calojetEta[i_jet], calojetPhi[i_jet]);
                if ( current_delta_r < min_delta_r_calo ) {
                  // min_delta_r_nocorr = deltaR(gLLP_daughter_eta[id], gLLP_daughter_phi[id], jetEta[i_jet], jetPhi[i_jet]);
                  min_delta_r_calo = current_delta_r;
                  match_calojet_index = i_jet;
		}
		//std::cout << i_jet << " min dR = " << min_delta_r << std::endl;
	      }// end matching to calojets
              for ( int i_jet = 0; i_jet < nJets; i_jet++ ) {
                double current_delta_r = deltaR(gLLP_daughter_eta_ecalcorr[id+2], gLLP_daughter_phi_ecalcorr[id+2] , jetEta[i_jet], jetPhi[i_jet]);
                if ( current_delta_r < min_delta_r ) {
                  min_delta_r = current_delta_r;
                  match_jet_index = i_jet;
                }
              }//end matching to jets ecal

              if ( min_delta_r < 0.3 ) {
                gLLP_daughter_match_jet_index[id+2] = match_jet_index;
                gLLP_min_delta_r_match_jet[id+2] = min_delta_r;
                jet_matched[match_jet_index] = true;
                // std::cout<<"gllp daughted match jet index "<<gLLP_daughter_match_jet_index[id+2]<<std::endl;
                // std::cout<<"matched "<<jet_matched[match_jet_index]<<std::endl;
              }
              if ( min_delta_r_calo < 0.3 ) {
                gLLP_daughter_match_calojet_index[id+2] = match_calojet_index;
                gLLP_min_delta_r_match_calojet[id+2] = min_delta_r_calo;
              }
            }//for daughters loop
          }//if particle ID = 36
        }//if found daughters
      } //if matched LLP found

    } //if enable LLP

  }// for loop of genParticles
  return true;
};



bool llp_ntupler::fillTrigger(const edm::Event& iEvent)
{

  //fill trigger information
  const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
  // std::cout << "\n === TRIGGER PATHS === " << std::endl;
  //------------------------------------------------------------------
  //Option to save all HLT path names in the ntuple per event
  //Expensive option in terms of ntuple size
  //------------------------------------------------------------------
  nameHLT->clear();
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i)
  {
    string hltPathNameReq = "HLT_";
    //if (triggerBits->accept(i))
    if ((names.triggerName(i)).find(hltPathNameReq) != string::npos) nameHLT->push_back(names.triggerName(i));
    /*
    std::cout << "Trigger " << names.triggerName(i) <<
    ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
    ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")
    << std::endl;
    if ((names.triggerName(i)).find(hltPathNameReq) != string::npos && triggerBits->accept(i)) std::cout << "Trigger " << names.triggerName(i) <<
    ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")
    << std::endl;
    */
  }
  /*
  std::cout << "n triggers: " <<  nameHLT->size() << std::endl;
  std::cout << "====================" << std::endl;
  for ( unsigned int i = 0; i < nameHLT->size(); i++ )
  {
    std::cout << i << " -> " << nameHLT->at(i) << std::endl;
  }
  */
  //------------------------------------------------------------------
  // Save trigger decisions in array of booleans
  //------------------------------------------------------------------
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i)
  {
    string hltPathNameReq = "HLT_";
    if ((names.triggerName(i)).find(hltPathNameReq) == string::npos) continue;
    if ((names.triggerName(i)).find_last_of("_") == string::npos) continue;
    int lastUnderscorePos = (names.triggerName(i)).find_last_of("_");
    string hltPathNameWithoutVersionNumber = (names.triggerName(i)).substr(0,lastUnderscorePos);

    for (unsigned int j = 0; j < NTriggersMAX; ++j)
    {
      if (triggerPathNames[j] == "") continue;
      if (hltPathNameWithoutVersionNumber == triggerPathNames[j])
      {
        triggerDecision[j] = triggerBits->accept(i);
        //triggerHLTPrescale[j] = triggerPrescales->getPrescaleForIndex(i);
      }
    }
  }
  //------------------------------------------------------------------
  // Print Trigger Objects
  //------------------------------------------------------------------
/*
  for (pat::TriggerObjectStandAlone trigObject : *triggerObjects)
  {
    //cout << "triggerObj: " << trigObject.pt() << " " << trigObject.eta() << " " << trigObject.phi() << "\n";
    //bool foundRazor = false;
    //Need to unpack the filter labels before checking
    trigObject.unpackFilterLabels(iEvent, *triggerBits);
    for(int j=0; j<int(trigObject.filterLabels().size());j++)
    {
      //if ((trigObject.filterLabels())[j] == "hltRsqMR200Rsq0p0196MR100Calo") foundRazor = true;
      // trigObject.unpackPathNames(names);
      // cout << "filter: " << (trigObject.pathNames())[j] << " " << (trigObject.filterLabels())[j] << "\n";
      //cout << "filter: " << (trigObject.filterLabels())[j] << "\n";
    }
  }
*/
//define this as a plug-in
  return true;
};


int llp_ntupler::cscChamber(double x, double y, double z) {
  double r = sqrt(x*x+y*y);
  int sign_z = TMath::Sign(1.0, z);
  // if (r > 80 && r < 283 && abs(z) > 568  && abs(z) < 632) return sign_z*11;
  // if (r > 255 && r < 465.0 && abs(z) > 668.3 && abs(z) < 724) return sign_z*12;
  // if (r > 485.5 && r < 695.5 && abs(z) > 686 && abs(z) < 724) return sign_z*13;
  // if (r > 118.5 && r < 345 && abs(z) > 791 && abs(z) < 849.5) return sign_z*21;
  // if (r > 337.5 && r < 695.5 && abs(z) > 791 && abs(z) < 849.5) return sign_z*22;
  // if (r > 140.5 && r < 345 && abs(z) > 911.5 && abs(z) < 970) return sign_z*31;
  // if (r > 337.5 && r < 695.5 && abs(z) > 911.5 && abs(z) < 970) return sign_z*32;
  // if (r > 157.5 && r < 345 && abs(z) > 1002 && abs(z) < 1060.5) return sign_z*41;
  // if (r > 337.5 && r < 695.5 && abs(z) > 1002 && abs(z) < 1060.5) return sign_z*42;
  if (r < 283 && abs(z) > 568  && abs(z) < 632) return sign_z*11;
  if (r < 470.0 && abs(z) > 668.3 && abs(z) < 724) return sign_z*12;
  if (r > 480.0 && abs(z) > 686 && abs(z) < 724) return sign_z*13;
  if (r < 345 && abs(z) > 791 && abs(z) < 849.5) return sign_z*21;
  if (r > 337.5 && abs(z) > 791 && abs(z) < 849.5) return sign_z*22;
  if (r < 345 && abs(z) > 911.5 && abs(z) < 970) return sign_z*31;
  if (r > 337.5 && abs(z) > 911.5 && abs(z) < 970) return sign_z*32;
  if (r < 345 && abs(z) > 1002 && abs(z) < 1060.5) return sign_z*41;
  if (r > 337.5 && abs(z) > 1002 && abs(z) < 1060.5) return sign_z*42;
  return -999;
};

int llp_ntupler::cscStation(double x, double y, double z) {
  double r = sqrt(x*x+y*y);
  // z = abs(z);
  int sign_z = TMath::Sign(1.0, z);
  if (r < 283 && abs(z) > 568  && abs(z) < 632) return sign_z*1;
  if (r < 470.0 && abs(z) > 668.3 && abs(z) < 724) return sign_z*1;
  if (r > 480.0 && abs(z) > 686 && abs(z) < 724) return sign_z*1;
  if (r < 345 && abs(z) > 791 && abs(z) < 849.5) return sign_z*2;
  if (r > 337.5 && abs(z) > 791 && abs(z) < 849.5) return sign_z*2;
  if (r < 345 && abs(z) > 911.5 && abs(z) < 970) return sign_z*3;
  if (r > 337.5 && abs(z) > 911.5 && abs(z) < 970) return sign_z*3;
  if (r < 345 && abs(z) > 1002 && abs(z) < 1060.5) return sign_z*4;
  if (r > 337.5 && abs(z) > 1002 && abs(z) < 1060.5) return sign_z*4;
  return -999;
};


DEFINE_FWK_MODULE(llp_ntupler);
