#ifndef Included_Utils_H

#define Included_Utils_H

#include <cstdlib>
#include <ctime>

float getRandomFloat(float a, float b) {
    int randInt = std::rand(); // Gera um número inteiro aleatório entre 0 e RAND_MAX
    float randFloat = static_cast<float>(randInt) / RAND_MAX; // Normaliza para o intervalo [0, 1]
    float scaledFloat = a + randFloat * (b - a); // Escala para o intervalo desejado

    return scaledFloat;
}

void init(const char *file_name, GLuint &textureID) {
    int width, height, channels;
    unsigned char* image = stbi_load(file_name, &width, &height, &channels, STBI_rgb_alpha);

    if (!image) {
        std::cerr << "Erro ao carregar a imagem do sprite!" << std::endl;
        return;
    }

    // Gerar a textura do sprite
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // Definir os parâmetros de filtragem da textura (opcional)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Liberar a memória dos dados da imagem
    stbi_image_free(image);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

#endif
