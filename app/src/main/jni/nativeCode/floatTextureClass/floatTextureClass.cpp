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

#include "floatTextureClass.h"
#include "misc.h"
#include "texture.h"

/**
 * Class constructor
 */
FloatTextureClass::FloatTextureClass() {

    MyLOGD("FloatTextureClass::FloatTextureClass");
    initsDone = false;

}

FloatTextureClass::~FloatTextureClass() {

    MyLOGD("FloatTextureClass::FloatTextureClass");

}


void InitLoadReadTexture(){

    int textureWidth = 512;
    int textureHeight = 512;
    int numberOfChannels = 4;

    // initialize a half-float texture
    GLuint textureName;
    InitializeFloatTexture(textureName, textureWidth, textureHeight, numberOfChannels);

    // create a matrix with random entries and convert it to half-floats
    int matType;
    if (numberOfChannels == 1) {
        matType = CV_32F;
    } else if (numberOfChannels == 2) {
        matType = CV_32FC2;
    } else if (numberOfChannels == 3) {
        matType = CV_32FC3;
    } else if (numberOfChannels == 4) {
        matType = CV_32FC4;
    }
    cv::Mat randomMat(textureHeight, textureWidth, matType);
    cv::randu(randomMat, -200, 200); // entries of the mat are chosen in range [-200,200]

    // load the matrix into the texture
    LoadFloatTexture(textureName, textureWidth, textureHeight, numberOfChannels, randomMat);

    // read back contents of the texture into a matrix
    cv::Mat outputMat = ReadFloatTexture(textureName, textureWidth, textureHeight,
                                         numberOfChannels);

    cv::Scalar sumOfDiff = cv::sum(randomMat - outputMat);
    MyLOGD("Sum of difference of entries in 0-th channel = %f", sumOfDiff.col(0));

    // print some entries for comparison
    PrintCVMat(randomMat.col(5).rowRange(1, 4));
    PrintCVMat(outputMat.col(5).rowRange(1, 4));

    
    // this is a dummy check that the texture can be rendered to
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, textureName, 0);
    CheckFramebufferStatus();
    // set the texture as render target
    GLenum attach_mode = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &attach_mode);

}

/**
 * Perform inits, create objects for detecting corners and rendering image
 */
void FloatTextureClass::PerformGLInits() {

    MyLOGD("FloatTextureClass::PerformGLInits");

    glesVersion = MyGLInits();

    if(glesVersion != 3) {
        // cannot proceed further since GLES 3 is not available
        initsDone = true;
        return;
    }

    // create FBO for offscreen rendering
    GLuint fb;
    glGenFramebuffers(1, &fb);
    // bind offscreen framebuffer (that is, skip the window-specific render target)
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    InitLoadReadTexture();

    // bind default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    CheckGLError("FloatTextureClass::PerformGLInits");

}


/**
 * Render to the display
 */
void FloatTextureClass::Render() {

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    CheckGLError("FloatTextureClass::Render");

}

/**
 * set the viewport, function is also called when user changes device orientation
 */
void FloatTextureClass::SetViewport(int width, int height) {

    screenHeight = height;
    screenWidth = width;
    glViewport(0, 0, width, height);
    CheckGLError("Cube::SetViewport");
}
