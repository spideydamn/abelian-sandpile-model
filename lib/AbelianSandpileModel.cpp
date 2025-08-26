#include "AbelianSandpileModel.h"

void ClearTwoDimensionalMemory(uint64_t**& array, uint16_t x) {
    for (int i = 0; i < x; ++i) {
        delete[] array[i];
    }
    delete[] array;
}

uint64_t** PushBackRow(uint64_t** grid, uint16_t x, uint16_t y) {
    x *= 2;

    auto** new_grid = new uint64_t*[x]{};

    for (uint16_t i = 0; i < x; ++i) {
        new_grid[i] = new uint64_t[y]{};
    }

    for (uint16_t i = 0; i < x / 2; ++i) {
        for (uint16_t j = 0; j < y; ++j) {
            new_grid[i][j] = grid[i][j];
        }
    }

    return new_grid;
}

uint64_t** PushFrontRow(uint64_t** grid, uint16_t x, uint16_t y) {
    x *= 2;

    auto** new_grid = new uint64_t*[x]{};

    for (uint16_t i = 0; i < x; ++i) {
        new_grid[i] = new uint64_t[y]{};
    }

    for (uint16_t i = 0; i < x / 2; ++i) {
        for (uint16_t j = 0; j < y; ++j) {
            new_grid[i + x / 2][j] = grid[i][j];
        }
    }

    return new_grid;
}

uint64_t** PushBackCol(uint64_t** grid, uint16_t x, uint16_t y) {
    y *= 2;

    auto** new_grid = new uint64_t*[x]{};

    for (uint16_t i = 0; i < x; ++i) {
        new_grid[i] = new uint64_t[y]{};
        for (uint16_t j = 0; j < y / 2; ++j) {
            new_grid[i][j] = grid[i][j];
        }
    }

    return new_grid;
}

uint64_t** PushFrontCol(uint64_t** grid, uint16_t x, uint16_t y) {
    y *= 2;

    auto** new_grid = new uint64_t*[x]{};

    for (uint16_t i = 0; i < x; ++i) {
        new_grid[i] = new uint64_t[y]{};
        for (uint16_t j = 0; j < y / 2; ++j) {
            new_grid[i][j + y / 2] = grid[i][j];
        }
    }

    return new_grid;
}

void AbelianSandpileModel::SetSize(uint16_t given_x, uint16_t given_y) {
    for (uint16_t i = 0; i < this->x; ++i) {
        delete[] this->grid[i];
    }
    delete[] this->grid;

    this->x = given_x;
    this->y = given_y;

    this->lower_bound_x = 0;
    this->lower_bound_y = 0;
    this->upper_bound_x = given_x - 1;
    this->upper_bound_y = given_y - 1;

    this->grid = new uint64_t *[this->x];
    for (uint16_t i = 0; i < this->x; ++i) {
        this->grid[i] = new uint64_t[this->y]{};
    }
}

void AbelianSandpileModel::SetCell(uint16_t given_x, uint16_t given_y, uint64_t amount) const {
    this->grid[given_x][given_y] = amount;
}

