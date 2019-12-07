INFO-6046-01-19F Media Fundamentals
Project 3 by:
	Dylan Kirkby,
	Brian Cowan,
	Ivan Parkhomenko

Project created with Visual Studios 2019 Community Edition.
Please build and run it via Visual Studios 2019.

Project directory currently includes DLL files for x64, can run both debug and release.
To run x86 the DLL files will need to be replaced with files located in:
	..\common\lib\Win32\Debug	
&	..\common\lib\Win32\Release

External configuration stored at
	"MediaFundamentalsP3\Project\MediaFundamentalsP3\config_files"
.wav generation requires "C:\tmp" to already exist.
Place generated .wav file at
	"MediaFundamentalsP3\Project\MediaFundamentalsP3\WAV_files"

Assignment performs text to speech requirements first on load, before allowing user input

Controls:
num row:
	1		Pause/Unpause Australian Radio
	2		Pause/Unpause UK Top 40'S
	3		Pause/Unpause KPop Radio
	
	4		Start recording
	5 - 0	Enable corresponding DSP on master channel group
	
	Space	Play program generated .wav