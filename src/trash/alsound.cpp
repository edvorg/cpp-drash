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

#include "alsound.h"
#include <iostream>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <fstream>
#include <map>
#include <cstring>

namespace Sound
{

using namespace std;
    
class Block
{
public:
    void *data;
    size_t size;
    size_t red;
};
    
typedef std::map< ALuint, unsigned int > BuffersT;
    
static ALCdevice* g_Device = NULL;
static ALCcontext* g_Context = NULL;
static bool g_SysInitialized = false;
static unsigned int g_TotalInstances = 0;
static BuffersT g_Buffers;

size_t ReadOgg( void *_ptr, size_t _size, size_t _nmemb, void *_datasource );
int SeekOgg( void *_datasource, ogg_int64_t _offset, int _whence );
long TellOgg( void *_datasource);
int CloseOgg( void *_datasource);

AlSound::AlSound():
    mSourceId(0),
    mBufferId(0)
{
    g_TotalInstances++;
    
    if ( g_TotalInstances == 1 )
    {
        if (!InitSubsystem())
        {
            return;
        }
    }
    
    alGenSources( 1, &mSourceId );    
    alGenBuffers( 1, &mBufferId );
    g_Buffers.insert( make_pair( mBufferId, 1 ) );
    cout<<"AlSound() "<<mBufferId<<endl;
    
    this->SetPitch(1.0f);
    this->SetGain(1.0f);
    this->SetPos( 0, 0, 0 );
    this->SetVel( 0, 0, 0 );
    this->SetLoop(false);
}
    
AlSound::AlSound( const AlSound &_src )
{  
    g_TotalInstances++;
    
    alGenSources( 1, &mSourceId );
    
    mBufferId = _src.mBufferId;
    BuffersT::iterator i = g_Buffers.find( mBufferId );
    if ( i != g_Buffers.end() )
    {
        i->second++;
    }
    cout<<"AlSound(AlSound&) "<<mBufferId<<endl;
    
    this->SetPitch(1.0f);
    this->SetGain(1.0f);
    this->SetPos( 0, 0, 0 );
    this->SetVel( 0, 0, 0 );
    this->SetLoop(false);
}
    
AlSound::~AlSound()
{
    cout<<"~AlSound() "<<mBufferId<<endl;
    g_TotalInstances--;
    
    if (!g_SysInitialized)
    {
        return;
    }
    
    BuffersT::iterator i = g_Buffers.find( mBufferId );
    if ( i != g_Buffers.end() )
    {
        i->second--;
        
        if ( i->second == 0 )
        {
            alDeleteBuffers( 1, &mBufferId );
            g_Buffers.erase(i);
        }
    }

    alDeleteSources( 1, &mSourceId );
    
    if ( g_TotalInstances == 0 )
    {
        ReleaseSubsystem();
    }
}
    
AlSound &AlSound::operator =( const AlSound &_src )
{
    cout<<"operator=(AlSound&) "<<mBufferId<<" to "<<_src.mBufferId<<endl;
    BuffersT::iterator i = g_Buffers.find( mBufferId );
    if ( i != g_Buffers.end() )
    {
        i->second--;
        
        if ( i->second == 0 )
        {
            alDeleteBuffers( 1, &mBufferId );
            g_Buffers.erase(i);
        }
    }
    
    mBufferId = _src.mBufferId;
    i = g_Buffers.find( mBufferId );
    if ( i != g_Buffers.end() )
    {
        i->second++;
    }
    
    return *this;
}
    
bool AlSound::IsValid() const
{
    return g_SysInitialized && alIsBuffer(mBufferId) && alIsSource(mSourceId);
}
    
void AlSound::SetListenerPos( float _x, float _y, float _z )
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alListener3f( AL_POSITION, _x, _y, _z );
}

void AlSound::SetListenerVel( float _x, float _y, float _z )
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alListener3f( AL_VELOCITY, _x, _y, _z );
}

void AlSound::SetListenerOri( float _x, float _y, float _z )
{  
    if (!g_SysInitialized)
    {
        return;
    }
    
    alListener3f( AL_ORIENTATION, _x, _y, _z );
}
    
bool AlSound::LoadFromFile( const char *_path )
{
    if (!g_SysInitialized)
    {
        return false;
    }
    
    if ( _path == NULL )
    {
        cout<<"_path is NULL"<<endl;
        return false;
    }
    
    if ( strstr( _path, ".ogg") == NULL )
    {
        cout<<"only ogg supported"<<endl;
        return false;
    }
    
    char* buf = NULL;
    size_t size = 0;
    FILE *file = NULL;
    
    file = fopen( _path, "rb" );
    
    if (file == NULL)
    {
        cout<<"filee "<<_path<<" not found"<<endl;
        return false;
    }
    
    fseek( file, 0, SEEK_END );
    size = ftell(file);
    fseek( file, 0, SEEK_SET );
    
    if ( size <= 0 )
    {
        return false;
    }
    
    buf = new char[size];
    
    if ( fread( buf, size, 1, file ) != 1 )
    {
        delete [] buf;
        fclose(file);
        return false;
    }
    
    this->LoadFromMem( buf, size );
    
    delete [] buf;
    fclose(file);
    
    cout<<_path<<" loaded"<<endl;
    
    return true;
}

