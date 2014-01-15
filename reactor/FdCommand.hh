#ifndef REACTOR_REACTOR_FDCOMMAND_HEADER
#define REACTOR_REACTOR_FDCOMMAND_HEADER

#include <reactor/FdEvent.hh>

#include <util/Command.hh>

typedef util::Command1<void, const FdEvent &> FdCommand;

#endif // REACTOR_REACTOR_FDCOMMAND_HEADER
