# Microsoft Developer Studio Project File - Name="qw_client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=qw_client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "qw_client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qw_client.mak" CFG="qw_client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qw_client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "qw_client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "qw_client - Win32 GLRelease" (based on "Win32 (x86) Application")
!MESSAGE "qw_client - Win32 GLDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /GX /O2 /I "../scitech/include" /I "../qw_common" /I "../common" /I "../qw_client" /I "../qw_server" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "QUAKEWORLD" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxguid.lib ..\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Gm /GX /ZI /Od /I "../common" /I "../qw_common" /I "qw_client" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "qw_client___Win32_GLRelease"
# PROP BASE Intermediate_Dir "qw_client___Win32_GLRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GLRelease"
# PROP Intermediate_Dir "GLRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /GX /O2 /I "../scitech/include" /I "../qw_common" /I "../common" /I "../qw_client" /I "../qw_server" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "QUAKEWORLD" /FR /YX /FD /c
# ADD CPP /nologo /GX /O2 /I "../scitech/include" /I "../qw_common" /I "../common" /I "../qw_client" /I "../qw_server" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "QUAKEWORLD" /D "GLQUAKE" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ..\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"qwcl_Release/qw_client.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 dxguid.lib ..\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"GLRelease/glqw_client.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "qw_client___Win32_GLDebug"
# PROP BASE Intermediate_Dir "qw_client___Win32_GLDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "qw_client___Win32_GLDebug"
# PROP Intermediate_Dir "qw_client___Win32_GLDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /GX /O2 /I "../scitech/include" /I "../qw_common" /I "../common" /I "../qw_client" /I "../qw_server" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "QUAKEWORLD" /D "GLQUAKE" /FR /YX /FD /c
# ADD CPP /nologo /GX /O2 /I "../scitech/include" /I "../qw_common" /I "../common" /I "../qw_client" /I "../qw_server" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "QUAKEWORLD" /D "GLQUAKE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ..\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"GLRelease/glqw_client.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 dxguid.lib ..\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"GLRelease/glqw_client.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "qw_client - Win32 Release"
# Name "qw_client - Win32 Debug"
# Name "qw_client - Win32 GLRelease"
# Name "qw_client - Win32 GLDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\common\cd_wrapper.c
# End Source File
# Begin Source File

SOURCE=..\common\cl_cam.c
# End Source File
# Begin Source File

SOURCE=.\cl_demo.c
# End Source File
# Begin Source File

SOURCE=..\common\cl_ents.c
# End Source File
# Begin Source File

SOURCE=..\common\cl_input.c
# End Source File
# Begin Source File

SOURCE=..\common\cl_main.c
# End Source File
# Begin Source File

SOURCE=.\cl_parse.c
# End Source File
# Begin Source File

SOURCE=.\cl_pred.c
# End Source File
# Begin Source File

SOURCE=..\common\cl_tent.c
# End Source File
# Begin Source File

SOURCE=..\common\cmd.c
# End Source File
# Begin Source File

SOURCE=..\common\common.c
# End Source File
# Begin Source File

SOURCE=..\common\console.c
# End Source File
# Begin Source File

SOURCE=..\common\crc.c
# End Source File
# Begin Source File

SOURCE=..\common\cvar.c
# End Source File
# Begin Source File

SOURCE=..\common\cvars.c
# End Source File
# Begin Source File

SOURCE=.\d_edge.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_fill.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_init.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_modech.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_part.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_polyse.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_scan.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_sky.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_sprite.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\d_surf.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_vars.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_zpoint.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\draw.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_draw.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_mesh.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_refrag.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_rmain.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_rmisc.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_rsurf.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_test.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_vidnt.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_warp.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\host.c
# End Source File
# Begin Source File

SOURCE=..\common\in_win.c
# End Source File
# Begin Source File

SOURCE=..\common\keys.c
# End Source File
# Begin Source File

SOURCE=..\common\lib_replace.c
# End Source File
# Begin Source File

SOURCE=..\common\mathlib.c
# End Source File
# Begin Source File

SOURCE=..\common\mdfour.c
# End Source File
# Begin Source File

SOURCE=.\menu.c
# End Source File
# Begin Source File

SOURCE=..\common\model.c
# End Source File
# Begin Source File

SOURCE=..\qw_common\net_chan.c
# End Source File
# Begin Source File

SOURCE=..\common\net_com.c
# End Source File
# Begin Source File

SOURCE=..\qw_common\net_udp.c
# End Source File
# Begin Source File

SOURCE=..\common\nonintel.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\pcx.c
# End Source File
# Begin Source File

SOURCE=..\common\pmove.c
# End Source File
# Begin Source File

SOURCE=..\common\pmovetst.c
# End Source File
# Begin Source File

SOURCE=..\common\qargs.c
# End Source File
# Begin Source File

SOURCE=..\common\qendian.c
# End Source File
# Begin Source File

SOURCE=..\common\quakefs.c
# End Source File
# Begin Source File

SOURCE=..\common\quakeio.c
# End Source File
# Begin Source File

