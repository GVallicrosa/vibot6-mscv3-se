%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         GUI                          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                      %
%    Suman Raj Bista(Manager)          %
%    Pablo Speciale	                   %
%    Nolang Fanani	                   %
%    Nathanael Lemessa Baisa	       %
%    Taman Upadhaya	                   %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function varargout = gui_matlab(varargin)
% GUI_MATLAB MATLAB code for gui_matlab.fig
%      GUI_MATLAB, by itself, creates a new GUI_MATLAB or raises the existing
%      singleton*.
%
%      H = GUI_MATLAB returns the handle to a new GUI_MATLAB or the handle to
%      the existing singleton*.
%
%      GUI_MATLAB('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI_MATLAB.M with the given input arguments.
%
%      GUI_MATLAB('Property','Value',...) creates a new GUI_MATLAB or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before gui_matlab_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to gui_matlab_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help gui_matlab

% Last Modified by GUIDE v2.5 20-Dec-2011 01:53:36

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @gui_matlab_OpeningFcn, ...
                   'gui_OutputFcn',  @gui_matlab_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT
% global DisplayIndex; % index of the image currently displayed


%% --- Executes just before gui_matlab is made visible.
function gui_matlab_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to gui_matlab (see VARARGIN)

% Choose default command line output for gui_matlab
handles.output = hObject;
clc;

global DisplayIndex ;
global Processflag;
global Options;

Processflag = 0;
DisplayIndex = 1; % we should start with one?

% Load all options to the checkboxes
intermediate = false;
if Options.NHS_output
    set(handles.NHSSegmentation, 'Value', 1);
    intermediate = true;
end
if Options.POST_out_noise
    set(handles.NoiseRemoval, 'Value', 1);
    intermediate = true;
end
if Options.POST_out_clean
    set(handles.ObjectElimination, 'Value', 1);
    intermediate = true;
end
if Options.CE_output
    set(handles.ContourExtraction, 'Value', 1);
    intermediate = true;
end
if Options.RO_output
    set(handles.GetRotationalOffset, 'Value', 1);
    intermediate = true;
end
if Options.GIELIS_output
    set(handles.GelisShapeReconstruction, 'Value', 1);
    intermediate = true;
end
if intermediate
    set(handles.IntermediateResults, 'Value', 1);
end

%     %for remembering the previously selected folder 
% handles.macroFolder = cd;
% 
% strIniFile = fullfile(handles.macroFolder, 'imageFolder.mat');
%     %checks whether imageFolder.mat exists or not
% 	if exist(strIniFile, 'file')
% 		% Pull out values and stuff them in structure initialValues.
% 		initialValues = load('imageFolder.mat');
% 		% Assign the image folder from the lastUsedImageFolder field of the
% 		% structure.
% 	    handles.ImageFolder = initialValues.lastUsedImageFolder;
handles.ImageFolder = Options.lastFolder; % I check that I have this option in mainGUI.m
% 	else
% 		% If the file is not there, point the image folder to the current
% 		% directory.  Then save it out in our mat file.
% 		handles.ImageFolder = cd;
% 		% Save the image folder in our ini file.
% 		lastUsedImageFolder = handles.ImageFolder;
% 		save(strIniFile, 'lastUsedImageFolder');
%     end
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes gui_matlab wait for user response (see UIRESUME)
% uiwait(handles.figure1);


%% --- Outputs from this function are returned to the command line.
function varargout = gui_matlab_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


%% --- Executes on button press in SelectImageFolder.
function SelectImageFolder_Callback(hObject, eventdata, handles)
% hObject    handle to SelectImageFolder (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global Options;
folder = uigetdir(handles.ImageFolder,'Select folder');
% returnValue will be 0 (a double) if they click cancel.
% returnValue will be the path (a string) if they clicked OK.
    if folder ~= 0
        % Assign the value if they didn't click cancel.
 		handles.ImageFolder = folder;
        handles = LoadImageList(handles);
        guidata(hObject, handles);
		% Save the image folder in our ini file.
% 		lastUsedImageFolder = handles.ImageFolder;
% 		save('imageFolder.mat', 'lastUsedImageFolder');
        Options.lastFolder = handles.ImageFolder;		
        save opt.mat -struct Options;
    else
        msgbox('No folder specified as input for function LoadImageList.');
        return;
    end
%===========================================================================    
 %% ---- Load up the listbox with image files from the selected folder -----
function handles=LoadImageList(handles)
		ListOfImageNames = {};
        folder = handles.ImageFolder;
        if ~isempty(handles.ImageFolder) 
            if exist(folder,'dir') == false
                msgbox(['Folder ' folder ' does not exist.']);
                return;
            end
        else
            msgbox('No folder specified as input for function LoadImageList.');
            return;
        end
    % If it gets to here, the folder is good.
        ImageFiles = dir([handles.ImageFolder '/*.*']);
        for Index = 1:length(ImageFiles)
            baseFileName = ImageFiles(Index).name;
            [folder, name, extension] = fileparts(baseFileName);
            extension = upper(extension);
            switch lower(extension)
                case {'.png', '.bmp', '.jpg', '.tif'}
                % Allow only PNG, TIF, JPG, or BMP images
                ListOfImageNames = [ListOfImageNames baseFileName];
                otherwise
            end
        end
        set(handles.ImageList,'string',ListOfImageNames);
        return;
%========================================================================================
%% --- Executes on button press in Options.
function Options_Callback(hObject, eventdata, handles)
% hObject    handle to Options (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
optionsGUI(); % all options will be loaded from the global variable Options

% --- Executes on button press in IntermediateResults.
function IntermediateResults_Callback(hObject, eventdata, handles)
% hObject    handle to IntermediateResults (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of IntermediateResults


% --- Executes on button press in GetRotationalOffset.
function GetRotationalOffset_Callback(hObject, eventdata, handles)
% hObject    handle to GetRotationalOffset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of GetRotationalOffset


% --- Executes on button press in ContourExtraction.
function ContourExtraction_Callback(hObject, eventdata, handles)
% hObject    handle to ContourExtraction (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of ContourExtraction


% --- Executes on button press in FinalOutput.
function FinalOutput_Callback(hObject, eventdata, handles)
% hObject    handle to FinalOutput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of FinalOutput


% --- Executes on button press in GelisShapeReconstruction.
function GelisShapeReconstruction_Callback(hObject, eventdata, handles)
% hObject    handle to GelisShapeReconstruction (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of GelisShapeReconstruction


% --- Executes on button press in RecoverDeformedShape.
function RecoverDeformedShape_Callback(hObject, eventdata, handles)
% hObject    handle to RecoverDeformedShape (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of RecoverDeformedShape


% --- Executes on button press in NHSSegmentation.
function NHSSegmentation_Callback(hObject, eventdata, handles)
% hObject    handle to NHSSegmentation (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of NHSSegmentation


% --- Executes on button press in RGB2IHLS.
function RGB2IHLS_Callback(hObject, eventdata, handles)
% hObject    handle to RGB2IHLS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of RGB2IHLS


% --- Executes on button press in ObjectElimination.
function ObjectElimination_Callback(hObject, eventdata, handles)
% hObject    handle to ObjectElimination (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of ObjectElimination


% --- Executes on button press in NoiseRemoval.
function NoiseRemoval_Callback(hObject, eventdata, handles)
% hObject    handle to NoiseRemoval (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of NoiseRemoval


% --------------------------------------------------------------------
function Untitled_1_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


%% --- Executes on button press in Process.
function Process_Callback(hObject, eventdata, handles)
% hObject    handle to Process (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%   global ihls;
%   global nhs;
%   global noiseRem;
%   global cleanImg;
%   global countourImg;
   global Processflag;
%   global Offset;
%   global Output;
%   global OutputImg;
%   global valid_contour;
%  global currfilename; % all next is inside Options variable
%   color = 'red';
%   aspectRatio = 1500;
%   lowRatio = 0.25;
%   highRatio = 1.3;
%   distanceError = 1.414;
%   GIELIS_norm = 1;
%   GIELIS_func = 1;

% Read options
global Options;
nImg = 0;  % number of selected image outputs
nFile = 0; % number of selected file outputs
if get(handles.IntermediateResults, 'Value')
    Options.intermediate_output = true;
else
    Options.intermediate_output = false;
end
if get(handles.NHSSegmentation, 'Value')
    Options.NHS_output = true;
    nImg = nImg + 1;
else
    Options.NHS_output = false;
end
if get(handles.NoiseRemoval, 'Value')
    Options.POST_out_noise = true;
    nImg = nImg + 1;
else
    Options.POST_out_noise = false;
end
if get(handles.ObjectElimination, 'Value')
    Options.POST_out_clean = true;
    nImg = nImg + 1;
end
if get(handles.ContourExtraction, 'Value') % need another for file
    Options.CE_output = true;
    nImg = nImg + 1;
    nFile = nFile + 1;
else
    Options.CE_output = false;
end
if get(handles.GetRotationalOffset, 'Value')
    Options.RO_output = true;
    nFile = nFile + 1;
else
    Options.RO_output = false;
end
if get(handles.GelisShapeReconstruction, 'Value')
    Options.GIELIS_output = true;
    nFile = nFile + 1;
else
    Options.GIELIS_output = false;
end
save opt.mat -struct Options;

im = get(handles.ImageList,'UserData');
if size(im) == 0
   msgbox('No Image Selected from the list');
   return;   
end
% ihls = im; % I think we don't need this image

% currfilename= get(handles.text1,'string');
Options.currfilename = get(handles.text1,'string');
[nhs,noiseRem,cleanImg,valid_contour,countourImg,Offset,Output,OutputImg] = ProcessImage(im); % less parameters

% Test variable to share images (first we will have to test wich outputs we want)
global Images;
Images = cell(2, nImg + 2); % first row images, second names. We must check how many outputs we have
Images{1,1} = im; % input image
Images{2,1} = Options.currfilename; % original name for the input image
i = 2;
if Options.intermediate_output
    if Options.NHS_output % maybe we can do all this inside processimage, also the save
        Images{1,i} = nhs; % after segmentation
        Images{2,i} = 'NHS segmentation';
        i = i + 1;
    end
    if Options.POST_out_noise
        Images{1,i} = noiseRem; % after noise removal
        Images{2,i} = 'Noise removal';
        i = i + 1;
    end
    if Options.POST_out_clean
        Images{1,i} = cleanImg; % after morphological operations
        Images{2,i} = 'Cleaned image';
        i = i + 1;
    end
    if Options.CE_output
        Images{1,i} = countourImg; % after contour extraction
        Images{2,i} = 'Contour extraction';
        i = i + 1;
    end
end
Images{1,i} = OutputImg; % final output image
Images{2,i} = 'Output image';

Processflag = 1;
msgbox('Done Processing');
 
%  setappdata(gui_matlab,'ihls',im);
%  setappdata(gui_matlab,'nhsSegmentation',nhs);
%  setappdata(gui_matlab,'noiseRemoval',noiseRem);
%  setappdata(gui_matlab,'objectElimination',cleanImg);
 
%% --- Executes on button press in SaveAll.
function SaveAll_Callback(hObject, eventdata, handles)
% hObject    handle to SaveAll (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global ihls;
global nhs;
global noiseRem;
global cleanImg;
global countoutImg;
global Processflag;
global Offset;
global Output;
global OutputImg;
global valid_contour;
  
if Processflag == 1 % ensure image already processed
    fname= get(handles.text1,'string');
    if(get(handles.RGB2IHLS,'Value') == 1)
        IMname = ['output/',fname(1:length(fname)-3), '_ihls.png'];
        imwrite(ihls,IMname,'PNG','BitDepth',8); 
    end
    if(get(handles.NHSSegmentation,'Value') == 1)
        IMname = ['output/',fname(1:length(fname)-3), '_nhs.png'];
        imwrite(nhs,IMname,'PNG','BitDepth',1); 
    end
    if(get(handles.NoiseRemoval,'Value') == 1)
        IMname = ['output/',fname(1:length(fname)-3), '_noiseRem.png'];
        imwrite(noiseRem,IMname,'PNG','BitDepth',1); 
    end
    if(get(handles.ObjectElimination,'Value') == 1)
       IMname = ['output/',fname(1:length(fname)-3), '_cleanImg.png'];
       imwrite(cleanImg,IMname,'PNG','BitDepth',1); 
    end
    if(get(handles.RecoverDeformedShape,'Value') == 1) % to delete
    end
    if(get(handles.ContourExtraction,'Value') == 1)
        IMname = ['output/',fname(1:length(fname)-3), '_countoutImg.png'];
        imwrite(countoutImg,IMname,'PNG','BitDepth',1); 
        Fname = ['output/',fname(1:length(fname)-3), '_cont','.txt'];
        save(Fname,'valid_contour','-ASCII');
    end
    if(get(handles.GetRotationalOffset,'Value') == 1)
        Fname = ['output/',fname(1:length(fname)-3), '_rotoffset.txt'];
        save(Fname,'Offset','-ASCII');
    end
    if(get(handles.GelisShapeReconstruction,'Value') == 1)
        Fname = ['output/',fname(1:length(fname)-3), '_shape','.txt'];
        save(Fname,'Output','-ASCII');
    end
    if(get(handles.FinalOutput,'Value') == 1)
        IMname = ['output/',fname(1:length(fname)-3), '_output.png'];
        imwrite(OutputImg,IMname,'PNG'); 
    end
end
     

%% --- Executes on selection change in ImageList.
function ImageList_Callback(hObject, eventdata, handles)
% hObject    handle to ImageList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns ImageList contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ImageList
 global DisplayIndex ;
 global Processflag;
 selectedIndex = get(handles.ImageList, 'value');
 ListOfImageNames = get(handles.ImageList, 'string');

 %if no files are loaded in list the list contains [Image List]
 %the first char [ is checked for whether the list is empty or not 
 flagempty = char(ListOfImageNames(selectedIndex)); 
  if  flagempty ~= '['
      baseImageFileName = strcat(cell2mat(ListOfImageNames(selectedIndex)));
      fullImageFileName = [handles.ImageFolder '/' baseImageFileName];
      imageArray = imread(fullImageFileName);
      ImageDisplay(imageArray);
      set(handles.text1,'string',baseImageFileName);
      set(hObject,'UserData',imageArray);
      DisplayIndex = 1; % was a 0
      Processflag = 0;
  end
 %=========================================================================
%% --- Displays image in image list -------  
function ImageDisplay(imageArray)
hold off;	% IMPORTANT NOTE: hold needs to be off in order for the "fit" feature to work correctly.
axesChildHandle = imshow(imageArray, [], 'InitialMagnification', 'fit');
return;
 
 %==========================================================================
%% --- Plot Image----------
function PlotFigure(ObjectArray)
hold off;
axesChildHandle = plot(ObjectArray(:,2),ObjectArray(:,1),'g','LineWidth', 3);
return;
 
%% --- Executes during object creation, after setting all properties.
function ImageList_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ImageList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


%% --- Executes on button press in Previous.
function Previous_Callback(hObject, eventdata, handles)
% hObject    handle to Previous (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% --- Executes on button press in Previous.
    global Processflag;
    if Processflag == 1
        updateDisplayIndex('prev',handles);
        ImageShow(handles);
    end
    
%% --- Executes on button press in Next.
function Next_Callback(hObject, eventdata, handles)
% hObject    handle to Previous (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
     global Processflag;
     if Processflag == 1
        updateDisplayIndex('next',handles);
        ImageShow(handles);
    end
 
%==========================================================================
%%--- show the intermediate results and o/p--------
function ImageShow(handles)
    global DisplayIndex;
%     global ihls;
%     global nhs;
%     global noiseRem;
%     global cleanImg;
%     global countourImg;
    global Processflag; % maybe we can save it in options also, putting it to 0 always when the gui starts
%     global Offset;
%     global Output;
    global Images;
    if Processflag == 1
        ImageDisplay(Images{1, DisplayIndex});
        set(handles.text1,'string',Images{2, DisplayIndex});
%      switch DisplayIndex
%          case 1
%             ImageDisplay(ihls);
%             set(handles.text1,'string','RGB2IHLS Result');
%          case 2
%              ImageDisplay(nhs);
%              set(handles.text1,'string','NHS Segmentation Result');
%          case 3
%              ImageDisplay(noiseRem);
%              set(handles.text1,'string','After Noise Removal');
%          case 4
%              ImageDisplay(cleanImg);
%              set(handles.text1,'string','After Object Elimination');
%          case 5
%              set(handles.text1,'string','Recover Deformed Shape');
%          case 6
%               ImageDisplay(countoutImg);
%              set(handles.text1,'string','Contour');
%          case 7
%               set(handles.text1,'string',['Rotational Offset Value = ' num2str(rad2deg(Offset))]);
%          case 8
%              PlotFigure(Output);
%              set(handles.text1,'string','Gelis Shape Reconstruction');
%          case 9
%              PlotFigure(Output);
%              set(handles.text1,'string','Output');
%          otherwise
%      end     
    end
return;
%==========================================================================
%% -- update DisplayIndex according to check box
function updateDisplayIndex(buttontype,handles)
    global DisplayIndex; % this can go inside Options
    global Images; % here we save all output images, also the input
    [m n] = size(Images);
%    value = 0; % no need to initialize
    if strcmp(buttontype, 'prev')
        value = -1;
    end
    if strcmp(buttontype, 'next')
        value = 1;
    end
    DisplayIndex = DisplayIndex + value;
    if DisplayIndex < 1
        DisplayIndex = 1;
    elseif DisplayIndex > n % number of avaiable outputs
        DisplayIndex = n;
    end
%     presentdisplay = DisplayIndex + value;
%     if(get(handles.IntermediateResults,'Value')==1)
%         flag = 1;
%         while flag == 1
%             switch presentdisplay
%                 case 1
%                     if(get(handles.RGB2IHLS,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                     end
%                 case 2
%                      if(get(handles.NHSSegmentation,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                     end
%                 case 3
%                      if(get(handles.NoiseRemoval,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                     end
%                     case 4
%                     if(get(handles.ObjectElimination,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                     end
%                 case 5
%                      if(get(handles.RecoverDeformedShape,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                     end
%                 case 6
%                      if(get(handles.ContourExtraction,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                      end
%                 case 7
%                     if(get(handles.GetRotationalOffset,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                     end
%                 case 8
%                      if(get(handles.GelisShapeReconstruction,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                     end
%                 case 9
%                      if(get(handles.FinalOutput,'Value') == 1)
%                         flag = 0;
%                     else
%                         presentdisplay = presentdisplay + value;
%                      end
%                 otherwise
%                     if (presentdisplay > 9)
%                         presentdisplay = 9;
%                     end
%                     if (presentdisplay <1)
%                         presentdisplay = 1;
%                     end
%                     flag = 0;
%             end
%         end
%         DisplayIndex = presentdisplay;
%     end 
   return;
                
%============================================================================= 

%% --- Executes when figure1 is resized.
function figure1_ResizeFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
