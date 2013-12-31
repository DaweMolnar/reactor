#ifndef REACTOR_FDCOMMAND_HEADER
#define REACTOR_FDCOMMAND_HEADER

#include "Command.hh"
#include "FdEvent.hh"

typedef Command1<void, const FdEvent &> FdCommand;

#endif // REACTOR_FDCOMMAND_HEADER
