//
//  File.cpp
//  openaltest
//
//  Created by Edward Knyshov on 10/16/12.
//  Copyright (c) 2012 Alawar Stargaze. All rights reserved.
//

#include "AlSound.h"
#include <iostream>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <fstream>

namespace Sound
{

using namespace std;

static ALCdevice* g_Device = nullptr;
static ALCcontext* g_Context = nullptr;
    
class Block
{
public:
    void *data;
    size_t size;
    size_t red;
};
    
size_t ReadOgg(void *ptr, size_t size, size_t nmemb, void *datasource);
int SeekOgg(void *datasource, ogg_int64_t offset, int whence);
long TellOgg(void *datasource);
int CloseOgg(void *datasource);

bool AlSound::mSysInitialized = false;
unsigned int AlSound::mTotalInstances = 0;

AlSound::AlSound():
    mSourceId(0),
    mBufferId(0)
{
    mTotalInstances++;
    
    if ( mTotalInstances == 1 )
    {
        if (!InitSubsystem())
        {
            return;
        }
    }
    
    alGenSources( 1, &mSourceId );
    alGenBuffers( 1, &mBufferId );
    
    this->SetPitch(1.0f);
    this->SetGain(1.0f);
    this->SetPos( 0, 0, 0 );
    this->SetVel( 0, 0, 0 );
    this->SetLoop(false);
}

AlSound::~AlSound()
{
    mTotalInstances--;
    
    if (!mSysInitialized)
    {
        return;
    }
    
    if (mBufferId)
    {
        alDeleteBuffers( 1, &mBufferId );
        mBufferId = 0;
    }
    
    if (mSourceId)
    {
        alDeleteSources( 1, &mSourceId );
        mSourceId = 0;
    }
    
    if ( mTotalInstances == 0 )
    {
        ReleaseSubsystem();
    }
}

void AlSound::SetListenerPos( float _x, float _y, float _z )
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alListener3f( AL_POSITION, _x, _y, _z );
}

void AlSound::SetListenerVel( float _x, float _y, float _z )
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alListener3f( AL_VELOCITY, _x, _y, _z );
}

void AlSound::SetListenerOri( float _x, float _y, float _z )
{  
    if (!mSysInitialized)
    {
        return;
    }
    
    alListener3f( AL_ORIENTATION, _x, _y, _z );
}
    
bool AlSound::LoadFromFile( const char *_path )
{
    if (!mSysInitialized)
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
    
    return true;
}

bool AlSound::LoadFromMem( void *_data, size_t _size )
{
    if (!mSysInitialized)
    {
        return false;
    }
    
    ov_callbacks callbacks = {0};
    int block_size = 0;
    OggVorbis_File vorbis_file = {0};
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
    
    alSourcei( mSourceId, AL_BUFFER, mBufferId );
    
    return false;
}
    
void AlSound::Play()
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alSourcePlay(mSourceId);
}

void AlSound::Pause()
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alSourcePause(mSourceId);
}

void AlSound::Stop()
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alSourceStop(mSourceId);
}
    
void AlSound::SetLoop( bool _loop )
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alSourcei( mSourceId, AL_LOOPING, _loop ? 1 : 0 );
}
    
void AlSound::SetPitch( float _pitch )
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alSourcef( mSourceId, AL_PITCH, 1.0f );
}
    
void AlSound::SetGain( float _gain )
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alSourcef( mSourceId, AL_GAIN, 1.0f );
}
    
void AlSound::SetPos( float _x, float _y, float _z )
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alSource3f( mSourceId, AL_POSITION, 0, 0, 0 );
}
    
void AlSound::SetVel( float _x, float _y, float _z )
{
    if (!mSysInitialized)
    {
        return;
    }
    
    alSource3f( mSourceId, AL_VELOCITY, 0, 0, 0 );
}
    
bool AlSound::InitSubsystem()
{
    if (mSysInitialized)
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
    
    mSysInitialized = true;
    
    SetListenerPos( 0, 0, 0 );
    SetListenerVel( 0, 0, 0 );
    SetListenerOri( 0, 0, 0 );
    
    return true;
}

void AlSound::ReleaseSubsystem()
{
    if (!mSysInitialized)
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
    
    mSysInitialized = false;
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
    
    if ( pos < 0 || b->size < pos )
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