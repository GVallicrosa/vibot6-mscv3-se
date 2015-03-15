

# Objectives #
To "Ramp Up" the requirements of point 5.4 c) of the Project's Requirements.

Retrieval of Rotation Offset

To fit the curve onto a set of 2D points, determination of the rotational
offset is required for a correct initialization.


# Members #
| **Name** | **Main Topic** | **Role** |
|:---------|:---------------|:---------|
| Raquel Gil |  | Manager |
| Saurabh Jain |  | Team Leader |
| Suman Raj Bista |  | Team Member|
| Taman Upadhaya |  | Team Member |
| Yukti Suri |  | Team Member |
| Zhongyang Liu |  | Team Member |


# Meetings #
**6 October**

1. Reading rotational offset part from the thesis and understand the matlab code.

**12 October**

1. Discussion among ourselves for the problems we were facing(Differences in Implementation and thesis)

2. Conclusion was that we need to talk to Yohan about our common problems.

**14 October**

1. Meeting with Yohan. Many queries were cleared about why and how.

2. Conclusion: Better understanding of concept and got to know about the scope of improvement.

**18 October**

1. Made the presentation keeping improvements in mind.

2. Meeting logs were filled.

# Questions #
| | **Question** | **Answer** |
|:|:-------------|:-----------|
|1 | What is rotational offset ? | Angle required so that the contour gets aligned to the Gielis curve before matching starts. |
|2 | Why rotational offset is required ? | The rotational offset is a very sensitive parameter for Gielis curves recovery. In other words, if the initial rotational offset is not correct, the minimization routine might strongly converge to local minimum. |
|3 | How to get the rotational offset? | Given the contour points of the figure: 1. Change to polar coordinates: (x,y)->(r,theta) 2. Sort (r,theta) points by the angle theta from [0, 2pi) 3. Get the minimum radius of the sorted points by comparing the two previous and the two next points. |

# References #

https://docs.google.com/present/edit?id=0Aaw6ezwYzkaiZGY2ZGdyNXhfMjQ4Y2Zmc3I4YzY&hl=en_US