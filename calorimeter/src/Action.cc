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
  Event* event = new Event;
  SetUserAction(new Run(event));
}


void Action::Build() const
{
  SetUserAction(new PrimaryGenerator);
  auto event = new Event;
  SetUserAction(new Event);
  SetUserAction(new Run(event));
}  