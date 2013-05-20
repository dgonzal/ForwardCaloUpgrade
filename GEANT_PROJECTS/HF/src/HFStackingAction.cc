//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: HFStackingAction.cc,v 1.6 2013/05/17 19:19:10 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <iostream>
#include "HFStackingAction.hh"
#include "HFDetectorConstruction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFStackingAction::HFStackingAction(HFDataFormat *df)
: gammaCounter(0)
, m_lCutLow(350.)
, m_fibreNA(-1.)
, m_scsfNA(-1.)
, m_nFibre(1.457)
, m_nClad(1.419)
, m_nScin(1.59)
, m_nScinClad(1.49)
, m_rFibre(0.306*mm)
, m_df(df)
, m_optDef( G4OpticalPhoton::OpticalPhotonDefinition() )
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFStackingAction::~HFStackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
HFStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{

  if ( m_fibreNA < 0. ) m_fibreNA = sqrt(m_nFibre*m_nFibre-m_nClad*m_nClad);
  if ( m_scsfNA < 0. ) m_scsfNA = sqrt(m_nScin*m_nScin-m_nScinClad*m_nScinClad);

  G4ClassificationOfNewTrack classification = fUrgent;

  const G4VPhysicalVolume * volume = aTrack->GetVolume();
  const G4String & vName = volume->GetName();

  if ( !volume ) return classification;

  // kill optical photon
  //if ( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) classification = fKill;

  // check if track is an optical photon in a fibre
  if(aTrack->GetDefinition() == m_optDef )
  { // particle is optical photon
    if(aTrack->GetParentID()>0)
    { // particle is secondary

      //classification = fKill;

      const G4DynamicParticle *aParticle = aTrack->GetDynamicParticle();
      const double E = aParticle->GetTotalEnergy();
      const double lambda = hbarc*twopi/E*1.e+6;
      const G4ThreeVector & mom = aTrack->GetMomentumDirection();
      const double px = mom.x();
      const double py = mom.y();
      const double pz = mom.z();
      
      const G4ThreeVector & pos = aTrack->GetPosition();
      const double x = pos.x();
      const double y = pos.y();
      const double z = pos.z();
      const double t = aTrack->GetGlobalTime();

      //////////////////////////////////////////////////
      // Find the fiber's central axis, and the displancement
      // of the photon production point from the that axis in
      // a plane perpendicular to the fiber.
      const G4ThreeVector & trans = volume->GetTranslation();
      const G4ThreeVector rho = pos - trans;
      const double rhox = rho.x();
      const double rhoy = rho.y();

      /////////////////////////////////////////////////
      // calculate the angle the photon makes with the core/clad
      // interface surface normal.  
      // eta is the angle between the photon and the surface normal 
      // in the transverse plane
      // psi is the angle between the photon and fiber's central axis.
      const double sinEta = (rhox*py-rhoy*px)/m_rFibre;
      const double sinPsi = sqrt(1.-pz*pz);
      //const double na = sinPsi*cos(asin(sinEta));  // equal to cos(Xi)
     
      //////////////////////
      // This is the old method to check photon acceptance
      const double na = sqrt(px*px+py*py);
      
      //const G4VProcess * proc = aTrack->GetCreatorProcess();
      //const G4String & procName = proc ? proc->GetProcessName() : "no";
     
      if ( lambda <= m_lCutLow || pz < 0. ) classification = fKill;

      if ( vName.contains("fib") &&  lambda > m_lCutLow && na < m_fibreNA ) { 
        gammaCounter++;
	StackingStruct st(lambda,E,na,x,y,z,t);
        m_df->fillStackingAction(st,fCherenkov);
      } else if ( vName.contains("scsf") &&  lambda > m_lCutLow && na < m_scsfNA ) { 
        gammaCounter++;
	StackingStruct st(lambda,E,na,x,y,z,t);
        m_df->fillStackingAction(st,fScintillation);
      } 
    }

  } else {

    const G4DynamicParticle * particle = aTrack->GetDynamicParticle();
    const double E = particle->GetTotalEnergy();
    const int pdgId = particle->GetPDGcode();
    const G4ThreeVector & mom = aTrack->GetMomentum();
    const G4ThreeVector & pos = aTrack->GetPosition();


    ParticleStruct pt(pdgId,mom,pos,E);
    m_df->fillParticle(pt);

  }


  return classification;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HFStackingAction::NewStage()
{
  G4cout << "Number of optical photons produced in this event : "
         << gammaCounter << " R = " << m_rFibre << " NA = " << m_fibreNA << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HFStackingAction::PrepareNewEvent()
{ gammaCounter = 0; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
