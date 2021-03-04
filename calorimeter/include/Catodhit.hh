#ifndef CatodHit_h
#define CatodHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"
#include "G4LogicalVolume.hh"

class G4AttDef;
class G4AttValue;

class Catodhit : public G4VHit
{
  public:
    CatodHit();
    CatodHit(G4int iCol,G4int iRow);
    CatodHit(const CatodHit &right);
    virtual ~CatodHit();

    const CatodHit& operator=(const CatodHit& right);
    G4bool operator==(const CatodHit& right) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    virtual void Draw() {}
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    void SetColumnID(G4int z) { fColumnID = z; }
    G4int GetColumnID() const { return fColumnID; }

    void SetRowID(G4int z) { fRowID = z; }
    G4int GetRowID() const { return fRowID; }

    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }


    void AddNpe(){ fNpe +=1; }
    G4double GetNpe() const { return fNpe; }

  private:
    G4int fColumnID;
    G4int fRowID;
    G4double fEdep;
    G4int fNpe;
};
using CalorHitsCollection = G4THitsCollection<CatodHit>;

extern G4ThreadLocal G4Allocator<CatodHit>* CalorHitAllocator;

inline void* CatodHit::operator new(size_t)
{
    if(!CalorHitAllocator){
        CalorHitAllocator = new G4Allocator<CatodHit>;
    }
    void *hit;
    hit = (void *) CalorHitAllocator->MallocSingle();
    return hit;
}

inline void CatodHit::operator delete(void *hit)
{
    if(!CalorHitAllocator){
        CalorHitAllocator = new G4Allocator<CatodHit>;
    }
    CalorHitAllocator->FreeSingle((CatodHit*) hit);
}

#endif