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

#include "myGLFunctions.h"
#include <sstream>
#include "myLogger.h"

/**
 * Basic initializations for GL.
 */
int MyGLInits() {

    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LEQUAL);

    MyLOGI("OpenGL %s, GLSL %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    // check if the device supports GLES 3 or GLES 2
    const char* versionStr = (const char*)glGetString(GL_VERSION);
    int glesVersion;
    if (strstr(versionStr, "OpenGL ES 3.") && gl3stubInit()) {
        MyLOGD("Device supports GLES 3");
        glesVersion = 3;
    } else {
        MyLOGD("Device supports GLES 2");
        glesVersion = 2;
    }

    CheckGLError("MyGLInits");
    return glesVersion;
}

/**
 * Checks for OpenGL errors.
 */
void CheckGLError(std::string funcName){

    GLenum err = glGetError();
    if (err == GL_NO_ERROR) {
        return;
    } else {
        MyLOGF("[FAIL GL] %s", funcName.c_str());
    }

    switch(err) {

        case GL_INVALID_ENUM:
            MyLOGE("GL_INVALID_ENUM: GLenum argument out of range");
            break;

        case GL_INVALID_VALUE:
            MyLOGE("GL_INVALID_VALUE: numeric argument out of range");
            break;

        case GL_INVALID_OPERATION:
            MyLOGE("GL_INVALID_OPERATION: operation illegal in current state");
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            MyLOGE("GL_INVALID_FRAMEBUFFER_OPERATION: framebuffer object is not complete");
            break;

        case GL_OUT_OF_MEMORY:
            MyLOGE( "GL_OUT_OF_MEMORY: not enough memory left to execute command");
            break;

        default:
            MyLOGE("unlisted error");
            break;
    }
}


/**
 * Checks framebuffer status.
 * Copied out of OpenGL ES spec, modified to deliver a return value.
 */
int CheckFramebufferStatus(){

    GLenum status =
            (GLenum) glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE) {
        return 0;
    } else {
        MyLOGD("[FAIL FBO]");
    }

    switch(status) {

        case GL_FRAMEBUFFER_UNDEFINED:
            MyLOGD("GL_FRAMEBUFFER_UNDEFINED");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            MyLOGD("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            MyLOGD("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            MyLOGD("GL_FRAMEBUFFER_UNSUPPORTED");
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            MyLOGD("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
            break;

        default:
            MyLOGD("FBO unlisted error");
            break;
    }

    return -1;
}