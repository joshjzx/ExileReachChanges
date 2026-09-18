/*
 * Copyright (c) WowCommunity - 2023
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

// @todo add rare ogre to harpy roost 156986
class exiles_reach_harpy_roost_playerscript : public PlayerScript
{
public:
    exiles_reach_harpy_roost_playerscript() : PlayerScript("exiles_reach_harpy_roost_playerscript") {}
    
    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_HARPY_ROOST:
            {
                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_SHUJA_BO },
                        (!QuestAccepted(player, QUEST_H_HARPY_CULLING)
                        || !QuestAccepted(player,  QUEST_H_PURGE_THE_TOTEMS)
                        || !QuestAccepted(player,  QUEST_H_THE_RESCUE_OF_HERBERT_GLOOMBURST))
                        && !QuestAcceptedOrCompleted(player,  QUEST_H_MESSAGE_TO_BASE ));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_HERBERT_RITUAL }, 
                    !QuestCompletedOrRewarded(player, QUEST_H_THE_RESCUE_OF_HERBERT_GLOOMBURST)
                    && !player->GetSummonedCreatureByEntry(NPC_HARPY_ROOST_HERBERT_RITUAL));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_HERBERT_AFTER_RITUAL }, QuestCompletedOrRewarded(player, QUEST_H_THE_RESCUE_OF_HERBERT_GLOOMBURST) && !QuestAcceptedOrCompleted(player, QUEST_H_MESSAGE_TO_BASE));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_TOTEMS }, true);
                }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13815 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_13333 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_HENRY_KEELA },
                        (!QuestAccepted(player, QUEST_HARPY_CULLING)
                        || !QuestAccepted(player,  QUEST_PURGE_THE_TOTEMS)
                        || !QuestAccepted(player,  QUEST_THE_RESCUE_OF_MEREDY_HUNTSWELL))
                        && !QuestAcceptedOrCompleted(player,  QUEST_MESSAGE_TO_BASE ));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_MEREDY_RITUAL }, 
                        !QuestCompletedOrRewarded(player, QUEST_THE_RESCUE_OF_MEREDY_HUNTSWELL)
                        && !player->GetSummonedCreatureByEntry(NPC_HARPY_ROOST_MEREDY_RITUAL));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_MEREDY_AFTER_RITUAL }, QuestCompletedOrRewarded(player, QUEST_THE_RESCUE_OF_MEREDY_HUNTSWELL) && !QuestAcceptedOrCompleted(player, QUEST_MESSAGE_TO_BASE));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_13328 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_TOTEMS }, true);
                }
            }
            break;
            }
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

// Henry Garrick, Shuja Grimaxe (Ogre Ruins, at the way of Harpy Roost)
// Henry Garrick, Shuja Grimaxe (Harpy Roost, after Bloodbeak fight)
struct npc_exiles_reach_ogre_ruins_156833_167219_156859_167307 : public ScriptedAI
{
    npc_exiles_reach_ogre_ruins_156833_167219_156859_167307(Creature* creature) : ScriptedAI(creature) {}

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_THE_HARPY_PROBLEM)
        {
            // @todo set every talk to player
            Talk(0, player);  // Kee-La and I will scout ahead. We'll meet you to the north, just outside the harpy den.

            if (Creature* keela = me->FindNearestCreature(NPC_OGRE_RUINS_KEELA_BEFORE_HARPY_ROOST, 50.f))
                keela->m_Events.AddEventAtOffset(new DelayedMoveEvent(player, keela, WayToHarpyRoost, 0, true, true), 10ms);

            me->m_Events.AddEventAtOffset(new DelayedMoveEvent(player, me, WayToHarpyRoost, 0, true, true), 10ms);
        }

        if (quest->GetQuestId() == QUEST_H_THE_HARPY_PROBLEM)
        {
            Talk(0, player);  // I will join Bo farther north, where the harpies roost. Meet me there.

            me->m_Events.AddEventAtOffset(new DelayedMoveEvent(player, me, WayToHarpyRoost, 0, true, true), 10ms);
        }

        if (quest->GetQuestId() == QUEST_MESSAGE_TO_BASE)
        {
            Talk(0, player);  // The captain will be thrilled! Let's get back to camp and deliver the good news.
            
            std::vector<uint32> summons = {NPC_HARPY_ROOST_HENRY_AFTER_RITUAL, NPC_HARPY_ROOST_MEREDY_AFTER_RITUAL, NPC_HARPY_ROOST_KEELA_AFTER_RITUAL};

            me->m_Events.AddEventAtOffset(new DelayedMoveEventWithSummons(player, WayToHarpyRoost, summons, 10s), 10ms);
        }

        if (quest->GetQuestId() == QUEST_H_MESSAGE_TO_BASE)
        {
            Talk(0, player);  // Well fought. We are one step closer to crushing those pitiful ogres.
            
            std::vector<uint32> summons = {NPC_HARPY_ROOST_SHUJA_AFTER_RITUAL, NPC_HARPY_ROOST_HERBERT_AFTER_RITUAL, NPC_HARPY_ROOST_BO_AFTER_RITUAL};

            me->m_Events.AddEventAtOffset(new DelayedMoveEventWithSummons(player, WayToHarpyRoost, summons, 10s), 10ms);
        }
    }
};

// Blood Beak Event
class BloodBeakFightEvent : public BasicEvent
{
public:
    BloodBeakFightEvent(Player* player, Creature* creature, int8 round) : _player(player), _creature(creature), _round(round) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        // 40:16 2 harpy ambusher summoned
        // 40:45 1 hunting worg 1 harpy ambusher
        // 40:55 keela / bo spawned on the bridge and running to the loc
        // 41:05 henry / shuja spawned on the bridge and running to the loc
        // 41:07 3 hunting worg
        // 41:28 1 harpy ambusher / 2 worg
        // 41:41 something huge coming conversation
        // 41:45 bloodbeak flying in
        // when bloodbeak dies remove ritual phase, spawn clone, remove aura scene, clone casts arcane shit, convo, runoff on the bridge and despawn

        switch (_round)
        {
            case 1:
            summons = {NPC_HARPY_ROOST_HARPY_AMBUSHER, NPC_HARPY_ROOST_HARPY_AMBUSHER};
            _player->m_Events.AddEventAtOffset(new BloodBeakFightEvent(_player, _creature, 2), 30s);
                break;
            case 2:
            summons = {NPC_HARPY_ROOST_HUNTING_WORG, NPC_HARPY_ROOST_HARPY_AMBUSHER};
            _player->m_Events.AddEventAtOffset(new BloodBeakFightEvent(_player, _creature, 3), 10s);
                break;
            case 3:
            if (_player->IsInAlliance())
                summons = {NPC_HARPY_ROOST_FIGHTER_KEELA};
            else
                summons = {NPC_HARPY_ROOST_FIGHTER_BO};            
            _player->m_Events.AddEventAtOffset(new BloodBeakFightEvent(_player, _creature, 4), 10s);
                break;
            case 4:
            if (_player->IsInAlliance())
                summons = {NPC_HARPY_ROOST_FIGHTER_HENRY};
            else
                summons = {NPC_HARPY_ROOST_FIGHTER_SHUJA};                
            _player->m_Events.AddEventAtOffset(new BloodBeakFightEvent(_player, _creature, 5), 2s);
                break;
            case 5:
            summons = {NPC_HARPY_ROOST_HUNTING_WORG, NPC_HARPY_ROOST_HUNTING_WORG, NPC_HARPY_ROOST_HUNTING_WORG};
            _player->m_Events.AddEventAtOffset(new BloodBeakFightEvent(_player, _creature, 6), 19s);
                break;
            case 6:
            summons = {NPC_HARPY_ROOST_HARPY_AMBUSHER, NPC_HARPY_ROOST_HUNTING_WORG, NPC_HARPY_ROOST_HUNTING_WORG};
            _player->m_Events.AddEventAtOffset(new BloodBeakFightEvent(_player, _creature, 7), 13s);
                break;
            case 7:
            summons = {};
            if (_player->IsInAlliance())
                StartPrivateConversation(_player, CONVERSATION_BLOOD_BEAK_INCOMING_A, { _creature });
            else    
                StartPrivateConversation(_player, CONVERSATION_BLOOD_BEAK_INCOMING_H, { _creature });
            _player->m_Events.AddEventAtOffset(new BloodBeakFightEvent(_player, _creature, 8), 13s);
                break;
            case 8:
            summons = {NPC_HARPY_ROOST_BLOODBEAK};
                break;
        }

        for (uint32 entry : summons)
        {
            Position summonPos;

            switch (entry)
            {
                case NPC_HARPY_ROOST_BLOODBEAK:
                summonPos = BloodBeakSpawnPos;
                    break;
                case NPC_HARPY_ROOST_HARPY_AMBUSHER:
                case NPC_HARPY_ROOST_HUNTING_WORG:
                summonPos = _creature->GetRandomNearPosition(30.f);
                    break;
                case NPC_HARPY_ROOST_FIGHTER_KEELA:
                case NPC_HARPY_ROOST_FIGHTER_BO:
                case NPC_HARPY_ROOST_FIGHTER_HENRY:
                case NPC_HARPY_ROOST_FIGHTER_SHUJA:
                default:
                summonPos = BloodBeakFighterSpawnPos;
                    break;
            }

            if (Creature* summon = _player->SummonCreature(entry, summonPos, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10min, 0U, 0U, _player->GetGUID()))
            {
                if (entry == NPC_HARPY_ROOST_HARPY_AMBUSHER || entry == NPC_HARPY_ROOST_HUNTING_WORG)
                    summon->GetMotionMaster()->MoveCirclePath(_creature->GetPositionX(), _creature->GetPositionY(), _creature->GetPositionZ(), 10.f, true, 10);

                if (entry == NPC_HARPY_ROOST_BLOODBEAK)
                    summon->GetMotionMaster()->MovePoint(0, BloodBeakMovePos);
            }
        }

        return true;
    }

private:
    Player* _player;
    Creature* _creature;
    int8 _round;
    std::vector<uint32> summons;
};

// Herbert Gloomburst, Meredy Huntswell (Harpy Roost)
struct npc_exiles_reach_harpy_roost_167298_153211 : public ScriptedAI
{
    npc_exiles_reach_harpy_roost_167298_153211(Creature* creature) : ScriptedAI(creature) {}

    bool OnGossipSelect(Player* player, uint32 /*sender*/, uint32 /*action*/) override
    {
        if (!QuestAccepted(player, QUEST_THE_RESCUE_OF_MEREDY_HUNTSWELL) && !QuestAccepted(player, QUEST_H_THE_RESCUE_OF_HERBERT_GLOOMBURST))
            return false;

        Talk(0, player);
        
        if (Creature* clone = player->SummonCreature(me->GetEntry(), me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
        {
            player->m_Events.AddEventAtOffset(new BloodBeakFightEvent(player, clone, 1), 2s);
            clone->SetUninteractible(false);
        }

        CloseGossipMenuFor(player);

        return true;
    }

    void IsSummonedBy(WorldObject* /*summoner*/) override
    {
        me->RemoveNpcFlag(UNIT_NPC_FLAG_GOSSIP);
    }
};

// Henry Garrick, Shuja Grimaxe, Kee-La, Bo (Fighting with Bloodbeak) (Harpy Roost)
struct npc_exiles_reach_harpy_roost_155197_167301_155199_167300 : public ScriptedAI
{
    npc_exiles_reach_harpy_roost_155197_167301_155199_167300(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _events.Reset();
    }

    void JustEngagedWith(Unit* /*attacker*/) override // attack_started
    {
        if (me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_HENRY || me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_SHUJA)
        {
            _events.ScheduleEvent(EVENT_DAMAGE, 6s);
            _events.ScheduleEvent(EVENT_HEAL, 1s);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (timer <= diff)
        {
            if (Player* player = me->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (QuestCompleted(player, QUEST_H_THE_RESCUE_OF_HERBERT_GLOOMBURST) || QuestCompleted(player, QUEST_THE_RESCUE_OF_MEREDY_HUNTSWELL))
                    return;

                if (!me->IsInCombat())
                {     
                    me->FollowTarget(player);

                    if (me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_KEELA || me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_BO)
                    {
                        me->SetFollowAngle(float(M_PI_4));
                    }

                    if (me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_HENRY || me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_SHUJA)
                    {
                        me->SetFollowAngle(float(-M_PI_4));
                    }
                }
            }
            
            timer = 3000;
        }
        else
            timer -= diff;

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_HEAL:
            {
                // Checking healable targets in every 1 sec
                Unit* selectedTarget = nullptr;
                std::list<Unit*> unitList;
                me->GetFriendlyUnitListInRange(unitList, 30.0f, false);

                for (Unit* target : unitList)
                {
                    if (target->GetHealthPct() < 80)
                        selectedTarget = target;
                }
                
                if (!selectedTarget) // noone's below 80% hp
                    return;

                if (me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_SHUJA)
                    me->CastSpell(selectedTarget, SPELL_SHUJA_HEALING_WAVE);                        
                
                if (me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_HENRY)
                    me->CastSpell(selectedTarget, SPELL_HENRY_HEAL);
                
                _events.RescheduleEvent(EVENT_HEAL, 1s);
            }
                break;
            case EVENT_DAMAGE:
                if (me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_SHUJA)
                    DoCastVictim(SPELL_SHUJA_LIGHTNING_BOLT);                       
                
                if (me->GetEntry() == NPC_HARPY_ROOST_FIGHTER_HENRY)
                    DoCastVictim(SPELL_HENRY_HOLY_SMITE);

                _events.RescheduleEvent(EVENT_DAMAGE, 6s);
                break;
            default:
                break;
            }
        }
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        if (damage > me->GetHealth())
            damage = 0;
    }

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* player = summoner->ToPlayer();
        if (!player)
            return;

        me->SetMaxHealth(player->GetHealth());
        me->SetFullHealth();
        me->SetOwnerGUID(player->GetGUID()); // it's needed for following check
    }

