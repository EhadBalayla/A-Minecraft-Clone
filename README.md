# AMinecraftClone
a block game voxel engine i am making in C++ that is a Minecraft clone, i am trying to replicate Minecraft Java 1 to 1 but with extra QoL features like a Distant Horizon inspired LOD system and also heavily multithreaded

example videos:
https://youtu.be/KjVXoL0f9AY

## IMPORTANT NOTES
- at this point i am still working on the voxel engine stuff AKA the chunks systems, and while i made huge progress it is not done, particularly the wall i am facing is decorating the world...
- also as of the last commit the LODs were temporarily disabled cause i am working on implementing the Minecraft Alpha terrain which while i implemented the heightmap, the population isn't finished AND i still need to LODify it for the LODS to work with
- right now the clone has the heightmap generation of infdev 2010-02-27, infdev 2010-03-27, and infdev 2010-06-30 with population in WIP and unstable as hell cause its hard and also Notch back in the day wrote extremely shit code so it is extremely hard to transfer it to my C++ multithreaded setup
- because i am juggling between this and my polygonal game engine, AND i haven't perfected the voxel systems YET... i didn't get the time to make it a CMake buildable YET... and also the sorting of the files is the messy Visual Studio default, i will do it later on when i will perfect the voxel stuff...

# How To Install
since currently it is still not a CMake buildable or a Premake buildable... it just comes as a default Visual Studio solution... later on i will change it tho... just download the repo and make sure you got Visual Studio 2022...

## Gameplay Features
- if i remember correctly, for the current commit the game is set for the infdev 2010-06-30 terrain with the LODs temporarily disabled
- use the keybind 'C' to change between creative mode physics or survival mode physics (although the survival mode physics are still not perfect)
- the game automatically spawns the player with items of different blocks on the slots... since inventory system is still early and inventory management isn't implemented yet (i NEED to perfect the voxel stuff)
- left click to break blocks, right click to use the selected item in hotbar (which as of now places one of the 9 given blocks)
- again the inventory system is SUPER early so items dont display and inventory management doesn't work... but it IS there

## Technical Notes
- rendering uses OpenGL... later when the voxel systems will be perfected and the code will also be cleaned up, i will either transition exclusively to Vulkan or create a OpenGL/Vulkan hybrid just like my polygonal game engine
- windowing management currently uses SDL... i fucking hate SDL it is so unstable that i might later on switch to GLFW (will also make my life easier for Vulkan)
- the game is heavily multithreaded... right now the multithreading happens for the chunk generation and LOD chunk generation and the sound effects (using miniaudio.h), but i predict it will be much more multithreaded in the future too
- as for the audio being cancerous... yeah it was a failed attempt to load OGG files using miniaudio.h and stb_vorbis.h... i'll just convert Minecraft's sounds to either MP3 or WAV since miniaudio.h by default supports WAV