#ifndef FT_LOADER_HPP
#define FT_LOADER_HPP
#include <vector>
#include <iostream>
#include <exception>
#include "ft_math.hpp"

#pragma pack(push, 1)
typedef struct s_bitmapFileHeader{
	uint16_t Signature{0x4D42};
	uint32_t Size{0};
	uint32_t Reserved{0};
	uint32_t BitsOffset{0};
} t_bitmapFileHeader;

typedef struct s_bitmapInfoHeader{
	uint32_t HeaderSize{0};
	int32_t Width{0};
	int32_t Height{0};
	uint16_t Planes{1};
	uint16_t BitCount{0};
	uint32_t Compression{0};
	uint32_t SizeImage{0};
	int32_t PelsPerMeterX{0};
	int32_t PelsPerMeterY{0};
	uint32_t ClrUsed{0};
	uint32_t ClrImportant{0};
}   t_bitmapInfoHeader;


typedef struct s_bitmapColorHeader{
	uint32_t RedMask{0x00ff0000};
	uint32_t GreenMask{0x0000ff00};
	uint32_t BlueMask{0x000000ff};
	uint32_t AlphaMask{0xff000000};
	uint32_t CsType{0x73524742};
	uint32_t unused[16]{0};
}   t_bitmapColorHeader;

#pragma pack(pop)

namespace ftloader{
        void OBJ(
            const char *path,
            std::vector<ftmath::vec3> &out_vertices,
            std::vector<ftmath::vec2> &out_uvs,
            std::vector<ftmath::vec3> &out_normals,
            BoundBox &boundBox
        );
        float *OBJTOOPENGLVERTICES(
            unsigned int &out_size,
            const std::vector<ftmath::vec3> &in_vertices,
            const std::vector<ftmath::vec2> &in_uvs,
            const std::vector<ftmath::vec3> &in_normals,
            const ftmath::vec3 color[4]
        );
        unsigned char* BMP(
            const char *path,
            int &width,
            int &height
        );
        static uint32_t make_stride_aligned(uint32_t align_stride, const uint32_t &row_stride);
        static bool check_color_header(s_bitmapColorHeader &bmp_color_header);
}

class ftloader_OBJ_exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Error to load OBJ file !";
  }
};
#endif