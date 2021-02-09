#ifndef Action_h
#define Action_h 1

#include "G4VUserActionInitialization.hh"



class Action : public G4VUserActionInitialization
{
  public:
    Action();
    virtual ~Action();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
