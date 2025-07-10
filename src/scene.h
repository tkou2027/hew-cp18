#pragma once

enum SCENE
{
	SCENE_TITLE,  // タイトル画面
	SCENE_GAME,   // ゲーム本編
	SCENE_ENDING, // ゲーム結果表示画面

    // tests
    SCENE_TEST_RACING,
    SCENE_TEST_TILEMAP,
    SCENE_TEST_SPRITE,
    SCENE_TEST_FLUID,
    SCENE_TEST_DUNGEON,
    SCENE_TEST_ISO,

	SCENE_NUM // 終端コード
};

void scene_application_init();
SCENE scene_get_current();
void initScene();
void uninitScene();
void updateScene(float deltaTime);
void drawScene();

void setScene(SCENE next);
void checkScene();