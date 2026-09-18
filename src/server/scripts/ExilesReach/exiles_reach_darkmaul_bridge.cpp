/*
 * Copyright (c) Fluxurion - 2023
 *
 * Begin in 2021, rewritten in 2022, re-rewritten in 2023.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

 /*
 @TODO: Use conversations instead of scheduled talk
 */

#include "exiles_reach.h"

class exiles_reach_darkmaul_bridge_playerscript : public PlayerScript
{
public:
    exiles_reach_darkmaul_bridge_playerscript() : PlayerScript("exiles_reach_darkmaul_bridge_playerscript") {}
    
    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_DARKMAUL_BRIDGE:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_CITADEL_GORGROTH_AND_FRIENDS }, player->HasAura(SPELL_SUMMON_PRISONERS_H));

                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_BRIDGE_HERBERT_CRENNA_SHUJA_BREKA_STANDING }, !player->HasAura(SPELL_SUMMON_PRISONERS_H) && player->GetQuestStatus(QUEST_H_TO_DARKMAUL_CITADEL) && !player->GetSummonedCreatureByEntry(NPC_OGRE_RUINS_BREKA_TO_DARKMAUL_CITADEL) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_BREKA));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_BRIDGE_15492 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_BRIDGE_15493 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_BRIDGE_15447 }, true);
                }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_BRIDGE_GARRICK_HENRY_MEREDY_RALIA_STANDING }, !player->HasAura(SPELL_SUMMON_PRISONERS_A) && player->GetQuestStatus(QUEST_TO_DARKMAUL_CITADEL) && !player->GetSummonedCreatureByEntry(NPC_OGRE_RUINS_TORGOK_FIGHTER_A) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_GARRICK));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_BRIDGE_13419 }, !player->HasAura(SPELL_OGRE_TRANSFORM));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_BRIDGE_13568 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_BRIDGE_13840 }, !player->HasAura(SPELL_OGRE_TRANSFORM));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_BRIDGE_14847 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_BRIDGE_15275 }, true);
                }
            }
            break;
            }
        }
    }

    void OnQuestStatusChange(Player* player, uint32 questId) override
    {
        CheckExilesReachPhaseShift(player);

        // Remove summons if player abandons or rewards a quest
        if (player->GetMapId() == MAP_NPE)
        {
            if (player->GetQuestStatus(questId) == QUEST_STATUS_NONE || player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
            {
                std::vector<uint32> activeSummons;
                switch (questId)
                {
                case QUEST_H_RIGHT_BENEATH_THEIR_EYES:
                    activeSummons.push_back(NPC_DARKMAUL_BRIDGE_CRENNA);
                    activeSummons.push_back(NPC_DARKMAUL_BRIDGE_HERBERT);
                    activeSummons.push_back(NPC_DARKMAUL_BRIDGE_SHUJA);
                    activeSummons.push_back(NPC_DARKMAUL_BRIDGE_BREKA);        
                    if (player->HasAura(SPELL_SUMMON_PRISONERS_H))
                    {
                        player->RemoveAura(SPELL_SUMMON_PRISONERS_H);
                        player->SetObjectScale(1.f);
                    }
                    break;
                case QUEST_RIGHT_BENEATH_THEIR_EYES:
                    activeSummons.push_back(NPC_DARKMAUL_BRIDGE_MEREDY);
                    activeSummons.push_back(NPC_DARKMAUL_BRIDGE_RALIA);
                    activeSummons.push_back(NPC_DARKMAUL_BRIDGE_HENRY);
                    activeSummons.push_back(NPC_DARKMAUL_BRIDGE_GARRICK);
                    if (player->HasAura(SPELL_SUMMON_PRISONERS_A))
                    {
                        player->RemoveAura(SPELL_SUMMON_PRISONERS_A);
                        player->SetObjectScale(1.f);
                    }
                    if (player->HasAura(SPELL_OGRE_DISGUISE))
                    {
                        player->RemoveAura(SPELL_OGRE_DISGUISE);
                    }
                    break;
                }

                for (uint32 entry : activeSummons)
                    if (player->GetSummonedCreatureByEntry(entry))
                        player->UnsummonCreatureByEntry(entry);
            }
        }
    }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        if (player->HasAura(SPELL_SUMMON_PRISONERS_H))
        {
            player->RemoveAura(SPELL_SUMMON_PRISONERS_H);
            player->SetObjectScale(1.f);
        }

        if (player->HasAura(SPELL_SUMMON_PRISONERS_A))
        {
            player->RemoveAura(SPELL_SUMMON_PRISONERS_A);
            player->SetObjectScale(1.f);
        }

        if (player->HasAura(SPELL_OGRE_DISGUISE))
        {
            player->RemoveAura(SPELL_OGRE_DISGUISE);
        }
    }

    // Phase Update in every 1sec
    void OnPlayerWorldObjectUpdate(Player* player, uint32 diff) override
    {
        if (timer <= diff)
        {
            CheckExilesReachPhaseShift(player);

            timer = 1000;
        }
        else
            timer -= diff;
    }

