-- Exiles Reach: bind the Giant Boar Trample script (mows the undead down)
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (305557, 'spell_giant_boar_trample_305557');

-- Exiles Reach: post-Torgak quest-giver is Captain Garrick (245394), not 156807
-- (which wrongly reused Jaina's model 88316). Point the spawn and the
-- Stocking Up / Repair Yourself quest starters+enders at 245394.
UPDATE `creature` SET `id`=245394 WHERE `guid`=42560 AND `id`=156807;
UPDATE `creature_queststarter` SET `id`=245394 WHERE `id`=156807 AND `quest` IN (55194, 85678);
UPDATE `creature_questender` SET `id`=245394 WHERE `id`=156807 AND `quest` IN (55194, 85678);

-- Exiles Reach: Captain Garrick (245394) must be a quest-giver (npcflag bit 0x2)
UPDATE `creature_template` SET `npcflag` = `npcflag` | 2 WHERE `entry` = 245394;

-- Exiles Reach post-Torgak: revert the quest-giver to Jaina (156807) and give
-- Quartermaster Richter the repair flag.
-- (Corrects 2026_08_24_01/02 which wrongly swapped in Captain Garrick 245394.)
UPDATE `creature` SET `id`=156807 WHERE `guid`=42560 AND `id`=245394;
UPDATE `creature_queststarter` SET `id`=156807 WHERE `id`=245394 AND `quest` IN (55194, 85678);
UPDATE `creature_questender` SET `id`=156807 WHERE `id`=245394 AND `quest` IN (55194, 85678);
UPDATE `creature_template` SET `npcflag`=0 WHERE `entry`=245394;
UPDATE `creature_template` SET `npcflag` = `npcflag` | 4096 WHERE `entry` = 156800;

-- Exiles Reach: after the ride, Captain Garrick (245394) is the quest-giver for
-- Stocking Up on Supplies (55194); Repair Yourself (85678) is no longer offered.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 2 WHERE `entry` = 245394;

DELETE FROM `creature_queststarter` WHERE `id` = 156807 AND `quest` IN (55194, 85678);
DELETE FROM `creature_questender`  WHERE `id` = 156807 AND `quest` IN (55194, 85678);

INSERT INTO `creature_queststarter` (`id`, `quest`, `VerifiedBuild`) VALUES
(245394, 55194, 0);
INSERT INTO `creature_questender` (`id`, `quest`, `VerifiedBuild`) VALUES
(245394, 55194, 0);

-- Exiles Reach: post-ride reunion dialogue (Wrathion + Jaina) as a server-driven conversation.
-- The scene (2341) references conversation lines 88006-88013; the Wrathion broadcasts
-- (302254-302258) already exist, Jaina's (302259-302261) were empty placeholders.
-- NOTE: the broadcast_text updates for 302259-302261 live in the hotfixes DB.

DELETE FROM `conversation_actors` WHERE `ConversationId` = 51000;
DELETE FROM `conversation_line_template` WHERE `Id` BETWEEN 88006 AND 88013;
DELETE FROM `conversation_template` WHERE `Id` = 51000;

INSERT INTO `conversation_template` (`Id`, `FirstLineId`, `TextureKitId`, `Flags`, `ScriptName`, `VerifiedBuild`) VALUES
(51000, 88006, 0, 0, '', 0);

INSERT INTO `conversation_line_template` (`Id`, `UiCameraID`, `ActorIdx`, `Flags`, `ChatType`, `VerifiedBuild`, `Unk`) VALUES
(88006, 0, 0, 0, 0, 0, 0),
(88007, 0, 0, 0, 0, 0, 0),
(88008, 0, 0, 0, 0, 0, 0),
(88009, 0, 0, 0, 0, 0, 0),
(88010, 0, 0, 0, 0, 0, 0),
(88011, 0, 1, 0, 0, 0, 0),
(88012, 0, 1, 0, 0, 0, 0),
(88013, 0, 1, 0, 0, 0, 0);

INSERT INTO `conversation_actors` (`ConversationId`, `ConversationActorId`, `ConversationActorGuid`, `ConversationActorNearId`, `Idx`, `CreatureId`, `CreatureDisplayInfoId`, `NoActorObject`, `ActivePlayerObject`, `VerifiedBuild`) VALUES
(51000, 80000, 0, 0, 0, 156799, 107146, 0, 0, 0),
(51000, 80001, 0, 0, 1, 156807, 88316, 0, 0, 0);