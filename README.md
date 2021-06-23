# mercy-detector <img style=img align="right" src="https://raw.githubusercontent.com/DaviidK/mercy-detector/main/Documentation/Images/mercy_cartoon.png" />

![output gif](https://github.com/DaviidK/mercy-detector/blob/cleanup/Documentation/Images/methods_comparison.gif)

## Background -- Rewrite to make less academic 
Existing research has demonstrated that it is possible to parse visual data from a video feed of a game and analyze different aspects of gameplay. Static 2D elements such as a GUI used to represent health or energy have been studied in-depth, and no longer represent a particularly challenging task for computer vision projects. 3D modelled objects pose a similar challenge to identifying an object in real-life.

Overwatch is a first-person shooter developed by Blizzard Entertainment. The game allows players to select different characters and fight to complete specific tasks. These characters (specifically called "heroes") each have unique weapons and abilities, which are displayed using specific models and animations.
![character/action reference](https://github.com/DaviidK/mercy-detector/blob/cleanup/Documentation/Images/mercy_states.png)

## About -- make minor changes, past tense!
Our goal is to create a project that investigates different techniques for interpreting a video feed of Overwatch gameplay. Our project will extract data from this feed and use it to understand certain aspects of the current game state.

![System Overview Diagram](https://github.com/DaviidK/mercy-detector/blob/cleanup/Documentation/Images/system_overview_diagram.PNG)
We will focus on the 3D object held by the player’s hero, which will move slightly with hero movement and execute different animations depending on the inputs given by the player. Using this information, we will determine two aspects of gameplay:

1. Which hero is currently being played
2. What action is the held weapon performing at a given point in time

![Mercy's Heads Up Display. The 3D modelled weapon moves slightly to show player movement and is subject to lighting effects. Each hero has a unique weapon](https://raw.githubusercontent.com/DaviidK/mercy-detector/main/Documentation/Images/hero_demo.png)

![Weapon action demonstration. Some weapons are capable of performing multiple actions. These result in slight changes in the weapon shape and color](https://raw.githubusercontent.com/DaviidK/mercy-detector/main/Documentation/Images/weapon_action.png)

In its final stage, we want our project to receive a set of frames of Overwatch gameplay and use them to output both the hero being played and their current action. We hope for this system to be performant enough to operate in real-time, which would allow for the analysis to be applied to a live video stream.

## Setup -- move to lower 
1. Install [OpenCV](https://opencv.org/), including [extra modules](https://github.com/opencv/opencv_contrib)
    - OpenCV Libraries used: 
      ```
      opencv2/
        core.hpp
        core/mat.hpp
        highgui.hpp
        imgcodecs.hpp
        imgproc.hpp
        objdetect.hpp
        opencv.hpp
        shape/shape_distance.hpp
      ```
2. Configure OpenCV to use C++17
3. Download mercy-detector repository
4. Open `mercy-detector/Detection_Algorithm/Src/mercy_detector.cpp`
5. Edit constant variables to enable/disable matching methods
6. Run **`mercy_detector.cpp`**

## Publically Available Resources to Use:
- OpenCV for image processing
- Overwatch for image sourcing
- Git/Github
- Visual Studio 2019 Community

## Bibliography:
- K. Kozłowski, M. Korytkowski, and D. Szajerman, “Visual Analysis of Computer Game Output Video Stream for Gameplay Metrics,” in Computational Science – ICCS 2020, vol. 12141, V. V. Krzhizhanovskaya, G. Závodszky, M. H. Lees, J. J. Dongarra, P. M. A. Sloot, S. Brissos, and J. Teixeira, Eds. Cham: Springer International Publishing, 2020, pp. 538–552.