SOURCE=..\common\r_aclip.c
# End Source File
# Begin Source File

SOURCE=..\common\r_alias.c
# End Source File
# Begin Source File

SOURCE=..\common\r_bsp.c
# End Source File
# Begin Source File

SOURCE=..\common\r_draw.c
# End Source File
# Begin Source File

SOURCE=..\common\r_edge.c
# End Source File
# Begin Source File

SOURCE=..\common\r_efrag.c
# End Source File
# Begin Source File

SOURCE=..\common\r_light.c
# End Source File
# Begin Source File

SOURCE=..\common\r_main.c
# End Source File
# Begin Source File

SOURCE=..\common\r_misc.c
# End Source File
# Begin Source File

SOURCE=..\common\r_part.c
# End Source File
# Begin Source File

SOURCE=..\common\r_sky.c
# End Source File
# Begin Source File

SOURCE=..\common\r_sprite.c
# End Source File
# Begin Source File

SOURCE=..\common\r_surf.c
# End Source File
# Begin Source File

SOURCE=..\common\r_vars.c
# End Source File
# Begin Source File

SOURCE=..\common\register_check.c
# End Source File
# Begin Source File

SOURCE=..\common\sbar.c
# End Source File
# Begin Source File

SOURCE=..\common\screen.c
# End Source File
# Begin Source File

SOURCE=..\common\skin.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_dma.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_mem.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_mix.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_win.c
# End Source File
# Begin Source File

SOURCE=.\sw_cl_parse.c
# End Source File
# Begin Source File

SOURCE=..\common\sw_rpart.c
# End Source File
# Begin Source File

SOURCE=..\common\sw_view.c
# End Source File
# Begin Source File

SOURCE=..\common\sys_common.c
# End Source File
# Begin Source File

SOURCE=.\sys_win.c
# End Source File
# Begin Source File

SOURCE=..\common\vid_win.c

!IF  "$(CFG)" == "qw_client - Win32 Release"

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\view.c
# End Source File
# Begin Source File

SOURCE=..\common\wad.c
# End Source File
# Begin Source File

SOURCE=..\common\world.c
# End Source File
# Begin Source File

SOURCE=..\common\zone.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\qw_common\bothdefs.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\bspfile.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\client.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\cmd.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\common.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\console.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\cvar.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\model.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\net.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\pmove.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\protocol.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\quakedef.h
# End Source File
# Begin Source File

SOURCE=..\qw_common\screen.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Asm files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\d_draw.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_draw.s
InputName=d_draw

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\d_draw.s
InputName=d_draw

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\d_draw.s
InputName=d_draw

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_draw16.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_draw16.s
InputName=d_draw16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\d_draw16.s
InputName=d_draw16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\d_draw16.s
InputName=d_draw16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_parta.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_parta.s
InputName=d_parta

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\d_parta.s
InputName=d_parta

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\d_parta.s
InputName=d_parta

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_polysa.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_polysa.s
InputName=d_polysa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\d_polysa.s
InputName=d_polysa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\d_polysa.s
InputName=d_polysa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_scana.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_scana.s
InputName=d_scana

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\d_scana.s
InputName=d_scana

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\d_scana.s
InputName=d_scana

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_spr8.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_spr8.s
InputName=d_spr8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\d_spr8.s
InputName=d_spr8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\d_spr8.s
InputName=d_spr8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_varsa.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_varsa.s
InputName=d_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\d_varsa.s
InputName=d_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\d_varsa.s
InputName=d_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\math.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_aclipa.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_aclipa.s
InputName=r_aclipa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\r_aclipa.s
InputName=r_aclipa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\r_aclipa.s
InputName=r_aclipa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_aliasa.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_aliasa.s
InputName=r_aliasa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\r_aliasa.s
InputName=r_aliasa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\r_aliasa.s
InputName=r_aliasa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_drawa.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_drawa.s
InputName=r_drawa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\r_drawa.s
InputName=r_drawa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\r_drawa.s
InputName=r_drawa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_edgea.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_edgea.s
InputName=r_edgea

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\r_edgea.s
InputName=r_edgea

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\r_edgea.s
InputName=r_edgea

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_varsa.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_varsa.s
InputName=r_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\r_varsa.s
InputName=r_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\r_varsa.s
InputName=r_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\snd_mixa.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\surf16.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\surf16.s
InputName=surf16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\surf16.s
InputName=surf16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\surf16.s
InputName=surf16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\surf8.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\surf8.s
InputName=surf8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\surf8.s
InputName=surf8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\surf8.s
InputName=surf8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\sys_dosa.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\sys_dosa.s
InputName=sys_dosa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\worlda.s

!IF  "$(CFG)" == "qw_client - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "..\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	rem del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 Debug"

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLRelease"

# Begin Custom Build
OutDir=.\GLRelease
InputPath=..\common\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qw_client - Win32 GLDebug"

# Begin Custom Build
OutDir=.\qw_client___Win32_GLDebug
InputPath=..\common\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /I "\quakeforge\quakeforge\common" /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	..\gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project