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

class exiles_reach_quilboar_briarpatch_playerscript : public PlayerScript
{
public:
    exiles_reach_quilboar_briarpatch_playerscript() : PlayerScript("exiles_reach_quilboar_briarpatch_playerscript") {}
	
    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_QUILBOAR_BRIARPATCH:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_QUILBOAR_BRIARPATCH }, true);

                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_QUILBOAR_BRIARPATCH_FIZZLEPOP }, QuestAcceptedOrCompleted(player, QUEST_H_DOWN_WITH_THE_QUILBOAR));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_QUILBOAR_BRIARPATCH_15477 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_QUILBOAR_BRIARPATCH_MITHDRAN }, !player->GetSummonedCreatureByEntry(NPC_QUILBOAR_BRIARPATCH_MITHDRAN_KNEELING) && (!player->GetQuestStatus(QUEST_H_FORBIDDEN_QUILBOAR_NECROMANCY) || !player->GetQuestStatus(QUEST_H_DOWN_WITH_THE_QUILBOAR)));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_QUILBOAR_BRIARPATCH_BREKA_ARRIVED }, !player->GetSummonedCreatureByEntry(NPC_ABANDONED_CAMP_BREKA_WALKING) && (!player->GetQuestStatus(QUEST_H_FORBIDDEN_QUILBOAR_NECROMANCY) || !player->GetQuestStatus(QUEST_H_DOWN_WITH_THE_QUILBOAR)));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_MITHDRAN }, QuestCompleted(player, QUEST_H_FORBIDDEN_QUILBOAR_NECROMANCY));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_BREKA, PHASE_HORDE_DARKMAUL_PLAINS_FIZZLEPOP }, QuestCompleted(player, QUEST_H_DOWN_WITH_THE_QUILBOAR));
                }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_AUSTIN }, !player->GetSummonedCreatureByEntry(NPC_QUILBOAR_BRIARPATCH_AUSTIN_KNEELING) && (!player->GetQuestStatus(QUEST_FORBIDDEN_QUILBOAR_NECROMANCY) || !player->GetQuestStatus(QUEST_DOWN_WITH_THE_QUILBOAR)));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_GARRICK }, !player->GetSummonedCreatureByEntry(NPC_ABANDONED_CAMP_GARRICK_WALKING) && (!player->GetQuestStatus(QUEST_FORBIDDEN_QUILBOAR_NECROMANCY) || !player->GetQuestStatus(QUEST_DOWN_WITH_THE_QUILBOAR)));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_15276 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_16917 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_16917 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_SPRINGSTOCK }, QuestAcceptedOrCompleted(player, QUEST_DOWN_WITH_THE_QUILBOAR));
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

/* ## NPC SCRIPTS > ## */
// Warlord Breka Grimaxe, Captain Garrick (Quilboar Briarpatch)
struct npc_exiles_reach_quilboar_briarpatch_166824_156662 : public ScriptedAI
{
    npc_exiles_reach_quilboar_briarpatch_166824_156662(Creature* creature) : ScriptedAI(creature) {}

    void MoveInLineOfSight(Unit* who) override
    {
        if (metFriend)
            return;

        Creature* mithdranOrAustin = who->ToCreature();
        if (!mithdranOrAustin)
            return;

        if (mithdranOrAustin->GetEntry() != NPC_QUILBOAR_BRIARPATCH_MITHDRAN_KNEELING &&
            mithdranOrAustin->GetEntry() != NPC_QUILBOAR_BRIARPATCH_AUSTIN_KNEELING)
            return;

        metFriend = true;

        // Safe way to get the player
        if (Player* player = me->SelectNearestPlayer(30.0f))
        {
            // Only Mithdran talks
            if (mithdranOrAustin->GetEntry() == NPC_QUILBOAR_BRIARPATCH_MITHDRAN_KNEELING)
                mithdranOrAustin->AI()->Talk(1, player);
        }

        me->GetMotionMaster()->MovePoint(1, -143.075f, -2640.596f, 48.804f, true);
    }

