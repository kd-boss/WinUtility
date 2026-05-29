#include <WinUtility\System.h>
#include <WinUtility\BaseWindow.h>
#include <vector>
#include <cmath>
#include <mmeapi.h>

class WaveOut
{
    std::vector<byte> m_abuff,m_bbuff;
    unsigned int m_sampleRate;
    unsigned int m_freqMin,m_freqMax,m_freqInit,iFreq;
    double fAngle;
    HWAVEOUT hWaveOut;
    PWAVEHDR pWaveHdr1, pWaveHdr2;
    void FillBuffer(std::vector<byte>&, int iFreq);   
    bool bShutOff;

public:
    WaveOut();
    ~WaveOut();
    void Stop();
    void Start(int iFMin, int iFMax, int iFStart, Window owner);
    void Start(int iFMin, int iFMax, int iFStart);
    void OnOpen();
    void OnDone(LPARAM lparam);
    void OnClose();
};