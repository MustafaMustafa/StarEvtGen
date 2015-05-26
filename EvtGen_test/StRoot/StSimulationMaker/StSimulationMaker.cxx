/*/-----------------

  This class is for the eventGen production

  Auther:  Xiaozhi 5/1/2015 xiaozhi@uic.edu

  ----------------*/

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

//#ifdef EVTGEN_EXTERNAL
#include "EvtGenExternal/EvtExternalGenList.hh"
//#endif

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
#include "StRoot/StarGenerator/StarEvtGenDecayer/StarEvtGenDecayer.h"
#include "TNtuple.h"
#include "TFile.h"
#include "time.h"
#include "StSimulationMaker.h"
 
using namespace std;
const double accp_eta=1.0;
const int MaxDaughter=50;

StSimulationMaker::StSimulationMaker()
{

}
StSimulationMaker::~StSimulationMaker()
{

}
StSimulationMaker::StSimulationMaker(int Parent_pdg,int Number_Event,TString outfile)
{

  parent_PDG=Parent_pdg;
  NEvet=Number_Event;
  outfile_name=outfile;
  std::cout<< "constructor"<<"  "<<outfile<<std::endl;
}
void StSimulationMaker::OpenFile()
{
 mfile=new TFile(outfile_name,"RECREATE");
  Tree = new TTree("Tree","Tree");
  std::cout<< "file is open "<<"  "<<outfile_name<<std::endl;
}
void StSimulationMaker::Write()
{

  mfile->cd();
  Tree->Write();
  mfile->Close();
}
void StSimulationMaker::make()
{ 
   
 
 /************************************************ set up the EvetGen *****************************/ 
   
 EvtStdlibRandomEngine * eng = new EvtStdlibRandomEngine();
  eng->setSeed(NULL);
  EvtRandom::setRandomEngine((EvtRandomEngine*)eng);
  EvtAbsRadCorr* radCorrEngine = 0;
  std::list<EvtDecayBase*> extraModels;
  
//#ifdef EVTGEN_EXTERNAL
  EvtExternalGenList genList;
  radCorrEngine = genList.getPhotosModel();
  extraModels = genList.getListOfModels();
//#endif

  
  //  ************************************************ set up the EvetGen *****************************// 
  
  //Initialize the generator - read in the decay table and particle properties
  TString Decay_2010_DEC="/star/u/xiao00/Run14/work_AuAu200_NPE/Simulation_NPE/Event_Gen/evtgen/1.3.0/DECAY_2010.DEC";
  TString Evt_pdl="/star/u/xiao00/Run14/work_AuAu200_NPE/Simulation_NPE/Event_Gen/evtgen/1.3.0/evt.pdl";
  EvtGen *myGenerator=new EvtGen(Decay_2010_DEC,Evt_pdl,(EvtRandomEngine*)eng,radCorrEngine, &extraModels);
  StarEvtGenDecayer *Decay_Event=new StarEvtGenDecayer(myGenerator); 
  
  
  EvtId Parent_EvtGenID=EvtPDL::evtIdFromLundKC(parent_PDG); 
  
  
   TString Decay_file="Decay_table/NPE.DEC";
  TClonesArray *_array=new TClonesArray("TParticle");   
  //    TString Decay_file="Decay_table/NPE_Pythia.DEC";
  
  Decay_Event->setDecayTable(Decay_file);

  Double_t pt=0;
  Double_t eta=0;
  Double_t phi=0;
  Double_t px=0;
  Double_t py=0;
  Double_t pz=0;
   
 
  
   int Ndaughter=0;    


 

  Int_t   mpdg[MaxDaughter];
  double  mPx[MaxDaughter];
  double  mPy[MaxDaughter];
  double  mPz[MaxDaughter];
  double  mE[MaxDaughter];  
  Int_t  dpdg[MaxDaughter]; 
  Int_t  dParent_pdg[MaxDaughter]; 
  double  dPx[MaxDaughter];
  double  dPy[MaxDaughter];
  double  dPz[MaxDaughter];
  double  dE[MaxDaughter];  
  double  dvx[MaxDaughter];
  double  dvy[MaxDaughter];
  double  dvz[MaxDaughter];
  double  dct[MaxDaughter];


  Tree->Branch("Ndaughter",&Ndaughter,"Ndaughter/I");
  Tree->Branch("mpdg",mpdg,"mpdg[Ndaughter]/I");
  Tree->Branch("mPx",mPx,"mPx[Ndaughter]/D");
  Tree->Branch("mPy",mPy,"mPy[Ndaughter]/D");
  Tree->Branch("mPz",mPz,"mPz[Ndaughter]/D");
  Tree->Branch("mE",mE,"mE[Ndaughter]/D");
  Tree->Branch("dpdg",dpdg,"dpdg[Ndaughter]/I");
  Tree->Branch("dParent_pdg",dpdg,"dParent_pdg[Ndaughter]/I");
  Tree->Branch("dPx",dPx,"dPx[Ndaughter]/D");
  Tree->Branch("dPy",dPy,"dPy[Ndaughter]/D");
  Tree->Branch("dPz",dPz,"dPz[Ndaughter]/D");
  Tree->Branch("dE",dE,"dE[Ndaughter]/D");
  Tree->Branch("dvx",dvx,"dvx[Ndaughter]/D");
  Tree->Branch("dvy",dvy,"dvy[Ndaughter]/D");
  Tree->Branch("dvz",dvz,"dvz[Ndaughter]/D");
  Tree->Branch("dct",dct,"dct[Ndaughter]/D");
 
 
 
  for(Int_t iEvet=0;iEvet<NEvet;iEvet++){  //  Event Loop              
    
    if(iEvet%100==0)
      cout<< "---------working on Event "<<iEvet<<endl;
    Ndaughter=0;        
    get_kinematics(pt,eta,phi,px,py,pz);
    Double_t m0= EvtPDL::getMass(Parent_EvtGenID);
    Double_t E=sqrt(m0*m0+px*px+py*py+pz*pz);
    TLorentzVector *p_mom=new TLorentzVector(px,py,pz,E);  
    
    ResolveDecay(Decay_Event,Ndaughter,parent_PDG,p_mom,mpdg,mPx,mPy, mPz,mE,dpdg,dParent_pdg,dPx,dPy,dPz,dE,dvx,dvy,dvz,dct);   
    //  continue;
    //    Decay_Event->Decay(parent_PDG,p_mom);
    // int  NDughter= Decay_Event->ImportParticles(_array);
    
    // for(int iDaughter_0=0;iDaughter_0<_array->GetEntries();iDaughter_0++)
    //   {
    // 	TParticle* particle = (TParticle*)_array->At(iDaughter_0);
	

	//SaveDaughter(particle,Ndaughter,parent_PDG,p_mom,mpdg,mPx,mPy, mPz,mE,dpdg,dParent_pdg,dPx,dPy,dPz,dE,dvx,dvy,dvz,dct);
	
	// if(Continue_Decay(particle->GetPdgCode()))
	//   {
	//     TClonesArray *_array_daughter=new TClonesArray("TParticle");	    
	//     // cout<< " continu  "<<particle->GetPdgCode()<<endl;
	//     TLorentzVector *mother=new TLorentzVector(particle->Px(),particle->Py(),particle->Pz(),particle->Energy());
	//     Decay_Event->Decay(particle->GetPdgCode(),mother);
	//     Int_t n=Decay_Event->ImportParticles(_array_daughter);
	//     for(Int_t iDaughter_1=0;iDaughter_1<_array_daughter->GetEntries();iDaughter_1++)
	//       {
		
	// 	TParticle* particle_1 = (TParticle*)_array_daughter->At(iDaughter_1);
	// 	SaveDaughter(particle_1,Ndaughter,particle->GetPdgCode(),mother,mpdg,mPx,mPy, mPz,mE,dpdg,dParent_pdg,dPx,dPy,dPz,dE,dvx,dvy,dvz,dct);
	// 	// if(Continue_Decay(particle_1->GetPdgCode()))
	// 	//   {
	// 	//     cout<< _array_daughter->GetEntries()<< " second "<< particle_1->GetPdgCode()<<endl;
	// 	//     TClonesArray *_array_daughter_2=new TClonesArray("TParticle");	    
	// 	//     TLorentzVector *mother_2=new TLorentzVector(particle_1->Px(),particle_1->Py(),particle_1->Pz(),particle_1->Energy());
	// 	//     Decay_Event->Decay(particle_1->GetPdgCode(),mother_2);
	// 	//     Int_t n=Decay_Event->ImportParticles(_array_daughter_2);
	// 	//     // for(int iDaughter=0;iDaughter<_array_daughter_2->GetEntries();iDaughter++)
	// 	//     //   {
	// 	//     // 	TParticle  *particle_2=(TParticle *) _array_daughter_2->At(iDaughter);
	// 	//     // 	// 	SaveDaughter(particle_2,Ndaughter,particle_1->GetPdgCode(),mother_2,mpdg,mPx,mPy, mPz,mE,dpdg,dParent_pdg,dPx,dPy,dPz,dE,dvx,dvy,dvz,dct);
	// 	//     // 	//	if(Continue_Decay(particle_2->GetPdgCode()))
	// 	//     // 	//  {
	// 	//     // 	// cout<< "third Decay"<< particle_2->GetPdgCode()<<endl;
	// 	//     // 	// }         
	// 	//     //   } 
		    
		     
	// 	//   }
	// 	// }
	    
	    
	    
	//   }
	
	// mpdg[iDaughter]=parent_PDG;
	// mPx[iDaughter]=px;
	// mPy[iDaughter]=py;
	// mPz[iDaughter]=pz;
	// mE[iDaughter]=E;  
	// dpdg[iDaughter]=particle->GetPdgCode();
	// dParent_pdg[iDaughter]=particle->GetFirstMother();

	// dPx[iDaughter]=particle->Px();
	// dPy[iDaughter]=particle->Py();
	// dPz[iDaughter]=particle->Pz();
	// dE[iDaughter]=particle->Energy();  
	// dvx[iDaughter]=particle->Vx();
	// dvy[iDaughter]=particle->Vy();
	// dvz[iDaughter]=particle->Vz();
	// dct[iDaughter]=particle->T();
	//if(fabs(particle->GetPdgCode())==11)
	//	cout<< particle->GetPdgCode()<<endl;

	// if(Continue_Decay(particle->GetPdgCode()));
	// {
	//   std::cout<<" this particle can continue Decay!!  "<< endl;	
	
	// }
    // } 
 
 

    Tree->Fill();
   
      p_mom->Delete();
   
  }// Evt loop
}

