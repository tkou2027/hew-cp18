#include "application.h"
#include "halwindow.h"
#include "draw.h"
#include "scene.h"
#include "input.h"
#include "timer.h"
#include "audio.h"
#include "world.h"
#include "item.h"
// timer
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

static ConsoleSetting prevConsoleSetting;
static int fpsCounter = 0;
static const DWORD debug_timer = 500;

void DispFPS();
void update(float);
void draw();

void applicationInit(int width, int height, const wchar_t* title) {
    // init console window
    initWindow(&prevConsoleSetting, width, height, title);
    //for (int i = 0; i < 25; ++i) {
    //    printf("%d\n", i);
    //}

    // init buffer
    buffer_init(width, height);

    // input
    input_init();

    // init global controllers (audio, time)
    // タイマー分解能を高精度に設定
    // 1/1000秒ごとに1ずつ増えるタイマーを使える
    timeBeginPeriod(1);
    timer_init(timeGetTime());
    audio_init(); // load all bgms, nomater what scene, as there won't be many???

    // game objects
    world_init(); // load map data
    item_init(); // configs

    // init game
    scene_application_init();
    initScene();
}

void applicationRun() {
    // game loop
    int execLastTime;	// ゲーム処理をした時間（タイマー値）
    int fpsLastTime;	// デバッグ表示をした時間（タイマー値）
    int currentTime;	// 現在の時間（タイマー値）
    int frameCount;		// ゲームの処理をした回数

    execLastTime =
        fpsLastTime = timeGetTime();	// 現在のタイマー値取得

    // 変数を0に初期化
    currentTime =
        frameCount = 0;

    // ==========================
    // この辺で初期化処理を行う
    // ==========================

    // ここからゲームループ
    do
    {
        currentTime = timeGetTime();	// 現在のタイマー値
        if ((currentTime - fpsLastTime) >= debug_timer)	// タイマーの差が500ms以上かどうか
        {	// 処理回数と経過時間から1秒間に何回処理できるかを計算
            fpsCounter = frameCount * 1000 / (currentTime - fpsLastTime);
            fpsLastTime = currentTime;	// 現在のタイマー値を保存
            frameCount = 0;				// 処理回数リセット
        }

        // 前回ゲーム処理時のタイマー値と現在のタイマー値の差が
        // 1/60秒(17ms)以上になっていたら処理を実行
        float deltaTime = currentTime - execLastTime;
        if (deltaTime >= (1000 / 60))
        {
            execLastTime = currentTime;// 現在のタイマー値保存
            timer_update(currentTime, deltaTime);

            //// 更新処理
            update(deltaTime * 0.001f);
            //// 描画処理
            draw();

            //// シーン切り替え
            checkScene();

            frameCount++;// ゲーム処理回数アップ

            // デバッグ時のみ表示
#ifdef _DEBUG
            DispFPSBuffer(fpsCounter);
#endif
        }
    } while (!input_if_key_pressed(InputKey_Exit)); // ESC押すまでループ
}

void applicationDestroy() {
    audio_destroy();
    resetWindow(&prevConsoleSetting);
    buffer_destroy();
    world_destroy();
}

void update(float deltaTime) {
    updateScene(deltaTime);
}

void draw() {
    drawScene();
    // buffer_draw_update();
    buffer_draw_swap();
}