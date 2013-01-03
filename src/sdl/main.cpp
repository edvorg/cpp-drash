// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#include <SDL/SDL.h>
#include <GL/gl.h>
#include "../app/app.h"
#include "../test/test.h"
#include "../diag/logger.h"
#include "../diag/timer.h"

using namespace drash;

EventKey ConvertKey(SDLKey _key);
EventKey ConvertButton(int _button);
void WindowSpaceToScreenSpace(CVec2f &_from);

int main(int _argc, char **_argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG_ERR("SDL_Init() failed");
        return 0;
    }

    SDL_WM_SetCaption("Drash", nullptr);

    if (SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL) == nullptr)
    {
        LOG_ERR("SDL_SetVideoMode() failed");
        return 0;
    }

    CApp *app = nullptr;

    for (int i = 0; i < _argc; i++)
    {
        if (strcmp("--test", _argv[i]) == 0)
        {
            if (++i < _argc)
            {
                app = test::StartApp(_argv[i]);

                if (app == nullptr)
                {
                    LOG_ERR("drash::test::StartApp() failed");
                }

                break;
            }
        }
    }

    CTimer timer;
    timer.Reset(true);

    if (app != nullptr)
    {
        if (app->Init() == true)
        {
            glViewport(0, 0, 800, 600);
            app->GetDebugDrawSystem().SetAspectRatio(800.0 / 600.0);

            bool exit = false;
            SDL_Event e;

            app->SetQuitHandler([&exit] ()
            {
                exit = true;
            });

            for (;;)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        exit = true;
                        break;
                    }
                    else if (e.type == SDL_KEYDOWN)
                    {
                        app->GetEventSystem().BeginEvent(ConvertKey(e.key.keysym.sym));
                    }
                    else if (e.type == SDL_KEYUP)
                    {
                        app->GetEventSystem().EndEvent(ConvertKey(e.key.keysym.sym));
                    }
                    else if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        CVec2f pos(e.button.x, e.button.y);
                        WindowSpaceToScreenSpace(pos);
                        app->SetCursorPos(pos);
                        app->GetEventSystem().BeginEvent(ConvertButton(e.button.button));
                    }
                    else if (e.type == SDL_MOUSEBUTTONUP)
                    {
                        CVec2f pos(e.button.x, e.button.y);
                        WindowSpaceToScreenSpace(pos);
                        app->SetCursorPos(pos);
                        app->GetEventSystem().EndEvent(ConvertButton(e.button.button));
                    }
                    else if (e.type == SDL_MOUSEMOTION)
                    {
                        CVec2f pos(e.motion.x, e.motion.y);
                        WindowSpaceToScreenSpace(pos);
                        app->SetCursorPos(pos);
                    }
                }

                timer.Tick();
                app->Step(timer.GetDeltaTime());
                glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                app->Render();
                SDL_GL_SwapBuffers();

                if (exit == true)
                {
                    break;
                }
            }
        }
        else
        {
            LOG_ERR("CApp::Init() failed");
        }

        app->Release();
        delete app;
        app = nullptr;
    }

    SDL_Quit();

    return 0;
}

drash::EventKey ConvertKey(SDLKey _key)
{
    switch (_key)
    {
    case SDLK_q:
        return drash::EventKeyQ;
    case SDLK_w:
        return drash::EventKeyW;
    case SDLK_e:
        return drash::EventKeyE;
    case SDLK_r:
        return drash::EventKeyR;
    case SDLK_a:
        return drash::EventKeyA;
    case SDLK_s:
        return drash::EventKeyS;
    case SDLK_d:
        return drash::EventKeyD;
    case SDLK_f:
        return drash::EventKeyF;
    case SDLK_z:
        return drash::EventKeyZ;
    case SDLK_x:
        return drash::EventKeyX;
    case SDLK_c:
        return drash::EventKeyC;
    case SDLK_v:
        return drash::EventKeyV;
    case SDLK_SPACE:
        return drash::EventKeySpace;
    case SDLK_ESCAPE:
        return drash::EventKeyEscape;
    case SDLK_LCTRL:
        return drash::EventKeyControl;
    case SDLK_LSHIFT:
        return drash::EventKeyShift;
    case SDLK_LALT:
        return drash::EventKeyAlt;
    case SDLK_LMETA:
        return drash::EventKeyMeta;
    default:
        return drash::EventKeyUnknown;
    }
}

drash::EventKey ConvertButton(int _button)
{
    switch (_button)
    {
    case SDL_BUTTON_LEFT:
        return drash::EventKeyLeftButton;
    case SDL_BUTTON_RIGHT:
        return drash::EventKeyRightButton;
    case SDL_BUTTON_MIDDLE:
        return drash::EventKeyMiddleButton;
    default:
        return drash::EventKeyUnknown;
    }
}

void WindowSpaceToScreenSpace(CVec2f &_from)
{
    _from.mX /= 800.0;
    _from.mY /= 600.0;

    _from.mX -= 0.5;
    _from.mY -= 0.5;
    _from.mY *= -1;
}