    void MovementInform(uint32 /*type*/, uint32 id) override
    {
        if (id != 1)
            return;

        me->SetFacingTo(6.2f, true);
        me->SetStandState(UNIT_STAND_STATE_KNEEL);
        me->DespawnOrUnsummon(1s);
    }

private:
    bool metFriend = false;
};

// Mithdran Dawntracker, Austin Huxworth (Quilboar Briarpatch)
struct npc_exiles_reach_quilboar_briarpatch_166996_154327 : public ScriptedAI
{
    npc_exiles_reach_quilboar_briarpatch_166996_154327(Creature* creature) : ScriptedAI(creature) {}

    void OnQuestAccept(Player* player, Quest const* /*quest*/) override
    {
        // Fix the quest status check (operator precedence + correct status)
        bool hasQuests = false;
        if (player->IsInHorde())
        {
            hasQuests = player->GetQuestStatus(QUEST_H_DOWN_WITH_THE_QUILBOAR) == QUEST_STATUS_INCOMPLETE &&
                player->GetQuestStatus(QUEST_H_FORBIDDEN_QUILBOAR_NECROMANCY) == QUEST_STATUS_INCOMPLETE;
        }
        else if (player->IsInAlliance())
        {
            hasQuests = player->GetQuestStatus(QUEST_DOWN_WITH_THE_QUILBOAR) == QUEST_STATUS_INCOMPLETE &&
                player->GetQuestStatus(QUEST_FORBIDDEN_QUILBOAR_NECROMANCY) == QUEST_STATUS_INCOMPLETE;
        }

        if (!hasQuests)
            return;

        Creature* tempMithdranOrAustin = player->SummonCreature(me->GetEntry(), me->GetPosition(),
            TEMPSUMMON_MANUAL_DESPAWN, 0s, 0, 0, player->GetGUID());
        if (!tempMithdranOrAustin)
            return;

        uint32 brekaOrGarrickEntry = player->IsInHorde() ? NPC_QUILBOAR_BRIARPATCH_BREKA_KNEELING
            : NPC_QUILBOAR_BRIARPATCH_GARRICK_KNEELING;

        Creature* tempBrekaOrGarrick = player->SummonCreature(brekaOrGarrickEntry,
            ExilesNpcPositions[QUEST_H_DOWN_WITH_THE_QUILBOAR][NPC_QUILBOAR_BRIARPATCH_BREKA_KNEELING],
            TEMPSUMMON_MANUAL_DESPAWN, 0s, 0, 0, player->GetGUID());
        if (!tempBrekaOrGarrick)
            return;

        ObjectGuid mithdranGuid = tempMithdranOrAustin->GetGUID();
        ObjectGuid brekaGuid = tempBrekaOrGarrick->GetGUID();

        tempBrekaOrGarrick->AI()->Talk(0, player);

        player->GetScheduler()
            .Schedule(5s + 500ms, [mithdranGuid, player](TaskContext /*context*/)
                {
                    if (Creature* mithdran = ObjectAccessor::GetCreature(*player, mithdranGuid))
                        mithdran->AI()->Talk(0, player);
                })
            .Schedule(10s, [mithdranGuid, brekaGuid, player](TaskContext /*context*/)
                {
                    Creature* mithdran = ObjectAccessor::GetCreature(*player, mithdranGuid);
                    Creature* breka = ObjectAccessor::GetCreature(*player, brekaGuid);
                    if (!mithdran || !breka)
                        return;

                    breka->AI()->Talk(1, player);

                    mithdran->SetStandState(UNIT_STAND_STATE_STAND);
                    breka->SetStandState(UNIT_STAND_STATE_STAND);

                    mithdran->GetMotionMaster()->MovePoint(1, -128.411f, -2637.33f, 48.4554f, true);
                    breka->GetMotionMaster()->MoveFollow(mithdran, PET_FOLLOW_DIST, breka->GetFollowAngle());
                })
            .Schedule(13s, [mithdranGuid, player](TaskContext /*context*/)
                {
                    if (Creature* mithdran = ObjectAccessor::GetCreature(*player, mithdranGuid))
                        mithdran->GetMotionMaster()->MovePoint(2, -109.327f, -2646.74f, 52.5968f, true);
                })
            .Schedule(16s, [mithdranGuid, brekaGuid, player](TaskContext /*context*/)
                {
                    if (Creature* mithdran = ObjectAccessor::GetCreature(*player, mithdranGuid))
                    {
                        mithdran->GetMotionMaster()->MovePoint(3, -81.9396f, -2644.04f, 57.433f, true);
                        mithdran->DespawnOrUnsummon(5s);
                    }
                    if (Creature* breka = ObjectAccessor::GetCreature(*player, brekaGuid))
                        breka->DespawnOrUnsummon(5s);
                });
    }
};

