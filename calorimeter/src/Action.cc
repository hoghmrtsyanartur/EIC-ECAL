#include "Action.hh"
#include "PrimeryGenerator.hh"
#include "Run.hh"
#include "Event.hh"


Action::Action()
 : G4VUserActionInitialization()
{}


Action::~Action()
{}


void Action::BuildForMaster() const
{
  SetUserAction(new Run);
}


void Action::Build() const
{
  SetUserAction(new PrimaryGenerator);
  SetUserAction(new Run);
  SetUserAction(new Event);
}  