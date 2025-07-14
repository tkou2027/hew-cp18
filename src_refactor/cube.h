#pragma once
#include <memory> // for std::unique_ptr

#include "pixel.h"
#include "block.h"

namespace hoge
{
	class Cube {
    public:
        enum class FaceDir {
            TOP,
            FRONT,
        };

        enum class FacePattern {
            SOLID,
            STRIPE
        };
        struct FaceInfo {
            FaceDir dir{ FaceDir::TOP };
            FacePattern pattern{ FacePattern::SOLID };

            std::unique_ptr<Color[]> colors;
            std::unique_ptr<Pixel[]> pixels;
            std::unique_ptr<Color[]> shadow_colors;
            std::unique_ptr<Pixel[]> shadow_pixels;

            bool has_pixel_mix;
            Pixel pixel_mix;
        };
        static void GetFaceInfo(FaceDir face_dir, const BlockInfo& block_info, FaceInfo& face_info);
    private:
        static void GetFaceInfoGround(FaceDir face_dir, const BlockInfo& block_info, FaceInfo& face_info);
        static void GetFaceInfoGroundTop(const BlockInfo& block_info, FaceInfo& face_info);
        static void GetFaceInfoGroundSide(const BlockInfo& block_info, FaceInfo& face_info);
	};
}