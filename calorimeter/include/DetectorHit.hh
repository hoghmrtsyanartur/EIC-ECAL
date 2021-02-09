#ifndef DetectorHit_h
#define DetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

class DetectorHit : public G4VHit
{
  public:
    DetectorHit();
    DetectorHit(const DetectorHit&);
    virtual ~DetectorHit();

    const DetectorHit& operator=(const DetectorHit&);
    G4bool operator==(const DetectorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    virtual void Draw() {}
    virtual void Print();

    void Add(G4double de, G4double dl);

    G4double GetEdep() const;
    G4double GetTrackLength() const;
      
  private:
    G4double fEdep;        // Energy deposition
    G4double fTrackLength; // Track length
};
using CalorHitsCollection = G4THitsCollection<DetectorHit>;

extern G4ThreadLocal G4Allocator<DetectorHit>* CalorHitAllocator;

inline void* DetectorHit::operator new(size_t)
{
    if(!CalorHitAllocator){
        CalorHitAllocator = new G4Allocator<DetectorHit>;
    }
    void *hit;
    hit = (void *) CalorHitAllocator->MallocSingle();
    return hit;
}

inline void DetectorHit::operator delete(void *hit)
{
    if(!CalorHitAllocator){
        CalorHitAllocator = new G4Allocator<DetectorHit>;
    }
    CalorHitAllocator->FreeSingle((DetectorHit*) hit);
}

inline void DetectorHit::Add(G4double de, G4double dl){
    fEdep += de;
    fTrackLength += dl;
}
inline G4double DetectorHit::GetEdep() const{
    return fEdep;
}
inline G4double DetectorHit::GetTrackLength() const{
    return fTrackLength;
}


#endif