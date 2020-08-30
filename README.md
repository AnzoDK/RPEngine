# RPEngine
A small 2D Game Engine with help from the RPAudio lib
## Intro
Welcome to the (Slightly Useless) Game Engine: "Rosenørn Engine" <br> This is a small project I've wanted to make since the day I started having fun in unity back in 2015 - And today I've **FINALLY** started!<br>

## Relevant stuff for you
The engine works on the idea of derived classes, so if you don't like to make your own derived classes based on the engines core classes, then this is not a project for you :)<br>
You will also lose your sanity if you ever try to read the source files in this project, due to the code being **VERY** far from perfect<br><br>
With all of that out of the way - Let's introduce you to the (awful?) engine :D

## Relevant for Windows users
I have no clue about how Windows users even compile programs in the first place, but I've managed to compile a few versions of the engine for windows successfully and even saw them running on a real Windows machine first hand!
So if you ever want to try and compile a Windows version in a linux terminal or the stuff alike, be my guest - I already made a little trick to make it easier. If you run <br>
```bash
./dependency-builder --use-dev --Windows && make lib OS=Windows
```
You should be able to build the DLL that you can use for what ever you want.

## Usage
The engine is coded in a way that makes it ~~harder~~ easier for everyone to understand and implement into your project. First and foremost you must include the header `RPEngine.h`. 

#### Note:
If your RPEngine.h is complaining about needing RPAudio.h try and run `./dependency-builder.sh` and copy the includes folder into your project.

## Basic insight
To use the engine it is a good idea to know how it does stuff so here is a list of common functions and their usecases - Remember to read the "derived" section, to get more info on what functions you should and shouldn't override
- [The Engine Itself](#the-engine-itself)

## The Engine Itself
The main class is called `RosenoernEngine` and is residing in the namespace `rp`. It includes a few public static properties:
- ##### int FPS
  - FPS is a static int that you can call to get the engines framerate - This is not necessarely the framerate that the engine runs at, but the framerate that it attempts to achieve.
-  ##### int mouseX
   - mouseX keeps track of the mouse's x position at all times, this was my attempt to avoid writing code that checked the position more than once.
-  ##### int mouseY
   - Same as mouseX, but for the Y value.
-  ##### int width
   - width stores the width of the main window, and should always be used to calculate the size of objects on the screen, unless you want large screens to have smaller objects and small screens to have large obejcts
-  ##### int height
   - height is the same as width, but for the window height - just as width, it is recommended to calucate objects sizes from this value
- ##### SDL_Renderer* mainRender
   - mainRender is the primary SDL_Renderer used to render objects to the screen - the reason for it being static was my dumb implementation of the texture handling
- ##### SDL_Window* mainWin
   - mainWin is the primary window in SDL and is the default window that objects are going to be written to. It is static for some reason, and I forgot why.
 
 #### Common Functions
 The engine contains a few functions that should always be used instead of worked around.
- ##### `RosenoernEngine::RosenoernEngine(bool _debug, int buffers)`
   - The default constructor for the engine it takes 2 arguments and both should always be supplied.
     - `bool _debug`: Indicates debug behaviour, this doesn't do anything for the engine at the moment, but does set a few variables in the RPAudio library
     - `int buffers`: The amount of audio buffers that should be available - It is worth noting that the RPAudio doesn't clear them very often, so a number larger than 5 and smaller than 10 is recommened. It is also good to know that the engine will crash if it runs out of buffers and cant clear any of them.
 - ##### `void RosenoernEngine::Init()`
   - The Engine initializer. This should always be run after the initial object declaration. If you fail to run this function before any engine logic you will end up with udefined behaviour.
- ##### `RosenoernAudio& RosenoernEngine::GetAudioController()`
   - This function returns a refrence to the audiocontroller generated in the Init() function. As of now, you'll have to ask the audiocontroller directly if you want to play audio - Please check [RPAudio](https://github.com/AnzoDK/RPAudio) for more info on using the audiocontroller.
## TODO: Write about functions and classes