private:
    EventMap _events;
    enum
    {
        EVENT_HEAL = 1,
        EVENT_DAMAGE,
    };
    uint32 timer = 3000;
};

// Bloodbeak (Harpy Roost)
struct npc_exiles_reach_harpy_roost_153964 : public ScriptedAI
{
    npc_exiles_reach_harpy_roost_153964(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _events.Reset();
    }

    void JustEngagedWith(Unit* /*attacker*/) override // attack_started
    {
        _events.ScheduleEvent(EVENT_SWOOPING_LUNGE, 10s);
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
            case EVENT_SWOOPING_LUNGE:
                DoCastVictim(SPELL_SWOOPING_LUNGE);   
                _events.RescheduleEvent(EVENT_SWOOPING_LUNGE, 10s);
                break;
            default:
                break;
            }
        }
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        if (!attacker->IsPlayer())
            me->GetThreatManager().ClearThreat(attacker);

        // Kill by player only
        if (damage > me->GetHealth() && attacker->IsCreature())
            damage = 0;
    }

    void JustDied(Unit* killer) override
    {
        Player* player = killer->ToPlayer();
        if (!player)
            return;

        if (player->HasAura(SPELL_RITUAL_SCENE_HARPY_BEAM))
            player->RemoveAura(SPELL_RITUAL_SCENE_HARPY_BEAM);

        player->KilledMonsterCredit(NPC_HARPY_ROOST_BLOODBEAK);

        std::vector<Creature*> ritualDoneGoHome = std::vector<Creature*>{};

        if (Creature* meredy = player->GetSummonedCreatureByEntry(NPC_HARPY_ROOST_MEREDY_RITUAL))
        {
            meredy->CastSpell(meredy, SPELL_ARCANE_BURST, true);
            meredy->RemoveAllAuras();            
            meredy->NearTeleportTo(Position(495.57f, -2351.94f, 160.5f));
            meredy->AI()->Talk(1, player); // Hah! There! Come on, let's get outta here while we can!
            ritualDoneGoHome.push_back(meredy);
        }

        if (Creature* herbert = player->GetSummonedCreatureByEntry(NPC_HARPY_ROOST_HERBERT_RITUAL))
        {
            herbert->CastSpell(herbert, SPELL_ARCANE_BURST, true);
            herbert->RemoveAllAuras();
            herbert->NearTeleportTo(Position(495.57f, -2351.94f, 160.5f));
            herbert->AI()->Talk(1, player); // I'm free! Hahaha! Ahem. Let's make ourselves scarce, shall we?
            ritualDoneGoHome.push_back(herbert);
        }

        if (Creature* bo = player->GetSummonedCreatureByEntry(NPC_HARPY_ROOST_FIGHTER_BO))
            ritualDoneGoHome.push_back(bo);

        if (Creature* keela = player->GetSummonedCreatureByEntry(NPC_HARPY_ROOST_FIGHTER_KEELA))
            ritualDoneGoHome.push_back(keela);

        if (Creature* henry = player->GetSummonedCreatureByEntry(NPC_HARPY_ROOST_FIGHTER_HENRY))
            ritualDoneGoHome.push_back(henry);

        if (Creature* shuja = player->GetSummonedCreatureByEntry(NPC_HARPY_ROOST_FIGHTER_SHUJA))
            ritualDoneGoHome.push_back(shuja);

        for (Creature* creature : ritualDoneGoHome)
        {
            creature->m_Events.AddEventAtOffset(new DelayedMoveEvent(player, creature, BloodBeakFighterSpawnPos, 0, false, true), urand(3, 7) * 1s);
        }
    }