// Geolord Grek'og (Quilboar Briarpatch)
struct npc_exiles_reach_geolord_grekog_151091 : public ScriptedAI
{
    npc_exiles_reach_geolord_grekog_151091(Creature* creature) : ScriptedAI(creature)
    {
        me->SetEmoteState(EMOTE_STATE_READY_SPELL_OMNI);
    }

    void JustEngagedWith(Unit* attacker) override // attack_started
    {
        Player* player = attacker->ToPlayer();
        if (!player)
            return;

        me->SetEmoteState(EMOTE_ONESHOT_NONE);

        Talk(0);
    }

    void JustDied(Unit* attacker) override
    {
        // No need for personal summon

        std::list<Creature*> nearbyCreatures;
        me->GetCreatureListInGrid(nearbyCreatures, 50.f);

        for (Creature* creature : nearbyCreatures)
        {
            if (creature->GetEntry() == NPC_QUILBOAR_BRIARPATCH_FIZZLEPOP || creature->GetEntry() == NPC_QUILBOAR_BRIARPATCH_SPRINGSTOCK)
            {
                creature->RemoveAura(SPELL_NECROTIC_RITUAL);
                creature->AI()->Talk(1, attacker->ToPlayer());

                creature->GetMotionMaster()->MovePoint(0, 16.6666f, -2511.82f, 73.39143f, true);
                creature->AI()->Talk(1, attacker->ToPlayer());
                // And that's why I always bet on the Horde! Let's scram!
                // @TODO: Alliance text missing

                creature->GetScheduler().Schedule(Milliseconds(3000), [creature, this](TaskContext /*context*/)
                    {
                        creature->GetMotionMaster()->MovePoint(0, 58.815f, -2472.307f, 81.13f, true);
                        creature->DespawnOrUnsummon(7s, 60s);
                        me->DespawnOrUnsummon(30s, 37s); });
            }
        }
    }

    void DamageTaken(Unit* attacker, uint32& /*damage*/, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        uint32 maxhp = me->GetMaxHealth();
        uint32 hp = me->GetHealth();
        Player* player = attacker->ToPlayer();

        if ((hp < maxhp * 0.75) && (hp > maxhp * 0.6)) // if hp between 60-75%
        {
            // cast earthbolt
            me->CastSpell(player, SPELL_EARTHBOLT);
        }

        if ((hp < maxhp * 0.4) && (hp > maxhp * 0.3)) // if hp between 30-40%
        {
            // cast upheaval
            me->CastSpell(player, SPELL_UPHEAVAL);
        }
    }
};

/* ## MOB SCRIPTS > ## */

// Quilboar Warrior, Quilboar Geomacter (quilboar briarpatch)
struct npc_exiles_reach_quilboars_150237_150238 : public ScriptedAI
{
    npc_exiles_reach_quilboars_150237_150238(Creature* creature) : ScriptedAI(creature)
    {
        mid_talked = false;
    }

