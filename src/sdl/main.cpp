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

    glViewport(0, 0, 800, 600);

    drash::CApp *app = nullptr;

    for (int i = 0; i < _argc; i++)
    {
        if (strcmp("--test", _argv[i]) == 0)
        {
            if (++i < _argc)
            {
                app = drash::test::StartApp(_argv[i]);

                if (app == nullptr)
                {
                    LOG_ERR("drash::test::StartApp() failed");
                }

                break;
            }
        }
    }

    drash::CTimer timer;
    timer.Reset(true);

    if (app != nullptr)
    {
        if (app->Init() == true)
        {
            for (;;)
            {
                bool exit = false;
                SDL_Event e;

                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        exit = true;
                        break;
                    }
                }

                timer.Tick();
                app->Step(timer.GetDeltaTime());
                glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
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
