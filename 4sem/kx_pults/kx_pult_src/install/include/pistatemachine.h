/*! \file pistatemachine.h
 * \brief Base class for custom state machine
*/
/*
	PIP - Platform Independent Primitives
	State machine
	Copyright (C) 2016  Ivan Pelipenko peri4ko@yandex.ru

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PISTATEMACHINE_H
#define PISTATEMACHINE_H

#include "piobject.h"

/*! \brief Base class for custom state machine
 *
 * \section PIStateMachine_synopsis Synopsis
 * This class provide functionality of state machine.
 * You should inherit from this class, implement \a execution()
 * and \a transition() functions, set rules and periodically
 * call \a tick() function to proper work of machine.
 *
 * \section PIStateMachine_prepare Prepare for work
 * %State machine operates with "state", "rule" and "condition".
 * * "State" is some class (by default \c int), associated name and
 * optional "handler" - pointer to function executed on every \a tick();
 * * "Rule" define rule of transition from one machine state to other.
 * It is also has optional "handler";
 * * "Condition" is a part of rule and define possibility of transition.
 *
 * First of all you should define states of your machine by function
 * \a addState(). Then you should define transition rules for machine
 * by function \a addRule(). Finally you can set initial state by function
 * \a setInitialState() and provide periodically execution of function
 * \a tick().
 *
 * \section PIStateMachine_principle Principle of work
 * At any time the state machine is in some state. You can ask machine
 * to enter in new state by function \a switchToState(). If all conditions
 * done machine switch it state immediately, else machine remember request
 * and will be try switch to the new state every tick. Successfull state
 * switching execute function \a transition(), every tick execute
 * function \a execution() with current state. On successfull transition
 * if rule "handler" is not null it execute. Every \a tick() if current
 * state "handler" is not null it execute.
 *
 * \section PIStateMachine_conditions Conditions
 * Each rule has transition condition. Condition is array of pairs
 * (string, number). It means that every condition by name "string"
 * should be performed as least "number" times. Empty condition always
 * permits transition.
 *
 * %State machine have current performed conditions. You can read this
 * conditions by function \a currentConditions() and perform new
 * conditions by functions \a performCondition() and \a performConditions().
 * Currend conditions can de erased by function \a resetConditions().
 *
 * \section PIStateMachine_example Example
 * This is simple example demonstrates all features:
 * \snippet pistatemachine.cpp main
*/
template <typename Type = int>
class PIP_EXPORT PIStateMachine: public PIObject
{
	PIOBJECT_SUBCLASS(PIStateMachine, PIObject)
public:
	//! Constructs an empty state machine
	PIStateMachine(void * _parent = 0) {if (_parent == 0) parent_ = this; else parent_ = _parent; resetConditions();}
	~PIStateMachine() {;}

	//! %Condition is a pair (string, number)
	typedef PIPair<PIString, int> Condition;

	//! %Rule of transition between states of machine
	struct Rule {
		//! Constuctor
				Rule() {handler = 0; from = to = Type(); autoTransition = resetAllConditions = false;}
		//! Constuctor
		Rule(Type f, Type t, const PIStringList & c = PIStringList(), Handler h = 0, bool at = false, bool rac = false) {
			from = f;
			to = t;
			for (int i = 0; i < c.size_s(); ++i)
				conditions << Condition(c[i], 1);
			autoTransition = at;
			resetAllConditions = rac;
			handler = h;
		}
		//! Source state
		Type from;
		//! Destination state
		Type to;
		//! %Conditions of transition
		PIVector<Condition> conditions;
		//! Automatic transition
		bool autoTransition;
		//! Reset or not all performed conditions of machine on transition
		bool resetAllConditions;
		//! Pointer to function executed on transition
		Handler handler;
		//! Add condition of transition
		void addCondition(const PIString & name, int times = 1) {if (times > 0) conditions << Condition(name, times);}
		bool operator ==(const Rule & other) const {return (from == other.from) && (to == other.to);}
		bool operator !=(const Rule & other) const {return (from != other.from) || (to != other.to);}
	};

	//! %State of machine
	struct State {
		//! Constuctor
				State() {handler = 0; value = Type();}
		//! Constuctor
		State(Type v, const PIString & n = "", Handler h = 0) {value = v; name = n; handler = h;}
		//! %State value
		Type value;
		//! %State name
		PIString name;
		//! Pointer to function executed on tick
		Handler handler;
		bool operator ==(const State & other) const {return value == other.value;}
		bool operator !=(const State & other) const {return value != other.value;}
	};

	void * parent() const {return parent_;}
	void setParent(void * parent) {parent_ = parent;}

	//! Add state of machine
	void addState(Type value, const PIString & name = "", Handler handler = 0) {if (states_.contains(State(value, name))) return; states_ << State(value, name, handler);}

	//! States count
	int statesCount() const {return states_.size_s();}

	//! Remove all states
	void clearStates() {states_.clear();}


	//! Add rule of transition
	void addRule(Type from, Type to, const PIString & condition, Handler handler = 0, bool autoTransition = false, bool resetAllConditions = false) {if (rules_.contains(Rule(from, to))) return; rules_ << Rule(from, to, PIStringList(condition), handler, autoTransition, resetAllConditions);}

	//! Add rule of transition
	void addRule(Type from, Type to, Handler handler, bool autoTransition = false, bool resetAllConditions = false) {if (rules_.contains(Rule(from, to))) return; rules_ << Rule(from, to, PIStringList(), handler, autoTransition, resetAllConditions);}

