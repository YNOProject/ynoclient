/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GAME_BATTLER_H_
#define _GAME_BATTLER_H_

// Headers
#include <string>
#include <vector>
#include "rpg_state.h"
#include "system.h"

class Game_Actor;
class Game_Party_Base;

namespace Game_BattleAlgorithm {
	class AlgorithmBase;
}

typedef EASYRPG_SHARED_PTR<Game_BattleAlgorithm::AlgorithmBase> BattleAlgorithmRef;

/**
 * Game_Battler class.
 */
class Game_Battler {
public:
	/**
	 * Constructor.
	 */
	Game_Battler();

	/**
	 * Gets if battler has a state.
	 *
	 * @param state_id database state ID.
	 * @return whether the battler has the state.
	 */
	bool HasState(int state_id) const;

	/**
	 * Gets battler states.
	 *
	 * @return vector containing the IDs of all states the battler has.
	 */
	std::vector<int16_t> GetInflictedStates() const;

	/**
	 * Gets battler states.
	 * This returns the raw state list with not inflected states set to 0 and
	 * inflected states set to at least 1 (this maps to the turn count).
	 *
	 * @return vector containing state list
	 */
	virtual const std::vector<int16_t>& GetStates() const = 0;
	virtual std::vector<int16_t>& GetStates() = 0;

	/**
	 * Checks all states and returns the first restriction that different to
	 * normal or normal if that is the only restriction.
	 *
	 * @return First non-normal restriction or normal if not restricted
	 */
	int GetSignificantRestriction();

	/**
	 * Tests if the battler has a "No Action" condition like sleep.
	 *
	 * @return can act 
	 */
	bool CanAct();

	/**
	 * Gets current battler state with highest priority.
	 *
	 * @return the highest priority state affecting the battler.
	 *         Returns NULL if no states.
	 */
	const RPG::State* GetSignificantState();

	/**
	 * Gets the state probability by rate (A-E).
	 *
	 * @param state_id State to test
	 * @param rate State rate to get
	 * @return state rate (probability)
	 */
	int GetStateRate(int state_id, int rate);

	/**
	 * Gets probability that a state can be inflicted on this actor.
	 * 
	 * @param state_id State to test
	 * @return Probability of state infliction
	 */
	virtual int GetStateProbability(int state_id) = 0;

	/**
	 * Gets the characters name
	 *
	 * @return Character name
	 */
	virtual const std::string& GetName() const = 0;

	/**
	 * Gets the filename of the character sprite
	 *
	 * @return Filename of character sprite
	 */
	virtual const std::string& GetSpriteName() const = 0;

	/**
	 * Gets battler HP.
	 *
	 * @return current HP.
	 */
	virtual int GetHp() const = 0;

	/**
	 * Sets the current battler HP.
	 */
	virtual void SetHp(int hp) = 0;

	/**
	 * Increases/Decreases hp.
	 * Also handles death condition.
	 *
	 * @param hp relative hp change
	 */
	virtual void ChangeHp(int hp);

	/**
	 * Gets the battler max HP.
	 *
	 * @return current max HP.
	 */
	virtual int GetMaxHp() const;

	/**
	 * Gets if the battler has full hp.
	 *
	 * @return if battler is healty.
	 */
	virtual bool HasFullHp() const;

	/**
	 * Gets battler SP.
	 *
	 * @return current SP.
	 */
	virtual int GetSp() const = 0;

	/**
	 * Sets the current battler SP.
	 */
	virtual void SetSp(int _sp) = 0;

	/**
	 * Increases/Decreases sp.
	 *
	 * @param sp relative sp change
	 */
	virtual void ChangeSp(int sp);

	/**
	 * Gets the battler max SP.
	 *
	 * @return current max SP.
	 */
	virtual int GetMaxSp() const;

	/**
	 * Gets if the battler has full sp.
	 *
	 * @return if battler is full of magic.
	 */
	virtual bool HasFullSp() const;

	/**
	 * Gets the current battler attack.
	 *
	 * @return current attack.
	 */
	virtual int GetAtk() const;

	/**
	 * Gets the current battler defense.
	 *
	 * @return current defense.
	 */
	virtual int GetDef() const;

	/**
	 * Gets the current battler spirit.
	 *
	 * @return current spirit.
	 */
	virtual int GetSpi() const;

	/**
	 * Gets the current battler agility.
	 *
	 * @return current agility.
	 */
	virtual int GetAgi() const;

	/**
	 * Gets the maximum HP for the current level.
	 *
	 * @return max HP.
	 */
	virtual int GetBaseMaxHp() const = 0;

	/**
	 * Gets the maximum SP for the current level.
	 *
	 * @return max SP.
	 */
	virtual int GetBaseMaxSp() const= 0;

	/**
	 * Gets the attack for the current level.
	 *
	 * @return attack.
	 */
	virtual int GetBaseAtk() const = 0;

	/**
	 * Gets the defense for the current level.
	 *
	 * @return defense.
	 */
	virtual int GetBaseDef() const = 0;

	/**
	 * Gets the spirit for the current level.
	 *
	 * @return spirit.
	 */
	virtual int GetBaseSpi() const = 0;

	/**
	 * Gets the agility for the current level.
	 *
	 * @return agility.
	 */
	virtual int GetBaseAgi() const = 0;

	virtual bool IsHidden() const;
	virtual bool IsImmortal() const;

	virtual bool Exists() const;
	bool IsDead() const;

	/**
	 * Checks if the actor can use the skill.
	 *
	 * @param skill_id ID of skill to check.
	 * @return true if skill can be used.
	 */
	virtual bool IsSkillUsable(int skill_id) const;

