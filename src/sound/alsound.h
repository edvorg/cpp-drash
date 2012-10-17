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
