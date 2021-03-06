
#include "HFSteppingAction.hh"

#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "HFDataFormat.hh"

HFSteppingAction::HFSteppingAction(HFDataFormat *df)
:m_df(df)
,m_optDef( G4OpticalPhoton::OpticalPhotonDefinition() )
{ }

HFSteppingAction::~HFSteppingAction()
{ }

void HFSteppingAction::UserSteppingAction(const G4Step * step)
{

  // get volume of the current step
  G4VPhysicalVolume* preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // get next volume
  G4VPhysicalVolume * postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

  const G4String & preName = preVolume->GetName();
  const bool isFibre = preName.contains("fib") || preName.contains("clad");
  const bool isScinFibre = preName.contains("scsf") || preName.contains("cladScin");

  // get the particle
  G4Track * theTrack = step->GetTrack();
  const G4ThreeVector & pos = theTrack->GetPosition();

  const double time = theTrack->GetGlobalTime();
  if ( time > 100*ns ) theTrack->SetTrackStatus(fStopAndKill);

  // record photons tracked to PMT face
  if ( theTrack->GetDefinition() == m_optDef && preVolume != postVolume 
	&&  ( isFibre || isScinFibre ) ) {

    const G4String & postName = postVolume->GetName();
    if ( postName.contains("glass") ) {

      const G4DynamicParticle * theParticle = theTrack->GetDynamicParticle();
      const double wavelength = hbarc*twopi/theParticle->GetTotalEnergy()*1.e+6;

      const G4ThreeVector & pol = theParticle->GetPolarization();

      SteppingStruct st(pos,time,theTrack->GetLocalTime(),theTrack->GetTrackLength(),wavelength,pol.x(),pol.y());
      if ( wavelength > 350. )  {
  	//if ( isFibre ) m_df->fillSteppingAction( st, fCherenkov );
  	//else if ( isScinFibre ) m_df->fillSteppingAction( st, fScintillation );
      }

      // kill the track after readout
      theTrack->SetTrackStatus(fStopAndKill);

    } else if ( postName.contains("World") || postName.contains("tungsten") ) {
      // kill the track since it left the fiber
      theTrack->SetTrackStatus(fStopAndKill);
    }

  } else if ( preVolume == postVolume && preName.contains("scsf") ) {
    const double E = step->GetTotalEnergyDeposit();
    IoniStruct is(E,pos,time);
    //m_df->fillIonization(is); 
  } 

}