private:
    EventMap _events;
    enum
    {
        EVENT_SWOOPING_LUNGE = 1,
    };
};

/* ## MOB SCRIPTS > ## */

// Harpy Boltcaller, Harpy Snatcher, Hunting Worg (Harpy Roost)
struct npc_exiles_reach_harpy_roost_152843_152571_152998 : public ScriptedAI
{
    npc_exiles_reach_harpy_roost_152843_152571_152998(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _events.Reset();

        switch (me->GetEntry())
        {
            case NPC_HARPY_ROOST_HARPY_BOLTCALLER:
            _events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 3s);
                break;
            case NPC_HARPY_ROOST_HARPY_SNATCHER:
            _events.ScheduleEvent(EVENT_TALON_RAKE, 3s);
                break;
            case NPC_HARPY_ROOST_HARPY_HUNTING_WORG:
            _events.ScheduleEvent(EVENT_WORG_CLAW, 3s);
                break;
        }
    }

    void JustEngagedWith(Unit* attacker) override // attack_started
    {
        Player* player = attacker->ToPlayer();
        if (!player)
            return;

        if (me->GetEntry() == NPC_HARPY_ROOST_HARPY_BOLTCALLER || me->GetEntry() == NPC_HARPY_ROOST_HARPY_SNATCHER)
            if (roll_chance(50))
                Talk(0, player);
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
            case EVENT_LIGHTNING_BOLT:
                DoCastVictim(SPELL_HARPIE_LIGHTNINGBOLT);
                _events.RescheduleEvent(EVENT_LIGHTNING_BOLT, 6s);
                break;
            case EVENT_TALON_RAKE:
                DoCastVictim(SPELL_HARPIE_TALONRAKE);
                _events.RescheduleEvent(EVENT_TALON_RAKE, 6s);
                break;
            case EVENT_WORG_CLAW:
                DoCastVictim(SPELL_WORG_CLAW);
                _events.RescheduleEvent(EVENT_WORG_CLAW, 6s);
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
        EVENT_LIGHTNING_BOLT = 1,
        EVENT_TALON_RAKE,
        EVENT_WORG_CLAW
    };
};

