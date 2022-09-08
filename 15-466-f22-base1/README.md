# Dodging asteroid

Author: Yilin Wang 

Design: Player use arrow keys to control the moving direction of the little rocket to dodge the asteroids floating in the space. Once hit, the rocket crashes and the game ends

Screen Shot:<img width="516" alt="screenShot" src="https://user-images.githubusercontent.com/72194580/189007845-b96845bd-65b2-4606-9ef0-d8513b083d1b.png">

How Your Asset Pipeline Works:

(TODO: describe the steps in your asset pipeline, from source files to tiles/backgrounds/whatever you upload to the PPU466.)
My png spirit files are in /spirites and they are rocket8.png and asteroid8.png. I use pngProcessing.py to read in those pngs and that python file spit out rocket80.txt, rocket81.txt(those files are in the ppu tile readable format and stands for the bit0 and bit1 for rocket8 ); asteroid80.txt, asteroid81.txt(those files are in the ppu tile readable format and stands for the bit0 and bit1 for asteroid8 ); also the rocket8pal.txt and asteroid8pal.txt(those files store the color palette info for the rocket and the asteroid in the ppu palette format). Those txt files would be read in by the PlayMode.cpp and recorded in the data structure in the PlayMode.cpp file. 

(TODO: make sure the source files you drew are included. You can [link](your/file.png) to them to be a bit fancier.)
the path to those png files, txt files and the python file to process this images is: '/15-466-f22-base1/spirites'

How To Play:
Player use arrow keys to control the moving direction of the little rocket to dodge the asteroids floating in the space. Once hit, the rocket crashes and the game ends. The goal is to survive as long as possible. 

This game was built with [NEST](NEST.md).

 
