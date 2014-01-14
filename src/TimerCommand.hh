#ifndef REACTOR_TIMERCOMMAND_HEADER
#define REACTOR_TIMERCOMMAND_HEADER

#include "TimerEvent.hh"

#include <util/Command.hh>

typedef Command1<void, const TimerEvent &> TimerCommand;

#endif // REACTOR_TIMERCOMMAND_HEADER
