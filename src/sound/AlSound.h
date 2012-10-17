//
//  File.h
//  openaltest
//
//  Created by Edward Knyshov on 10/16/12.
//  Copyright (c) 2012 Alawar Stargaze. All rights reserved.
//

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
    AlSound();
    ~AlSound();
    
    /// Listener is one for all AlSound instances,
    /// that's why it's static
    static void SetListenerPos( float _x, float _y, float _z );
    static void SetListenerVel( float _x, float _y, float _z );
    static void SetListenerOri( float _x, float _y, float _z );
    
    bool LoadFromFile( const char *_path );
    bool LoadFromMem( void *_data, size_t _size );
    void Play();
    void Pause();
    void Stop();
    
    void SetLoop( bool _loop );
    
    void SetPitch( float _pitch );
    void SetGain( float _gain );
    
    void SetPos( float _x, float _y, float _z );
    void SetVel( float _x, float _y, float _z );
    
protected:
private:
    /// init sound subsystem
    static bool InitSubsystem();
    /// release sound subsystem
    static void ReleaseSubsystem();
    
    static bool mSysInitialized;
    static unsigned int mTotalInstances;
    
    unsigned int mSourceId;
    unsigned int mBufferId;
};

}

#endif /* defined(__openaltest__File__) */
