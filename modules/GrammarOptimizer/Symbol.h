#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "iostream"
#include "regex"

enum class SYMBOL_TYPE
{
	TERMINAL,
	NON_TERMINAL,
};

class Symbol
{
public:
	explicit Symbol(std::string value)
		: m_value(std::move(value))
	{
	}

	[[nodiscard]] std::string GetValue() const
	{
		return m_value;
	}

	[[nodiscard]] virtual SYMBOL_TYPE GetType() const = 0;

	bool operator==(const Symbol& rhs) const
	{
		return (GetType() == rhs.GetType()) && m_value == rhs.m_value;
	}

	virtual ~Symbol() = default;

private:
	std::string m_value;
};

using Rule = std::vector<std::shared_ptr<Symbol>>;
using Rules = std::vector<Rule>;

class NonTerminal : public Symbol
{
public:
	explicit NonTerminal(std::string value, const std::vector<Rule>& rules = {})
	: Symbol(std::move(value))
	{}

	void AddRule(const Rule& rule)
	{
		if (std::find(m_rules.begin(), m_rules.end(), rule) == m_rules.end())
		{
			m_rules.push_back(rule);
		}
	}

	void ClearRules()
	{
		m_rules.clear();
	}

	[[nodiscard]] Rules& GetRules()
	{
		return m_rules;
	}

	[[nodiscard]] SYMBOL_TYPE GetType() const override
	{
		return SYMBOL_TYPE::NON_TERMINAL;
	}

private:
	Rules m_rules;
};

class Terminal : public Symbol
{
public:
	explicit Terminal(std::string value)
		: Symbol(std::move(value))
	{}

	[[nodiscard]] SYMBOL_TYPE GetType() const override
	{
		return SYMBOL_TYPE::TERMINAL;
	}
};