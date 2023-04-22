#pragma once
#include <glm\glm.hpp>
#include <map>
#include <vector>
#include "../Animation/Character.h"

enum StateType
{
	Idle,
	Pursue,
	Attack,
	Dead
};

class State
{
public:
	State(StateType type) : m_StateType(type) { }
	virtual ~State() { }
	virtual StateType GetType() const { return m_StateType; }
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual const char* C_Str() = 0;
	Character* pcharacter;
private:
	StateType m_StateType;
};

class IdleState : public State
{
public:
	IdleState(const glm::vec3& position, float radius);
	virtual ~IdleState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual const char* C_Str() override { return "IdleState"; }

private:
	glm::vec3 m_Position;
	float m_Radius;
};

class PursueState : public State
{
public:
	PursueState(unsigned int enemyId);
	virtual ~PursueState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual const char* C_Str() override { return "PursueState"; }

private:
	unsigned int m_EnemyId;
};

class AttackState : public State
{
public:
	AttackState(unsigned int enemyId);
	virtual ~AttackState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual const char* C_Str() override { return "AttackState"; }

private:
	unsigned int m_EnemyId;
};

class DeadState : public State
{
public:
	DeadState();
	virtual ~DeadState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual const char* C_Str() override { return "DeadState"; }

private:
	unsigned int m_EnemyId;
};

class stateMachine
{
public:
	stateMachine();
	~stateMachine();

	void initState();
	void AddTransition(StateType from, StateType to);
	State* GetCurrentState();
	void SetState(State* state);

	Character* pcharacter;
private:
	std::map<StateType, std::vector<StateType>> m_ValidTransitions;

	State* m_CurrentState;
};