	//! Add rule of transition
	void addRule(Type from, Type to, const PIStringList & conditions = PIStringList(), Handler handler = 0, bool autoTransition = false, bool resetAllConditions = false) {if (rules_.contains(Rule(from, to))) return; rules_ << Rule(from, to, conditions, handler, autoTransition, resetAllConditions);}

	//! Add rule of transition
	void addRule(const Rule & rule) {if (rules_.contains(rule)) return; rules_ << rule;}

	//! Rules count
	int rulesCount() const {return rules_.size_s();}

	//! Remove all rules
	void clearRules() {rules_.clear();}


	//! Setup initial state. \a reset() will set machine state to "value"
	void setInitialState(Type value) {
		for (int i = 0; i < states_.size_s(); ++i)
			if (states_[i].value == value) {
				init_ = state_ = states_[i];
				return;
			}
	}

	/** \brief Try to switch machine state to state "to"
	 * \details If there is rule of transition exists and this rule conditions
	 * is performed then machine switched to new state immediately. Otherwise machine
	 * will be try to enter to new state every \a tick().
	 * \return \c true if state switched immediately, otherwise \c false */
	bool switchToState(Type to) {
		switch_to = to;
		for (int i = 0; i < rules_.size_s(); ++i) {
			Rule & r(rules_[i]);
			if ((r.from != state_.value) || (r.to != to)) continue;
			if (!checkConditions(r)) continue;
			State ts = findState(to);
			if (r.handler != 0 && parent_ != 0) r.handler(parent_);
			transition(state_, ts);
			state_ = ts;
			resetConditions(r);
			return true;
		}
		return false;
	}

	//! Reset machine state to initial and clear all conditions
	void reset() {state_ = init_; resetConditions();}

	//! Returns current state of machine
	const State & currentState() const {return state_;}


	//! Reset all performed conditions
	void resetConditions() {cond.clear(); cond_count = 0;}

	//! Reset performed condition with name "name"
	void resetCondition(const PIString & name) {
		for (int i = 0; i < cond.size_s(); ++i)
			if (cond[i].first == name) {
				cond.remove(i);
				i--;
			}
	}

	//! Perform condition with name "name" "times" times.
	void performCondition(const PIString & name, int times = 1) {
		if (times <= 0) return;
		for (int i = 0; i < cond.size_s(); ++i)
			if (cond[i].first == name) {
				cond[i].second += times;
				return;
			}
		cond << Condition(name, times);
	}

	//! Perform every condition with name from "names" one time.
	void performConditions(const PIStringList & names) {
		bool ok;
		for (int n = 0; n < names.size_s(); ++n) {
			ok = false;
			for (int i = 0; i < cond.size_s(); ++i) {
				if (cond[i].first == names[n]) {
					cond[i].second++;
					ok = true;
					break;
				}
			}
			if (ok) continue;
			cond << Condition(names[n], 1);
		}
	}

	//! Returns all current performed conditions
	const PIVector<Condition> & currentConditions() const {return cond;}

	Type * currentState_ptr() {return &state_.value;}
	int * conditionsCount_ptr() {cond_count = cond.size_s(); return &cond_count;}

//! \handlers
//! \{

	//! \fn void tick()
	//! \brief Main function of machine. Execute \a execution() and check if need to switch state

	//! \fn void tick(void * data, int delim)
	//! \brief Main function of machine. Execute \a execution() and check if need to switch state

//! \}

	EVENT_HANDLER(void, tick) {tick(0, 0);}
	EVENT_HANDLER2(void, tick, void * , data, int, delim) {
		execution(state_);
		if (state_.handler != 0 && parent_ != 0) state_.handler(parent_);
		if (switch_to != state_.value) switchToState(switch_to);
		else {
			piForeachC (Rule & r, rules_) {
				if (!r.autoTransition || r.from != state_.value) continue;
				if (checkConditions(r)) {
					switchToState(r.to);
					break;
				}
			}
		}
	}

protected:

	//! Reimplement this function to process current state of machine
	virtual void execution(const State & state) {;}

	//! Reimplement this function to process switching current state of machine
	virtual void transition(const State & from, const State & to) {;}

private:
	State findState(Type value) {
		for (int i = 0; i < states_.size_s(); ++i)
			if (states_[i].value == value)
				return states_[i];
		return State();
	}
	bool checkConditions(const Rule & rule) {
		//if (cond.size_s() < rule.conditions.size_s()) return false;
		int oc = 0;
		for (int i = 0; i < cond.size_s(); ++i) {
			PIString & rn(cond[i].first);
			for (int j = 0; j < rule.conditions.size_s(); ++j) {
				if (rn != rule.conditions[j].first) continue;
				if (cond[i].second < rule.conditions[j].second) return false;
				oc++;
			}
		}
		return (rule.conditions.size_s() == oc);
	}
	void resetConditions(const Rule & rule) {
		if (rule.resetAllConditions) {
			cond.clear();
			return;
		}
		for (int i = 0; i < cond.size_s(); ++i) {
			PIString & rn(cond[i].first);
			for (int j = 0; j < rule.conditions.size_s(); ++j) {
				if (rn != rule.conditions[j].first) continue;
				cond[i].second -= rule.conditions[j].second;
				if (cond[i].second <= 0) {
					cond.remove(i);
					i--;
				}
			}
		}
	}

	PIVector<State> states_;
	PIVector<Rule> rules_;
	State init_, state_;
	Type switch_to;
	void * parent_;
	int cond_count;
	PIVector<Condition> cond;

};


#endif // PISTATEMACHINE_H