/* ## SPELL SCRIPTS > ## */

// ID - 317559 Swooping Lunge
class spell_swooping_lunge_317559 : public SpellScriptLoader
{
public:
    spell_swooping_lunge_317559() : SpellScriptLoader("spell_swooping_lunge_317559") {}

    class spell_swooping_lunge_317559_SpellScript : public SpellScript
    {
        void SetDest(SpellDestination& dest)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;
            
            JumpArrivalCastArgs args;
            args.SpellId = SPELL_SWOOPING_LUNGE_DMG;
            args.Target = caster->GetGUID();
            caster->GetMotionMaster()->MoveJump(1, dest._position.GetPosition(), 30.f, 10.f, 1004U/*, &args*/);
        }

        void Register() override
        {
            OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_swooping_lunge_317559_SpellScript::SetDest, EFFECT_0, TARGET_DEST_DEST);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_swooping_lunge_317559_SpellScript();
    }
};

/* ## GAMEOBJECT SCRIPTS > ## */

// Harpy Totem (Harpy Roost)
struct go_exiles_reach_harpy_roost_350803_327146 : public GameObjectAI
{
    go_exiles_reach_harpy_roost_350803_327146(GameObject* go) : GameObjectAI(go) { }

    bool OnGossipHello(Player* player) override
    {
        if (player->GetQuestStatus(QUEST_H_PURGE_THE_TOTEMS) || player->GetQuestStatus(QUEST_PURGE_THE_TOTEMS))
        {            
            if (Creature* burningTotem = player->SummonCreature(NPC_HARPY_ROOST_BURNING_TOTEM, me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
            {
                burningTotem->CastSpell(burningTotem, SPELL_BURNING_TOP, true);    
                player->KillCreditGO(me->GetEntry());
                me->DestroyForPlayer(player);
                
                return true;
            }
        }

        return false;
    }
};

void AddSC_exiles_reach_harpy_roost()
{
    // Player Scripts
    new exiles_reach_harpy_roost_playerscript();

    // NPC Scripts
    RegisterCreatureAI(npc_exiles_reach_ogre_ruins_156833_167219_156859_167307);
    RegisterCreatureAI(npc_exiles_reach_harpy_roost_167298_153211);
    RegisterCreatureAI(npc_exiles_reach_harpy_roost_155197_167301_155199_167300);
    RegisterCreatureAI(npc_exiles_reach_harpy_roost_153964);

    // Mob Scripts
    RegisterCreatureAI(npc_exiles_reach_harpy_roost_152843_152571_152998);

    // Spell Scripts
    new spell_swooping_lunge_317559();

    // GameObject Scripts
    RegisterGameObjectAI(go_exiles_reach_harpy_roost_350803_327146);
}
