#ifndef REACTOR_REACTOR_TIMERCOMMAND_HEADER
#define REACTOR_REACTOR_TIMERCOMMAND_HEADER

#include <reactor/TimerEvent.hh>

#include <util/Command.hh>

typedef util::Command1<void, const TimerEvent &> TimerCommand;

#endif // REACTOR_REACTOR_TIMERCOMMAND_HEADER
