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

#include <GL/glew.h>
#include <SDL/SDL.h>
#include "../app/app.h"
#include "../test/test.h"
#include "../diag/logger.h"
#include "../diag/timer.h"

using namespace drash;

EventKey ConvertKey(SDLKey _key);
EventKey ConvertButton(int _button);
void WindowSpaceToScreenSpace(CVec2f &_from);

const double gWindowWidth = 800;
const double gWindowHeight = 600;

int main(int _argc, char **_argv)
{
    bool fail = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG_ERR("SDL_Init() failed");
        return 0;
    }

    SDL_WM_SetCaption("Drash", nullptr);

    if (SDL_SetVideoMode(gWindowWidth, gWindowHeight, 32, SDL_HWSURFACE | SDL_OPENGL) == nullptr)
    {
        LOG_ERR("SDL_SetVideoMode() failed");
        fail = true;
    }

    if (glewInit() != GLEW_OK)
    {
		LOG_ERR("glewInit() failed");
        fail = true;
    }

    CApp *app = nullptr;

    if (fail == false)
    {
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
    }

    CTimer timer;
    timer.Reset(true);

    if (fail == false && app != nullptr)
    {
        if (app->Init() == true)
        {
            glViewport(0, 0, gWindowWidth, gWindowHeight);
            app->GetDebugDrawSystem().SetAspectRatio(gWindowWidth / gWindowHeight);
            app->GetUISystem().SetAspectRatio(gWindowWidth / gWindowHeight);
            app->GetUISystem().SetWidth(gWindowWidth);

            bool exit = false;
            SDL_Event e;

            app->SetQuitHandler([&exit] ()
            {
                exit = true;
            });

            auto update_cursor = [&app] (int _x, int _y)
            {
                CVec2f pos(_x, _y);
                WindowSpaceToScreenSpace(pos);
                app->SetCursorPos(pos);
                int x;
                int y;
                app->GetUISystem().ScreenSpaceToUISpace(pos, x, y);
                app->GetUISystem().SetCursorPos(x, y);
            };

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
                        update_cursor(e.button.x, e.button.y);
                        app->GetUISystem().BeginEvent();
                        app->GetEventSystem().BeginEvent(ConvertButton(e.button.button));
                    }
                    else if (e.type == SDL_MOUSEBUTTONUP)
                    {
                        update_cursor(e.button.x, e.button.y);
                        app->GetUISystem().EndEvent();
                        app->GetEventSystem().EndEvent(ConvertButton(e.button.button));
                    }
                    else if (e.type == SDL_MOUSEMOTION)
                    {
                        update_cursor(e.motion.x, e.motion.y);
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
        return EventKeyQ;
    case SDLK_w:
        return EventKeyW;
    case SDLK_e:
        return EventKeyE;
    case SDLK_r:
        return EventKeyR;
    case SDLK_a:
        return EventKeyA;
    case SDLK_s:
        return EventKeyS;
    case SDLK_d:
        return EventKeyD;
    case SDLK_f:
        return EventKeyF;
    case SDLK_z:
        return EventKeyZ;
    case SDLK_x:
        return EventKeyX;
    case SDLK_c:
        return EventKeyC;
    case SDLK_v:
        return EventKeyV;
    case SDLK_SPACE:
        return EventKeySpace;
    case SDLK_ESCAPE:
        return EventKeyEscape;
    case SDLK_LCTRL:
        return EventKeyControl;
    case SDLK_LSHIFT:
        return EventKeyShift;
    case SDLK_LALT:
        return EventKeyAlt;
    case SDLK_LMETA:
        return EventKeyMeta;
    default:
        return EventKeyUnknown;
    }
}

drash::EventKey ConvertButton(int _button)
{
    switch (_button)
    {
    case SDL_BUTTON_LEFT:
        return EventKeyLeftButton;
    case SDL_BUTTON_RIGHT:
        return EventKeyRightButton;
    case SDL_BUTTON_MIDDLE:
        return EventKeyMiddleButton;
    case SDL_BUTTON_WHEELUP:
        return EventKeyWheelUp;
    case SDL_BUTTON_WHEELDOWN:
        return EventKeyWheelDown;
    default:
        return EventKeyUnknown;
    }
}

void WindowSpaceToScreenSpace(CVec2f &_from)
{
    _from.mX /= gWindowWidth;
    _from.mY /= gWindowHeight;

    _from.mX -= 0.5;
    _from.mY -= 0.5;
    _from.mY *= -1;
}