void StSimulationMaker::ResolveDecay(StarEvtGenDecayer *Decay_Event,int &Ndaughter,Int_t Parent_pdg,TLorentzVector *p_mom,Int_t mpdg[ ],double mPx[ ],double mPy[ ],double  mPz[ ], double mE[ ], Int_t dpdg[ ], Int_t dParent_pdg[ ], double dPx[ ],double dPy[ ], double  dPz[ ],double dE[ ], double dvx[ ], double dvy[ ], double dvz[ ], double  dct[ ])
{
  //return;
  int N=0;
  TClonesArray *Array=new TClonesArray("TParticle");

  Decay_Event->Decay(Parent_pdg,p_mom);
  N=Decay_Event->ImportParticles(Array);   

  for(int iDaughter=0;iDaughter<Array->GetEntries();iDaughter++)
    {
      TParticle* particle = (TParticle*)Array->At(iDaughter);
      SaveDaughter(particle,Ndaughter,Parent_pdg,p_mom,mpdg,mPx,mPy, mPz,mE,dpdg,dParent_pdg,dPx,dPy,dPz,dE,dvx,dvy,dvz,dct);
      
      //  continue;
      if(Continue_Decay(particle->GetPdgCode()))
      	{
      	  cout<< " Resolev third Decay"<< particle->GetPdgCode()<<endl;
	   int Parent_pdg =particle->GetPdgCode();
	   TLorentzVector *p_mom=new TLorentzVector(particle->Px(),particle->Py(),particle->Pz(),particle->Energy());  
		// p_mom->SetPxPyPzE(particle->Px(),particle->Py(),particle->Pz(),particle->Energy());  
	    ResolveDecay(Decay_Event,Ndaughter,Parent_pdg,p_mom,mpdg,mPx,mPy, mPz,mE,dpdg,dParent_pdg,dPx,dPy,dPz,dE,dvx,dvy,dvz,dct);         
	}  
      //    p_mom->Delete();     
      // cout<<" Resolve Decay"<< N<<"  "<<Array<<endl;
    }
   Array->Delete();
}
void StSimulationMaker::SaveDaughter(TParticle *particle,int &Ndaughter,Int_t Parent_pdg,TLorentzVector *p_mom,Int_t mpdg[ ],double mPx[ ],double mPy[ ],double  mPz[ ], double mE[ ], Int_t dpdg[ ], Int_t dParent_pdg[ ], double dPx[ ],double dPy[ ], double  dPz[ ],double dE[ ], double dvx[ ], double dvy[ ], double dvz[ ], double  dct[ ])
{
   if(!particle||!p_mom) 
   return ;

  mpdg[Ndaughter]=Parent_pdg;
  mPx[Ndaughter]=p_mom->Px();
  mPy[Ndaughter]=p_mom->Py();
  mPz[Ndaughter]=p_mom->Pz();
  mE[Ndaughter]=p_mom->E();  
  dpdg[Ndaughter]=particle->GetPdgCode();
  dParent_pdg[Ndaughter]=particle->GetFirstMother();
  dPx[Ndaughter]=particle->Px();
  dPy[Ndaughter]=particle->Py();
  dPz[Ndaughter]=particle->Pz();
  dE[Ndaughter]=particle->Energy();  
  dvx[Ndaughter]=particle->Vx();
  dvy[Ndaughter]=particle->Vy();
  dvz[Ndaughter]=particle->Vz();
  dct[Ndaughter]=particle->T();

  Ndaughter++;

}
void StSimulationMaker::get_kinematics(Double_t& pt, Double_t& eta, Double_t& phi, Double_t& px, Double_t& py, Double_t& pz)
{
  gRandom->SetSeed();
  pt = gRandom->Uniform(0.2,20);
  eta = gRandom->Uniform(-accp_eta,accp_eta);
  phi = TMath::TwoPi() * gRandom->Rndm();
  px = pt * cos(phi);
  py = pt * sin(phi);
  pz = pt / tan(2 * atan(exp(-eta)));
}
void StSimulationMaker::SaveTree()
{
  mfile->cd();
  Tree->Write();
  mfile->Close();

}
bool StSimulationMaker::Continue_Decay(int pdg)
{

  bool bContinue=false;

  if(fabs(pdg)==511) // B0
    bContinue=true;

  if(fabs(pdg)==421) // D0
    bContinue=true;

  if(fabs(pdg)==413) // D*
    bContinue=true;
 if(fabs(pdg)==411) // D+
    bContinue=true;


  return bContinue;


}
