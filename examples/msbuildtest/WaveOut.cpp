#include "WaveOut.h"
#include <string>
#include <numbers>

WaveOut::WaveOut() : fAngle(0.0), m_sampleRate(11025 * 2), bShutOff(false), hWaveOut(nullptr),pWaveHdr1(nullptr),pWaveHdr2(nullptr),iFreq(440)
{
    m_abuff.resize(11025 * 4,0);
    m_bbuff.resize(11025 * 4,0);
}   

WaveOut::~WaveOut() 
{
    
    if(pWaveHdr1)
    {
        waveOutUnprepareHeader(hWaveOut,pWaveHdr1,sizeof(WAVEHDR)); 
        delete pWaveHdr1;
    }
    if(pWaveHdr2)
    {
        waveOutUnprepareHeader(hWaveOut, pWaveHdr2,sizeof(WAVEHDR));
        delete pWaveHdr2; 
    } 
        
    if(hWaveOut) waveOutClose(hWaveOut);
}

void WaveOut::FillBuffer(std::vector<byte>& buff ,int iFreq)
{
    for(auto a = buff.begin(); a != buff.end(); a++)
    {
        auto sinval = static_cast<float>(std::sin(fAngle));
        *a = (byte)(int)(255.0f * sinval);
        fAngle += 1.0f * (2.0f * std::numbers::pi * iFreq / m_sampleRate) + 0.15f * (2.0f * std::numbers::pi * (2.0f * iFreq) / m_sampleRate) + 0.10f * (2.0f * std::numbers::pi * (3.0f * iFreq) / m_sampleRate); 
        if(fAngle > (2.0 * std::numbers::pi))
        {
            fAngle -= (2.0 * std::numbers::pi);
        }
    }
}

void WaveOut::Stop()
{
    if(!bShutOff)
        bShutOff = true;
    
}

void WaveOut::Start(int iFMin, int iFMax, int iFStart, Window owner)
{
    iFreq = iFStart;
    if(hWaveOut == nullptr)
    {
        pWaveHdr1 = new WAVEHDR();
        pWaveHdr2 = new WAVEHDR();
        WAVEFORMATEX waveFormat{0};

        waveFormat.wFormatTag = WAVE_FORMAT_PCM;
        waveFormat.nChannels = 1;
        waveFormat.nSamplesPerSec = m_sampleRate;
        waveFormat.nAvgBytesPerSec = m_sampleRate;
        waveFormat.nBlockAlign = 1;
        waveFormat.wBitsPerSample = 8;
        waveFormat.cbSize = sizeof(WAVEFORMATEX);
        
        if(waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat,(DWORD_PTR)owner.m_hwnd,0, CALLBACK_WINDOW) != MMSYSERR_NOERROR)
        {
            delete pWaveHdr1;
            delete pWaveHdr2;
            hWaveOut = NULL;
            MessageBox::Show(TEXT("Error opening waveform audio device!"),TEXT("wintest"),MessageBoxButtons::Ok,MessageBoxIcon::Exclamation);
            return;
        }

        pWaveHdr1->lpData = (LPSTR)  &m_abuff[0];
        pWaveHdr1->dwBufferLength =  m_abuff.size();
        pWaveHdr1->dwBytesRecorded = 0;
        pWaveHdr1->dwUser =          0;
        pWaveHdr1->dwFlags =         0;
        pWaveHdr1->dwLoops =         1;
        pWaveHdr1->lpNext =          NULL;
        pWaveHdr1->reserved =        0;

        waveOutPrepareHeader(hWaveOut,pWaveHdr1, sizeof(WAVEHDR));

        pWaveHdr2->lpData = (LPSTR)  &m_bbuff[0];
        pWaveHdr2->dwBufferLength =  m_abuff.size();
        pWaveHdr2->dwBytesRecorded = 0;
        pWaveHdr2->dwUser =          0;
        pWaveHdr2->dwFlags =         0;
        pWaveHdr2->dwLoops =         1;
        pWaveHdr2->lpNext =          NULL;
        pWaveHdr2->reserved =        0;
        
        waveOutPrepareHeader(hWaveOut,pWaveHdr2, sizeof(WAVEHDR));

    }
    else
    {
        bShutOff = TRUE;
        waveOutReset(hWaveOut);    
    }
}

void WaveOut::OnOpen()
{
    FillBuffer(m_abuff,iFreq);
    waveOutWrite(hWaveOut,pWaveHdr1, sizeof(WAVEHDR));

    FillBuffer(m_bbuff,iFreq);
    waveOutWrite(hWaveOut, pWaveHdr2, sizeof(WAVEHDR));
}

void WaveOut::OnDone(LPARAM lparam)
{
    if(bShutOff)
    {
        waveOutClose(hWaveOut);
    }
    PWAVEHDR hdr = (PWAVEHDR)lparam;
    if(hdr == pWaveHdr1)
    {
        FillBuffer(m_abuff,iFreq);
    }   
    else
    {
        FillBuffer(m_bbuff,iFreq);
    }
    waveOutWrite(hWaveOut, hdr, sizeof(WAVEHDR));
}

void WaveOut::OnClose()
{
    waveOutUnprepareHeader(hWaveOut, pWaveHdr1, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, pWaveHdr2, sizeof(WAVEHDR));

    delete pWaveHdr1;
    delete pWaveHdr2;
    hWaveOut = NULL;
}