ECXcx_Vid2Wav
=============
pinfutech/ECXcx_Vid2Wav is a program used to generate WAV files from a Vid file, mainly used for FreeSWITCH.

ThirdPartyTools
===============
pinfutech/ThirdPartyTools collects 3rd-party libraries used by pinfutech/* opensource projects.
It provides pre-built libraries compiled with Microsoft Visual Studio 2008 sp1, and vcredist_x86.exe for VS2008 sp1 x86 redistributable package.

Usage
==============================================
1. git clone https://github.com/pinfutech/ECXcx_Vid2Wav.git
2. cd ECXcx_Vid2Wav
3. git clone https://github.com/pinfutech/ThirdPartyTools.git
4. copy used DLLs from ThirdPartyTools/bin/ to bin/
5. build and run

Config ExtCmds, in data/ECXcx_Vid2Wav.ini
=========================================
Without Transcoder.Stereo2Mono.x ExtCmdGroup configed, the output is stereo.
If you want to USE for FreeSWITCH, recommendation is to set Transcoder.Stereo2Mono.x ExtCmdGroup:
example to use lame:
[ExtCmds]
Transcoder.Stereo2Mono.1=lame -m m "%s.wav" "%s.mp3"
Transcoder.Stereo2Mono.2=lame --decode "%s.mp3" "%s.wav"

Voice Engines, in data/VoicEngine.ini
=====================================
All voice engines should be configed here.
example for "Microsoft Anna - English (United States)"
[VoiceEngineAnna]
name=anna										// voice engine shown in context menu, and the output directory
lang=English									// voice language shown in context menu
desc=Microsoft Anna - English (United States)	// voice engine name in system
ldkw=en\\us										// language directory keyword, filter of voice engine for special language: VID file in en\\us directory, only show English voice engine

3rd-party libraries used
========================
1. MPC-4.1.2
	https://www.ociweb.com/products/mpc/
	license: license.MPC
2. msinttypes-r29
    https://code.google.com/archive/p/msinttypes/
	license: ThirdPartyTools/license.msinttypes
3. wxWidgets-3.1.0
    http://www.wxwidgets.org/
	license: ThirdPartyTools/license.wxWidgets

