#define _CRT_SECURE_NO_WARNINGS
#include "sprite.h"
#include "draw.h"

#include <stdlib.h> // malloc
#include <stdio.h> // file
#include <string.h> // string atoi


void sprite_init(Sprite* p_sprite, int width, int height) {
    int size = width * height;
    p_sprite->width = width;
    p_sprite->height = height;
    p_sprite->pixels = (Pixel*)malloc(sizeof(Pixel) * size);
    p_sprite->alpha = (bool*)malloc(sizeof(bool) * size);
}

void sprite_init(Sprite* p_sprite, int width, int height, const wchar_t* text, Color color) {
    sprite_init(p_sprite, width, height);
    int size = width * height;
    for (int i = 0; i < size; ++i) {
        p_sprite->pixels[i] = create_pixel(text[i], color);
    }
}

//void sprite_load(Sprite* p_sprite, const char* filename) {
//    FILE* fp;
//    fopen_s(&fp, filename, "r");
//
//    if (fp == NULL) {
//        // TODO
//        return;
//    }
//
//    const int line_buffer_size = 256;
//    char line[line_buffer_size];
//    // read first two line for metadata
//    if (!fgets(line, line_buffer_size, fp)
//        || !fgets(line, line_buffer_size, fp)) {
//        // TODO
//        return;
//    }
//    // split second line
//    char seps[] = " ,\t\n";
//    char* token;
//    char* next_token = NULL;
//
//    // width
//    token = strtok_s(line, seps, &next_token);
//    int width = atoi(token);
//    // height
//    token = strtok_s(NULL, seps, &next_token);
//    int height = atoi(token);
//
//    printf("%d %d", width, height);
//}

void sprite_load(Sprite* p_sprite, const wchar_t* filename) {
    FILE* fp;
    fp = _wfopen(filename, L"r,ccs=UTF-8");

    if (fp == NULL) {
        // TODO
        return;
    }

    const int line_buffer_size = 256;
    wchar_t line[line_buffer_size];
    // read first two line for metadata
    if (!fgetws(line, line_buffer_size, fp)
        || !fgetws(line, line_buffer_size, fp)) {
        // TODO
        return;
    }
    // split second line
    wchar_t seps[] = L" ,\t\n";
    wchar_t* token;
    wchar_t* next_token = NULL;

    // width
    token = wcstok_s(line, seps, &next_token);
    int width = _wtoi(token);
    // height
    token = wcstok_s(NULL, seps, &next_token);
    int height = _wtoi(token);

    sprite_init(p_sprite, width, height);

    // wprintf(L"%d %d\n", width, height);
    // content
    // for (int i = 0; i < height; ++i) {
        // for (int j = 0; j < height; ++j)
    //wchar_t c[3];
    for (int i = 0; i < height; ++i) {
        // skip
        fgetws(line, line_buffer_size, fp);
    }
    for (int i = 0; i < width * height; ++ i) {
        fgetws(line, line_buffer_size, fp);
        wchar_t text = line[0];
        int colors[6];
        for (int i = 0; i < 6; ++i) {
            token = wcstok_s(i == 0 ? line + 2 : NULL, seps, &next_token);
            colors[i] = _wtoi(token);
        }
        p_sprite->pixels[i] = create_pixel(
            text,
            create_color(colors[0], colors[1], colors[2]),
            create_color(colors[3], colors[4], colors[5])
        );
    }
}

void sprite_destroy(Sprite* p_sprite) {
    free(p_sprite->pixels);
    free(p_sprite->alpha);
}

// copited from draw.cpp
void sprite_draw_sprite(Coord2 pos, const Sprite* sprite) {
    for (int i = 0; i < sprite->height; ++i) {
        for (int j = 0; j < sprite->width; ++j) {
            draw_pixel(
                Coord2{ pos.x + j, pos.y + i },
                sprite->pixels[i * sprite->width + j]
            );
        }
    }
}

void sprite_draw_sprite_alpha(Coord2 pos, const Sprite* sprite) {
    for (int i = 0; i < sprite->height; ++i) {
        for (int j = 0; j < sprite->width; ++j) {
            draw_pixel_alpha(
                Coord2{ pos.x + j, pos.y + i },
                sprite->pixels[i * sprite->width + j]
            );
        }
    }
}