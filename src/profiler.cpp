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

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>

#include "windows.h"
#include "profiler.h"





//void* (*new)(size_t, const std::experimental::source_location&);
/*#define PROFILE 1
#if !PROFILE
void* operator new(size_t size) {
    //std::cout << boost::stacktrace::stacktrace() << "\n";
    std::cout << "Allocated " << size << " bytes of memory!\n";
    return malloc(size);
}

void operator delete(void* data) {
    std::cout << "Freed some memory!\n";
    free(data);
}
#endif*/

Instrumentor::Instrumentor()
    : m_CurrentSession(nullptr), m_ProfileCount(0)
{
}

void Instrumentor::BeginSession(const std::string& name, const std::string& filepath = "results.json")
{
    m_OutputStream.open(filepath);
    WriteHeader();
    m_CurrentSession = new InstrumentationSession{ name };
}

void Instrumentor::EndSession()
{
    WriteFooter();
    m_OutputStream.close();
    delete m_CurrentSession;
    m_CurrentSession = nullptr;
    m_ProfileCount = 0;
}

void Instrumentor::WriteProfile(const ProfileResult& result)
{
    if (m_ProfileCount++ > 0)
        m_OutputStream << ",";

    std::string name = result.Name;
    std::replace(name.begin(), name.end(), '"', '\'');

    m_OutputStream << "{";
    m_OutputStream << "\"cat\":\"function\",";
    m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
    m_OutputStream << "\"name\":\"" << name << "\",";
    m_OutputStream << "\"ph\":\"X\",";
    m_OutputStream << "\"pid\":0,";
    m_OutputStream << "\"tid\":0,";
    m_OutputStream << "\"ts\":" << result.Start;
    m_OutputStream << "}";

    m_OutputStream.flush();
}

void Instrumentor::WriteHeader()
{
    m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
    m_OutputStream.flush();
}

void Instrumentor::WriteFooter()
{
    m_OutputStream << "]}";
    m_OutputStream.flush();
}

Instrumentor& Instrumentor::Get()
{
    static Instrumentor instance;
    return instance;
}


InstrumentationTimer::InstrumentationTimer(const char* name)
    : m_Name(name), m_Stopped(false)
{
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

InstrumentationTimer::~InstrumentationTimer()
{
    if (!m_Stopped)
        Stop();
}

void InstrumentationTimer::Stop()
{
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

    Instrumentor::Get().WriteProfile({ m_Name, start, end });

    m_Stopped = true;

}

