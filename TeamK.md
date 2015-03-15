


---


# Objectives #
To RampUP Req. Doc. 5.4 b) of the Project's Requirements.

> Contour Smoothing

As the contours provided by polynomial interpolation suffer from outliers
and staircase effect, contour smoothing is required. For this purpose,
Robust Loess Smoothing algorithm is used.

# Members #
| **Name** | **Role** |
|:---------|:---------|
| Nathanael Lemessa Baisa |Manager |
| Ozan Tonkal | Technical Manager |
| Pablo Speciale | Team Member|
| Raquel Gil |  Team Member |
| Suman Raj Bista |  Team Member |



# Design Specifications #

## Input ##

Binary image of size NxM which is contour points generated from polynomial interpolation.

## Description ##

As the contours provided by polynomial interpolation made during contour extraction suffer from outliers and staircase effect, contour smoothing is needed . To remove outliers and staircase effect, we use the Robust Lowess Smoothing algorithm which carries out (robust) locally-weighted time series and scatter plot smoothing for both equispaced and non-equispaced data.

Robust lowess method considers two weighting parameters, local weight and robustness
weight. The local weight f, which can be defined by the user, indicates the fraction of data
used in the procedure.The larger the value of f, more percentage of the data is included, thus more smoothing is performed.

## Output ##

Smoothed contour points of the detected objects which is actually in a binary image.


# Meetings #

[Group K, Meeting 16102011](https://docs.google.com/document/d/1aLnMT8tdDRvh6AtYaUwtfCIxuoLoC8_ID80VkGX7k1Y/edit?hl=es)

# Questions #
| |**Questions**|**Answers**|
|:|:------------|:----------|
| 1 |**What is the purpose of counter smoothing ?**                   | From polynomial interpolation we get all the points of the **contour** which suﬀer from outliers and staircase eﬀect. Counter smoothing removes such effect.(For figure refer to slide) |
| 2 |**What is the algorithm to remove such effects ?**               | The Robust Lowess Smoothing algorithm.|
| 3 |**Why we don't require contour smoothing in this project ?**     | No Polynomial Interpolation means No Contour Smoothing.|

# References #
[Contour Smoothing Presentation](https://docs.google.com/viewer?a=v&pid=explorer&chrome=true&srcid=0B6w6ezwYzkaiMmI2ZmQ2OWUtNDU3NS00MGE3LWI1ZDYtYjA0Yzc4NDIzZGQw&hl=en_US)