private:
    uint32 timer = 1000;
};

/* ## NPC SCRIPTS > ## */

// Warlord Breka Grimaxe, Captain Garrick (Darkmaul Bridge)
struct npc_exiles_reach_darkmaul_bridge_167596_156941 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_bridge_167596_156941(Creature* creature) : ScriptedAI(creature) {}

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        // At horde side the actors have waypoint until the bushes at darkmaul bridge.
        if (quest->GetQuestId() == QUEST_H_TO_DARKMAUL_CITADEL)
        {
            Talk(0, player);
            // Recruit, we push forward! The rest of you, hold this position.
            
            if (Creature* clone = player->SummonCreature(me->GetEntry(), me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
            {
                clone->m_Events.AddEventAtOffset(new DelayedMovePathEvent(player, clone, ToDarkMaulCitadelPathBrekaSize, ToDarkMaulCitadelPathBreka, ToDarkMaulCitadelPathBrekaSize, false, false), urand(3, 7) * 1s);
            }

            if (Creature* herbert = player->FindNearestCreature(NPC_HARPY_ROOST_HERBERT_AFTER_RITUAL, 50.f))
            {
                if (Creature* clone = player->SummonCreature(herbert->GetEntry(), herbert->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                {
                    clone->m_Events.AddEventAtOffset(new DelayedMovePathEvent(player, clone, ToDarkMaulCitadelPathHerbertSize, ToDarkMaulCitadelPathHerbert, ToDarkMaulCitadelPathHerbertSize, false, false), urand(3, 7) * 1s);
                }
            }

            if (Creature* crenna = player->FindNearestCreature(NPC_OGRE_RUINS_CRENNA_CAMP, 50.f))
            {
                if (Creature* clone = player->SummonCreature(crenna->GetEntry(), crenna->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                {
                    clone->m_Events.AddEventAtOffset(new DelayedMovePathEvent(player, clone, ToDarkMaulCitadelPathCrennaSize, ToDarkMaulCitadelPathCrenna, ToDarkMaulCitadelPathCrennaSize, false, false), urand(3, 7) * 1s);
                }
            }

            if (Creature* shuja = player->FindNearestCreature(NPC_OGRE_RUINS_SHUJA_CAMP, 50.f))
            {
                if (Creature* clone = player->SummonCreature(shuja->GetEntry(), shuja->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                {
                    clone->m_Events.AddEventAtOffset(new DelayedMovePathEvent(player, clone, ToDarkMaulCitadelPathShujaSize, ToDarkMaulCitadelPathShuja, ToDarkMaulCitadelPathShujaSize, false, false), urand(3, 7) * 1s);
                }
            }
        }
        
        if (quest->GetQuestId() == QUEST_H_RIGHT_BENEATH_THEIR_EYES)
        {
            if (Creature* breka = player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_BREKA, 50.f))
            {
                player->SummonCreature(breka->GetEntry(), breka->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            }

            if (Creature* shuja = player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_SHUJA, 50.f))
            {
                player->SummonCreature(shuja->GetEntry(), shuja->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            }

            if (Creature* herbert = player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_HERBERT, 50.f))
            {
                player->SummonCreature(herbert->GetEntry(), herbert->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            }

            if (Creature* crenna = player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_CRENNA, 50.f))
            {
                player->SummonCreature(crenna->GetEntry(), crenna->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            }
        }

        if (quest->GetQuestId() == QUEST_RIGHT_BENEATH_THEIR_EYES)
        {
            if (Creature* garrick = player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_GARRICK, 50.f))
            {
                player->SummonCreature(garrick->GetEntry(), garrick->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            }

            if (Creature* henry = player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_HENRY, 50.f))
            {
                player->SummonCreature(henry->GetEntry(), henry->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            }

            if (Creature* ralia = player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_RALIA, 50.f))
            {
                player->SummonCreature(ralia->GetEntry(), ralia->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            }

            if (Creature* meredy = player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_MEREDY, 50.f))
            {
                player->SummonCreature(meredy->GetEntry(), meredy->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            }
        }

        me->CastSpell(player, SPELL_UPDATE_PHASESHIFT);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != EFFECT_MOTION_TYPE)
            return;

        if (id == ToDarkMaulCitadelPathBrekaSize)            
            me->SetNpcFlag(UNIT_NPC_FLAG_QUESTGIVER);  
    }

    void IsSummonedBy(WorldObject* summoner) override
    {
        me->CastSpell(summoner, SPELL_UPDATE_PHASESHIFT, true);
        me->RemoveNpcFlag(UNIT_NPC_FLAG_QUESTGIVER);
    }
};

// Herbert Gloomburst (Darkmaul Bridge)
struct npc_exiles_reach_darkmaul_bridge_167598 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_bridge_167598(Creature* creature) : ScriptedAI(creature) {}

    bool OnGossipSelect(Player* player, uint32 /*sender*/, uint32 /*action*/) override
    {
        if (!QuestAccepted(player, QUEST_H_RIGHT_BENEATH_THEIR_EYES))
            return false;

        Talk(0, player);
        
        player->KilledMonsterCredit(me->GetEntry());

        me->CastSpell(player, SPELL_OGRE_TRANSFORMATION_HERBERT);

        CloseGossipMenuFor(player);

        return true;
    }

    void IsSummonedBy(WorldObject* /*summoner*/) override
    {
        me->SetNpcFlag(UNIT_NPC_FLAG_GOSSIP);
    }
};

// Meredy Huntswell (Darkmaul Bridge)
struct npc_exiles_reach_darkmaul_bridge_156943 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_bridge_156943(Creature* creature) : ScriptedAI(creature) {}

    // Meredy Huntswell
    bool OnGossipSelect(Player* player, uint32 /*sender*/, uint32 /*action*/) override
    {
        if (!QuestAccepted(player, QUEST_RIGHT_BENEATH_THEIR_EYES))
            return false;

        Talk(0, player);

        player->KilledMonsterCredit(me->GetEntry());

        me->CastSpell(player, SPELL_OGRE_TRANSFORMATION_HERBERT);

        CloseGossipMenuFor(player);

        return true;
    }

    void IsSummonedBy(WorldObject* /*summoner*/) override
    {
        me->SetNpcFlag(UNIT_NPC_FLAG_GOSSIP);
    }
};

// Chained Prisoners (Darkmaul Bridge) (167624, 167627, 167626, 167625, 153566, 156947, 156948, 153565)
struct npc_exiles_reach_darkmaul_bridge_prisoners : public ScriptedAI
{
    npc_exiles_reach_darkmaul_bridge_prisoners(Creature* creature) : ScriptedAI(creature) {}

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* player = summoner->ToPlayer();
        if (!player)
            return;

        me->SetOwnerGUID(player->GetGUID());
        me->FollowTarget(player);
        
        me->SetSpeed(MOVE_RUN, player->GetSpeed(MOVE_RUN));

        if (player->IsInHorde())
            me->SetSpeed(MOVE_RUN, player->GetSpeed(MOVE_RUN) * 1.2f);

        me->CastSpell(player, SPELL_UPDATE_PHASESHIFT, true);
        me->CastSpell(player, SPELL_CHAINED, true);

        if (Creature* crenna = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_CRENNA))
            crenna->DespawnOrUnsummon();

        if (Creature* herbert = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_HERBERT))
            herbert->DespawnOrUnsummon();

        if (Creature* shuja = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_SHUJA))
            shuja->DespawnOrUnsummon();

        if (Creature* breka = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_BREKA))
            breka->DespawnOrUnsummon();

        if (Creature* meredy = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_MEREDY))
            meredy->DespawnOrUnsummon();

        if (Creature* ralia = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_RALIA))
            ralia->DespawnOrUnsummon();

        if (Creature* henry = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_HENRY))
            henry->DespawnOrUnsummon();

        if (Creature* garrick = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_GARRICK))
            garrick->DespawnOrUnsummon();
    }
};