    void JustEngagedWith(Unit* who) override
    {
        std::map<uint32, uint32> talkData =
        {
            {150281, 176079}, // Death to the outsiders!
            {150283, 176082}, // More sacrifices for the ritual!
            {150282, 176080}, // Our thorns will strangle you!
            {150278, 176076}, // The geolord demands your demise!
            {150276, 176074}, // The ogres give us strength!
        };

        if (me->HasAura(SPELL_QUILBOAR_SLEEP))
            me->RemoveAura(SPELL_QUILBOAR_SLEEP);

        if (roll_chance(30))
        {
            auto randomTalk = Trinity::Containers::SelectRandomContainerElement(talkData);
            if (who->IsPlayer())
            {
                me->PlayObjectSound(randomTalk.first, me->GetGUID(), who->ToPlayer(), randomTalk.second);
                me->Say(randomTalk.second);
            }
        }

        me->AddDelayedEvent(3000, [this, who]()
            {
                if (me->GetEntry() == 150237)
                    me->CastSpell(who, SPELL_BRUTAL_STRIKE);
                else
                    me->CastSpell(who, SPELL_EARTH_BOLT_2); });
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        uint32 maxhp = me->GetMaxHealth();
        uint32 hp = me->GetHealth();
        Player* player = attacker->ToPlayer();

        if ((hp < maxhp * 0.3) && (hp > maxhp * 0.2))
        {
            if (me->GetEntry() == 150237)
                me->CastSpell(player, SPELL_BRUTAL_STRIKE);
            else
                me->CastSpell(player, SPELL_EARTH_BOLT_2);

            if (!mid_talked)
            {
                std::map<uint32, uint32> talkData =
                {
                    {150279, 176077}, // We serve the geolord!
                    {150277, 176075}, // Your blood will earn our reward!
                    {150280, 176078}, // Witness our power!
                };
                mid_talked = true;

                if (roll_chance(30))
                {
                    auto randomTalk = Trinity::Containers::SelectRandomContainerElement(talkData);
                    me->PlayDirectSound(randomTalk.first, player, randomTalk.second);
                    me->Say(randomTalk.second);
                }
            }
        }

        if (damage > hp) // on last hit before die
        {
            std::map<uint32, uint32> talkData =
            {
                {150284, 176083}, // I return... to... the earth...
                {150286, 176085}, // Ogres... will... kill you...
                {150285, 176084}, // Our tribe... will... survive...
                {150287, 176086}, // The geolord... will not... lose...
                {150288, 176088}, // Thorns... take... you...
            };

            if (roll_chance(30))
            {
                auto randomTalk = Trinity::Containers::SelectRandomContainerElement(talkData);
                me->PlayDirectSound(randomTalk.first, player, randomTalk.second);
                me->Say(randomTalk.second);
            }
        }
    }

private:
    bool mid_talked;
};

// Ogre Overseer (quilboar briarpatch)
struct npc_exiles_reach_ogre_overseer_156676 : public ScriptedAI
{
    npc_exiles_reach_ogre_overseer_156676(Creature* creature) : ScriptedAI(creature)
    {
        me->SetObjectScale(2.f); // i was lazy for sql query
    }

    void JustEngagedWith(Unit* who) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        me->Say(178950);
        me->PlayDirectSound(150269, player, 178950);
        me->AddDelayedEvent(3000, [this, player]()
            { me->CastSpell(player, SPELL_EARTHSHATTER); });
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        uint32 maxhp = me->GetMaxHealth();
        uint32 hp = me->GetHealth();
        Player* player = attacker->ToPlayer();

        if ((hp < maxhp * 0.7) && (hp > maxhp * 0.4))
        {
            me->CastSpell(player, SPELL_BACKHAND);
        }
        if ((hp < maxhp * 0.3) && (hp > maxhp * 0.1))
        {
            me->CastSpell(attacker, SPELL_EARTHSHATTER);
        }

        if (damage > hp) // on last hit before die
            Talk(0);
    }
};

void AddSC_exiles_reach_quilboar_briarpatch()
{
    // Player Scripts
    new exiles_reach_quilboar_briarpatch_playerscript();
    // NPC Scripts
    RegisterCreatureAI(npc_exiles_reach_quilboar_briarpatch_166824_156662);
    RegisterCreatureAI(npc_exiles_reach_quilboar_briarpatch_166996_154327);
    RegisterCreatureAI(npc_exiles_reach_geolord_grekog_151091);
    // Mob Scripts
    RegisterCreatureAI(npc_exiles_reach_quilboars_150237_150238);
    RegisterCreatureAI(npc_exiles_reach_ogre_overseer_156676);
}
