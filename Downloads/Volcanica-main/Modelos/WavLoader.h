#ifndef WAV_LOADER_H
#define WAV_LOADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <AL/al.h> // OpenAL header

class WavLoader {
public:
    bool load(const std::string& filename, std::vector<char>& audioData, ALenum& format, ALsizei& freq);

private:
    uint32_t readUInt32(std::ifstream& file);
    uint16_t readUInt16(std::ifstream& file);
};

#endif
#pragma once
