#ifndef REACTOR_REACTOR_TIMERCOMMAND_HEADER
#define REACTOR_REACTOR_TIMERCOMMAND_HEADER

#include <reactor/TimerEvent.hh>

#include <util/Command.hh>

namespace reactor {

typedef util::Command1<void, const TimerEvent &> TimerCommand;

} // namespace reactor

#endif // REACTOR_REACTOR_TIMERCOMMAND_HEADER
