# WinUtility Contained Edit Test


This example deliberately subclasses (swaps its message map for our own) an edit control, inspecting WM_CHAR, and letting the edit control handle it it's self. 

If you watch the window text, that's the key you typed, but the edit control is non the wiser that we're looking at it's messsages. 

ControlWindowT<TBase> where TBase is any win32 window. 

If users want more functionaliy they're welcome to let me know on github. 