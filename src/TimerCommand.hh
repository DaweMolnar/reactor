#ifndef REACTOR_TIMERCOMMAND_HEADER
#define REACTOR_TIMERCOMMAND_HEADER

#include "Command.hh"
#include "TimerEvent.hh"

typedef Command1<void, const TimerEvent &> TimerCommand;

#endif // REACTOR_TIMERCOMMAND_HEADER
