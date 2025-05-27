#include "WavLoader.h"

uint32_t WavLoader::readUInt32(std::ifstream& file) {
    uint8_t bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    return (uint32_t(bytes[0]) | (uint32_t(bytes[1]) << 8) | (uint32_t(bytes[2]) << 16) | (uint32_t(bytes[3]) << 24));
}

uint16_t WavLoader::readUInt16(std::ifstream& file) {
    uint8_t bytes[2];
    file.read(reinterpret_cast<char*>(bytes), 2);
    return (uint16_t(bytes[0]) | (uint16_t(bytes[1]) << 8));
}

bool WavLoader::load(const std::string& filename, std::vector<char>& audioData, ALenum& format, ALsizei& freq) {
    
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[WavLoader] Error: No se pudo abrir el archivo: " << filename << std::endl;
        return false;
    }

    // Leer encabezado RIFF
    char riff[4];
    file.read(riff, 4);
    if (strncmp(riff, "RIFF", 4) != 0) {
        std::cerr << "[WavLoader] Error: No es un archivo RIFF válido.\n";
        return false;
    }

    file.ignore(4); // File size
    char wave[4];
    file.read(wave, 4);
    if (strncmp(wave, "WAVE", 4) != 0) {
        std::cerr << "[WavLoader] Error: No es un archivo WAVE.\n";
        return false;
    }

    // Leer chunks hasta encontrar "fmt "
    char chunkId[4];
    uint32_t chunkSize;
    uint16_t audioFormat = 0, numChannels = 0, bitsPerSample = 0;
    uint32_t sampleRate = 0;
    bool fmtFound = false, dataFound = false;
    uint32_t dataSize = 0;
    std::streampos dataPos;

    while (file.read(chunkId, 4)) {
        chunkSize = readUInt32(file);

        if (strncmp(chunkId, "fmt ", 4) == 0) {
            audioFormat = readUInt16(file);
            numChannels = readUInt16(file);
            sampleRate = readUInt32(file);
            file.ignore(6); // byteRate (4) + blockAlign (2)
            bitsPerSample = readUInt16(file);
            file.ignore(chunkSize - 16); // Saltar resto si hay
            fmtFound = true;
        }
        else if (strncmp(chunkId, "data", 4) == 0) {
            dataSize = chunkSize;
            dataPos = file.tellg();
            file.seekg(chunkSize, std::ios::cur);
            dataFound = true;
        }
        else {
            // Saltar cualquier otro chunk
            file.seekg(chunkSize, std::ios::cur);
        }

        // Salir si ya tenemos todo lo necesario
        if (fmtFound && dataFound)
            break;
    }

    if (!fmtFound || !dataFound) {
        std::cerr << "[WavLoader] Error: No se encontró el chunk 'fmt ' o 'data'.\n";
        return false;
    }

    // Establecer formato para OpenAL
    if (audioFormat != 1) {
        std::cerr << "[WavLoader] Error: Solo se admite PCM sin comprimir.\n";
        return false;
    }

    if (bitsPerSample == 8) {
        format = (numChannels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
    }
    else if (bitsPerSample == 16) {
        format = (numChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    }
    else {
        std::cerr << "[WavLoader] Error: Bits por muestra no soportado: " << bitsPerSample << "\n";
        return false;
    }

    freq = sampleRate;

    // Leer los datos del audio
    audioData.resize(dataSize);
    file.seekg(dataPos);
    file.read(audioData.data(), dataSize);

    return true;
}
