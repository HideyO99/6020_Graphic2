#include "stateMachine.h"

IdleState::IdleState(const glm::vec3& position, float radius)
	:State(Idle)
{
	m_Position = position;
	m_Radius = radius;
}

IdleState::~IdleState()
{
}

void IdleState::OnEnter()
{
}

void IdleState::OnExit()
{
}

PursueState::PursueState(unsigned int enemyId)
	:State(Pursue)
{
	m_EnemyId = enemyId;
}

PursueState::~PursueState()
{
}

void PursueState::OnEnter()
{
}

void PursueState::OnExit()
{
}

CatchState::CatchState(unsigned int enemyId)
	:State(Catch)
{
	m_EnemyId = enemyId;
}

CatchState::~CatchState()
{
}

void CatchState::OnEnter()
{
}

void CatchState::OnExit()
{
}

stateMachine::stateMachine()
{
	m_CurrentState = nullptr;
	m_CurrentState = new IdleState(glm::vec3(), 0);
}

stateMachine::~stateMachine()
{
}

void stateMachine::AddTransition(StateType from, StateType to)
{
	std::vector<StateType>& stateVec = m_ValidTransitions[from];
	if (std::find(stateVec.begin(), stateVec.end(), to) == stateVec.end())
	{
		stateVec.push_back(to);
	}
}

State* stateMachine::GetCurrentState()
{
	return m_CurrentState;
}

void stateMachine::SetState(State* state)
{
	printf("StateMachine::SetState: %s\n", state->C_Str());
	if (m_CurrentState->GetType() == state->GetType())
	{
		printf(" Already the current state!\n");
		return;
	}
	std::vector<StateType>& stateVec = m_ValidTransitions[m_CurrentState->GetType()];
	if (std::find(stateVec.begin(), stateVec.end(), state->GetType()) == stateVec.end())
	{
		printf("!! Not a valid transition!\n");
		// No valid transition was added from the curren state to the new state
		return;
	}
	m_CurrentState->OnExit();
	delete m_CurrentState;

	m_CurrentState = state;
	m_CurrentState->OnEnter();
}
