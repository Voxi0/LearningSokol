#include"../Engine.hpp"

// Constructor - Load Texture Image File and Create Texture Object
sgl::Texture::Texture(sg_image_desc &imgDesc, const std::string &texImgFilePath) {
    // Flip Texture Images Vertically Upon Loading Them
    stbi_set_flip_vertically_on_load(true);

    // Load Texture Image File
    int texWidth, texHeight, texNrChannels;
    stbi_uc* texImg {stbi_load(texImgFilePath.c_str(), &texWidth, &texHeight, &texNrChannels, 4)};
    if(texImg) {
        // Load Texture Image Data Into Sokol Image Desc Struct
        imgDesc.width = texWidth;
        imgDesc.height = texHeight;
        imgDesc.data.subimage[0][0].ptr = texImg;
        imgDesc.data.subimage[0][0].size = texWidth * texHeight * 4;

        // Create Texture Object
        this->texture = sg_make_image(imgDesc);
    } else {
        // Display Error Message
        std::cerr << "Failed to Load Texture Image File: " << texImgFilePath << std::endl;
    }

    // Free Texture Image Data From Memory
    stbi_image_free(texImg);
}

// Getters
sg_image sgl::Texture::getImage(void) {return this->texture;}
