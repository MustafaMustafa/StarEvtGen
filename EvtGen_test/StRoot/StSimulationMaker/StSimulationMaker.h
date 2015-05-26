/*/-----------------

  This class is for the eventGen production

  Auther:  Xiaozhi 5/1/2015 xiaozhi@uic.edu

  ----------------*/

/*
#include "EvtGen/EvtGen.hh"

#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtHepMCEvent.hh"
#include "EvtGenBase/EvtStdlibRandomEngine.hh"
#include "EvtGenBase/EvtAbsRadCorr.hh"
#include "EvtGenBase/EvtDecayBase.hh"

#ifdef EVTGEN_EXTERNAL
#include "EvtGenExternal/EvtExternalGenList.hh"
#endif
*/

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <vector>
#include <fstream>

#include <TFile.h>
#include <TH1F.h>
#include"TParticle.h"
#include"TLorentzVector.h"
#include"TClonesArray.h"
#include "TVirtualMCDecayer.h"
#include "TClonesArray.h"
#include "TString.h"
#include "vector"
#include "TRandom.h"
//#include "StRoot/StarGenerator/StarEvtGenDecayer/StarEvtGenDecayer.h"
#include "TNtuple.h"
#include "TFile.h"
#include "time.h"
#include "StMaker.h"

/* using namespace std; */
/* const double accp_eta=1.0; */
/* const int MaxDaughter=10; */

class StarEvtGenDecayer; 

class StSimulationMaker : public StMaker 
{
 public:
  StSimulationMaker();
  ~StSimulationMaker();
  StSimulationMaker(int,int,TString );

  void OpenFile();
  void make();
  void Write();
  void SaveTree();
  void get_kinematics(Double_t& pt, Double_t& eta, Double_t& phi, Double_t& px, Double_t& py, Double_t& pz); 
  void ResolveDecay(StarEvtGenDecayer *Decay_Event,int &Ndaughter,Int_t Parent_pdg,TLorentzVector *p_mom,Int_t mpdg[ ],double mPx[ ],double mPy[ ],double  mPz[ ], double mE[ ], Int_t dpdg[ ], Int_t dParent_pdg[ ], double dPx[ ],double dPy[ ], double  dPz[ ],double dE[ ], double dvx[ ], double dvy[ ], double dvz[ ], double  dct[ ]);
  void SaveDaughter(TParticle *Particle,int &Ndaughter,int, TLorentzVector *,Int_t mpdg[ ],double mPx[ ],double mPy[ ],double  mPz[ ],double mE[ ], Int_t dpdg[ ], Int_t dParent_pdg[ ], double dPx[ ],double dPy[ ], double  dPz[ ],double dE[ ], double dvx[ ], double dvy[ ], double dvz[ ], double  dct[ ]);
  bool Continue_Decay(int);
 private:
 TFile *mfile;
 TTree *Tree;
 TString outfile_name;
 int parent_PDG;
 int NEvet;
 // static   int MaxDaughter=50;
 //EvtId Parent_EvtGenID;


    ClassDef(StSimulationMaker, 1)
};

