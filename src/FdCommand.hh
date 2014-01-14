#ifndef REACTOR_FDCOMMAND_HEADER
#define REACTOR_FDCOMMAND_HEADER

#include "FdEvent.hh"

#include <util/Command.hh>

typedef util::Command1<void, const FdEvent &> FdCommand;

#endif // REACTOR_FDCOMMAND_HEADER
