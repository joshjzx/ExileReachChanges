-- Exiles Reach: correct the post-Torgak reunion scene packages.
-- 2341 (Alliance, cast by SPELL_REUNION_SCENE_A) -> 4778 (11.2.7 ExilesReach Alliance IGC)
-- 2497 (Horde) already correctly maps to 4779.
UPDATE `scene_template` SET `ScriptPackageID` = 4778 WHERE `SceneId` = 2341;
