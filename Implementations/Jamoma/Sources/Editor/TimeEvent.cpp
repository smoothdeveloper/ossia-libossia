#include "Editor/JamomaTimeEvent.h"

#include <algorithm>
#include <iostream> //! \todo to remove. only here for debug purpose

# pragma mark -
# pragma mark Life cycle

JamomaTimeEvent::JamomaTimeEvent(TimeEvent::ExecutionCallback callback,
                                 shared_ptr<TimeNode> aTimeNode,
                                 shared_ptr<Expression> anExpression) :
mCallback(callback),
mTimeNode(aTimeNode),
mExpression(anExpression),
mStatus(TimeEvent::Status::NONE),
mObserveExpression(false)
{
  mState = State::create();
}

JamomaTimeEvent::~JamomaTimeEvent()
{}

# pragma mark -
# pragma mark Execution

void JamomaTimeEvent::happen()
{
  if (mStatus != TimeEvent::Status::PENDING)
    throw runtime_error("only PENDING event can happens");
  
  mStatus = TimeEvent::Status::HAPPENED;
  
  // stop previous TimeConstraints
  for (auto& timeConstraint : previousTimeConstraints())
  {
    timeConstraint->stop();
  }
  
  // start next TimeConstraints
  for (auto& timeConstraint : nextTimeConstraints())
  {
    timeConstraint->start();
  }
  
  (mCallback)(mStatus);
}

void JamomaTimeEvent::dispose()
{
  if (mStatus != TimeEvent::Status::PENDING)
    throw runtime_error("only PENDING event can be disposed");
  
  mStatus = TimeEvent::Status::DISPOSED;
  
  (mCallback)(mStatus);
}

# pragma mark -
# pragma mark Edition

void JamomaTimeEvent::addState(const std::shared_ptr<State> state)
{
  // store a state if it is not already stored
  if (std::find(mState->stateElements().begin(),
           mState->stateElements().end(),
           state) == mState->stateElements().end())
  {
    mState->stateElements().push_back(state);
  }
}

void JamomaTimeEvent::removeState(const std::shared_ptr<State> state)
{
  mState->stateElements().erase(std::find(mState->stateElements().begin(), mState->stateElements().end(), state));
}

# pragma mark -
# pragma mark Accessors

const shared_ptr<TimeNode> & JamomaTimeEvent::getTimeNode() const
{
  return mTimeNode;
}

const shared_ptr<State> & JamomaTimeEvent::getState() const
{
  return mState;
}

const shared_ptr<Expression> & JamomaTimeEvent::getExpression() const
{
  return mExpression;
}

TimeEvent & JamomaTimeEvent::setExpression(const std::shared_ptr<Expression> expression)
{
  mExpression = expression;
  return *this;
}

TimeEvent::Status JamomaTimeEvent::getStatus() const
{
  return mStatus;
}

# pragma mark -
# pragma mark Implementation specific

void JamomaTimeEvent::setStatus(Status status)
{
  mStatus = status;
  (mCallback)(mStatus);
}

bool JamomaTimeEvent::getObserveExpression()
{
  return mObserveExpression;
}

void JamomaTimeEvent::setObserveExpression(bool observeExpression)
{
  if (observeExpression != mObserveExpression)
  {
    mObserveExpression = observeExpression;
    
    //! \todo active expression observation
  }
}
