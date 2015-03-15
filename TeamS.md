

# Objectives #
To suggest testing mechanisms at different stages of our project

To develop GUI in QT and Matlab

# Members #
| **Name** | **Role** |
|:---------|:---------|
| Suman Raj Bista | Manager |
| Pablo Speciale | Member |
| Ozan Tonkal | Member |
| Nolang Fanani | Member |
| Nathanael Lemessa Baisa | Member |
|Taman Upadhaya | Member |

# Meetings #
#### October 19,2011 ####
Work division for studying different kinds of testing mechanisms

#### October 26 2011 ####
Discussion about testing methods and preparation of report. The meeting log is available here
[MeetingLog10-26-2011](https://docs.google.com/document/d/18KxTrRr7QQXFXsMmgC5khvIhggYPykAGTBwDcLt-NYA/edit?hl=en_US#)

#### November 1,2011 ####
Review of  [Software Testing Report](https://docs.google.com/document/d/1taOXreZJKWHshbyNGalSH5IWHMaHoAODTE7s3I3Q2Wk/edit?hl=en_US#)

#### November 10,2011 ####
Discussion about implementation of GUI according to   [GUIDesign](https://docs.google.com/viewer?a=v&pid=explorer&chrome=true&srcid=0B5di_0iaJKEVNzkyOGVmMWItNDkzYy00ZDYxLTkyNzgtZjA5MWI1NTJjMjgx&hl=es).
Division of Work. The meeting log is available here [MeetingLog10-11-2011](https://docs.google.com/document/d/1hBKXTv4NDLvKP832IR869Neb7_1LSm81P8KZVVr_ry4/edit?hl=en_US)

# Activity #
I) Software Testing
> [Software Testing Report](https://docs.google.com/document/d/1taOXreZJKWHshbyNGalSH5IWHMaHoAODTE7s3I3Q2Wk/edit?hl=en_US#)

II) Graphical User Interface(GUI)
> GUI coding started

> a) [Repository for GUI in QT](https://vibot6-mscv3-se.googlecode.com/svn/trunk/project/c++/gui/)

> b)[Repository for GUI in Matlab](https://vibot6-mscv3-se.googlecode.com/svn/trunk/project/matlab/gui/)

# Issues #
> a) [Software Testing Issues](http://code.google.com/p/vibot6-mscv3-se/issues/detail?id=3)

> b) [QT GUI Issues](http://code.google.com/p/vibot6-mscv3-se/issues/detail?id=4)

> c)[Matlab GUI Issues](http://code.google.com/p/vibot6-mscv3-se/issues/detail?id=5)

# Questions #
| | **Questions** | **Answers**|
|:|:--------------|:-----------|
| 1 | **Why is Testing stage important in software engineering?**     | A primary purpose of testing is to check if the software works properly, and to detect software failures so that defects may be discovered and corrected.|
| 2 | **What are levels of testing in our project?**                  | **a) Unit Testing :-** Unit Testing is done at the lowest level. In procedural programming a unit may be an individual function or procedure. In object-oriented programming a unit is usually an interface,such as a class. For our case the individual matlab function or a C++ class  may be the **unit** for this type of test. Unit tests are generally created by programmers.<br><b>b) Integration Testing :-</b>Integration Testing is performed when two or more tested units are combined into a larger structure. In our case it may can be done when we call function(s) from other function.e.g when we combine rotation offset with shape reconstruction. <br><b>c) System Testing :-</b> System Testing tends to affirm the end-to-end quality of the entire system. System test is often based on the functional/requirement specification of the system. For our case it is done when entire software is ready. <br>
<tr><td> 3 </td><td> <b>What testing approach will we use in our project?</b>           </td><td>  We will use bottom up approach i.e. we begin from unit/component testing to system testing (integration testing). For Integration test, first we use black box approach with Valantine’s data set. If required, then we go for white box (in the case black box fails).</td></tr>
<tr><td> 4 </td><td> <b>What are Defect testing and validation testing ?</b>            </td><td> <b>a) Defect Testing :-</b> Tests designed to reveal the presence of defects in a system. <br><b>b) Validation Testing :-</b> Tests designed to show that the software meets its requirements. </td></tr>
<tr><td> 5 </td><td> <b>Which GUI environment are we using in our project ?</b>         </td><td> We are using Qt for developing GUI in C++. Qt is a cross-platform application framework that is widely used for developing application software with a graphical user interface. For GUI in matlab,we are using the GUIDE Toolbox. </td></tr></tbody></table>

<h1>References</h1>
1. <a href='https://docs.google.com/viewer?a=v&pid=explorer&chrome=true&srcid=0B5di_0iaJKEVNzkyOGVmMWItNDkzYy00ZDYxLTkyNzgtZjA5MWI1NTJjMjgx&hl=es'>GUIDesign</a>

2. <a href='http://en.wikipedia.org/wiki/Software_testing'>Software Testing</a>