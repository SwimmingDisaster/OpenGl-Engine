//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>





//void* (*new)(size_t, const std::experimental::source_location&);
/*#define PROFILE 1
#if PROFILE
void* operator new(size_t size);

void operator delete(void* data);
#endif
*/
struct ProfileResult
{
    std::string Name;
    long long Start, End;
};

struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
private:
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;
public:
    Instrumentor();

    void BeginSession(const std::string& name, const std::string& filepath);
    void EndSession();

    void WriteProfile(const ProfileResult& result);

    void WriteHeader();

    void WriteFooter();

    static Instrumentor& Get();
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name);

    ~InstrumentationTimer();

    void Stop();
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};


#if PROFILE
#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)
#define PROFILE_BEGIN_SESSION(name, filePath) Instrumentor::Get().BeginSession(name, filePath)
#define PROFILE_END_SESSION() Instrumentor::Get().EndSession()
#define PROFILE_SCOPE(name) InstrumentationTimer COMBINE(timer,__LINE__)(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_BEGIN_SESSION(name, filePath)
#define PROFILE_END_SESSION()
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif