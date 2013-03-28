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
//
// $Id: HFPrimaryGeneratorAction.hh,v 1.2 2013/03/20 16:38:48 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HFPrimaryGeneratorAction_h
#define HFPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class HFPrimaryGeneratorMessenger;
class HFDataFormat;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HFPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    HFPrimaryGeneratorAction();
    HFPrimaryGeneratorAction( double l);
   ~HFPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);

    void SetOptPhotonPolar();
    void SetOptPhotonPolar(G4double);
    void SetInitDist(double);
    void SetBeamWidth(double);

    inline void SetDataFormat(HFDataFormat *df) { m_df = df; }

  private:

    void initialize();

    G4ParticleGun* particleGun;
    HFPrimaryGeneratorMessenger* gunMessenger;

    double m_initDist; // initial distance from the origin
    double m_beamWidth; // make a square beam of this width

    HFDataFormat * m_df;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*HFPrimaryGeneratorAction_h*/
