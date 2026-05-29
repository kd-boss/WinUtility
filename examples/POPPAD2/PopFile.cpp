#include "poppad2.h"
#include <format>
#include <fstream>
#include <shlobj.h>
#include <objbase.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <knownfolders.h>
#include <propvarutil.h>
#include <propkey.h>
#include <strsafe.h>
#include <shtypes.h>
#include <combaseapi.h>
#include <filesystem>

void PopPad2::PopFileInitialize()
{
}

// BOOL PopFileOpenDlg(PTSTR szFileName);
// BOOL PopFileSaveDlg(PTSTR szFilePath);
// BOOL PopFileRead(PCTSTR szFileToRead);
// BOOL PopFileWrite(PTSTR szFileToWrite);
BOOL PopPad2::PopFileOpenDlg(std::tstring &fileName, const std::tstring &dialogTitle)
{

    COMDLG_FILTERSPEC c_rgSaveTypes[] = {
        {TEXT("Text Files (*.TXT)"), TEXT("*.txt")},
        {TEXT("ASCII Files (*.ASC)"), TEXT("*.asc")},
        {TEXT("All Files (*.*)"), TEXT("*.*")}};

    IFileDialog *pfd;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        DWORD dwFlags;
        hr = pfd->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
            if (SUCCEEDED(hr))
            {
                hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                if (SUCCEEDED(hr))
                {
                    hr = pfd->SetDefaultExtension(TEXT("*.TXT"));
                    if (SUCCEEDED(hr))
                    {
                        hr = pfd->Show(NULL);
                        if (SUCCEEDED(hr))
                        {
                            IShellItem *psiResult;
                            hr = pfd->GetResult(&psiResult);
                            if (SUCCEEDED(hr))
                            {
                                PWSTR szFilePath = NULL;
                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &szFilePath);
                                if (SUCCEEDED(hr))
                                {
                                    fileName = szFilePath;
                                    psiResult->Release();
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
        pfd->Release();
    }
    return false;
}

BOOL PopPad2::PopFileSaveDlg(std::tstring &fileName, const std::tstring &dialogTitle)
{
    COMDLG_FILTERSPEC c_rgSaveTypes[] = {
        {TEXT("Text Files (*.TXT)"), TEXT("*.txt")},
        {TEXT("ASCII Files (*.ASC)"), TEXT("*.asc")},
        {TEXT("All Files (*.*)"), TEXT("*.*")}};

    IFileDialog *pfd;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        DWORD dwFlags;
        hr = pfd->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
            if (SUCCEEDED(hr))
            {
                hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                if (SUCCEEDED(hr))
                {
                    hr = pfd->SetDefaultExtension(TEXT("*.TXT"));
                    if (SUCCEEDED(hr))
                    {
                        hr = pfd->Show(NULL);
                        if (SUCCEEDED(hr))
                        {
                            IShellItem *psiResult;
                            hr = pfd->GetResult(&psiResult);
                            if (SUCCEEDED(hr))
                            {
                                PWSTR szFilePath = NULL;
                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &szFilePath);
                                if (SUCCEEDED(hr))
                                {
                                    fileName = szFilePath;
                                    psiResult->Release();
                                    CoTaskMemFree(szFilePath);
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
        pfd->Release();
    }
    return false;
}

BOOL PopPad2::PopFileRead(const std::tstring &fileName)
{
    System::Utility::invalid_handle m_fileHandle;
    
    m_fileHandle.reset(CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL));
    if (m_fileHandle)
    {
        auto iFileLength = GetFileSize(m_fileHandle.get(), NULL);
        buffer.resize(iFileLength);
        DWORD dwBytesRead = 0;
        ReadFile(m_fileHandle.get(), &buffer[0], buffer.size(), &dwBytesRead, NULL);
        auto iUiniTest = IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_REVERSE_SIGNATURE;
        if (IsTextUnicode(buffer.data(), buffer.size(), &iUiniTest))
        {
            auto pText = (wchar_t *)&buffer[2];
            auto iFileLength = dwBytesRead - 2;
            if (iUiniTest & IS_TEXT_UNICODE_REVERSE_SIGNATURE)
            {
                for (unsigned int i = 0; i < iFileLength / 2; i++)
                {
                    if(i < iFileLength)
                    std::swap(pText[2 * i], pText[2 * i + 1]);
                }
            }
            m_edit.SetWindowText(pText);
        }
        else
        {
            std::string dat = {buffer.begin(), buffer.end()};
            lbuffer = System::Utility::string_convert(dat);
            m_edit.SetWindowText(lbuffer.c_str());
        }
    }
    return TRUE;
}

BOOL PopPad2::PopFileWrite(const std::tstring &fileName)
{
    std::tstring text;
    m_edit.GetWindowText(text);
    std::ios::openmode mode;
    if (std::filesystem::exists(fileName))
    {
        auto res = MessageBox::Show(std::format(TEXT("{:s} already exists, do you wish to overwrite it?"), fileName).c_str(), TEXT("POPPAD2"), MessageBoxButtons::YesNoCancel, MessageBoxIcon::Information);
        switch (res)
        {
        case MessageBoxResult::Yes:
        {
            std::filesystem::remove(fileName);
            mode = std::ios::out | std::ios::app;
        }
        break;
        case MessageBoxResult::No:
        {
            mode = std::ios::out | std::ios::app;
        }
        break;
        case MessageBoxResult::Cancel:
        {
            return FALSE;
        }
        break;
        default:
            return FALSE;
            break;
        }
    }
    else
    {
        mode =  std::ios::out | std::ios::app;
    }
    if (fileName.contains(TEXT(".txt")))
    {

        std::wfstream fs;
        fs.open(fileName.c_str(), mode);
        if (fs.is_open())
        {
            fs.write(text.c_str(), text.length());
            fs.close();
        }
    }
    else if (fileName.contains(TEXT(".asc")))
    {
        std::fstream fs;
        fs.open(System::Utility::string_convert(fileName).c_str(), mode);
        if (fs.is_open())
        {
            fs.write(System::Utility::string_convert(fileName).c_str(), text.length());
            fs.close();
        }
    }
    else
    {
        MessageBox::Show(TEXT("Unknown File extention"), TEXT("POPPAD2"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
    }
    return TRUE;
}

void PopPad2::OnFileOpen(UINT uNotifyCode, int nID, Window wndCtl)
{
    PopFileInitialize();
    std::wstring out(MAX_PATH, L'\0');
    std::wstring label = L"POPPAD2";
    if (PopFileOpenDlg(out, label))
    {
        PopFileRead(out.c_str());
        DoCaption(out.c_str());
        m_filename = out;
    }
}

void PopPad2::OnFileNew(UINT uNotifyCode, int nID, Window wndCtl)
{
    m_edit.SetWindowTextW(TEXT(""));
    m_edit.Invalidate();
    m_filename.clear();
    bNeedSave = true;
    DoCaption(TEXT("Untitled"));
    PopFileInitialize();
}

void PopPad2::OnFileSave(UINT uNotifyCode, int nID, Window wndCtl)
{
    if (m_filename.length() > 0)
    {
        if (PopFileWrite(m_filename.c_str()))
        {
            bNeedSave = false;

            return;
        }
        else
        {
            MessageBox::Show(std::format(TEXT("Could not write file {:s}"), m_filename).c_str(), TEXT("POPPAD2"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
            return;
        }
    }
    PopFileInitialize();
    OnFileSaveAs(uNotifyCode, nID, wndCtl);
}

void PopPad2::OnFileSaveAs(UINT uNotifyCode, int nID, Window wndCtl)
{
    
    m_filename.resize(MAX_PATH, '\0');
    if (PopFileSaveDlg(m_filename, TEXT("POPPAD2")))
    {
        DoCaption(const_cast<wchar_t *>(m_filename.c_str()));
        if (PopFileWrite(m_filename))
        {
            bNeedSave = false;
            return;
        }
        else
        {
            MessageBox::Show(std::format(TEXT("Could not write file {:s}"), m_filename).c_str(), TEXT("POPPAD2"), MessageBoxButtons::Ok, MessageBoxIcon::Information);
        }
    }
    else
    {
        HR(__HRESULT_FROM_WIN32(GetLastError()));
    }
}