/* ## MOB SCRIPTS > ## */

// Killclaw the Terrible (Killclaw's Lair)
struct npc_exiles_reach_killclaw_153266 : public ScriptedAI
{
    npc_exiles_reach_killclaw_153266(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _events.Reset();

        _events.ScheduleEvent(EVENT_SEVERING_SWIPE, 6s);
        _events.ScheduleEvent(EVENT_TRAMPLING_CHARGE, 9s);
        _events.ScheduleEvent(EVENT_CLAW, 14s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_SEVERING_SWIPE:
                DoCastVictim(SPELL_SEVERING_SWIPE);
                _events.RescheduleEvent(EVENT_SEVERING_SWIPE, 6s);
                break;
            case EVENT_TRAMPLING_CHARGE:
            {
                Unit* victim = me->GetVictim();
                if (!victim)
                    return;

                me->CastSpell(me, SPELL_TRAMPLING_CHARGE);
                _events.RescheduleEvent(EVENT_TRAMPLING_CHARGE, 14s);
            }
                break;
            case EVENT_CLAW:
                DoCastVictim(SPELL_CLAW);
                _events.RescheduleEvent(EVENT_CLAW, 14s);
                break;
            default:
                break;
            }
        }
    }

private:
    EventMap _events;
    enum
    {
        EVENT_SEVERING_SWIPE = 1,
        EVENT_CLAW,
        EVENT_TRAMPLING_CHARGE
    };
};

