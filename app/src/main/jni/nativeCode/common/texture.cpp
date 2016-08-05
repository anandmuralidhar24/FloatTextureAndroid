/*
 *    Copyright 2016 Anand Muralidhar
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "texture.h"
#include "myLogger.h"
#include "misc.h"

/**
 * Initializes a float texture: generate the texture name, bind it, etc.
 */
void InitializeFloatTexture(GLuint &textureName, int textureWidth, int textureHeight,
                            int numberOfChannels) {

    GLenum internalFormat, textureFormat;
    if (numberOfChannels == 1) {
        internalFormat = GL_R32F;
        textureFormat = GL_RED;
    } else if (numberOfChannels == 2) {
        internalFormat = GL_RG32F;
        textureFormat = GL_RG;
    } else if (numberOfChannels == 3) {
        internalFormat = GL_RGB32F;
        textureFormat = GL_RGB;
    } else if (numberOfChannels == 4) {
        internalFormat = GL_RGBA32F;
        textureFormat = GL_RGBA;
    }

    // generate it
    glGenTextures(1, &textureName);

    // make active and bind
    glBindTexture(GL_TEXTURE_2D, textureName);

    // turn off filtering and wrap modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // define texture with float format
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureWidth,
                 textureHeight, 0, textureFormat, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // check if that worked
    CheckGLError("InitializeFloatTexture");
}

/**
 * loads new data into float texture by replacing previous data
 */
void LoadFloatTexture(GLuint textureName, int textureWidth, int textureHeight,
                      int numberOfChannels, cv::Mat inputData) {

    //!! assumes that inputData matches texture in number of channels!!
    if(numberOfChannels != inputData.channels()) {
        MyLOGE("Number of channels in input matrix does not match with texture!");
        return;
    }

    GLenum textureFormat;
    if (numberOfChannels == 1) {
        textureFormat = GL_RED;
    } else if (numberOfChannels == 2) {
        textureFormat = GL_RG;
    } else if (numberOfChannels == 3) {
        textureFormat = GL_RGB;
    } else if (numberOfChannels == 4) {
        textureFormat = GL_RGBA;
    }

    // bind the texture and update its contents with the matrix's data
    glBindTexture(GL_TEXTURE_2D, textureName);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureWidth, textureHeight, textureFormat, GL_FLOAT,
                    inputData.data);

    CheckGLError("LoadFloatTexture");
}

/**
 * Read data from float texture into opencv matrix
 */
cv::Mat ReadFloatTexture(GLuint textureName, int textureWidth, int textureHeight,
                         int numberOfChannels) {

    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, textureName, 0);
    // check if that worked
    CheckFramebufferStatus();

    glReadBuffer(GL_COLOR_ATTACHMENT0);

    GLenum textureFormat;
    int matType;
    if (numberOfChannels == 1) {
        textureFormat = GL_RED;
        matType = CV_32F;
    } else if (numberOfChannels == 2) {
        textureFormat = GL_RG;
        matType = CV_32FC2;
    } else if (numberOfChannels == 3) {
        textureFormat = GL_RGB;
        matType = CV_32FC3;
    } else if (numberOfChannels == 4) {
        textureFormat = GL_RGBA;
        matType = CV_32FC4;
    }
    cv::Mat outputMat = cv::Mat::zeros(textureHeight, textureWidth, matType);

    // read data from texture into opencv Mat
    glReadPixels(0, 0, textureWidth, textureHeight, textureFormat,
                 GL_FLOAT, outputMat.data);

    CheckGLError("ReadFloatTexture");
    return outputMat;
}
