In order to test the code:

- add test.cpp and test.h to the project file (QT Pro file).
- using the test class, generate an object.
- set the filename of that object to "image.jpg"
- and use method startTesting to start testing.
- the result will be given as an image file.

P.S. The image.jpg file should be placed to the build 
- folder of the project! Or you have to simply give the path
- of the image file (e.g. "C:/Users/Public/Desktop/image.jpg")
- for the method setFileName(QString fileName)

**************************************************
Example:

Test testObject;
testObject.setFileName("image.jpg");
testObject.startTesting();

**************************************************

- As data points and rotational offsets will be received
- from other classes (there must be a common class which
- follows the steps of algorithms by generating objects 
- of classes, passing one's output to another's input),
- in the common class, instead of reading an image, those
- inputs should be provided directly. Hence, it will be quite
- similar to the code of startTesting method.

Please contact me for any question!

ozantonkal@gmail.com
