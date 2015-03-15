# 1. High Level Design #

UML will be required for designing component diagrams, sequence diagrams, flowchart, etc. as a preliminary step for implementation.

# 2. Low Level Design #

Pseudo coding will be required in order to represent codes in general form. This will additionally require proper understanding of each component, like color segmentation, contour extraction, curve fitting, etc. UML will be utilized for designing class diagrams, use case diagrams for each component of the project.

# 3. Coding Requirements #

C++ and MATLAB will be required for core implementation. Additionally, OpenCV and MATLAB Image Toolbox will be required for implementing various components.

# 4. GUI Requirements #

GUI enables user to select one or more images which are to be processed. GUI will provide multiple options for each stage of the process sequence so that user can see the output of each stage. Furthermore, user will have an option to save the output at any stage.

# 5. Implementation Requirements #

## 5.1	Color Image Segmentation ##

To have an improved color space, the RGB image has to be converted to IHLS.

To find a particular region of interest, NHS (Normalized Hue-Saturation) algorithm is required.

## 5.2 Post-processing ##

### Noise Removal ###
In order to remove the noise after the previous step, Median Filtering is applied. Morphological operations like erosion, dilation, closing and hole-filling are used to get clean contours.

### Object Elimination ###
To remove the detected non- road sign objects two strategies are used: area of an object and aspect ratio.

### Recovery of Deformed Road Sign ###
To recover deformed road sings, Convex-Hull algorithm is used, which provides only the outermost points of the shape. Consequently, Scan-Line Polygon Filling algorithm is used to fill up the shape.


## 5.3 Contour Extraction ##

### Polynomial Interpolation ###

Polynomial Interpolation is used to interpolate the extreme points resulting from the Convex-Hull algorithm.

## 5.4 Curve Fitting ##

### Gielis Curves ###

Gielis Curves can be seen as a generic model to unify the representation of road signs, and will be required to detect the shapes of road signs.

### Contour Smoothing ###

As the contours provided by polynomial interpolation suffer from outliers and staircase effect, contour smoothing is required. For this purpose, Robust LoessSmoothing algorithm is used.

### Retrieval of Rotation Offset ###

To fit the curve onto a set of 2D points, determination of the rotational offset is required for a correct initialization.

### Shape Reconstruction ###

Reconstructing a Gielis curve can be posed as a minimization problem of a cost function. For this, Levenberg-Marquardt algorithm for Least Square Minimization of Nonlinear Models is required.


# 6. Testing Requirements #

The data set should include circular, triangular, rectangular, and octagonal road signs in various colors. To quantify the accuracy of the method, sensitivity, specificity and accuracy measures are required.

The testing should include component testing, integration testing and system testing, followed by statistical analysis.

# 7. Documentation Requirements #

Requirements for documentation are conceptual material (background information that users need to know), reference material (explanation of fields, examples, technical details, etc.) and tutorials (step-by-step worked examples that users can follow).

# 8. Project Maintenance Requirements #

In order to maintain code repository, Google Code (Subversion) will be used.