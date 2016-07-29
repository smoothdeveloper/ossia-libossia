#include "Loop_impl.hpp"
#include <ossia/detail/algorithms.hpp>

namespace impl
{

JamomaLoop::JamomaLoop(time_value patternDuration,
                       time_constraint::ExecutionCallback patternConstraintCallback,
                       time_event::ExecutionCallback patternStartEventCallback,
                       time_event::ExecutionCallback patternEndEventCallback) :
JamomaTimeProcess(),
mPatternStartEventCallback(patternStartEventCallback),
mPatternEndEventCallback(patternEndEventCallback),
mPatternConstraintCallback(patternConstraintCallback)
{
  mPatternStartNode = time_node::create();
  mPatternStartNode->emplace(mPatternStartNode->timeEvents().begin(), [&] (time_event::Status result) { PatternStartEventCallback(result); });

  mPatternEndNode = time_node::create();
  mPatternEndNode->emplace(mPatternEndNode->timeEvents().begin(), [&] (time_event::Status result) { PatternEndEventCallback(result); });

  // create a pattern TimeConstraint with all durations equal by default
  mPatternConstraint = time_constraint::create([=] (time_value position, time_value date, const State& state) {
      return PatternConstraintCallback(position, date, state); },
                                              mPatternStartNode->timeEvents()[0],
                                              mPatternEndNode->timeEvents()[0],
                                              patternDuration,
                                              patternDuration,
                                              patternDuration);

  // set pattern TimeConstraint's clock in external mode
  mPatternConstraint->setDriveMode(clock::DriveMode::EXTERNAL);
}

JamomaLoop::~JamomaLoop()
{}

# pragma mark -
# pragma mark Execution

StateElement JamomaLoop::offset(time_value offset)
{
  if (parent->getRunning())
    throw std::runtime_error("parent time constraint is running");

  // reset internal mOffsetState
  mOffsetState.clear();

  time_value patternOffset = std::fmod((double)offset, (double)mPatternConstraint->getDurationNominal());
  flattenAndFilter(mOffsetState, mPatternConstraint->offset(patternOffset));

  // compile mOffsetState with all HAPPENED event's states
  if (mPatternConstraint->getStartEvent()->getStatus() == time_event::Status::HAPPENED)
    flattenAndFilter(mOffsetState, mPatternConstraint->getStartEvent()->getState());

  return mOffsetState;
}

StateElement JamomaLoop::state()
{
  if (!parent->getRunning())
    throw std::runtime_error("parent time constraint is not running");

  // if date hasn't been processed already
  time_value date = parent->getDate();
  if (date != mLastDate)
  {
    auto prev_last_date = mLastDate;
    mLastDate = date;

    // reset internal State
    mCurrentState.clear();

    // process the loop from the pattern start TimeNode
    ptr_container<time_event> statusChangedEvents;
    std::shared_ptr<JamomaTimeNode> n = std::dynamic_pointer_cast<JamomaTimeNode>(mPatternStartNode);
    n->process(statusChangedEvents);

    // add the state of each newly HAPPENED TimeEvent
    for (const auto& timeEvent : statusChangedEvents)
      if (timeEvent->getStatus() == time_event::Status::HAPPENED)
        flattenAndFilter(mCurrentState, timeEvent->getState());

    // make time flow for the pattern constraint
    if (mPatternConstraint->getRunning())
    {
      if (mPatternConstraint->getDriveMode() != clock::DriveMode::EXTERNAL)
        throw std::runtime_error("the pattern constraint clock is supposed to be in EXTERNAL drive mode");

      if (mPatternConstraint->getRunning())
      {
        // don't tick if the pattern constraint is starting to avoid double ticks
        auto& startEvent = mPatternConstraint->getStartEvent();
        bool not_starting = none_of(statusChangedEvents,
                                    [&] (const std::shared_ptr<time_event>& ev)
                                    {
                                      return ev->getStatus() == time_event::Status::HAPPENED && ev == startEvent;
                                    });

        if (not_starting)
        {
          // no such event found : not starting
            // no such event found : not starting
            if(prev_last_date == Infinite)
                mPatternConstraint->tick();
            else
                mPatternConstraint->tick((date - prev_last_date) * 1000.);
        }
        else
        {
            // TODO we should advance the loop a bit at least.
        }
      }

      // if the pattern constraint is still running after the tick
      if (mPatternConstraint->getRunning())
        flattenAndFilter(mCurrentState, mPatternConstraint->state());
    }

    // if the pattern end event happened : stop and reset the loop
    if (mPatternConstraint->getEndEvent()->getStatus() == time_event::Status::HAPPENED)
      stop();
  }

  //! \see mCurrentState is filled below in JamomaLoop::PatternConstraintCallback
  return mCurrentState;
}

# pragma mark -
# pragma mark Execution - Implementation specific

void JamomaLoop::start()
{}

void JamomaLoop::stop()
{
  mPatternConstraint->stop();

  mPatternConstraint->offset(Zero);

  std::shared_ptr<JamomaTimeEvent> start = std::dynamic_pointer_cast<JamomaTimeEvent>(mPatternConstraint->getStartEvent());
  start->setStatus(time_event::Status::PENDING);

  std::shared_ptr<JamomaTimeEvent> end = std::dynamic_pointer_cast<JamomaTimeEvent>(mPatternConstraint->getEndEvent());
  end->setStatus(time_event::Status::NONE);
}

void JamomaLoop::pause()
{
  mPatternConstraint->pause();
}

void JamomaLoop::resume()
{
  mPatternConstraint->resume();
}

# pragma mark -
# pragma mark Accessors

const std::shared_ptr<time_constraint> JamomaLoop::getPatternTimeConstraint() const
{
  return mPatternConstraint;
}

const std::shared_ptr<time_node> JamomaLoop::getPatternStartTimeNode() const
{
  return mPatternStartNode;
}

const std::shared_ptr<time_node> JamomaLoop::getPatternEndTimeNode() const
{
  return mPatternEndNode;
}

void JamomaLoop::PatternConstraintCallback(
    time_value position,
    time_value date,
    const State&)
{
  if (mPatternConstraintCallback)
  {
    // add the state of the pattern TimeConstraint
    flattenAndFilter(mCurrentState, mPatternConstraint->state());

    (mPatternConstraintCallback)(position, date, mCurrentState);
  }
}

void JamomaLoop::PatternStartEventCallback(time_event::Status newStatus)
{
  if (mPatternStartEventCallback)
    (mPatternStartEventCallback)(newStatus);
}

void JamomaLoop::PatternEndEventCallback(time_event::Status newStatus)
{
  if (mPatternEndEventCallback)
    (mPatternEndEventCallback)(newStatus);
}

}
