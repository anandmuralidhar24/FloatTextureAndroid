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

#ifndef FLOATTEXTURECLASS_H
#define FLOATTEXTURECLASS_H

#include "myLogger.h"
#include "myGLFunctions.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <mutex>

class FloatTextureClass {
public:
    FloatTextureClass();
    ~FloatTextureClass();
    void    PerformGLInits();
    void    Render();
    void    SetViewport(int width, int height);
    int     GetGLESVersion(){return glesVersion;}
    bool    IsInitsDone(){return initsDone;}
private:

    bool    initsDone;
    int     screenWidth, screenHeight;
    int glesVersion;

};

#endif //FLOATTEXTURECLASS_H
