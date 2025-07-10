#include "platform.h"
#include "scene.h"
#include "input.h"
#include "scene_title.h"
#include "scene_game.h"
#include "scene_ending.h"

// *****************************************************************************
// グローバル変数
// *****************************************************************************

// 起動時に動作させたいシーンをセットしておく
static SCENE g_Scene; // 現在作動中のシーン
static SCENE g_SceneNext; // 移行先のシーン

void scene_application_init() {
    g_Scene = SCENE_TITLE;
    g_SceneNext = SCENE_TITLE;
}

SCENE scene_get_current() {
    return g_Scene;
}

// シーン初期化
void initScene(void) {
    // InitKey();
    switch (g_Scene) {
    case SCENE_TITLE:
        // タイトル画面の初期化
        scene_title_init();
        break;
    case SCENE_GAME:
        // ゲーム本編の初期化
        scene_game_init();
        break;
    case SCENE_ENDING:
        // リザルト画面の初期化
        scene_ending_init();
        break;
    }
}

// シーン終了処理
void uninitScene(void) {
    switch (g_Scene) {
    case SCENE_TITLE:
        // タイトル画面の終了処理
        break;
    case SCENE_GAME:
        // ゲーム本編の終了処理
        break;
    case SCENE_ENDING:
        // リザルト画面の終了処理
        break;

        // tests
    case SCENE_TEST_RACING:
        break;
    case SCENE_TEST_TILEMAP:
        break;
    }
}

// シーン更新処理
void updateScene(float deltaTime) {
    // UpdateKey();
    input_update();
    switch (g_Scene) {
    case SCENE_TITLE:
        // タイトル画面の更新処理
        scene_title_update();
        break;
    case SCENE_GAME:
        // ゲームの更新処理
        scene_game_update();
        break;
    case SCENE_ENDING:
        // リザルト画面の更新処理
        scene_ending_update();
        break;
    }
}

// シーンの描画処理
void drawScene(void) {
    switch (g_Scene) {
    case SCENE_TITLE:
        // タイトル画面の描画処理
        scene_title_draw();
        break;
    case SCENE_GAME:
        // ゲーム本編の描画処理
        scene_game_draw();
        break;
    case SCENE_ENDING:
        // リザルト画面の描画処理
        scene_ending_draw();
        break;
    }
}

// 移行先シーンのセット
void setScene(SCENE next) {
    g_SceneNext = next;
}

//　シーン切り替え
void checkScene(void) {
    // 移行先シーンがセットされているかを確認する
    if (g_SceneNext >= SCENE_NUM) {
        // might becase of initializing g_Scene on definition
        g_SceneNext = g_Scene;
        return;
    }
    if (g_Scene != g_SceneNext) {
        // 現在作動中のシーンを終了させる
        uninitScene();

        // 現在作動中のシーンを移行先シーンで更新する
        g_Scene = g_SceneNext;

        // 更新したシーンを初期化を行う
        initScene();

        // reset keyboard input
        platform_resetkey();
    }
}