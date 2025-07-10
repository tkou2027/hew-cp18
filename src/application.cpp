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
    // �^�C�}�[����\�������x�ɐݒ�
    // 1/1000�b���Ƃ�1��������^�C�}�[���g����
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
    int execLastTime;	// �Q�[���������������ԁi�^�C�}�[�l�j
    int fpsLastTime;	// �f�o�b�O�\�����������ԁi�^�C�}�[�l�j
    int currentTime;	// ���݂̎��ԁi�^�C�}�[�l�j
    int frameCount;		// �Q�[���̏�����������

    execLastTime =
        fpsLastTime = timeGetTime();	// ���݂̃^�C�}�[�l�擾

    // �ϐ���0�ɏ�����
    currentTime =
        frameCount = 0;

    // ==========================
    // ���̕ӂŏ������������s��
    // ==========================

    // ��������Q�[�����[�v
    do
    {
        currentTime = timeGetTime();	// ���݂̃^�C�}�[�l
        if ((currentTime - fpsLastTime) >= debug_timer)	// �^�C�}�[�̍���500ms�ȏォ�ǂ���
        {	// �����񐔂ƌo�ߎ��Ԃ���1�b�Ԃɉ��񏈗��ł��邩���v�Z
            fpsCounter = frameCount * 1000 / (currentTime - fpsLastTime);
            fpsLastTime = currentTime;	// ���݂̃^�C�}�[�l��ۑ�
            frameCount = 0;				// �����񐔃��Z�b�g
        }

        // �O��Q�[���������̃^�C�}�[�l�ƌ��݂̃^�C�}�[�l�̍���
        // 1/60�b(17ms)�ȏ�ɂȂ��Ă����珈�������s
        float deltaTime = currentTime - execLastTime;
        if (deltaTime >= (1000 / 60))
        {
            execLastTime = currentTime;// ���݂̃^�C�}�[�l�ۑ�
            timer_update(currentTime, deltaTime);

            //// �X�V����
            update(deltaTime * 0.001f);
            //// �`�揈��
            draw();

            //// �V�[���؂�ւ�
            checkScene();

            frameCount++;// �Q�[�������񐔃A�b�v

            // �f�o�b�O���̂ݕ\��
#ifdef _DEBUG
            DispFPSBuffer(fpsCounter);
#endif
        }
    } while (!input_if_key_pressed(InputKey_Exit)); // ESC�����܂Ń��[�v
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