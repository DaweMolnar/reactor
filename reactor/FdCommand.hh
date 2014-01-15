#ifndef REACTOR_REACTOR_FDCOMMAND_HEADER
#define REACTOR_REACTOR_FDCOMMAND_HEADER

#include <reactor/FdEvent.hh>

#include <util/Command.hh>

namespace reactor {

typedef util::Command1<void, const FdEvent &> FdCommand;

} // namespace reactor

#endif // REACTOR_REACTOR_FDCOMMAND_HEADER
