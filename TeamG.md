

# Members #
  * Arash
  * Big
  * Eduardo
  * Dadhichi
  * Hamed

# Description #
This groups is responsible of reading and understanding colour space conversion (from RGB to IHLS). As well as colour segmentation, namely LT and NHS methods. (Corresponding to section 2.4 of Valentine thesis.) For further readings please check [ColourSegmentation](ColourSegmentation.md) page.

# Meetings #

## October 7 ##
Big, Eduardo and Arash were present.

We discussed what are the requirements and deliverables of our group.

A task list was created as follow:
  * Implement the Matlab code for IHLS in C++.
  * Same for LT (Luminance Thresholding)
  * Same for NHS
  * Comment the Matlab code
Note: please make sure that all the C++ codes are well unit-tested.

## October 10 ##
All members were present.

We summarised what we had discussed in the precious meeting. Then we assigned the tasks to different members. Big and Eduardo work together on LT. Dadhichi, Hamed and Arash work together on colour space conversion from RGB to IHLS, as well as NHS.

## October 14 ##
All members were present.

All tasks were completed successfully. Members decided to drop implementation tasks, since it was not required according to management.

Necessary explanations were written in their corresponding wiki page.

## October 17 ##
All members were present.

Presentation was created for Wednesday.

# Questions #
| | **Question** | **Answer** |
|:|:-------------|:-----------|
| 1. | What is IHLS? | Improved Hue Luminance Saturation. IHLS is one of the most common cylindrical-coordinate representations of points in an RGB colour model. In brief IHLS is a colour space. (Check the picture at the end of the page)|
| 2. | What is NHS? | Normalised Hue Saturation. By using "Look-up table" road-signs are distinguished from background images. |
| 3. | What is the starting point for Hue? | The hue angle is traditionally measured starting at the direction corresponding to pure red. |

![http://upload.wikimedia.org/wikipedia/commons/thumb/a/a0/Hsl-hsv_models.svg/400px-Hsl-hsv_models.svg.png](http://upload.wikimedia.org/wikipedia/commons/thumb/a/a0/Hsl-hsv_models.svg/400px-Hsl-hsv_models.svg.png)