bool AlSound::LoadFromMem( void *_data, size_t _size )
{
    if (!g_SysInitialized)
    {
        return false;
    }
    
    ov_callbacks callbacks;
    int block_size = 0;
    OggVorbis_File vorbis_file;
    vorbis_info *info = NULL;
    int current_section = 0;
    long total_ret = 0;
    long ret = 0;
    char *PCM = NULL;

    callbacks.close_func = CloseOgg;
    callbacks.read_func = ReadOgg;
    callbacks.seek_func = SeekOgg;
    callbacks.tell_func = TellOgg;
    
    Block block;
    block.data = _data;
    block.size = _size;
    block.red = 0;
    
    if ( ov_open_callbacks( &block, &vorbis_file, NULL, -1, callbacks ) < 0 )
    {
        cout<<"error 3";
        return false;
    }

    block_size = ov_pcm_total( &vorbis_file, -1 ) * 4;
    
    if (block_size < 1)
    {
        return false;
    }
    
    PCM = new char[block_size];
    
    info = ov_info( &vorbis_file, -1 );
    
    // read cycle
    while ( total_ret < block_size )
    {
        ret = ov_read( &vorbis_file,
                      PCM + total_ret,
                      block_size - total_ret,
                      0,
                      2,
                      1,
                      &current_section );
        
        if (ret == 0)// end of file
        {
            break;
        }
        else if (ret < 0)// stream error
        {
            delete [] PCM;
            ov_clear(&vorbis_file);
            return false;
        }
        else
        {
            total_ret += ret;
        }
    }
    
    if ( total_ret > 0 )
    {
        alBufferData( mBufferId,
                     (info->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
                     (void *)PCM,
                     total_ret,
                     info->rate );
    }
    
    delete [] PCM;
    
    ov_clear(&vorbis_file);
    
    cout<<_size<<" bytes loaded"<<endl;
    
    return true;
}
    
void AlSound::Play()
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alSourcei( mSourceId, AL_BUFFER, mBufferId );
    alSourcePlay(mSourceId);
}

void AlSound::Pause()
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alSourcePause(mSourceId);
}

void AlSound::Stop()
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alSourceStop(mSourceId);
}
    
void AlSound::SetLoop( bool _loop )
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alSourcei( mSourceId, AL_LOOPING, _loop ? 1 : 0 );
}
    
void AlSound::SetPitch( float _pitch )
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alSourcef( mSourceId, AL_PITCH, _pitch );
}
    
void AlSound::SetGain( float _gain )
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alSourcef( mSourceId, AL_GAIN, _gain );
}
    
void AlSound::SetPos( float _x, float _y, float _z )
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alSource3f( mSourceId, AL_POSITION, _x, _y, _z );
}
    
void AlSound::SetVel( float _x, float _y, float _z )
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alSource3f( mSourceId, AL_VELOCITY, _x, _y, _z );
}
    
float AlSound::GetFullTime()
{
    if (!g_SysInitialized)
    {
        return 0;
    }

    return 0;
}
    
float AlSound::GetTime()
{ 
    if (!g_SysInitialized)
    {
        return 0;
    }

    return 0;
}
    
bool AlSound::InitSubsystem()
{
    if (g_SysInitialized)
    {
        return true;
    }
    
    g_Device = alcOpenDevice(NULL);
    
    if ( g_Device == NULL )
    {
        cout<<"error 1";
        return false;
    }
    
    g_Context = alcCreateContext( g_Device, NULL );
    
    alcMakeContextCurrent(g_Context);
    
    if ( g_Context == NULL )
    {
        cout<<"error 2";
        return false;
    }
    
    g_SysInitialized = true;
    
    SetListenerPos( 0, 0, 0 );
    SetListenerVel( 0, 0, 0 );
    SetListenerOri( 0, 0, 0 );
    
    cout<<"OpenAl initialized"<<endl;
    
    return true;
}

void AlSound::ReleaseSubsystem()
{
    if (!g_SysInitialized)
    {
        return;
    }
    
    alcMakeContextCurrent(NULL);
    
    if (g_Context != NULL)
    {
        alcDestroyContext(g_Context);
        g_Context = NULL;
    }
    
    if (g_Device != NULL)
    {
        alcCloseDevice(g_Device);
        g_Device = NULL;
    }
    
    g_SysInitialized = false;
    
    cout<<"OpenAl released"<<endl;
}
    
size_t ReadOgg( void *_ptr, size_t _size, size_t _nmemb, void *_datasource )
{
    auto b = (Block*)_datasource;
    size_t toread = 0;
    
    if ( (char*)b->data + b->red < (char*)b->data + b->size )
    {
        toread = min( _nmemb * _size, b->size - b->red );
        memcpy( (char*)_ptr, (const char*)b->data + b->red, toread );
        b->red += toread;
        return toread;
    }
    else
    {
        return 0;
    }
}

int SeekOgg( void *_datasource, ogg_int64_t _offset, int _whence )
{
    auto b = (Block*)_datasource;
    size_t pos = 0;
    
    switch (_whence)
    {
        case SEEK_SET:
            pos = _offset;
            break;
            
        case SEEK_CUR:
            pos = b->red + _offset;
            break;
            
        case SEEK_END:
            pos = b->size;
            break;
            
        default:
            pos = 0;
            break;
    }
    
    if ( b->size < pos )
    {
        return -1;
    }
    
    b->red = pos;
    
    return 0;
}

long TellOgg( void *_datasource )
{
    auto b = (Block*)_datasource;
    return b->red;
}

int CloseOgg( void *_datasource )
{
    return 0;
}

}