void AbelianSandpileModel::Move() {
    auto **temp = new uint64_t *[this->x];
    for (uint16_t i = 0; i < this->x; ++i) {
        temp[i] = new uint64_t[this->y];
        for (uint16_t j = 0; j < this->y; ++j) {
           temp[i][j] = this->grid[i][j];
        }
    }

    uint64_t** intermediate_value = nullptr;

    for (uint16_t i = this->lower_bound_x; i < this->upper_bound_x + 1; ++i) {
        for (uint16_t j = this->lower_bound_y; j < this->upper_bound_y + 1; ++j) {
            if (this->grid[i][j] >= 4) {
                if (i - 1 < 0) {
                    intermediate_value = PushFrontRow(this->grid, this->x, this->y);
                    ClearTwoDimensionalMemory(this->grid, this->x);
                    this->grid = intermediate_value;

                    intermediate_value = PushFrontRow(temp, this->x, this->y);
                    ClearTwoDimensionalMemory(temp, this->x);
                    temp = intermediate_value;

                    this->lower_bound_x += (this->x - 1);
                    this->upper_bound_x += (this->x - 1);
                    i += this->x;
                    this->x *= 2;
                }
                if (j - 1 < 0) {
                    intermediate_value = PushFrontCol(this->grid, this->x, this->y);
                    ClearTwoDimensionalMemory(this->grid, this->x);
                    this->grid = intermediate_value;

                    intermediate_value = PushFrontCol(temp, this->x, this->y);
                    ClearTwoDimensionalMemory(temp, this->x);
                    temp = intermediate_value;

                    this->lower_bound_y += (this->y - 1);
                    this->upper_bound_y += (this->y - 1);
                    j += this->y;
                    this->y *= 2;
                }
                if (i + 1 >= x) {
                    intermediate_value = PushBackRow(this->grid, this->x, this->y);
                    ClearTwoDimensionalMemory(this->grid, this->x);
                    this->grid = intermediate_value;

                    intermediate_value = PushBackRow(temp, this->x, this->y);
                    ClearTwoDimensionalMemory(temp, this->x);
                    temp = intermediate_value;

                    this->x *= 2;
                }
                if (j + 1 >= y) {
                    intermediate_value = PushBackCol(this->grid, this->x, this->y);
                    ClearTwoDimensionalMemory(this->grid, this->x);
                    this->grid = intermediate_value;

                    intermediate_value = PushBackCol(temp, this->x, this->y);
                    ClearTwoDimensionalMemory(temp, this->x);
                    temp = intermediate_value;

                    this->y *= 2;
                }

                if (i - 1 < this->lower_bound_x) {
                    this->lower_bound_x = i - 1;
                }
                if (i + 1 > this->upper_bound_x) {
                    this->upper_bound_x = i + 1;
                }
                if (j - 1 < this->lower_bound_y) {
                    this->lower_bound_y = j - 1;
                }
                if (j + 1 > this->upper_bound_y) {
                    this->upper_bound_y = j + 1;
                }

                temp[i][j] -= 4;

                ++temp[i - 1][j];
                ++temp[i][j - 1];
                ++temp[i + 1][j];
                ++temp[i][j + 1];
            }
        }
    }

    ClearTwoDimensionalMemory(this->grid, this->x);

    this->grid = temp;
}

AbelianSandpileModel::AbelianSandpileModel() :
grid(nullptr), x(0), y(0), lower_bound_x(0), lower_bound_y(0),upper_bound_x(0), upper_bound_y(0) {}

