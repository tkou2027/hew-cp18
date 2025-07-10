#include "platform.h"
#include "scene.h"
#include "input.h"
#include "scene_title.h"
#include "scene_game.h"
#include "scene_ending.h"

// *****************************************************************************
// �O���[�o���ϐ�
// *****************************************************************************

// �N�����ɓ��삳�������V�[�����Z�b�g���Ă���
static SCENE g_Scene; // ���ݍ쓮���̃V�[��
static SCENE g_SceneNext; // �ڍs��̃V�[��

void scene_application_init() {
    g_Scene = SCENE_TITLE;
    g_SceneNext = SCENE_TITLE;
}

SCENE scene_get_current() {
    return g_Scene;
}

// �V�[��������
void initScene(void) {
    // InitKey();
    switch (g_Scene) {
    case SCENE_TITLE:
        // �^�C�g����ʂ̏�����
        scene_title_init();
        break;
    case SCENE_GAME:
        // �Q�[���{�҂̏�����
        scene_game_init();
        break;
    case SCENE_ENDING:
        // ���U���g��ʂ̏�����
        scene_ending_init();
        break;
    }
}

// �V�[���I������
void uninitScene(void) {
    switch (g_Scene) {
    case SCENE_TITLE:
        // �^�C�g����ʂ̏I������
        break;
    case SCENE_GAME:
        // �Q�[���{�҂̏I������
        break;
    case SCENE_ENDING:
        // ���U���g��ʂ̏I������
        break;

        // tests
    case SCENE_TEST_RACING:
        break;
    case SCENE_TEST_TILEMAP:
        break;
    }
}

// �V�[���X�V����
void updateScene(float deltaTime) {
    // UpdateKey();
    input_update();
    switch (g_Scene) {
    case SCENE_TITLE:
        // �^�C�g����ʂ̍X�V����
        scene_title_update();
        break;
    case SCENE_GAME:
        // �Q�[���̍X�V����
        scene_game_update();
        break;
    case SCENE_ENDING:
        // ���U���g��ʂ̍X�V����
        scene_ending_update();
        break;
    }
}

// �V�[���̕`�揈��
void drawScene(void) {
    switch (g_Scene) {
    case SCENE_TITLE:
        // �^�C�g����ʂ̕`�揈��
        scene_title_draw();
        break;
    case SCENE_GAME:
        // �Q�[���{�҂̕`�揈��
        scene_game_draw();
        break;
    case SCENE_ENDING:
        // ���U���g��ʂ̕`�揈��
        scene_ending_draw();
        break;
    }
}

// �ڍs��V�[���̃Z�b�g
void setScene(SCENE next) {
    g_SceneNext = next;
}

//�@�V�[���؂�ւ�
void checkScene(void) {
    // �ڍs��V�[�����Z�b�g����Ă��邩���m�F����
    if (g_SceneNext >= SCENE_NUM) {
        // might becase of initializing g_Scene on definition
        g_SceneNext = g_Scene;
        return;
    }
    if (g_Scene != g_SceneNext) {
        // ���ݍ쓮���̃V�[�����I��������
        uninitScene();

        // ���ݍ쓮���̃V�[�����ڍs��V�[���ōX�V����
        g_Scene = g_SceneNext;

        // �X�V�����V�[�������������s��
        initScene();

        // reset keyboard input
        platform_resetkey();
    }
}