/* ## SPELL SCRIPTS > ## */
// ID - 210131 Trampling Charge
class spell_trampling_charge_210131 : public SpellScriptLoader
{
public:
    spell_trampling_charge_210131() : SpellScriptLoader("spell_trampling_charge_210131") {}

    class spell_trampling_charge_210131_SpellScript : public SpellScript
    {
        void SetTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;
            
            Position chargeDest = caster->GetFirstCollisionPosition(40.f, 0.f);

            targets.clear();
            std::list<Unit*> targetList;
            caster->GetAttackableUnitListInRange(targetList, 40.f);
            targetList.remove_if([caster, chargeDest](Unit* target)
                {
                    return !target->IsInBetween(caster->GetPosition(), chargeDest, 2.f);
                });

            for (Unit* target : targetList)
                targets.push_back(target);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_trampling_charge_210131_SpellScript::SetTargets, EFFECT_0, TARGET_UNIT_RECT_CASTER_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_trampling_charge_210131_SpellScript::SetTargets, EFFECT_1, TARGET_UNIT_RECT_CASTER_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_trampling_charge_210131_SpellScript();
    }
};

// ID - 313583 Ogre Transformation
class CrennaRaliaCastEvent : public BasicEvent
{
public:
    CrennaRaliaCastEvent(Player* player) : _player(player) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {

        if (Creature* crenna = _player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_CRENNA, 50.f))
        {
            crenna->CastSpell(_player, SPELL_OGRE_TRANSFORMATION_CRENNA);
        }

        if (Creature* ralia = _player->FindNearestCreature(NPC_DARKMAUL_BRIDGE_RALIA, 50.f))
        {
            ralia->CastSpell(_player, SPELL_OGRE_TRANSFORMATION_CRENNA);
        }

        return true;
    }

private:
    Player* _player;
};

class spell_ogre_transformation_313583 : public SpellScriptLoader
{
public:
    spell_ogre_transformation_313583() : SpellScriptLoader("spell_ogre_transformation_313583") {}

    class spell_ogre_transformation_313583_AuraScript : public AuraScript
    {
        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            Player* player = target->ToPlayer();
            if (!caster || !target || !player)
                return;

            player->m_Events.AddEventAtOffset(new CrennaRaliaCastEvent(player), 500ms);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            Player* player = target->ToPlayer();
            if (!caster || !target || !player)
                return;

            if (player->IsInAlliance())
            {
                uint32 activeSummons[4] = { NPC_DARKMAUL_BRIDGE_MEREDY, NPC_DARKMAUL_BRIDGE_RALIA, NPC_DARKMAUL_BRIDGE_HENRY, NPC_DARKMAUL_BRIDGE_GARRICK };
                for (uint32 entry : activeSummons)
                    if (player->GetSummonedCreatureByEntry(entry))
                        player->UnsummonCreatureByEntry(entry);

                player->CastSpell(player, SPELL_OGRE_DISGUISE, true);
                player->CastSpell(player, SPELL_SUMMON_PRISONERS_A, true);
            }
            else
                player->CastSpell(player, SPELL_SUMMON_PRISONERS_H, true);

            player->SetObjectScale(1.5f);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_ogre_transformation_313583_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_ogre_transformation_313583_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ogre_transformation_313583_AuraScript();
    }
};

void AddSC_exiles_reach_darkmaul_bridge()
{
    // Player Scripts
    new exiles_reach_darkmaul_bridge_playerscript();

    // Npc Scripts    
    RegisterCreatureAI(npc_exiles_reach_darkmaul_bridge_167596_156941);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_bridge_167598);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_bridge_156943);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_bridge_prisoners);
    
    // Mob Scripts    
    RegisterCreatureAI(npc_exiles_reach_killclaw_153266);

    // Spell Scripts
    new spell_trampling_charge_210131();
    new spell_ogre_transformation_313583();
}