	/**
	 * Applies the effects of an item.
	 * Tests if using that item makes any sense (e.g. for HP healing
	 * items if there are any HP to heal)
	 *
	 * @param item_id ID if item to use
	 * @return true if item affected anything
	 */
	virtual bool UseItem(int item_id);

	/**
	 * Applies the effects of a skill.
	 * Tests if using that skill makes any sense (e.g. for HP healing
	 * skills if there are any HP to heal)
	 * Does not reduce the MP, use Game_Party->UseSkill for this.
	 *
	 * @param skill_id ID of skill to use
	 * @return true if skill affected anything
	 */
	virtual bool UseSkill(int skill_id);

	/**
	 * Calculates the Skill costs including all modifiers.
	 *
	 * @param skill_id ID of skill to calculate.
	 * @return needed skill cost.
	 */
	int CalculateSkillCost(int skill_id) const;

	/**
	 * Sets the battler attack modifier.
	 *
	 * @param modifier attack modifier
	 */
	void SetAtkModifier(int modifier);

	/**
	 * Sets the battler defense modifier.
	 *
	 * @param modifier defense modifier.
	 */
	void SetDefModifier(int modifier);

	/**
	 * Sets the battler spirity modifier.
	 *
	 * @param modifier spirity modifier.
	 */
	void SetSpiModifier(int modifier);

	/**
	 * Sets the battler agility modifier.
	 *
	 * @param current attack.
	 */
	void SetAgiModifier(int modifier);

	/**
	 * Adds a State.
	 *
	 * @param state_id ID of state to add.
	 */
	virtual void AddState(int state_id);

	/**
	 * Removes a State.
	 *
	 * @param state_id ID of state to remove.
	 */
	virtual void RemoveState(int state_id);

	/**
	 * Removes all states which end after battle.
	 */
	virtual void RemoveBattleStates();

	/**
	 * Removes all states.
	 */
	virtual void RemoveAllStates();
	
	/**
	 * Gets X position on battlefield
	 *
	 * @return X position in battle scene
	 */
	virtual int GetBattleX() const = 0;

	/**
	 * Gets Y position on battlefield
	 *
	 * @return Y position in battle scene
	 */
	virtual int GetBattleY() const = 0;

	virtual int GetHue() const;

	virtual int GetBattleAnimationId() const = 0;

	virtual int GetHitChance() const = 0;

	virtual int GetCriticalHitChance() const = 0;

	/**
	 * @return If battler is charged (next attack double damages)
	 */
	bool IsCharged() const;

	/**
	 * Sets charge state (next attack double damages)
	 *
	 * @param charge new charge state
	 */
	void SetCharged(bool charge);

	/**
	* @return If battler is defending (next turn, defense is doubled)
	*/
	bool IsDefending() const;

	/**
	 * Sets defence state (next turn, defense is doubled)
	 *
	 * @param charge new charge state
	 */
	void SetDefending(bool defend);

	enum BattlerType {
		Type_Ally,
		Type_Enemy
	};

	virtual BattlerType GetType() const = 0;

	/**
	 * Convenience function to access the party based on the type of this
	 * battler. This function does not ensure that the battler is in the
	 * party.
	 * @return Party this member probably belongs to. 
	 */
	Game_Party_Base& GetParty() const;

	/**
	 * Gets the maximal gauge value.
 	 */
	int GetMaxGauge() const;

	/**
	 * Gets the current state of the battle gauge in percent.
	 * Used by RPG2k3 battle system.
	 *
	 * @return gauge in percent
	 */
	int GetGauge() const;

	/**
	 * Sets the gauge to a new percentage in range 0-100
	 * Used by RPG2k3 battle system.
	 *
	 * @param new_gauge new gauge value in percent
	 */
	void SetGauge(int new_gauge);

	/**
	 * Increments the gauge by current agi.
	 * The size of the step is altered by the multiplier (usually based on
	 * the highest agi of all battlers)
	 * Used by RPG2k3 battle system.
	 *
	 * @param multiplier gauge increment factor
	 */
	void UpdateGauge(int multiplier);

	/**
	 * Tests if the battler is ready for an action.
	 * Used by RPG2k3 battle system.
	 *
	 * @return If gauge is full
	 */
	bool IsGaugeFull() const;

	/**
	* Gets the current BattleAlgorithm (action to execute in battle)

	* @return Current algorithm or NULL if none
	*/
	const BattleAlgorithmRef GetBattleAlgorithm() const;

	/**
	 * Assigns a new battle algorithm (action to execute in battle) to the
	 * battler.
	 *
	 * @param battle_algorithm New algorithm to assign
	 */
	void SetBattleAlgorithm(const BattleAlgorithmRef battle_algorithm);

	/** 
	 * @return Current turn in battle
	 */
	int GetBattleTurn() const;

	/**
	 * Increases the turn counter of the actor and heals states that reached
	 * the required numbers of turns.
	 *
	 * @return Healed states
	 */
	std::vector<int16_t> NextBattleTurn();

	void SetLastBattleAction(int battle_action);

	int GetLastBattleAction() const;

	/**
	 * Initializes battle related data to there default values.
	 */
	void ResetBattle();

protected:
	/** Gauge for RPG2k3 Battle */
	int gauge;

	/** Battle action for next turn */
	BattleAlgorithmRef battle_algorithm;

	bool charged;
	bool defending;
	int atk_modifier;
	int def_modifier;
	int spi_modifier;
	int agi_modifier;
	int battle_turn;
	int last_battle_action;
};

#endif