void AbelianSandpileModel::MakePhoto(std::ofstream& output) const {
    uint16_t size_x = this->upper_bound_x - this->lower_bound_x + 1;
    uint16_t size_y = this->upper_bound_y - this->lower_bound_y + 1;
    auto **pixels = new uint8_t*[size_x];

    for (uint16_t i = 0; i < size_x; ++i) {
        pixels[i] = new uint8_t[(size_y + 1) / 2]{};
    }

    for (uint16_t i = 0; i < size_x; ++i) {
        for (uint16_t j = 0; j < size_y; ++j) {
            if (j % 2 == 0) {
                if (this->grid[i + this->lower_bound_x][j + this->lower_bound_y] <= 3) {
                    pixels[i][j / 2] += this->grid[i + this->lower_bound_x][j + this->lower_bound_y] << 4;
                } else {
                    pixels[i][j / 2] += 4 << 4;
                }
            } else {
                if (this->grid[i + this->lower_bound_x][j + this->lower_bound_y] <= 3) {
                    pixels[i][j / 2] += this->grid[i + this->lower_bound_x][j + this->lower_bound_y];
                } else {
                    pixels[i][j / 2] += 4;
                }
            }
        }
    }

//    for (uint16_t i = 0; i < size_x; ++i) {
//        for (uint16_t j = 0; j < (size_y + 1) / 2; ++j) {
//            std::cout << ((uint16_t)pixels[i][j] >> 4) << " " << ((uint16_t)pixels[i][j] & 15) << " ";
//        }
//        std::cout << '\n';
//    }
//
//    std::cout << "\n\n\n";


    uint8_t bmp_padding[3]{0, 0, 0};
    const int32_t padding_amount = (4 - ((size_y + 1) / 2) % 4) % 4;

    const uint16_t file_header_size = 14;
    const uint16_t information_header_size = 40;
    const uint16_t color_table_size = 20;
    const uint16_t file_size = file_header_size + information_header_size + color_table_size + size_x * ((size_y + 1) / 2) + padding_amount * size_x;

    uint8_t file_header[file_header_size];

    // File type
    file_header[0] = 'B';
    file_header[1] = 'M';

    // File size
    file_header[2] = file_size;
    file_header[3] = file_size >> 8;
    file_header[4] = file_size >> 16;
    file_header[5] = file_size >> 24;

    //Reserved 1 (Not used)
    file_header[6] = 0;
    file_header[7] = 0;

    //Reserved 2 (Not used)
    file_header[8] = 0;
    file_header[9] = 0;

    // Pixel data offset
    file_header[10] = file_header_size + information_header_size + color_table_size;
    file_header[11] = 0;
    file_header[12] = 0;
    file_header[13] = 0;

    uint8_t information_header[information_header_size + color_table_size];

    // Header size
    information_header[0] = 40;
    information_header[1] = 0;
    information_header[2] = 0;
    information_header[3] = 0;

    // Image width
    information_header[4] = size_y;
    information_header[5] = size_y >> 8;
    information_header[6] = size_y >> 16;
    information_header[7] = size_y >> 24;

    // Image height
    information_header[8] = size_x;
    information_header[9] = size_x >> 8;
    information_header[10] = size_x >> 16;
    information_header[11] = size_x >> 24;

    // Planes
    information_header[12] = 1;
    information_header[13] = 0;

    // Bits per pixel
    information_header[14] = 4;
    information_header[15] = 0;

    // Compression
    information_header[16] = 0;
    information_header[17] = 0;
    information_header[18] = 0;
    information_header[19] = 0;

    // Image size
    information_header[20] = 0;
    information_header[21] = 0;
    information_header[22] = 0;
    information_header[23] = 0;

    // X pixels per meter
    information_header[24] = 0;
    information_header[25] = 0;
    information_header[26] = 0;
    information_header[27] = 0;

    // Y pixels per meter
    information_header[28] = 0;
    information_header[29] = 0;
    information_header[30] = 0;
    information_header[31] = 0;

    // total colors
    information_header[32] = 5;
    information_header[33] = 0;
    information_header[34] = 0;
    information_header[35] = 0;

    // important colors
    information_header[36] = 0;
    information_header[37] = 0;
    information_header[38] = 0;
    information_header[39] = 0;


    // color 0
    information_header[40] = 255;
    information_header[41] = 255;
    information_header[42] = 255;
    information_header[43] = 0;

    // color 1
    information_header[44] = 0;
    information_header[45] = 170;
    information_header[46] = 0;
    information_header[47] = 0;

    // color 2
    information_header[48] = 85;
    information_header[49] = 255;
    information_header[50] = 255;
    information_header[51] = 0;

    // color 3
    information_header[52] = 170;
    information_header[53] = 0;
    information_header[54] = 170;
    information_header[55] = 0;

    // color 4
    information_header[56] = 0;
    information_header[57] = 0;
    information_header[58] = 0;
    information_header[59] = 0;


    output.write(reinterpret_cast<char*>(file_header), file_header_size);
    output.write(reinterpret_cast<char*>(information_header), information_header_size + color_table_size);

    uint8_t pixel[1];
    for (uint16_t i = 0; i < size_x; ++i) {
        for (uint16_t j = 0; j < static_cast<uint16_t>((size_y + 1) / 2); ++j) {
            pixel[0] = pixels[i][j];
            output.write(reinterpret_cast<char*>(pixel), 1);
        }
        output.write(reinterpret_cast<char*>(bmp_padding), padding_amount);
    }
};


