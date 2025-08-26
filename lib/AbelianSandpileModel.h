#pragma once
#include <iostream>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <cstdio>

#pragma pack(push, 1)
struct AbelianSandpileModel {
    uint64_t** grid;
    uint16_t x;
    uint16_t y;
    uint16_t lower_bound_x;
    uint16_t upper_bound_x;
    uint16_t lower_bound_y;
    uint16_t upper_bound_y;

    AbelianSandpileModel();

    void SetSize(uint16_t given_x, uint16_t given_y);

    void SetCell(uint16_t given_x, uint16_t given_y, uint64_t amount) const;

    void Move();

    void MakePhoto(std::ofstream& output) const;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct BMP {
};
#pragma pack(pop)