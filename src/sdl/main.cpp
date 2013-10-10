// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../diag/logger.h"
#include "../diag/timer.h"
#include "../misc/vec2.h"
#include "../mainscreen/mainscreen.h"
#include "../root/root.h"

#include "../appeventsystem/eventsystem.h"
#include "../appeventsystem/touchevent.h"

using namespace drash;

void WindowSpaceToScreenSpace(CVec2f &_from);

static double gWindowWidth = 1366;
static double gWindowHeight = 700;

int main(int, char **)
{
    bool fail = false;
    CRoot root;
    CRoot::CScreenPtr screen(new CMainScreen(root));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG_ERR("SDL_Init() failed");
        return 0;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         24);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  0);

    SDL_WM_SetCaption("Drash", nullptr);

    if (SDL_SetVideoMode(gWindowWidth, gWindowHeight, 32, SDL_HWSURFACE |
                                                          SDL_OPENGL |
                                                          SDL_GL_DOUBLEBUFFER) == nullptr)
    {
        LOG_ERR("SDL_SetVideoMode() failed");
        fail = true;
    }

    glViewport(0, 0, gWindowWidth, gWindowHeight);

    if (glewInit() != GLEW_OK)
    {
		LOG_ERR("glewInit() failed");
        fail = true;
    }

    int img_flags = IMG_INIT_PNG;

    if (IMG_Init(img_flags) != img_flags)
    {
        LOG_ERR("IMG_Init() failed");
        fail = true;
    }

    LOG_INFO("OpenGL version: "<<(const char*)glGetString(GL_VERSION));
    LOG_INFO("Vendor: "<<(const char*)glGetString(GL_VENDOR));
    LOG_INFO("GLSL version: "<<(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    if (fail == false)
    {        
        root.SetScreen(screen);

        glViewport(0, 0, gWindowWidth, gWindowHeight);
        root.GetGrengSystems().GetCameraManager().SetAspectRatio(gWindowWidth / gWindowHeight);
        root.GetScreen()->GetUISystem().SetAspectRatio(gWindowWidth / gWindowHeight);
        root.GetScreen()->GetUISystem().SetWidth(gWindowWidth);

        bool exit = false;
        SDL_Event e;

    //    app->SetQuitHandler([&exit] ()
    //    {
    //        exit = true;
    //    });

        auto update_cursor = [&root] (int _x, int _y)
        {
            CVec2f pos(_x, _y);
            WindowSpaceToScreenSpace(pos);
            //root.GetScreen()->GetEventSystem().SetCursorPos(pos);
//            int x;
//            int y;
//            root.GetScreen()->GetUISystem().ScreenSpaceToUISpace(pos, x, y);
//            root.GetScreen()->GetUISystem().SetCursorPos(x, y);
            root.GetScreen()->GetEventSystem().UpdateTouchPos(pos);
        };

        CTimer timer;
        timer.Reset(true);

        for (;;)
        {
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                {
                    exit = true;
                    break;
                }
//                else if (e.type == SDL_KEYDOWN)
//                {
//                    //root.GetScreen()->GetEventSystem().BeginEvent(ConvertKey(e.key.keysym.sym));
//                }
//                else if (e.type == SDL_KEYUP)
//                {
//                    //root.GetScreen()->GetEventSystem().EndEvent(ConvertKey(e.key.keysym.sym));
//                }
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    update_cursor(e.button.x, e.button.y);
                    root.GetScreen()->GetEventSystem().Touch();
                }
                else if (e.type == SDL_MOUSEBUTTONUP)
                {
                    update_cursor(e.button.x, e.button.y);
                    root.GetScreen()->GetEventSystem().TouchRelease();
                }
                else if (e.type == SDL_MOUSEMOTION)
                {
                    update_cursor(e.motion.x, e.motion.y);
                }
                else if (e.type == SDL_VIDEORESIZE)
                {
                    gWindowWidth = e.resize.w;
                    gWindowHeight = e.resize.h;
                    if (SDL_SetVideoMode(gWindowWidth, gWindowHeight, 32, SDL_HWSURFACE |
                                         SDL_OPENGL |
                                         SDL_GL_DOUBLEBUFFER) == nullptr)
                    {
                        LOG_ERR("SDL_SetVideoMode() failed");
                        exit = true;
                    }

                    glViewport(0, 0, gWindowWidth, gWindowHeight);
                    root.GetGrengSystems().GetCameraManager().SetAspectRatio(gWindowWidth / gWindowHeight);
                    root.GetScreen()->GetUISystem().SetAspectRatio(gWindowWidth / gWindowHeight);
                    root.GetScreen()->GetUISystem().SetWidth(gWindowWidth);
                }
            }

            timer.Tick();
            root.Step(timer.GetDeltaTime());
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            root.Render();
            SDL_GL_SwapBuffers();

            if (exit == true)
            {
                break;
            }
        }
    }

    IMG_Quit();
    SDL_Quit();

    return 0;
}

void WindowSpaceToScreenSpace(CVec2f &_from)
{
    _from.mX /= gWindowWidth;
    _from.mY /= gWindowHeight;

    _from.mX -= 0.5;
    _from.mY -= 0.5;
    _from.mY *= -1;
}
