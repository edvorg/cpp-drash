// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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

#ifndef __openaltest__File__
#define __openaltest__File__

#include <cstdlib>

namespace Sound
{

/*
No manual initialization is required.
Just create an instance and use Load* methods.
*/
class AlSound
{
public:
    explicit AlSound();
    /// when using this constructor, instances will use shred buffer,
    /// which will be destroyed with last sharing instance
    explicit AlSound( const AlSound &_src );
    virtual ~AlSound();
    
    AlSound &operator =( const AlSound &_src );
    
    bool IsValid() const;
    
    /// Listener is one for all AlSound instances,
    /// that's why it's static
    static void SetListenerPos( float _x, float _y, float _z );
    /// Listener is one for all AlSound instances,
    /// that's why it's static
    static void SetListenerVel( float _x, float _y, float _z );
    /// Listener is one for all AlSound instances,
    /// that's why it's static
    static void SetListenerOri( float _x, float _y, float _z );
    
    /// load sound from ogg file
    bool LoadFromFile( const char *_path );
    /// load sound from memory with ogg format
    bool LoadFromMem( void *_data, size_t _size );
    
    void Play();
    void Pause();
    void Stop();
    
    void SetLoop( bool _loop );
    
    void SetPitch( float _pitch );
    void SetGain( float _gain );
    
    void SetPos( float _x, float _y, float _z );
    void SetVel( float _x, float _y, float _z );
    
    float GetFullTime();
    float GetTime();
    
protected:
private:
    /// init sound subsystem
    static bool InitSubsystem();
    /// release sound subsystem
    static void ReleaseSubsystem();    
    
    unsigned int mSourceId;
    unsigned int mBufferId;
};

}

#endif /* defined(__openaltest__File__) */
