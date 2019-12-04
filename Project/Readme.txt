INFO-6046-01-19F Media Fundamentals
Project 1 Submission by Dylan Kirkby.

Project created with Visual Studios 2019 Community Edition.
Please build and run it via Visual Studios 2019.

Project directory currently includes DLL files for x64, can run both debug and release.
To run x86 the DLL files will need to be replaced with files located in:
	..\common\lib\Win32\Debug	
&	..\common\lib\Win32\Release

An attempt was made at using CMake, and there is a CMakeLists.txt present based 
heavily on Oscar Lara's example. The program seems to seach for the libraries 
at the project's pre CMake location.

Controls:
	ESC		Close GLFW console.

	Up		Increase volume of selected item.
	Down		Decrease volume of selected item.

	Up + Shift	Increase pitch of selected item up to 2.0 (double).
	Down + Shift	Decrease pitch of selected item up to 0.5 (half).

	Right		Select next audio item.
	Left		Select previous audio item.

	Right + Shift	Increase pan of selected item up to 1.0 (right only).
	Left + Shift	Decrease pan of selected item up to -1.0 (left only).

	Number Row	Select an parameter to view about selected audio item.
	1		Pause Status
	2		Volume
	3		Pitch
	4		Pan
	5		Time
	6		Frequency
	7		Format
	8		Type
	9		Number of Channels
	0		Number of Bits

	H		Toggle a breifer list of controls in the GLFW window.

	P		Pause and unpause current item - does not cause completed audio items to repeat.

	R		Repeats selected audio item from 0:00 - does work on completed items.
	
	R + SHIFT	Reset game state to start.

	Z		Player takes the attack option, enemy attacks back.
			The boss will sneak attack you, but skeletons don't.

	X		Player takes the heal action, enemy attacks anyways. 

Pressing Z & X repeatedly before the sfx have completed will revent the console from updating 
until all input operations are complete.
	