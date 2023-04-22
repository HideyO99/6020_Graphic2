#pragma once
#include <glm\glm.hpp>
#include <map>
#include <vector>

enum StateType
{
	Idle,
	Pursue,
	Catch
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

class CatchState : public State
{
public:
	CatchState(unsigned int enemyId);
	virtual ~CatchState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual const char* C_Str() override { return "CatchState"; }

private:
	unsigned int m_EnemyId;
};

class stateMachine
{
public:
	stateMachine();
	~stateMachine();

	void AddTransition(StateType from, StateType to);
	State* GetCurrentState();
	void SetState(State* state);

private:
	std::map<StateType, std::vector<StateType>> m_ValidTransitions;

	State* m_CurrentState;
};

