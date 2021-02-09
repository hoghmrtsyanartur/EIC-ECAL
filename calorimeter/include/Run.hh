#ifndef Run_h
#define Run_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class Run : public G4UserRunAction
{
  public:
    Run();
    virtual ~Run();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};

#endif