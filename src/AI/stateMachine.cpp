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
	pcharacter->LoopEnable();
	pcharacter->SetAnimation(0);
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
	pcharacter->LoopEnable();
	pcharacter->SetAnimation(1);
}

void PursueState::OnExit()
{
}

AttackState::AttackState(unsigned int enemyId)
	:State(Attack)
{
	m_EnemyId = enemyId;
}

AttackState::~AttackState()
{
	
}

void AttackState::OnEnter()
{
	pcharacter->LoopEnable();
	pcharacter->SetAnimation(2);
}

void AttackState::OnExit()
{
}

DeadState::DeadState()
	:State(Dead)
{
	
}

DeadState::~DeadState()
{
}

void DeadState::OnEnter()
{
	pcharacter->LoopDisable();
	pcharacter->SetAnimation(3);
}

void DeadState::OnExit()
{
}
stateMachine::stateMachine()
{
	m_CurrentState = nullptr;
	pcharacter = nullptr;
	//m_CurrentState = new IdleState(glm::vec3(), 0);
	initState();
}

stateMachine::~stateMachine()
{
}

void stateMachine::initState()
{
	AddTransition(Idle, Pursue);
	AddTransition(Pursue, Idle);
	AddTransition(Pursue, Attack);
	AddTransition(Attack, Idle);
	AddTransition(Idle, Dead);
	AddTransition(Pursue, Dead);
	AddTransition(Attack, Dead);
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
	state->pcharacter = this->pcharacter;
	printf("StateMachine::SetState: %s\n", state->C_Str());
	if (m_CurrentState != nullptr)
	{
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
	}
	m_CurrentState = state;
	state->pcharacter = this->pcharacter;
	m_CurrentState->OnEnter();
}

