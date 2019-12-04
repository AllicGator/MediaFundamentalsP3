AmethystEngine by:
	Dylan Kirkby,
	Brian Cowan,
	Ivan Parkhomenko

Created in Visual Studio Community 2019
Runs in x64 only, Release recommended for performance.

External files located in \AmethystEngine\data
Configuration files located in \AmethystEngine\data\config



FOR INFO6046 Media Fundamentals
3D Sounds, channel groups, and DSP effects

Sound effects implemented on our Physics Project 1
3D SFX:
Mario 'Bump'		Bumping Another Ball
Mario Jump			Jumping
Wilhelm Scream		Falling off the plane
'Run Away'			Moves with 'MusicSphere' (Model Object, second in container)

3 Channel groups:
	Cave-Water-BGM
	PrettyForest
	UnderWaterSounds
use '=' to select the first channel group, 
then left and right cursor keys to iterate over them.
Please see controls for more.

External configurationm stored in \AmethystEngine\data\config\audio\audio.ini
Audio files are from all over the place!



Controls:
Mouse:
	Move			Adjust camera viewing angle
Keyboard:
	`				Toggle Developer Console (Freeze physics and audio system updates)
	[				Select Model Objects
	]				Select Light Objects
	\				Select Complex Objects
	=				Select Audio Objects
	Left Cursor		Move backwards through currently selected object type
	Right Cursor	Move forwards through currently selected object type


	Normal Controls
	ESC				Exit Program
	F				Turn off currently selected light (no matter the object type selected)
	
	Model || Complex type selected
	A				Increase left velocity relative to camera
	D				Increase right velocity relative to camera
	W				Increase forward velocity relative to camera
	S				Increase backward velocity relative to camera
	SPACE			Jump	
	
	Audio type selected (Channel groups)
	1				Toggle ECHO dsp on channel group 1
	2				Toggle TREMOLO dsp on channel group 1
	3				Toggle DISTORTION dsp on channel group 1
	4				Toggle ITECHO dsp on channel group 2
	5				Toggle FLANGE dsp on channel group 2
	6				Toggle NORMALIZE dsp on channel group 2
	7				Toggle HIGHPASS dsp on channel group 3
	8				Toggle ITLOWPASS dsp on channel group 3
	9				Toggle PITCHSHIFT dsp on channel group 3	
	KEYPAD 0		Toggle Pause
	KEYPAD 1		Raise Volume (max 100)
	KEYPAD 2		Lower Volume (min 0)
	KEYPAD 4		Raise Pitch (max 10)
	KEYPAD 5		Lower Pitch (min 0)
	KEYPAD 7		Pan Left
	KEYPAD 8		Pan Right
	

	Developer Controls
	ESC				Exit Developer Console if open
	ENTER			Send Command
	BACKSPACE		Erase last character
	.				Increase object movespeed
	,				Decrease object movespeed
	
	Model || Lights || Complex type selected
	CONTROL:
		A			Move object left relative to camera
		D			Move object right relative to camera
		W			Move object forward relative to camera
		S			Move object backward relative to camera
		Q			Move object up relative to camera
		E			Move object down relative to camera
		
	Lights type only
	CONTROL:
		1			Increase Constant Attentuation
		2			Decrease Constant Attentuation
		3			Increase Linear Attentuation
		4			Decrease Linear Attentuation
		5			Increase Quadratic Attentuation
		6			Decrease Quadratic Attentuation
		V			Decrease Cone Inner Angle
		B			Increase Cone Inner Angle
		N			Decrease Cone Outer Angle
		M			Increase Cone Outer Angle
	
	COMMANDS
		TODO		...
		...			...