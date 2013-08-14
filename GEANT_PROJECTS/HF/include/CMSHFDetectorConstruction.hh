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
// $Id: CMSHFDetectorConstruction.hh,v 1.6 2013/05/17 19:19:10 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef CMSHFDetectorConstruction_h
#define CMSHFDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "CMSHFDetectorConstructionMessenger.hh"

class G4LogicalVolume;
class G4PhysicalVolume;
class G4Material;
class G4Box;
class G4Tubs;
class G4UserLimits;

class G4UniformMagField;

class HFStackingAction;
class HFPrimaryGeneratorAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CMSHFDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    CMSHFDetectorConstruction();
   ~CMSHFDetectorConstruction();

  public:
    G4VPhysicalVolume* Construct();

    // set the position of the centre of the calorimeter segment
    void SetPositionXYZ(const G4ThreeVector &detPos);

    // set the length of the calorimeter (longitudinal)
    void SetLength(G4double l);

    // set the width of the detector volume
    void SetWidth(G4double w);

    // set the fibre index of refraction
    void SetFibreIndex(G4double n);

    // set the cladding index of refraction
    void SetCladIndex(G4double n);

    // allow access to the user stacking action class CMSHFStackingAction
    void SetStackingAction( HFStackingAction * sa);

    // allow access to the particle gun
    void SetParticleGun( HFPrimaryGeneratorAction *gun);

    // set whether to check for overlaps in the geometry
    void SetOverlapCheck( G4bool check );

    // set the magnetic field
    void SetMagneticField( const G4ThreeVector &vec);

    // ---- accessor methods -----
    void PrintCalorParameters() { return; }
    
    // 
    G4ThreeVector GetPositionXYZ() const { return G4ThreeVector(m_xPos,m_yPos,m_zPos); }
    G4double GetLength() const { return m_length; }
    G4double GetFibreIndex() const { return m_nFib; }
    G4double GetCladIndex() const { return m_nClad; }
    G4double GetOverlapCheck() const { return m_checkOverlaps; }
    
    // compute the packing fraction
    G4double GetPackingFraction() const;

  private:
   // -------------- private member functions --------
   void DefineMaterials();
   void SetupWorld();
   void SetupGeometry();
   void SetupDetectors(); 
   void CalculateConstants();

   void ClearPhysicalVolumes();
   void ClearLogicalVolumes();


   //  ------------- private data -------------------
   G4bool m_isConstructed;

    // parameters
    G4double m_expHall_x;
    G4double m_expHall_y;
    G4double m_expHall_z;

    // position of calorimeter
    G4double m_zPos;
    G4double m_xPos;
    G4double m_yPos;

    // Tungsten block parameters
    G4double m_Wdx;
    G4double m_Wdy;

    G4double m_nFib;
    G4double m_nClad;
    G4double m_absFib;
    G4double m_absClad;
    G4double m_nSFib;
    G4double m_nSClad;
    G4double m_absSFib;
    G4double m_absSClad;
    G4double m_length;
    G4double m_nGlass;
    G4double m_absGlass;

    // radii for fibers and cladding ( C for Cherenkov and S for scintillation)
    G4double m_rCFib;
    G4double m_rCClad;
    G4double m_rSFib;
    G4double m_rSClad;

    // grid spacing
    G4double m_a;

    G4bool m_checkOverlaps;

    // materials
    G4Material * m_air;
    G4Material * m_quartz;
    G4Material * m_tungsten;
    G4Material * m_glass;
    G4Material * m_cladCher;
    G4Material * m_scsf78;
    G4Material * m_cladScin;
    G4Material * m_iron;

    // primitives
    G4Box * m_expHall_box;
    G4Box * m_tungBlock;
    G4Tubs * m_qFibreCher;
    G4Tubs * m_qFibreScin;
    G4Box * m_glass_box;
    G4Tubs * m_cladCher_tube;
    G4Tubs * m_cladScin_tube;

    // logical volumes
    G4LogicalVolume * m_expHall_log;
    G4LogicalVolume * m_tungBlock_log;
    G4LogicalVolume * m_qFibreCher_log;
    G4LogicalVolume * m_cladCher_log;
    G4LogicalVolume * m_qFibreScin_log;
    G4LogicalVolume * m_cladScin_log;
    G4LogicalVolume * m_glass_log;

    // physical volumes
    G4VPhysicalVolume * m_expHall_phys;
    G4VPhysicalVolume * m_tungBlock_phys;
    std::vector<G4VPhysicalVolume *> m_fibresCher;
    std::vector<G4VPhysicalVolume *> m_claddingCher;
    std::vector<G4VPhysicalVolume *> m_fibresScin;
    std::vector<G4VPhysicalVolume *> m_claddingScin;
    G4VPhysicalVolume * m_glass_phys;

    // uniform magnetic field
    G4UniformMagField *m_Bfield;

    CMSHFDetectorConstructionMessenger *m_messenger;
    HFStackingAction * m_stacking;
    HFPrimaryGeneratorAction *m_gun;

    G4UserLimits *m_fibLimits;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*CMSHFDetectorConstruction_h*/